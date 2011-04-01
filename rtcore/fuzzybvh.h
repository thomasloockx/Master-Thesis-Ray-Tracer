#ifndef RT_FUZZYBVH_H
#define RT_FUZZYBVH_H

#include <simplebvh.h>
#include <matrix4.h>
#include <raypacket.h>

namespace rt {

class SAHPartitioner;

class FuzzyBVH : public SimpleBVH 
{
public:
    FuzzyBVH(const std::vector<Triangle>& triangles,
             const std::vector<BoundingBox>& fuzzyBoxes,
             int nbPartitionBins);

	bool traverse(RayPacket& packet) const;
	bool traverse(RayPacket& packet, Stats& stats) const;

    void setTransformation(const Matrix4& transformation)
    {
        transformation_ = transformation;
    }
    const Matrix4& transformation() const
    {
        return transformation_;
    }

private:
   Matrix4 transformation_;

   void transform(const RayPacket& packet,
                  RayPacket& transformed) const;
};

} // namespace rt

#endif
