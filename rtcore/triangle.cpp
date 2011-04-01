#include <matrix.h>
#include <triangle.h>
#include <vector.h>
#include <utils.h>

#include <iostream>
#include <sstream>

rt::Triangle::Triangle()
	: a(Point()), b(Point()), c(Point()), color_(Color::GREEN)
{

}

rt::Triangle::Triangle(const Point& a, const Point& b, const Point& c)
	: a(a), b(b), c(c), color_(Color::GREEN)
{
	Vector edge1 = a - c;
	Vector edge2 = a - b;
	
	normal_ = edge2 * edge1;
	normal_.normalize();
}


rt::Triangle::Triangle(const Point& a, const Point& b, const Point& c,
					   const Color& color)
: a(a), b(b), c(c), color_(color)
{
	Vector edge1 = a - c;
	Vector edge2 = a - b;
	
	normal_ = edge2 * edge1;
	normal_.normalize();
}

rt::Triangle rt::Triangle::translate(const Vector& t) const
{
	return Triangle(a + t, b + t, c + t);
}

rt::Triangle rt::Triangle::transform(const Matrix& rot, 
									 const Vector& trans) const
{
	return Triangle(rot * a + trans, rot * b + trans, rot * c + trans);
}

rt::BoundingBox rt::Triangle::boundingBox() const
{
	return BoundingBox(
			min<float>(a.x(), b.x(), c.x()), max<float>(a.x(), b.x(), c.x()),	
			min<float>(a.y(), b.y(), c.y()), max<float>(a.y(), b.y(), c.y()),
			min<float>(a.z(), b.z(), c.z()), max<float>(a.z(), b.z(), c.z()));

	// THIS WAS A FUCKING BUG THAT COST ME 2 DAYS !!!!!!!
	// min<float>(a.z, b.z, b.z), max<float>(a.z, b.z, c.z));
}

rt::Triangle rt::Triangle::createMarker(const Point& p)
{
	float delta = .05f;
	return Triangle(p + Vector(delta, 0, 0),
					p + Vector(0, delta, 0),
					p + Vector(0, 0, delta), Color::RED);
}


rt::Triangle rt::Triangle::createLine(const Point& p1, const Point& p2)
{
	float d = 2.0f;
	return Triangle(p2, p1, p2 + Vector(d,-d,d));
}

bool rt::operator==(const rt::Triangle& a, const rt::Triangle& b)
{
	return  (a.getA() == b.getA()) &&
			(a.getB() == b.getB()) &&
			(a.getC() == b.getC()) &&
			(a.normal() == b.normal());
}

bool rt::operator!=(const Triangle& a, const Triangle& b)
{
	return !(a == b);
}

std::ostream& rt::operator<<(std::ostream& o, const Triangle& triangle)
{
	o << "Triangle::(" << triangle.getA() << ", " << triangle.getB() << ", " << triangle.getC() << ")";
	return o;
}

rt::Point rt::Triangle::center() const
{
	float x = (a.x() + b.x() + c.x()) / 3;
	float y = (a.y() + b.y() + c.y()) / 3;
	float z = (a.z() + b.z() + c.z()) / 3;

	return Point(x,y,z);
}

bool rt::Triangle::hit(const rt::Ray& ray, rt::HitRecord& hitRecord, float t0, float t1) const
{
	Vector e1(b - a);
	Vector e2(c - a);

	Vector p(ray.direction * e2);
	
	float det = e1 % p;

	if (det > -EPS && det < EPS) return false;

	float inv_det = 1.0F / det;

	Vector s(ray.origin - a);

	float beta = inv_det * (s % p);
	if (beta < 0.0F || beta > 1.0F) return false;

	Vector q(s * e1);

	float gamma = inv_det * (ray.direction % q);
	if (gamma < 0.0F || beta + gamma > 1.0F) return false;

	float t = inv_det * (e2 % q);

	if (t < t0 || t > t1) return false;

	hitRecord.color = color();
	hitRecord.normal = normal();
	hitRecord.t = t;
	hitRecord.direction = ray.direction;

	return true;
}
