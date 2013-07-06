// system includes
#include <cstdio>
// library includes
#include <FreeImage.h>
// myself
#include <image.h>


rt::Image::Image(int width, int height)
	: width_(width), height_(height)
{
	pixels_ = new Color[width_ * height_];		
}


rt::Image::~Image()
{
	delete[] pixels_;
}


void rt::Image::save(
    const std::string& name) const
{
    FIBITMAP *bitmap = FreeImage_Allocate(width_, height_, 24);
    
	for (int y = 0; y < height(); ++y) {
		for (int x = 0; x < width(); ++x) {
            const Color c = (*this)(x,y);
            RGBQUAD quad;
            quad.rgbRed   = c.r;
            quad.rgbGreen = c.g;
            quad.rgbBlue  = c.b;
            FreeImage_SetPixelColor(bitmap, x, y, &quad);
		}
	}

    if (FreeImage_Save(FIF_PNG, bitmap, name.c_str()) == false)
    {
        printf("Failed to save image\n");
    }
}
