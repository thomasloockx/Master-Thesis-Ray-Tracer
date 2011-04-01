#ifndef RT_MATRIX4_H
#define RT_MATRIX4_H

#include <matrix.h>
#include <vector.h>
#include <point.h>

#include <iostream>

namespace rt
{

/*
 * 4x4 Matrix implementation consisting of a rotation and
 * a translation.
 * 
 * This class is much more restricted than the matrix class
 * and it's only purpose is to be used in the two level
 * ray tracing scheme.
 *
 * @author	Thomas Loockx
 * @date	april 2009
 */
class Matrix4
{
public:
	Matrix4();
	Matrix4(const Matrix& rotation, const Vector& translation);

	Matrix rotation() const { return rotation_; }	
	Vector translation() const { return translation_; }
	Matrix4 inverse() const;

private:
	Matrix rotation_;
	Vector translation_;
};

Vector operator*(const Matrix4& m, const Vector& v);
Point operator*(const Matrix4& m, const Point& p);
std::ostream& operator<<(std::ostream& o, const Matrix4& m);

} // namespace rt

inline
rt::Matrix4::Matrix4()
: rotation_(Matrix()), translation_(Vector(-13,-43,1010))
{
}

inline
rt::Matrix4::Matrix4(const Matrix& rotation, const Vector& translation)
: rotation_(rotation), translation_(translation)
{
}

inline 
rt::Matrix4 rt::Matrix4::inverse() const 
{
	Matrix rotInv = rotation_.inverse();
	return Matrix4(rotInv, -1 * (rotInv * translation_));
}

inline
rt::Vector rt::operator*(const Matrix4& m, const Vector& v)
{
	return m.rotation() * v;
}

inline
rt::Point rt::operator*(const Matrix4& m, const Point& p)
{
	return m.rotation() * p + m.translation();
}

#endif
