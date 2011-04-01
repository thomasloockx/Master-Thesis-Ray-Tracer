#ifndef RT_CAL3DCONVERSIONS_H
#define RT_CAL3DCONVERSIONS_H

#include <matrix.h>
#include <matrix4.h>
#include <point.h>
#include <vector.h>

#include <cal3d.h>

#include <iostream>

namespace rt {

inline
CalVector toCalVector(const Vector& v) {
    return CalVector(v[0], v[1], v[2]);
}

inline
CalVector toCalVector(const Point& p) {
    return CalVector(p[0], p[1], p[2]);
}

inline
Point toPoint(const CalVector& v) {
    return Point(v.x, v.y, v.z);
}

inline
Vector toVector(const CalVector& v)
{
	return Vector(v.x, v.y, v.z);
}

inline 
Matrix toMatrix(const CalMatrix& m) {
	return Matrix(m.dxdx,m.dxdy,m.dxdz,
			      m.dydx,m.dydy,m.dydz,
				  m.dzdx,m.dzdy,m.dzdz);


}
#if 0
inline
CalMatrix toCalMatrix(const Matrix& m) {
    CalMatrix cm;
    cm.dxdx = m(0,0);
    cm.dydx = m(0,1);
    cm.dzdx = m(0,2);

    cm.dxdy = m(1,0);
    cm.dydy = m(1,1);
    cm.dzdy = m(1,2);

    cm.dxdz = m(2,0);
    cm.dydz = m(2,1);
    cm.dzdz = m(2,2);
    return cm;
}
#endif

inline
Matrix4 createMatrix(const CalMatrix& rotation, const CalVector& translation)
{
	return Matrix4(toMatrix(rotation), toVector(translation));
}

inline
std::ostream& operator<<(std::ostream& o, const CalVector& v)
{
    o << "CalVector::(" << v.x << ", " << v.y << ", " << v.z << ")";
    return o;
}

} // namespace rt

#endif
