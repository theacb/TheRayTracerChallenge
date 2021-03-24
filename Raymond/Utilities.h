#ifndef H_RAYMOND_UTILITIES
#define H_RAYMOND_UTILITIES

#include "math.h"
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>

// Conversions

double deg_to_rad(double degrees);

double rad_to_deg(double radians);

// Files

std::string pad_num(int num, int pad);

std::string generate_name(std::string name, std::string folder, int version);

// Templates

template<typename T>
inline T clip(const T & n, const T & lower, const T & upper)
{
	return std::max(lower, std::min(n, upper));
}

template<typename T>
inline T lerp(const double & t, const T & start, const T& end)
{
	return start * (1 - t) + end * t;
}

template<typename T>
inline T remap(const T & value, const T & in_min, const T & in_max, const T & out_min, const T & out_max)
{
	return out_min + (out_max - out_min) * ((value - in_min) / (in_max - in_min));
}

#endif
