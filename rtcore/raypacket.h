#ifndef RT_RAYPACKET_H
#define RT_RAYPACKET_H

#include <hitrecord.h>
#include <interval.h>
#include <ray.h>
#include <utils.h>

#include <cassert>
#include <set>

namespace rt {

class BoundingBox;

/*
 * Representation of a bundle of rays.
 *
 * Rays are assumed arranged like this (2x2 packet):
 * 		2	3
 *		0	1
 *
 * @author	Thomas Loockx
 * @date	march 2009
 */
class RayPacket
{
public:
	RayPacket(int nbRays);
	~RayPacket();

	int nbRays() const { return nbRays_; }
	int sqrtNbRays() const { return sqrtNbRays_; }

	Ray& ray(int index);
	const Ray& ray(int index) const;

	HitRecord& hitRecord(int index);
	HitRecord& hitRecord(int index) const;

	bool& hit(int index);
	const bool& hit(int index) const;

    // clusters hit by this packet 
    std::set<int> clusterHitIndices;

    struct IAData
    {
        Interval origin[3];
        Interval direction[3];
    } iaData;

    void calcIAData();

    // first ray to calc interval data
    int begin;

private:
	const int nbRays_;
	const int sqrtNbRays_;
	Ray* rays_;
	HitRecord* hitRecords_;
	bool* hits_;

	DISALLOW_COPY_AND_ASSIGN(RayPacket);
};

} // namespace rt

inline
rt::Ray& rt::RayPacket::ray(int index)
{
	return const_cast<Ray&>(
		const_cast<const RayPacket*>(this)->ray(index));
}

inline
const rt::Ray& rt::RayPacket::ray(int index) const
{
	assert(index >= 0 && index < nbRays());
	return rays_[index];
}

inline
rt::HitRecord& rt::RayPacket::hitRecord(int index)
{
	return const_cast<const RayPacket*>(this)->hitRecord(index);
}

inline
rt::HitRecord& rt::RayPacket::hitRecord(int index) const
{
	assert(index >= 0 && index < nbRays());
	return hitRecords_[index];
}

inline
bool& rt::RayPacket::hit(int index)
{
	return const_cast<bool&>(
		const_cast<const RayPacket*>(this)->hit(index));
}

inline
const bool& rt::RayPacket::hit(int index) const
{
	assert(index >= 0 && index < nbRays());
	return hits_[index];
}

#endif
