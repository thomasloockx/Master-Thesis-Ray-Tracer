#ifndef RT_SIMPLEBVH_H
#define RT_SIMPLEBVH_H

#include <boundingbox.h>
#include <bvh.h>
#include <hitrecord.h>
#include <matrix.h>
#include <point.h>
#include <raypacket.h>
#include <triangle.h>
#include <vector.h>
#include <utils.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <stdint.h>
#include <vector>

namespace rt {

class Ray;
class SAHPartitioner;
class Stats;

/*
 * Reference BVH tree implementation.
 *
 * @author	Thomas Loockx
 * @date	november 2008
 */
class SimpleBVH : public BVH
{
public:
	SimpleBVH(const std::vector<Triangle>& triangles, int nbPartitionBins = 16);
    ~SimpleBVH();
	
	void rebuild();

	const std::vector<Triangle>& triangles() const 
    { 
        return triangles_; 
    }
    void setTriangles(const std::vector<Triangle>& triangles)
    {
        const_cast< std::vector<Triangle>& >(triangles_) = triangles;
    }

protected:
	SimpleBVH(const std::vector<Triangle>& triangles, 
              const std::vector<BoundingBox>& boundingBoxes,
              int nbPartitionBins = 16);
	void build(int nodeIndex, int beginID, int endID, int axis);
	bool checkLeaf(const Node& n, const Ray& r, HitRecord& h) const;

	const std::vector<Triangle>& triangles_;
	const std::vector<BoundingBox>& boundingBoxes_;
	SAHPartitioner *partitioner_;
    BoundingBox createLeafBox(const Node& node) const;

private:
	void rebuild(int nodeIndex);

	friend std::ostream& operator<<(std::ostream& o, const Node& n);
};

std::ostream& operator<<(std::ostream& o, const SimpleBVH::Node& n);

} // namespace rt

#endif
