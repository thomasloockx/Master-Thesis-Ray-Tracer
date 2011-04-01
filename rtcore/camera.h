#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include <point.h>
#include <vector.h>
#include <ray.h>
#include <raypacket.h>

#include <cmath>
#include <iostream>

namespace rt {

/*
 * Implementation of a pinhole camera in 3D space.
 *
 * @author	Thomas Loockx
 * @date	09/2008
 */
class Camera
{
public:
	Camera();
	Camera(
		const Point& origin, 
		const Vector& direction, 
		const Vector& up, 
		float hfovy = 30.0F,
		float vfovy = 30.0F,
		float distance = 1.0F,
		int nbHorizontalPixels = 1024,
		int nbVerticalPixels = 1024);
	
	Ray createRay(int i, int j) const;
	void createPacket(int i, int j, RayPacket& packet) const;

	Point origin() const { return origin_; }
	Vector direction() const { return direction_; }
	Vector up() const { return up_; }
	float hfovy() const { return hfovy_; }
	float vfovy() const { return vfovy_; }
	float distance() const { return distance_; }

	Vector u() const { return u_; }
	Vector v() const { return v_; }
	Vector w() const { return w_; }

	int nbHorizontalPixels() const { return nbHorizontalPixels_; }
	int nbVerticalPixels() const { return nbVerticalPixels_; }

	float top() const { return top_; }
	float width() const { return width_; }
	float height() const { return height_; }


private:
	/* Position of the camera in space. */
	Point origin_;	
	/* Direction the camera is looking at. */
	Vector direction_;
	/* Vector that points to the sky in the scene. */
	Vector up_;
	/* Horizontal field of view in degrees. */
	float hfovy_;
	/* Vertical field of view in degrees. */
	float vfovy_;
	/* Distance between the origin and the screen. */
	float distance_;

	/* Camera coordinate system */
	Vector u_;
	Vector v_;
	Vector w_;

	/* pixel count */
	int nbHorizontalPixels_;
	int nbVerticalPixels_;
	
	/* Coordinates of the screen */
	float left_;
	float right_;
	float top_;
	float bottom_;
	float width_;
	float height_;

	/* Optimizations */
	float pixelWidth_;
	float pixelHeight_;

	void initCoordinateSystem();
};

std::ostream& operator<<(std::ostream& output, const Camera& c);

} // namespace rt 

inline
rt::Ray rt::Camera::createRay(int i, int j) const
{
	float us = left_ + pixelWidth_ * (i + 0.5);
	float vs = bottom_ + pixelHeight_ * (j + 0.5);

	// point on the screen
	Point s(origin() + (us * u()) + (vs * v()) + (-1 * distance() * w()));
	
	return Ray(origin(), s - origin());
}

inline
void rt::Camera::createPacket(int i, int j, RayPacket& packet) const
{
	for (int y = 0; y < packet.sqrtNbRays(); ++y) {
		for (int x = 0; x < packet.sqrtNbRays(); ++x) {
			packet.ray(y * packet.sqrtNbRays() + x) = createRay(i + x,j + y);
		}
	}
    packet.calcIAData();
}
#endif
