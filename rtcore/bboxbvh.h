#ifndef RT_BBOX_BVH
#define RT_BBOX_BVH

#include <boundingbox.h>
#include <bvh.h>
#include <ray.h>
#include <raypacket.h>
#include <utils.h>

#include <set>
#include <stack>

namespace rt {
 
struct Stats;

class BBoxBVH : public BVH
{
public:
	BBoxBVH(const BoundingBox* boxes, int nbBoxes);

private:
	const BoundingBox* boundingBoxes_;	
	void build(int nodeID, int beginID, int endID, int axis);

	bool checkLeaf(const Node& node, 
                   const Ray& ray, 
                   HitRecord& hitRecord) const;

    bool processLeaf(const Node& node, 
                     int firstActive,
                     RayPacket& packet) const; 
    bool processLeaf(const Node& node, 
                     int firstActive,
                     RayPacket& packet,
                     Stats& stats) const;
 
    friend class BBoxBVH_Test;
};

} // namespace rt

#endif
