#include <boundingbox.h>
#include <constants.h>
#include <interval.h>
#include <raypacket.h>
#include <triangle.h>

#include <iostream>
#include <algorithm>
#include <boost/foreach.hpp>

rt::BoundingBox::BoundingBox()
{
	corners_[0] = Point(INF, INF, INF);
	corners_[1] = Point(-INF, -INF, -INF);
}


rt::BoundingBox::BoundingBox(rt::Point nlb, rt::Point frt)
{
	corners_[0] = nlb;
	corners_[1] = frt;
}


rt::BoundingBox::BoundingBox(float xmin, float xmax, 
							 float ymin, float ymax, 
							 float zmin, float zmax)
{
	corners_[0] = Point(xmin, ymin, zmin);
	corners_[1] = Point(xmax, ymax, zmax);
}


bool rt::BoundingBox::hit(const rt::Ray& ray, float t0, float t1) const
{
	/* 
	 * OPTIMIZED 
	 * unrolled loop to ensure earlier overlap tests.
	 *
	 * Optimized according to RT News Volume 19 Number 1
     */
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
			
	tmin = ray.invDirection.x() * (corners_[1 - ray.sign[0]].x() - ray.origin.x());
	tymax = ray.invDirection.y() * (corners_[ray.sign[1]].y() - ray.origin.y());

	if (tmin > tymax)
		return false;

	tmax = ray.invDirection.x() * (corners_[ray.sign[0]].x() - ray.origin.x());
	tymin = ray.invDirection.y() * (corners_[1 - ray.sign[1]].y() - ray.origin.y());

	if ( tymin > tmax )
		return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

	tzmin = ray.invDirection.z() * (corners_[1 - ray.sign[2]].z() - ray.origin.z());
	tzmax = ray.invDirection.z() * (corners_[ray.sign[2]].z() - ray.origin.z());

	if ( (tmin > tzmax) || (tzmin > tmax) )
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return ( (tmin <= t1) && (tmax >= t0) );
}

bool rt::BoundingBox::miss(const RayPacket& packet) const
{
    Interval slabX(xmin(), xmax());
    Interval slabY(ymin(), ymax());
    Interval slabZ(zmin(), zmax());

    Interval tx = (slabX - packet.iaData.origin[0]) * packet.iaData.direction[0].reciproce();
    Interval ty = (slabY - packet.iaData.origin[1]) * packet.iaData.direction[1].reciproce();
    Interval tz = (slabZ - packet.iaData.origin[2]) * packet.iaData.direction[2].reciproce();

    /*
    std::cout << tx << std::endl;
    std::cout << ty << std::endl;
    std::cout << tz << std::endl;
    */

    Interval test = Interval(0,FLT_MAX) && tx && ty && tz;
    return test.left() > test.right();
}

float rt::BoundingBox::longestSideLength() const
{
	return max<float>(width(), height(), depth());
}

int rt::BoundingBox::longestAxis() const
{
	return Vector(width(), height(), depth()).maxComponentIndex(); 
}

float rt::BoundingBox::shortestSideLength() const
{
	return min<float>(width(), height(), depth());
}

int rt::BoundingBox::shortestAxis() const 
{
	return Vector(width(), height(), depth()).minComponentIndex();
}

bool rt::BoundingBox::contains(const rt::Point& p) const
{
	if (xmin() > p.x() || xmax() < p.x())
		return false;
	if (ymin() > p.y() || ymax() < p.y())
		return false;
	if (zmin() > p.z() || zmax() < p.z())
		return false;
	return true;
}

bool rt::BoundingBox::contains(const Triangle& t) const
{
	return contains(t.getA()) && contains(t.getB()) && contains(t.getC());
}

bool rt::BoundingBox::contains(const BoundingBox& bbox) const
{
	return contains(bbox[0]) && contains(bbox[1]);
}

bool rt::BoundingBox::isZeroAxis(int axis) const
{
	assert (axis >= 0 && axis < 3);
	if (axis == X_AXIS)
		return width() == 0;
	else if (axis == Y_AXIS)
		return height() == 0;
	else
		return depth() == 0;
}

