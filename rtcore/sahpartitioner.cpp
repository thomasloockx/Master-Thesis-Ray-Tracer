#include <constants.h>
#include <boundingbox.h>
#include <partitionalgorithms.h>
#include <sahpartitioner.h>
#include <triangle.h>
#include <utils.h>

#include <algorithm>
#include <climits>
#include <cfloat>
#include <iostream>

rt::SAHPartitioner::SAHPartitioner(const std::vector<Triangle>& triangles,
								   std::vector<int>& objectIDs,
								   int nbBins,
								   int nbObjectsPerLeaf)
: objectIDs_(objectIDs),
  bins_(*(new std::vector<Bin>(nbBins))),
  cost_(*(new std::vector<float>(nbBins - 1))),
  boundingBoxes_(*( new std::vector<BoundingBox>(triangles.size()) )),
  centroids_(*( new std::vector<Point>(triangles.size()) )),
  axis_(-4),
  split_(-1),
  nbObjectsPerLeaf_(nbObjectsPerLeaf)
{
    assert(triangles.size() == objectIDs.size());
	for (std::size_t i = 0; i < triangles.size(); ++i) {
		boundingBoxes_[i] = triangles[i].boundingBox();
		centroids_[i] = boundingBoxes_[i].center();
	}
}

rt::SAHPartitioner::SAHPartitioner(const BoundingBox* boundingBoxes, 
                                   std::vector<int>& objectIDs, 
                                   int nbBins,
                                   int nbObjectsPerLeaf)
: objectIDs_(objectIDs),
  bins_(*(new std::vector<Bin>(nbBins))),
  cost_(*(new std::vector<float>(nbBins - 1))),
  boundingBoxes_(*( new std::vector<BoundingBox>(objectIDs.size()) )),
  centroids_(*( new std::vector<Point>(objectIDs.size()) )),
  axis_(-4),
  split_(-1),
  nbObjectsPerLeaf_(nbObjectsPerLeaf)
{
    for (std::size_t i = 0; i < objectIDs.size(); ++i) {
		boundingBoxes_[i] = boundingBoxes[i];
		centroids_[i] = boundingBoxes_[i].center();
	}
}

rt::SAHPartitioner::~SAHPartitioner()
{
	delete &cost_;
	delete &bins_;
	delete &boundingBoxes_;
	delete &centroids_;
}

int rt::SAHPartitioner::partition(int beginID, int endID, int& outAxis)
{
	int nbObjects = endID - beginID;
	if (nbObjects <= nbObjectsPerLeaf()) {
		outAxis = -1;
		return -1;
	}

	// clean the bins
	for (std::size_t i = 0; i < bins_.size(); ++i) {
		bins_[i].nbTriangles_ = 0;
		bins_[i].boundingBox_ = BoundingBox();
	}

	initialize(beginID, endID);
	binTriangles(beginID, endID);
	leftSweep();
	rightSweep();
	
	TriangleCompare comp(centroids_, split_, axis_);
	std::vector<int>::iterator bound = std::partition(
										objectIDs_.begin() + beginID,
										objectIDs_.begin() + endID,
										comp);

	int median = bound - objectIDs_.begin();

	// rebalance
	if (median == beginID)
		++median;
	else if (median == endID)
		--median;

	outAxis = axis_;
	return median;
}

void rt::SAHPartitioner::initialize(int beginID, int endID)
{
	// calculate centroidbox
	float xmin = INF;
	float ymin = INF;
	float zmin = INF;
	float xmax = -INF;
	float ymax = -INF;
	float zmax = -INF;

	for (int i = beginID; i < endID; ++i) {
		Point p = centroids_[objectIDs_[i]];		

		xmin = std::min(xmin, p.x());
		ymin = std::min(ymin, p.y());
		zmin = std::min(zmin, p.z());

		xmax = std::max(xmax, p.x());
		ymax = std::max(ymax, p.y());
		zmax = std::max(zmax, p.z());
	}

	centroidBox_ = BoundingBox(xmin, xmax, ymin, ymax, zmin, zmax);
	axis_ = centroidBox_.longestAxis();
	boxLength_ = centroidBox_.longestSideLength();
}

void rt::SAHPartitioner::binTriangles(int beginID, int endID)
{
	float k0;
	if (axis_ == X_AXIS)
		k0 = centroidBox_.xmin();	
	else if (axis_ == Y_AXIS)
		k0 = centroidBox_.ymin();
	else
		k0 = centroidBox_.zmin();

	boxMin_ = k0;
	binWidth_ = boxLength_ / bins_.size();

	float k1 = bins_.size() * (1 - EPS) / boxLength_;
	
	for (int i = beginID; i < endID; ++i) {

		int binID = k1 * (centroids_[objectIDs_[i]](axis_) - k0);
		bins_[binID].nbTriangles_++;

		bins_[binID].boundingBox_ += boundingBoxes_[objectIDs_[i]];
	}
}

void rt::SAHPartitioner::leftSweep()
{
	float accNbTris = 0; 

    BoundingBox accBox;

	for (std::size_t i = 0; i < cost_.size(); ++i) {
        accBox += bins_[i].boundingBox_;
        float area = accBox.area();

		//float area = calculateArea(0, i);	

		accNbTris += bins_[i].nbTriangles_;
		cost_[i] = area * accNbTris;
	}
}

void rt::SAHPartitioner::rightSweep()
{
	float accNbTris = 0; 
	int split = 0;
	float minCost = FLT_MAX;

    BoundingBox accBox;
	for (int i = cost_.size()-1; i > 0; --i) {

        accBox += bins_[i].boundingBox_;
        float area = accBox.area();

        accNbTris += bins_[i].nbTriangles_;
		cost_[i-1] += area * accNbTris;

		if (cost_[i-1] < minCost) {
			minCost = cost_[i-1];
			split = i-1;
		}	
	}
	
	// split in world coords along the axis
	split_ = boxMin_ + binWidth_ * (split + 1);
}
