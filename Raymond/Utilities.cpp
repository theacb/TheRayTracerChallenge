#include "pch.h"
#include "Utilities.h"

double deg_to_rad(double degrees)
{
	return (degrees * M_PI) / 180.0;
}

double rad_to_deg(double radians)
{
	return (radians * 180.0) / M_PI;
}

std::string pad_num(int num, int pad)
{
	std::string ts = std::to_string(num);
	return std::string(pad - ts.length(), '0') + ts;
}

std::string generate_name(std::string name, std::string folder, int version)
{
	time_t rawtime = time(0);
	struct tm ltm;
	localtime_s(&ltm, &rawtime);

	std::string date_time_string = (
		pad_num(1900 + ltm.tm_year - 2000, 2) +
		pad_num(1 + ltm.tm_mon, 2) +
		pad_num(ltm.tm_mday, 2) +
		"-" +
		pad_num(ltm.tm_hour, 2) +
		pad_num(ltm.tm_min, 2) +
		pad_num(ltm.tm_sec, 2)
		);

	return (
		folder +
		"\\" +
		name +
		"_" +
		pad_num(version, 2) +
		"_" +
		date_time_string +
		".ppm"
		);
}

bool flt_cmp(const double & left_double, const double & right_double)
{
	return fabs(left_double - right_double) < EPSILON;
}
