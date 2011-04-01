#ifndef SAHPARTITIONER_H
#define SAHPARTITIONER_H

#include <point.h>
#include <boundingbox.h>
#include <constants.h>
#include <triangle.h>

#include <vector>

namespace rt {

class Triangle;
class BoundingBox;

/*
 * Partition triangles using binning according the SAH.
 *
 * @author	Thomas Loockx
 */
class SAHPartitioner
{
public:
	SAHPartitioner(const std::vector<Triangle>& triangles, 
				   std::vector<int>& triangleIDS, 
				   int nbBins,
				   int nbObjectsPerLeaf);
    SAHPartitioner(const BoundingBox* boundingBoxes, 
				   std::vector<int>& objectIDs, 
				   int nbBins,
				   int nbObjectsPerLeaf);
	~SAHPartitioner();

	int partition(int beginID, int endID, int& outAxis);

	int nbObjectsPerLeaf() const { return nbObjectsPerLeaf_; }

private:
	void initialize(int beginID, int endID);
	void binTriangles(int beginID, int endID);
	void leftSweep();
	void rightSweep();
	void calculateSplitPlane();
	float calculateArea(int begin, int end) const;

	struct Bin;

	std::vector<int>& objectIDs_;
	std::vector<Bin>& bins_;

	BoundingBox centroidBox_;
	std::vector<float>& cost_;
	std::vector<BoundingBox>& boundingBoxes_;
	std::vector<Point>& centroids_;

	int axis_;
	float split_;
	const int nbObjectsPerLeaf_;

	float boxLength_; // dominant length of the box
	float binWidth_;
	float boxMin_;

	struct Bin
	{
		Bin()
		{
			nbTriangles_ = 0;
		}
		int nbTriangles_;
		BoundingBox boundingBox_;
	};


class TriangleCompare
{
public:
	TriangleCompare(const std::vector<Point>& centroids, 
					float median, int axis)
	: centroids_(centroids), median_(median), axis_(axis)
	{
	}

	bool operator()(int index) const
	{
		return centroids_[index](axis_) <= median_;
	}

private:
	const std::vector<Point>& centroids_;
	float median_;
	int axis_;
};

friend class SAHPartitioner_Test;
};

} // namespace rt

/*
 * USED AS SCHLEMIEL PAINTERS ALGORITHM
 */
inline
float rt::SAHPartitioner::calculateArea(int begin, int end) const
{
	BoundingBox box;

	for (int i = begin; i <= end; ++i)
		box += bins_[i].boundingBox_;

	return box.area();
}

#endif
