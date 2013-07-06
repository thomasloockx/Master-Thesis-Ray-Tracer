#ifndef RT_IMAGE_H
#define RT_IMAGE_H

#include <color.h>

namespace rt {

/*
 * Representation of a pixmap.
 *
 * @author	Thomas Loockx
 * @date	10/2008
 */
class Image {

public:
	Image(int width,int height);
	~Image();

	int width() const { return width_; }
	int height() const { return height_; }

	Color& operator()(int i, int j); 		
	Color operator()(int i, int j) const;

	void save(const std::string& name) const;
private:
	const int width_;
	const int height_;
	Color* pixels_;

	// to prevent evil
	Image(const Image& other);
	Image operator=(const Image& other);
};

} // namespace rt

inline
rt::Color& rt::Image::operator()(int i, int j)
{
	return pixels_[i + j * height()];
}

inline
rt::Color rt::Image::operator()(int i, int j) const
{
	return pixels_[i + j * height()];
}

#endif
