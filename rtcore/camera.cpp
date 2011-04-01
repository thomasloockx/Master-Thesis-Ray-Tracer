#include <camera.h>
#include <utils.h>

#include <cassert>

rt::Camera::Camera()
{
}

rt::Camera::Camera(
	const Point& origin, 
	const Vector& direction, 
	const Vector& up, 
	float hfovy,
	float vfovy,
	float distance,
	int nbHorizontalPixels,
	int nbVerticalPixels)
: 	origin_(origin), 
	direction_(direction),
	up_(up),
	hfovy_(hfovy),
	vfovy_(vfovy),
	distance_(distance),
	nbHorizontalPixels_(nbHorizontalPixels),
	nbVerticalPixels_(nbVerticalPixels)
{
	assert(hfovy >= 0 && hfovy < 90);
	assert(vfovy >= 0 && vfovy < 90);

	initCoordinateSystem();
	top_ = fabs( tan(toRadians(vfovy)) * distance );
	bottom_ = -top_;

	right_ = fabs( tan(toRadians(hfovy)) * distance );
	left_ = -right_;

	width_ = fabs(right_ - left_);
	height_ = fabs(top_ - bottom_);

	pixelWidth_ = width_ / nbHorizontalPixels_;
	pixelHeight_ = height_ / nbVerticalPixels_;
}

void rt::Camera::initCoordinateSystem()
{
	w_ = -1 * direction_;
	w_.normalize();

	u_ = (up_ * w_);
	u_.normalize();

	v_ = w_ * u_;
}

std::ostream& rt::operator<<(std::ostream& output, const Camera& c)
{
	output << "Camera::(";
	output << "origin = " << c.origin() << ", ";
	output << "direction = " << c.direction() << ", ";
	output << "up = " << c.up() << ", ";
	output << "hfovy = " << c.hfovy() << ", ";
	output << "vfovy = " << c.vfovy() << ", ";
	output << "distance = " << c.distance() << ")";

	return output;
}
