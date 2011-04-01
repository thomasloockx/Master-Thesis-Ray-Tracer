#ifndef RT_TRIANGLE_H
#define RT_TRIANGLE_H

#include <point.h>
#include <boundingbox.h>
#include <color.h>
#include <hitrecord.h>
#include <constants.h>

#include <string>

namespace rt {

class Matrix;
class Vector;

/*
 * Triangle from a mesh with a normal at each vertex.
 *
 * @author	Thomas Loockx
 * @date	09/2009
 */
class Triangle
{
public:
	Triangle();
	Triangle(const Point& a, const Point& b, const Point& c);
	Triangle(const Point& a, const Point& b, const Point& c,
			 const Color& color);

	Point getA() const { return a; }
	Point getB() const { return b; }
	Point getC() const { return c; }
	void setA(const Point& a) { this->a = a; }
	void setB(const Point& b) { this->b = b; }
	void setC(const Point& c) { this->c = c; }

	Vector normal() const { return normal_; }
	void setNormal(const Vector& n) { normal_ = n; }

	Color color() const { return color_; }
	void setColor(const Color& c) { color_ = c; }

	bool hit(const Ray& ray, HitRecord& hitRecord, float t0 = 0, float t1 = INF) const; 
	BoundingBox boundingBox() const;
	Point center() const;

	Triangle translate(const Vector& t) const;
	Triangle transform(const Matrix& rot, const Vector& trans) const;

	/* Creates a triangle that marks the point. */
	static Triangle createMarker(const Point& p);
	/* Create a pair of triangles that make up a line. */
	static Triangle createLine(const Point& p1, const Point& p2);

private:
	/* Vertices */
	Point a;
	Point b;
	Point c;

	Color color_;
	Vector normal_;
};

bool operator==(const Triangle& a, const Triangle& b);
bool operator!=(const Triangle& a, const Triangle& b);
std::ostream& operator<<(std::ostream& o, const Triangle& triangle);

} // namespace rt

#endif 
