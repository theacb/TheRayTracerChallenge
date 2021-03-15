#ifndef H_RAYMOND_UTILITIES
#define H_RAYMOND_UTILITIES

#include "math.h"
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>

double deg_to_rad(double degrees);

double rad_to_deg(double radians);

std::string pad_num(int num, int pad);

std::string generate_name(std::string name, std::string folder, int version);

#endif