float rt::BoundingBox::dimension(int axis) const
{
	assert (axis >= 0 && axis < 3);
	if (axis == X_AXIS)
		return width();
	else if (axis == Y_AXIS)
		return height();
	else
		return depth();
}

void rt::BoundingBox::vertices(Point *p) const
{
	// ground plane
	p[0] = Point(xmin(), ymin(), zmin());
	p[1] = Point(xmax(), ymin(), zmin());
	p[2] = Point(xmax(), ymin(), zmax());
	p[3] = Point(xmin(), ymin(), zmax());

	// top plane
	p[4] = Point(xmin(), ymax(), zmin());
	p[5] = Point(xmax(), ymax(), zmin());
	p[6] = Point(xmax(), ymax(), zmax());
	p[7] = Point(xmin(), ymax(), zmax());
}

bool rt::operator==(const rt::BoundingBox& b1, const rt::BoundingBox& b2)
{
	return (b1.corners_[0] == b2.corners_[0]) &&
		   (b1.corners_[1] == b2.corners_[1]);
}

bool rt::operator!=(const BoundingBox& b1, const BoundingBox& b2)
{
	return !(b1 == b2);
}

rt::BoundingBox rt::operator+(const rt::BoundingBox& b1, const rt::BoundingBox b2)
{			
	return BoundingBox(b1) += b2;
}

std::vector<rt::Triangle> rt::BoundingBox::triangles() const
{	
	Vector tx(width(), 0, 0);
	Vector ty(0, height(), 0);
	Vector tz(0, 0, depth());

	Point p1(corners_[0]);
	Point p2 = p1 + tx;
	Point p3 = p1 + tx + ty;
	Point p4 = p1 + ty;
	Point p5 = p1 + ty + tz;
	Point p6 = p1 + tx + ty + tz;
	Point p7 = p1 + tx + tz;
	Point p8 = p1 + tz;

	Triangle t1(p1, p4, p2);
	Triangle t2(p4, p3, p2);
	Triangle t3(p4, p5, p3);
	Triangle t4(p3, p5, p6);
	Triangle t5(p3, p7, p2);
	Triangle t6(p3, p6, p7);
	Triangle t7(p1, p8, p2);
	Triangle t8(p2, p8, p7);
	Triangle t9(p1, p4, p8);
	Triangle t10(p4, p5, p8);
	Triangle t11(p5, p7, p8);
	Triangle t12(p5, p6, p7);

	std::vector<Triangle> triangles;
	triangles.push_back(t1);
	triangles.push_back(t2);
	triangles.push_back(t3);
	triangles.push_back(t4);
	triangles.push_back(t5);
	triangles.push_back(t6);
	triangles.push_back(t7);
	triangles.push_back(t8);
	triangles.push_back(t9);
	triangles.push_back(t10);
	triangles.push_back(t11);
	triangles.push_back(t12);

	return triangles;
}

rt::BoundingBox rt::BoundingBox::globalBox(const std::vector<Triangle*>& tris)
{
	BoundingBox global;

	BOOST_FOREACH(Triangle* triangle, tris)
		global += triangle->boundingBox();

	return global;
}

rt::BoundingBox rt::BoundingBox::globalBox(const std::vector<Triangle>& tris)
{
	BoundingBox global;

	BOOST_FOREACH(Triangle triangle, tris)
		global += triangle.boundingBox();

	return global;
}

rt::BoundingBox rt::BoundingBox::globalBox(const std::vector<Point> points)
{
	BoundingBox global;

	BOOST_FOREACH(Point p, points)
		global += BoundingBox(p, p);

	return global;
}

std::ostream& rt::operator<<(std::ostream& o, const rt::BoundingBox& box)
{
	o << "BoundingBox::(";
	o << "xmin=" << box.xmin() << ",";
	o << "xmax=" << box.xmax() << ",";
	o << "ymin=" << box.ymin() << ",";
	o << "ymax=" << box.ymax() << ",";
	o << "zmin=" << box.zmin() << ",";
	o << "zmax=" << box.zmax() << ")";

	return o;
}
