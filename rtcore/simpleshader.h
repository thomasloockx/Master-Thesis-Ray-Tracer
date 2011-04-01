#ifndef RT_SIMPLESHADER_H
#define RT_SIMPLESHADER_H

#include <hitrecord.h>
#include <constants.h>

#include <algorithm>
#include <cmath>
#include <cassert>

namespace rt {

/*
 * Simple shader that dots the ray direction with the normal.
 *
 * @author	Thomas Loockx
 * @date	09/2008
 */
class SimpleShader
{
public:
	Color diffuse(const HitRecord& hitRecord) const;				
	Color noShading(const HitRecord& hitRecord) const;
};

} // namespace rt

inline
rt::Color rt::SimpleShader::diffuse(const HitRecord& hitRecord) const
{
	float dot = fabs(hitRecord.direction % hitRecord.normal);
	int r =  std::min(255, static_cast<int>(dot * hitRecord.color.r));
	int g =  std::min(255, static_cast<int>(dot * hitRecord.color.g));
	int b =  std::min(255, static_cast<int>(dot * hitRecord.color.b));

	return Color(r,g,b);
}

inline
rt::Color rt::SimpleShader::noShading(const HitRecord& hitRecord) const
{
	return hitRecord.color;
}

#endif
