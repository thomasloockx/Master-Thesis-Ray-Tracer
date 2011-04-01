#include <hitrecord.h>
#include <constants.h>

rt::HitRecord::HitRecord()
	: t(INF)
{
}

bool rt::operator==(const rt::HitRecord& h1, const rt::HitRecord& h2)
{
	return 	(h1.t == h2.t) && (h1.direction == h2.direction) && 
			(h1.normal == h2.normal);
}

bool rt::operator!=(const rt::HitRecord& h1, const rt::HitRecord& h2)
{
	return !(h1 == h2);
}

bool rt::operator<(const rt::HitRecord& h1, const rt::HitRecord& h2)
{
	return (h1.t < h2.t);
}

bool rt::operator>(const rt::HitRecord& h1, const rt::HitRecord& h2)
{
	return (h1.t > h2.t);
}
