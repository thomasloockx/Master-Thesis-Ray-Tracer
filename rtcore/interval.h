#ifndef RT_INTERVAL_H
#define RT_INTERVAL_H 

#include <algorithm>
#include <cfloat>
#include <ostream>

namespace rt {

/*
 * Implementation of an interval. Assumes that left <= right.
 *
 * @author	floathomas Loockx
 * @date	10/2009
 */
class Interval
{
public:
	Interval();	
	Interval(float left, float right);	
	
	float left() const { return left_; }
	float right() const { return right_; }
	void setLeft(const float left) { left_ = left; }
	void setRight(const float right) { right_ = right; }	
	
	bool overlap(const Interval& other) const;	
	bool noOverlap(const Interval& other) const;	
    Interval negate() const;
    Interval reciproce() const;

private:
	float left_;
	float right_;
};

bool operator==(const Interval& lhs, const Interval& rhs);
bool operator!=(const Interval& lhs, const Interval& rhs);
Interval operator+(const Interval& i0, const Interval& i1);
Interval operator-(const Interval& i0, const Interval& i1);
Interval operator*(const Interval& i0, const Interval& i1);
Interval operator||(const Interval& i0, const Interval& i1);
Interval operator&&(const Interval& i0, const Interval& i1);
std::ostream& operator<<(std::ostream& o, const Interval& i);

} // namespace rt

inline
rt::Interval::Interval()
: left_(FLT_MAX), right_(-FLT_MAX)
{
}

inline
rt::Interval::Interval(float left, float right)
 : left_(left), right_(right)
{
}

inline
rt::Interval rt::Interval::negate() const
{
    return Interval(-right(), -left());
}

inline
rt::Interval rt::Interval::reciproce() const
{
    if (left() > 0) 
        return Interval(1 / right(), 1 / left());
    return Interval(-FLT_MAX, FLT_MAX);
}

/******************************************************************************
 * INTERVAL ARITHMETIC
 ******************************************************************************/

inline
bool rt::Interval::overlap(const rt::Interval& other) const
{
	return !(left() > other.right() || right() < other.left());
}

inline
bool rt::Interval::noOverlap(const rt::Interval& other) const
{
	return left() > other.right() || right() < other.left();
}

inline
bool rt::operator==(const rt::Interval& lhs, const rt::Interval& rhs)
{
	return lhs.left() == rhs.left() && lhs.right() == rhs.right();
}

inline
bool rt::operator!=(const rt::Interval& lhs, const rt::Interval& rhs)
{
	return !(lhs == rhs);
}

inline
rt::Interval rt::operator+(const Interval& i0, const Interval& i1)
{
    return Interval(i0.left() + i1.left(), i0.right() + i1.right());
}

inline
rt::Interval rt::operator-(const Interval& i0, const Interval& i1)
{
    return i0 + i1.negate();
}

inline
rt::Interval rt::operator*(const Interval& i0, const Interval& i1)
{
    // perfect for sse    
    float x0 = i0.left()  * i1.left();
    float x1 = i0.right() * i1.left();
    float x2 = i0.left()  * i1.right();
    float x3 = i0.right() * i1.right();

    float l = std::min(std::min(x0, x1), std::min(x2, x3));
    float r = std::max(std::max(x0, x1), std::max(x2, x3));
    return Interval(l,r);
}

inline
rt::Interval rt::operator||(const Interval& i0, const Interval& i1)
{
    return Interval(std::min(i0.left(), i1.left()), 
                    std::max(i0.right(), i1.right()));
}

inline
rt::Interval rt::operator&&(const Interval& i0, const Interval& i1)
{
    return Interval(std::max(i0.left(), i1.left()), 
                    std::min(i0.right(), i1.right()));
}

inline
std::ostream& rt::operator<<(std::ostream& o, const Interval& i)
{
    o << "[" << i.left() << "," << i.right() << "]";
    return o;
}

#endif
