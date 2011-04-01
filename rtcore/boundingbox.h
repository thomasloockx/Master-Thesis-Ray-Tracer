#ifndef RT_BOUNDINGBOX_H
#define RT_BOUNDINGBOX_H

#include <constants.h>
#include <point.h>
#include <ray.h>

#include <cmath>
#include <vector>

namespace rt {

class RayPacket;
class Triangle;

/*
 * Implementation of an axis aligned bounding box.
 *
 * @author	Thomas Loockx
 * @date	10/2008
 */
class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(Point nlb, Point frt);
	BoundingBox(float xmin, float xmax, 
				float ymin, float ymax, 
				float zmin, float zmax);

	bool hit(const Ray& ray, float t0 = 0, float t1 = INF) const;
    bool miss(const RayPacket& packet) const;

	float xmin() const { return corners_[0][0]; }
	float xmax() const { return corners_[1][0]; }
	float ymin() const { return corners_[0][1]; }
	float ymax() const { return corners_[1][1]; }
	float zmin() const { return corners_[0][2]; }
	float zmax() const { return corners_[1][2]; }

	float width() const { return fabs(xmax() - xmin()); }
	float height() const { return fabs(ymax() - ymin()); }
	float depth() const { return fabs(zmax() - zmin()); }
	
	Point center() const;
	float area() const;
	float longestSideLength() const; 
	int longestAxis() const;
	float shortestSideLength() const;
	int shortestAxis() const;
	bool contains(const Point& p) const;
	bool contains(const Triangle& t) const;
	bool contains(const BoundingBox& bbox) const;
	bool isZeroBox() const { return corners_[0] == corners_[1]; }
	bool isZeroAxis(int axis) const;
	float dimension(int axis) const;
	void vertices(Point *p) const;

	std::vector<Triangle> triangles() const;

	static BoundingBox globalBox(const std::vector<Triangle*>& tris);
	static BoundingBox globalBox(const std::vector<Triangle>& tris);
	static BoundingBox globalBox(const std::vector<Point> points);

	Point& operator[](int index) { return corners_[index]; }
	const Point& operator[](int index) const { return corners_[index]; }
	BoundingBox operator+=(const BoundingBox& rhs);
	friend bool operator==(const BoundingBox& b1, const BoundingBox& b2);

private:
	// corners_[0] = nearLeftBottom
	// corners_[1] = farRightTop
	Point corners_[2];
};

bool operator==(const BoundingBox& b1, const BoundingBox& b2);
bool operator!=(const BoundingBox& b1, const BoundingBox& b2);
std::ostream& operator<<(std::ostream& o, const BoundingBox& box);

/* Join operation on bounding boxes. */
BoundingBox operator+(const BoundingBox& b1, const BoundingBox b2);

} // namespace rt

inline
rt::BoundingBox rt::BoundingBox::operator+=(const rt::BoundingBox& rhs)
{
corners_[0] = Point(
		min(xmin(), rhs.xmin()),
		min(ymin(), rhs.ymin()),
		min(zmin(), rhs.zmin()));

    corners_[1] = Point(
		max(xmax(), rhs.xmax()),
		max(ymax(), rhs.ymax()),
		max(zmax(), rhs.zmax()));
	return *this;	
}

inline
rt::Point rt::BoundingBox::center() const
{
	return Point(
			xmin() + width()  / 2,
			ymin() + height() / 2,
			zmin() + depth()  / 2);
}

inline
float rt::BoundingBox::area() const
{
	Vector d = corners_[1] - corners_[0];
	return 2 * (d.x() * (d.y() + d.z()) + d.y() * d.z());
}

#endif
