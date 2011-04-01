#ifndef RT_VECTOR_H
#define RT_VECTOR_H

#include <constants.h>
#include <utils.h>

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

namespace rt {

/*
 * Implementation of a 3D vector.
 *
 * @author	Thomas Loockx
 * @date	09/2008
 */
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);

	float x() { return component[0]; }
	float y() { return component[1]; }
	float z() { return component[2]; }
	const float& x() const { return component[0]; }
	const float& y() const { return component[1]; }
	const float& z() const { return component[2]; }

	Vector& operator+=(const Vector& v);
	Vector& operator-=(const Vector& v);
	Vector& operator*=(float scalar);
	float& operator()(int index);
	float operator()(int index) const;
	float& operator[](int index);
	float operator[](int index) const;

	float norm() const;
	void normalize();
	unsigned int maxComponentIndex() const;
	unsigned int minComponentIndex() const;
	float maxComponent() const;
	float minComponent() const;

private:
	float component[3];

friend Vector operator+(const Vector& v, const Vector& w);
friend Vector operator-(const Vector& v, const Vector& w);
friend Vector operator*(const Vector& v, const Vector& w);
friend float  operator%(const Vector& v, const Vector& w);
friend Vector operator*(const Vector& v, float scalar);
friend Vector operator*(float scalar, const Vector& v);
friend bool operator==(const Vector& v, const Vector& w);
friend bool operator!=(const Vector& v, const Vector& w);
friend std::ostream& operator<<(std::ostream& output, const Vector& v);
};

Vector operator+(const Vector& v, const Vector& w);
Vector operator-(const Vector& v, const Vector& w);
Vector operator*(const Vector& v, const Vector& w);
float  operator%(const Vector& v, const Vector& w);
Vector operator*(const Vector& v, float scalar);
Vector operator*(float scalar, const Vector& v);
bool operator==(const Vector& v, const Vector& w);
bool operator!=(const Vector& v, const Vector& w);

std::ostream& operator<<(std::ostream& output, const Vector& v);

} // namespace rt

inline
rt::Vector::Vector()
{
}

inline
rt::Vector::Vector(float x, float y, float z)
{
	component[0] = x;
	component[1] = y;
	component[2] = z;
}

inline
rt::Vector& rt::Vector::operator+=(const rt::Vector& v)
{
	component[0] += v.x();
	component[1] += v.y();
	component[2] += v.z();
	return *this;	
}

inline
rt::Vector& rt::Vector::operator-=(const rt::Vector& v)
{
	component[0] -= v.x();
	component[1] -= v.y();
	component[2] -= v.z();
	return *this;
}

inline
rt::Vector& rt::Vector::operator*=(float scalar)
{
	component[0] *= scalar;
	component[1] *= scalar;
	component[2] *= scalar;
	return *this;
}

inline
float& rt::Vector::operator()(int index)
{
	assert (index >= 0 && index <= 2);
	return component[index];
}

inline
float rt::Vector::operator()(int index) const
{
	assert (index >= 0 && index <= 2);
	return component[index];
}

inline
float& rt::Vector::operator[](int index)
{
	assert (index >= 0 && index <= 2);
	return component[index];
}

inline
float rt::Vector::operator[](int index) const
{
	assert (index >= 0 && index <= 2);
	return component[index];
}

inline
float rt::Vector::norm() const
{
	return sqrt(x() * x() + y() * y() + z() * z());
}

inline
void rt::Vector::normalize()
{
	float inv_n = 1 / norm();
	component[0] *= inv_n;
	component[1] *= inv_n;
	component[2] *= inv_n;
}

inline
unsigned int rt::Vector::maxComponentIndex() const
{
	if (x() >= y() && x() >= z())
		return X_AXIS;
	else if (y() >= x() && y() >= z())
		return Y_AXIS;
	else 
		return Z_AXIS;
}

inline
float rt::Vector::maxComponent() const
{
	return std::max(x(), std::max(y(),z()));
}

inline 
unsigned int rt::Vector::minComponentIndex() const
{
	if (x() <= y() && x() <= z())
		return X_AXIS;
	else if (y() <= x() && y() <= z())
		return Y_AXIS;
	else
		return Z_AXIS;
}

inline 
float rt::Vector::minComponent() const
{
	return min<float>(x(),y(),z());
}

inline
rt::Vector rt::operator+(const rt::Vector& v, const rt::Vector& w)
{
	return Vector(v) += w;
}

inline
rt::Vector rt::operator-(const rt::Vector& v, const rt::Vector& w)
{
	return Vector(v) -= w;
}

inline
rt::Vector rt::operator*(const rt::Vector& v, const rt::Vector& w)
{
	return rt::Vector(v.y() * w.z() - v.z() * w.y(),
					  v.z() * w.x() - v.x() * w.z(),
					  v.x() * w.y() - v.y() * w.x());
}

inline
float  rt::operator%(const rt::Vector& v, const rt::Vector& w)
{
	return v.x() * w.x() + v.y() * w.y() + v.z() * w.z();
}

inline
rt::Vector rt::operator*(const rt::Vector& v, float scalar)
{
	return Vector(v) *= scalar;
}

inline
rt::Vector rt::operator*(float scalar, const rt::Vector& v)
{
	return Vector(v) *= scalar;
}

inline
bool rt::operator==(const rt::Vector& a, const rt::Vector& b)
{
	Vector dif = a - b;
	return fabs(dif.x()) <= rt::EPS && 
		   fabs(dif.y()) <= rt::EPS && 
		   fabs(dif.z()) <= rt::EPS;
}

inline
bool rt::operator!=(const rt::Vector& a, const rt::Vector& b)
{
	return !(a == b);
}

inline
std::ostream& rt::operator<<(std::ostream& output, const rt::Vector& v)
{
	output << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]";
	return output;
}

#endif
