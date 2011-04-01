#include <fuzzybvh.h>
#include <sahpartitioner.h>
#include <stats.h>

/******************************************************************************
 * CONSTRUCTION/DESTRUCTION
 ******************************************************************************/

rt::FuzzyBVH::FuzzyBVH(const std::vector<Triangle>& triangles,
                       const std::vector<BoundingBox>& fuzzyBoxes,
                       int nbPartitionBins)
: SimpleBVH(triangles, fuzzyBoxes, nbPartitionBins)
{
}

/******************************************************************************
 * TRAVERSAL 
 ******************************************************************************/

bool rt::FuzzyBVH::traverse(RayPacket& packet) const
{
    assert(stack_.empty());

	bool hit = false;
    stack_.push(StackRec(0,0));	

    RayPacket transformed(packet.nbRays());
    transform(packet, transformed);

	while (true) {

		Node& n = nodes_[stack_.top().nodeID];
		int firstActive = findFirst(transformed, stack_.top().firstActive, n.box);
	    stack_.pop();

		if (firstActive < packet.nbRays()) {
			if (n.isInner()) {
                processInner(n, firstActive, transformed);
            } else {
                /* 
                 * BBox hack 
                 * we need to keep it because the box is needed in other places
                 * of the traversal.
                 */
                BoundingBox box = n.box;
                n.box = createLeafBox(n);
                if (processLeaf(n, firstActive, packet)) {
                    hit = true;
                }

                // restore the box
                n.box = box; 
            }
		} 
		if (stack_.empty()) 
			return hit;
	}
}

/******************************************************************************
 * TRAVERSAL 
 ******************************************************************************/

bool rt::FuzzyBVH::traverse(RayPacket& packet, Stats& stats) const
{
    assert(stack_.empty());

	bool hit = false;
    stack_.push(StackRec(0,0));	

    RayPacket transformed(packet.nbRays());
    transform(packet, transformed);

	while (true) {

		Node& n = nodes_[stack_.top().nodeID];
		int firstActive = findFirst(transformed, 
                                    stack_.top().firstActive, 
                                    n.box, 
                                    stats);
	    stack_.pop();

		if (firstActive < packet.nbRays()) {
			if (n.isInner()) {
                processInner(n, firstActive, packet);
            } else {
                // BBox hack, see above
                BoundingBox box = n.box;
                n.box = createLeafBox(n);
                if (processLeaf(n, firstActive, packet, stats)) {
                    hit = true;
                }
                n.box = box; 
            }
		} 
		if (stack_.empty()) 
			return hit;
	}
}

void rt::FuzzyBVH::transform(const RayPacket& packet,
                             RayPacket& transformed) const
{
    Point newOrigin = transformation() * packet.ray(0).origin;
    for (int r = 0; r < packet.nbRays(); ++r) {
        transformed.ray(r) = Ray(newOrigin, 
                                 transformation() * packet.ray(r).direction);
    }
}
