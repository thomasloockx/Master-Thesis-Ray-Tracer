#include <ray.h>
#include <utils.h>

rt::Ray::Ray() 
{
}

bool rt::operator==(const Ray& r1, const Ray& r2)
{
	return (r1.origin == r2.origin) && (r1.direction == r2.direction);
}

bool rt::operator!=(const Ray& r1, const Ray& r2)
{
	return !(r1 == r2);
}

std::ostream& rt::operator<<(std::ostream& o, const Ray& ray)
{
	o << "Ray::( " << ray.origin << " " << ray.direction << ")";
	return o;
}
