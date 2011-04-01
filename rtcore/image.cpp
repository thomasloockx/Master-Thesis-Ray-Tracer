#include <image.h>


rt::Image::Image(int width, int height)
	: width_(width), height_(height)
{
	pixels_ = new Color[width_ * height_];		
	img_ = new QImage(width_, height_, QImage::Format_RGB32);
}

rt::Image::~Image()
{
	delete[] pixels_;
	delete img_;
}

void rt::Image::save(const std::string& name) const
{
	for (int y = 0; y < height(); ++y) {
		QRgb* scanline = (QRgb*) img_->scanLine(height() - y - 1);
		for (int x = 0; x < width(); ++x) {
			scanline[x] = qRgb((*this)(x,y).r,(*this)(x,y).g,(*this)(x,y).b);
		}
	}
	img_->save(QString(name.c_str()), "PNG");
}
