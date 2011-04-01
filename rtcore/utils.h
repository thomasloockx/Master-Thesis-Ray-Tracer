#ifndef UTILS_H
#define UTILS_H

#include "constants.h"

#include <string>
#include <iostream>
#include <utility>

#if 0
#include <boost/foreach.hpp>
#define foreach			BOOST_FOREACH
#define reverse_foreach	BOOST_REVERSE_FOREACH
#endif


// A macro to disallow the copy constructor and operator= functions
// // This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)

/*
 * Misc. utility functions and constants that come in handy everywhere.
 *
 * @author	Thomas Loockx
 * @date	09/2008
 */

inline
static float toRadians(float degrees)
{
	return degrees * rt::PI / 180;
}

inline
static float toDegrees(float radians)
{
	return radians * 180 / rt::PI;
}

template <class T> 
inline T max(T a, T b)
{
	return (a > b) ? a : b;
}

template <class T> 
inline T max(T a, T b, T c)
{
	return max(a, max(b, c));
}


template <class T> 
inline T min(T a, T b)
{ 
	return (a < b) ? a : b;
}

template <class T> 
inline T min(T a, T b, T c)
{
	return min(a, min(b,c));
}

#endif
