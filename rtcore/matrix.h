#ifndef RT_MATRIX_H
#define RT_MATRIX_H

#include <vector.h>
#include <point.h>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

namespace rt {

/*
 * 3x3 array implementation.
 *
 * @author	Thomas Loockx
 * @date	10/09/2008
 */
class Matrix
{
public:
	Matrix();
	Matrix(
	float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22);

	float& operator()(int i, int j);	
	float operator()(int i, int j) const;	

	float det() const;
	Vector solve(const Vector& rhs) const;
	Matrix transpose() const;
	Matrix inverse() const;

	Vector getRow(int i) const;
	Vector getColumn(int i) const;
	void setRow(int i, const Vector& t);
	void setColumn(int i, const Vector& t);

private:
    // elements stored per column
    // col1 - col2 - col3
    float elements_[9];


    friend Matrix operator+(const Matrix& m, const Matrix& n);
    friend Matrix operator-(const Matrix& m, const Matrix& n);
	friend Matrix operator*(const Matrix& m, const Matrix& n);
    friend Vector operator*(const Matrix& m, const Vector& v);
    friend Point operator*(const Matrix& m, const Point& p);
    friend Matrix operator*(const Matrix& m, float scalar);
    friend Matrix operator*(float scalar, const Matrix& m);
};

Matrix operator+(const Matrix& m, const Matrix& n);
Matrix operator-(const Matrix& m, const Matrix& n);
Matrix operator*(const Matrix& m, const Matrix& n);
bool operator==(const Matrix& m, const Matrix& n);
bool operator!=(const Matrix& m, const Matrix& n);
Vector operator*(const Matrix& m, const Vector& v);
Point operator*(const Matrix& m, const Point& p);
Matrix operator*(const Matrix& m, float scalar);
Matrix operator*(float scalar, const Matrix& m);

std::ostream& operator<<(std::ostream& o, const Matrix& m);

} // namespace rt

inline
rt::Matrix::Matrix()
{
}

inline
rt::Matrix::Matrix(
	float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22
)
{
    elements_[0] = m00;
    elements_[1] = m10;
    elements_[2] = m20;

    elements_[3] = m01;
    elements_[4] = m11;
    elements_[5] = m21;

    elements_[6] = m02;
    elements_[7] = m12;
    elements_[8] = m22;
}

inline 
float& rt::Matrix::operator()(int i, int j)
{
    return elements_[i + 3 * j];
}

inline 
float rt::Matrix::operator()(int i, int j) const
{
    return elements_[i + 3 * j];
}

inline 
float rt::Matrix::det() const
{
	return (*this)(0,0)*(*this)(1,1)*(*this)(2,2) + (*this)(0,1)*(*this)(1,2)*(*this)(2,0) + (*this)(0,2)*(*this)(1,0)*(*this)(2,1)
		  -(*this)(0,2)*(*this)(1,1)*(*this)(2,0) - (*this)(0,1)*(*this)(1,0)*(*this)(2,2) - (*this)(0,0)*(*this)(1,2)*(*this)(2,1);
}

inline
rt::Vector rt::Matrix::solve(const rt::Vector& rhs) const
{
	Matrix copy = (*this);
	float det = copy.det();
	
	copy.setColumn(0, rhs);
	float x1 = copy.det() / det;	
	copy = (*this);

	copy.setColumn(1, rhs);
	float x2 = copy.det() / det;
	copy = (*this);

	copy.setColumn(2, rhs);
	float x3 = copy.det() / det;

	return Vector(x1, x2, x3);
}

inline
rt::Vector rt::Matrix::getRow(int i) const
{
	return Vector(elements_[i],
                  elements_[i + 3],
                  elements_[i + 6]);
}

inline
rt::Vector rt::Matrix::getColumn(int i) const
{
    return Vector(elements_[3*i],
                  elements_[3*i+1],
                  elements_[3*i+2]);
}

inline
void rt::Matrix::setRow(int i, const rt::Vector& t)
{
	(*this)(i,0) = t.x();
	(*this)(i,1) = t.y();
	(*this)(i,2) = t.z();
}

inline
void rt::Matrix::setColumn(int i, const rt::Vector& t)
{
	(*this)(0,i) = t.x();
	(*this)(1,i) = t.y();	
	(*this)(2,i) = t.z();
}

