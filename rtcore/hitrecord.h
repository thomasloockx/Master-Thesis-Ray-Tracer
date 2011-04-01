#ifndef RT_HITRECORD_H
#define RT_HITRECORD_H

#include <ray.h>
#include <vector.h>
#include <color.h>

#include <set>

namespace rt {

/*
 * HitRecords contain information about the intersection between a ray and
 * an object. The information in a HitRecord is only valid when there is a "hit".
 *
 * @author	Thomas Loockx
 * @date	10/2008
 */
struct HitRecord
{
	HitRecord();
	void reset();

	float t;	/* Ray parameter */
	Vector direction; /* Direction of the ray. */
	Vector normal; /* Normal at the hit point */
	Color color; /* Color of the hitted object */
    std::set<int> indices; /* Indices of the objects hit. */
};

bool operator==(const HitRecord& h1, const HitRecord& h2);
bool operator!=(const HitRecord& h1, const HitRecord& h2);
bool operator<(const HitRecord& h1, const HitRecord& h2);
bool operator>(const HitRecord& h1, const HitRecord& h2);

} // namespace rt

inline 
void rt::HitRecord::reset()
{
	t = INF;
    indices.clear();
}

#endif
