#include <constants.h>
#include <hitrecord.h>
#include <partitionalgorithms.h>
#include <ray.h>
#include <sahpartitioner.h>
#include <simplebvh.h>
#include <stats.h>
#include <triangle.h>

#include <iostream>

/******************************************************************************
 * CONSTRUCTION/DESTRUCTION
 ******************************************************************************/

rt::SimpleBVH::SimpleBVH(const std::vector<Triangle>& triangles, 
                         int nbPartitionBins)
: BVH(int(triangles.size())),
  triangles_(triangles),
  boundingBoxes_( *(new std::vector<BoundingBox>(nbObjects())) ),
  partitioner_(new SAHPartitioner(triangles_, objectIDs_, nbPartitionBins, 2))
{
	for (int b = 0; b < nbObjects(); ++b) 
		const_cast< std::vector<BoundingBox>& >(boundingBoxes_)[b] 
			= triangles_[b].boundingBox();

    build(0, 0, nbObjects(), 0);
	nodes_.resize(nbNodes());
}

rt::SimpleBVH::SimpleBVH(const std::vector<Triangle>& triangles, 
                         const std::vector<BoundingBox>& boundingBoxes,
                         int nbPartitionBins)
: BVH(int(triangles.size())),
  triangles_(triangles),
  boundingBoxes_( *(new std::vector<BoundingBox>(nbObjects())) ),
  partitioner_(new SAHPartitioner(triangles_, objectIDs_, nbPartitionBins, 2))
{
    assert(boundingBoxes.size() == triangles.size());
    // ineffective
	for (int b = 0; b < nbObjects(); ++b) 
		const_cast< std::vector<BoundingBox>& >(boundingBoxes_)[b] 
			= boundingBoxes[b];

    build(0, 0, nbObjects(), 0);
	nodes_.resize(nbNodes());
}

rt::SimpleBVH::~SimpleBVH()
{
	delete partitioner_;
    delete &boundingBoxes_;
}

/******************************************************************************
 * BUILDING/REBUILDING  
 ******************************************************************************/

void rt::SimpleBVH::build(int nodeIndex, int beginID, int endID, int axis)
{
    assert(nodeIndex < int(nodes_.size()));
	Node& n = nodes_[nodeIndex];
	nbNodes_++;

	assert(beginID < endID);
	assert(endID <= int(triangles_.size()));

	int bestAxis = -1;
	int split = partitioner_->partition(beginID, endID, bestAxis);

	if (split != -1) {
        ++nextFreeNode_;
        n.makeInner(nextFreeNode_, bestAxis);
        ++nextFreeNode_; // make room for the left

		build(n.firstChild, beginID, split, (++axis) % 3);
		build(n.firstChild+ 1, split, endID, (++axis) % 3);		

		n.box = nodes_[n.firstChild].box + nodes_[n.firstChild+1].box;	
	} 
	else {
        n.makeLeaf(beginID, endID - beginID);
		
		for (int i = beginID; i < endID; ++i) {
			assert(i >= 0 && i < int(objectIDs_.size()));
			assert(objectIDs_[i] < int(boundingBoxes_.size()));
			n.box += boundingBoxes_[objectIDs_[i]];
		}
	}
}

void rt::SimpleBVH::rebuild()
{
	rebuild(0);		
}

void rt::SimpleBVH::rebuild(int nodeIndex)
{
	Node& n = nodes_[nodeIndex];

	if (n.isLeaf()) {
        n.box = createLeafBox(n);
	} 
	else {
		rebuild(n.firstChild);
		rebuild(n.firstChild+1);
		n.box = nodes_[n.firstChild].box + nodes_[n.firstChild+1].box;	
	}
}

rt::BoundingBox rt::SimpleBVH::createLeafBox(const Node& node) const
{
    assert(node.isLeaf());

    BoundingBox leafBox;
    int begin = node.firstChild;
    int end   = begin + node.nbObjects;
    for (int i = begin; i < end; ++i)
        leafBox += triangles_[objectIDs_[i]].boundingBox();

    return leafBox;
}

/******************************************************************************
 * TRAVERSAL
 ******************************************************************************/

bool rt::SimpleBVH::checkLeaf(const Node& n, const Ray& r, HitRecord& h) const
{
	int end = n.firstChild + n.nbObjects;	
	bool hasHit = false;

	for (int i = n.firstChild; i < end; ++i) {
		if (triangles_[objectIDs_[i]].hit(r, h, 0, h.t))
			hasHit = true;
	}
	return hasHit;
}

/******************************************************************************
 * FOR DEBUGGING
 ******************************************************************************/

std::ostream& rt::operator<<(std::ostream& o, const rt::SimpleBVH::Node& n)
{
	o << "Node::(";
	if (!n.isLeaf()) {
		o << "inner, ";
		o << "left = " << n.firstChild << ", ";
		o << "right = " << n.firstChild + 1 << ", ";
		o << "split = " << n.axis << ", ";
	}
	else {
		o << "leaf, ";
		o << "firstTriangleIndex = " << n.firstChild << ", ";
		o << "#triangles = " << n.nbObjects << ", ";
	}	
	o << n.box << ")";
	return o;
}
