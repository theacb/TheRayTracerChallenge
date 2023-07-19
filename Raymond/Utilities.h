#ifndef H_RAYMOND_UTILITIES
#define H_RAYMOND_UTILITIES

#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <cmath>

#include "Constants.h"

// Conversions

double deg_to_rad(double degrees);

double rad_to_deg(double radians);

// Random Number Utilities
// Taken from Ray Tracing in One Weekend by Peter Shirley
// https://raytracing.github.io/books/RayTracingInOneWeekend.html#antialiasing/somerandomnumberutilities
double random_double();

double random_double(double min, double max);

// Files

std::string pad_num(int num, int pad);

std::string generate_name(const std::string& name, const std::string& folder, int version, const std::string& suffix, std::chrono::duration<double, std::nano> dur);

// Display

std::ostream& clock_display(std::ostream& os, std::chrono::duration<double, std::nano> dur);

// Comparison

bool flt_cmp(const double & left_double, const double & right_double);

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

// Filters

double SincFunction(double x);

double LanczosFunction(double x, double a);

#endif