inline
rt::Matrix rt::Matrix::transpose() const
{
	Matrix trans;
	trans.setColumn(0, getRow(0));
	trans.setColumn(1, getRow(1));
	trans.setColumn(2, getRow(2));
	return trans;
}

inline
rt::Matrix rt::Matrix::inverse() const
{
	assert (det() != 0);

	Matrix inverse;
	inverse(0,0) = (*this)(2,2) * (*this)(1,1) - (*this)(2,1) * (*this)(1,2);
	inverse(0,1) = -((*this)(2,2) * (*this)(0,1) - (*this)(2,1) * (*this)(0,2));
	inverse(0,2) = (*this)(1,2) * (*this)(0,1) - (*this)(1,1) * (*this)(0,2);

	inverse(1,0) = -((*this)(2,2) * (*this)(1,0) - (*this)(2,0) * (*this)(1,2));
	inverse(1,1) = (*this)(2,2) * (*this)(0,0) - (*this)(2,0) * (*this)(0,2);
	inverse(1,2) = -((*this)(1,2) * (*this)(0,0) - (*this)(1,0) * (*this)(0,2));

	inverse(2,0) = (*this)(2,1) * (*this)(1,0) - (*this)(2,0) * (*this)(1,1);
	inverse(2,1) = -((*this)(2,1) * (*this)(0,0) - (*this)(2,0) * (*this)(0,1));
	inverse(2,2) = (*this)(1,1) * (*this)(0,0) - (*this)(1,0) * (*this)(0,1);

	return (1/det() * inverse);
}

inline
rt::Matrix rt::operator+(const rt::Matrix& m, const rt::Matrix& n)
{
	Matrix sum;
    for (int i = 0; i < 9; ++i)
        sum.elements_[i] = m.elements_[i] + n.elements_[i];
	return sum;
}

inline
rt::Matrix rt::operator-(const rt::Matrix& m, const rt::Matrix& n)
{
	Matrix diff;
    for (int i = 0; i < 9; ++i)
        diff.elements_[i] = m.elements_[i] - n.elements_[i];
	return diff;
}

inline
rt::Matrix rt::operator*(const rt::Matrix& m, const rt::Matrix& n)
{
	Matrix product;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			product(i,j) = m.getRow(i) % n.getColumn(j);
	return product;
}

inline
bool rt::operator==(const rt::Matrix& m, const rt::Matrix& n)
{
	return (m.getRow(0) == n.getRow(0)) &&
		   (m.getRow(1) == n.getRow(1)) &&
		   (m.getRow(2) == n.getRow(2));
}

inline
bool rt::operator!=(const rt::Matrix& m, const rt::Matrix& n)
{
	return !(m == n);
}

inline
rt::Vector rt::operator*(const rt::Matrix& m, const rt::Vector& v)
{
	Vector p;	
	p[0] = m.elements_[0]*v[0] + m.elements_[3]*v[1] + m.elements_[6]*v[2];
	p[1] = m.elements_[1]*v[0] + m.elements_[4]*v[1] + m.elements_[7]*v[2];
	p[2] = m.elements_[2]*v[0] + m.elements_[5]*v[1] + m.elements_[8]*v[2];
	return p;
}

inline
rt::Point rt::operator*(const rt::Matrix& m, const rt::Point& p)
{
	Point r;	
	r[0] = m.elements_[0]*p[0] + m.elements_[3]*p[1] + m.elements_[6]*p[2];
	r[1] = m.elements_[1]*p[0] + m.elements_[4]*p[1] + m.elements_[7]*p[2];
	r[2] = m.elements_[2]*p[0] + m.elements_[5]*p[1] + m.elements_[8]*p[2];
	return r;
}

inline
rt::Matrix rt::operator*(const rt::Matrix& m, float scalar)
{
	Matrix scaled = m;
    for (int i = 0; i < 9; ++i)
        scaled.elements_[i] *= scalar;
	return scaled;
}

inline
rt::Matrix rt::operator*(float scalar, const Matrix& m)
{
	return m * scalar;
}

inline
std::ostream& rt::operator<<(std::ostream& o, const Matrix& m)
{
    for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j)	{
			o << std::setw(20) << std::left << m(i,j);	
		}
		o << "\n";
	}	
    return o;
}
#endif
