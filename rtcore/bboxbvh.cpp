#include <bboxbvh.h>
#include <partitionalgorithms.h>
#include <stats.h>

#include <algorithm>
#include <cmath>
#include <stack>

rt::BBoxBVH::BBoxBVH(const BoundingBox* boxes, int nbBoxes)
: BVH(nbBoxes), boundingBoxes_(boxes)
{
	build(0, 0, nbBoxes, 0);
	nodes_.resize(nextFreeNode_ + 1);
}

void rt::BBoxBVH::build(int nodeID, int beginID, int endID, int axis)
{
	assert(beginID != endID);
	assert(nodeID < int(nodes_.size()));

	Node& n = nodes_[nodeID];

	if (endID - beginID == 1) {
		/* Create a leaf node */
		int bboxID = objectIDs_[beginID];
		n.box = boundingBoxes_[bboxID];
        n.makeLeaf(bboxID, 1);
	} 
	else {
		/* Create an inner node */
		int bestAxis = 0;
		int split = partitionMedian(boundingBoxes_, 
                                    objectIDs_,
									beginID, 
									endID, 
									bestAxis);

        ++nextFreeNode_;
        n.makeInner(nextFreeNode_, bestAxis);
		++nextFreeNode_; // make room for the right

		build(n.firstChild, beginID, split, (++axis) % 3);
		build(n.firstChild + 1, split, endID, (++axis) % 3);

		n.box = nodes_[n.firstChild].box + nodes_[n.firstChild+1].box;
	}
}

bool rt::BBoxBVH::checkLeaf(const Node& node, 
                            const Ray& ray, 
                            HitRecord& hitRecord) const
{
    // dummy implementation
    return node.box.hit(ray, 0, hitRecord.t);
}

bool rt::BBoxBVH::processLeaf(const Node& n, 
                              int firstActive,
                              RayPacket& p) const
{
    for (int r = firstActive; r < p.nbRays(); ++r) {
        if (n.box.hit(p.ray(r), 0, p.hitRecord(r).t)) {
            p.clusterHitIndices.insert(n.firstChild);
            return true;
        }
    }
    return false;
}

// Override because different counting of  #bbox tests
bool rt::BBoxBVH::processLeaf(const Node& n, 
                              int firstActive,
                              RayPacket& p,
                              Stats& stats) const
{
    for (int r = firstActive; r < p.nbRays(); ++r) {
        ++stats.nbBoundingBoxTests_;
        if (n.box.hit(p.ray(r))) {
            p.clusterHitIndices.insert(n.firstChild);
            return true;
        }
    }
    return false;
}
