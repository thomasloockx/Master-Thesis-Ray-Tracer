#ifndef RT_COLOR_H
#define RT_COLOR_H

#include <string>
#include <iostream>
#include <stdint.h>

namespace rt {

/*
 * Class representing an rgb color.
 *
 * @author	Thomas Loockx
 * @date	09/2008
 */
struct Color
{
	Color(uint8_t r, uint8_t g, uint8_t b);
	Color();

	uint8_t r;
	uint8_t g;
	uint8_t b;
	
	static Color hsvToRgb(float h, float s, float v);

	/* predefined colors */
	static const Color RED;
	static const Color YELLOW;
	static const Color GREEN;
	static const Color CYAN;
	static const Color BLUE;
	static const Color MAGENTA;
	static const Color WHITE;
};

Color operator*(const Color& c, float scalar);
Color operator*(float scalar, const Color& c);
bool operator==(const Color& a, const Color& b);
std::ostream& operator<<(std::ostream& o, const Color& c);

} // namespace rt

#endif
