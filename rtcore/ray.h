#ifndef RT_RAY_H
#define RT_RAY_H

#include <matrix4.h>
#include <point.h>
#include <vector.h>

#include <iostream>

namespace rt {

class HitRecord;

struct Ray
{
	Ray();
	Ray(Point origin, Vector direction);

	Point origin;
	Vector direction;
	Vector invDirection;	
	bool sign[3]; // true for positive

	Ray transform(const Matrix4& mat) const;
};

bool operator==(const Ray& r1, const Ray& r2);
bool operator!=(const Ray& r1, const Ray& r2);

std::ostream& operator<<(std::ostream& o, const Ray& ray);

} // namespace rt

inline
rt::Ray::Ray(Point origin, Vector direction)
	: origin(origin), direction(direction)
{
	invDirection[0] = 1 / direction.x();
	invDirection[1] = 1 / direction.y();
	invDirection[2] = 1 / direction.z();

	sign[0] = invDirection[0] > 0;
	sign[1] = invDirection[1] > 0;
	sign[2] = invDirection[2] > 0;
}

inline
rt::Ray rt::Ray::transform(const Matrix4& mat) const
{
	return Ray(mat * origin, mat * direction);
}

#endif
