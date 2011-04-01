#include <color.h>
#include <utils.h>

#include <sstream>
#include <assert.h>

rt::Color::Color(uint8_t r, uint8_t g, uint8_t b)
: r(r), g(g), b(b)
{
}

rt::Color::Color() 
: r(240), g(240), b(240)
{
}

rt::Color rt::Color::hsvToRgb(float h, float s, float v)
{
	int hi = (h / 60);
	float f = h/60 - hi;
	float p = v * (1 - s);
	float q = v *(1 - f * s);
	float t = v*(1 - (1 - f) * s);

	float r = 0;
	float g = 0;
	float b = 0;
	switch (hi) {
		case 0:
			r = 1;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = 1;
			b = p;
			break;
		case 2:
			r = p;
			g = 1;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = 1;
			break;
		case 4:
			r = t;
			g = p;
			b = 1;
			break;
		case 5:
			r = 1;
			g = p;
			b = q;
			break;
	}

	return Color(255 * r, 255 * g, 255 * b);
}

rt::Color rt::operator*(const Color& c, float scalar)
{
	assert(scalar >= 0);		
	float r = min<float>(c.r * scalar, 255);
	float g = min<float>(c.g * scalar, 255);
	float b = min<float>(c.b * scalar, 255);
	return Color(r, g, b);
}

rt::Color rt::operator*(float scalar, const Color& c)
{
	return c * scalar;
}

bool rt::operator==(const Color& a, const Color& b)
{
	return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}

std::ostream& rt::operator<<(std::ostream& o, const rt::Color& c)
{
	o << "Color::( " << static_cast<int>(c.r) << ", ";
    o << static_cast<int>(c.g) << " ,"; 
    o << static_cast<int>(c.b) << " )";
    return o;
}

const rt::Color rt::Color::RED = Color(255,0,0);
const rt::Color rt::Color::YELLOW= Color(255,255,0);
const rt::Color rt::Color::GREEN = Color(0,255,0);
const rt::Color rt::Color::CYAN= Color(0,255,255);
const rt::Color rt::Color::BLUE = Color(0,0,255);
const rt::Color rt::Color::MAGENTA= Color(255,0,255);
const rt::Color rt::Color::WHITE = Color(255,255,255);
