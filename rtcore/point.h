#ifndef RT_POINT_H
#define RT_POINT_H

#include "constants.h"

#include <sstream>
#include <vector.h>

#include <cmath>
#include <cassert>
#include <iostream>
#include <string>

namespace rt {

/*
 * Implementation of a 3D point.
 *
 * @author	Thomas Loockx
 * @date	09/2008
 */
class Point
{
public:
	Point();
	Point(float x, float y, float z);

	float x() { return components_[0]; }
	float y() { return components_[1]; }
	float z() { return components_[2]; }
	const float& x() const { return components_[0]; }
	const float& y() const { return components_[1]; }
	const float& z() const { return components_[2]; }

	float distance(const Point& p) const;

	Point& operator+=(const Vector& v);
	Point& operator-=(const Vector& v);
	float& operator()(int index);
	float operator()(int index) const;
	float& operator[](int index);
	float operator[](int index) const;

private:
	float components_[3];
};

Point operator+(const Vector& v, const Point& p);
Point operator+(const Point& p, const Vector& v);
Point operator*(float scalar, const Point& p);
Vector operator-(const Point& a, const Point& b);
bool operator==(const Point& a, const Point& b);
bool operator!=(const Point& a, const Point& b);
bool operator<(const Point& a, const Point& b);
std::ostream& operator<<(std::ostream& output, const Point& p);

} // namespace rt

inline
rt::Point::Point()
{
	components_[0] = 0;
	components_[1] = 0;
	components_[2] = 0;
}

inline
rt::Point::Point(float x, float y, float z)
{
	components_[0] = x;
	components_[1] = y;
	components_[2] = z;
}

inline
float& rt::Point::operator()(int index)
{
	assert (index >= 0 && index <= 2);
	return components_[index];
}

inline
float rt::Point::operator()(int index) const
{
	assert (index >= 0 && index <= 2);
	return components_[index];
}


inline
float& rt::Point::operator[](int index)
{
	assert (index >= 0 && index <= 2);
	return components_[index];
}

inline
float rt::Point::operator[](int index) const
{
	assert (index >= 0 && index <= 2);
	return components_[index];
}

inline
rt::Point rt::operator+(const rt::Vector& v, const rt::Point& p)
{
	return Point(p) += v;
}

inline
rt::Point rt::operator+(const rt::Point& p, const rt::Vector& v)
{
	return Point(p) += v;
}

inline
rt::Point rt::operator*(float scalar, const rt::Point& p)
{
	return Point(p.x() * scalar, p.y() * scalar, p.z() * scalar);
}

inline
bool rt::operator==(const rt::Point& a, const rt::Point& b)
{
	return fabs(a.x() - b.x()) <= rt::EPS && 
		   fabs(a.y() - b.y()) <= rt::EPS && 
		   fabs(a.z() - b.z()) <= rt::EPS;
}

inline
bool rt::operator!=(const rt::Point& a, const rt::Point& b)
{
	return !(a == b);
}

inline
bool rt::operator<(const rt::Point& a, const rt::Point& b)
{
	float norm1 = a.distance(Point());
	float norm2 = b.distance(Point());
	return norm1 < norm2;
}

inline
rt::Vector rt::operator-(const rt::Point& a, const rt::Point& b)
{
	return Vector(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

inline
rt::Point& rt::Point::operator+=(const rt::Vector& v)
{
	components_[0] += v.x();	
	components_[1] += v.y();
	components_[2] += v.z();
	return *this;
}

inline
rt::Point& rt::Point::operator-=(const rt::Vector& v)
{
	components_[0] -= v.x();	
	components_[1] -= v.y();
	components_[2] -= v.z();
	return *this;
}

inline
float rt::Point::distance(const rt::Point& p) const
{
	return ((*this) - p).norm();
}

inline
std::ostream& rt::operator<<(std::ostream& output, const rt::Point& p)
{
	output << "[" << p.x() << ", " << p.y() << ", " << p.z() << "]";
	return output;
}
#endif
