#include "Utilities.h"

double deg_to_rad(double degrees)
{
	return (degrees * M_PI) / 180.0;
}

double rad_to_deg(double radians)
{
	return (radians * 180.0) / M_PI;
}

double random_double()
{
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0); // NOLINT(cert-msc50-cpp)
}

double random_double(double min, double max)
{
	// Returns a random real in [min,max).
	return min + (max - min)*random_double();
}

std::string pad_num(int num, int pad)
{
	std::string ts = std::to_string(num);
	return std::string(pad - ts.length(), '0') + ts;
}

std::string generate_name(const std::string& name, const std::string& folder, int version, const std::string& suffix, std::chrono::duration<double, std::nano> dur)
{
	time_t rawtime = time(0);
	struct tm ltm{};
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

    std::ostringstream dur_oss;
    char fill = dur_oss.fill();
    dur_oss.fill('0');
    auto h = std::chrono::duration_cast<std::chrono::hours>(dur);
    dur -= h;
    auto m = std::chrono::duration_cast<std::chrono::minutes>(dur);
    dur -= m;
    auto s = std::chrono::duration_cast<std::chrono::seconds>(dur);
    dur -= s;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);

    dur_oss << std::setw(2) << h.count() << "."
            << std::setw(2) << m.count() << "."
            << std::setw(2) << s.count() << ".";
    dur_oss.fill(fill);

    dur_oss << ms.count();

	return (
            folder +
            "\\" +
            date_time_string +
            "__" +
            dur_oss.str() +
            "_" +
		name +
		"_" +
		pad_num(version, 2) +
        suffix +
		".ppm"
		);
}

// Based on an answer on Stack Overflow by Howard Hinnant
// https://stackoverflow.com/a/22593250/15062519
std::ostream& clock_display(std::ostream& os, std::chrono::duration<double, std::nano> dur)
{
	char fill = os.fill();
	os.fill('0');

	auto h = std::chrono::duration_cast<std::chrono::hours>(dur);
	dur -= h;
	auto m = std::chrono::duration_cast<std::chrono::minutes>(dur);
	dur -= m;
	auto s = std::chrono::duration_cast<std::chrono::seconds>(dur);
	dur -= s;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);

	os << std::setw(2) << h.count() << "h:"
		<< std::setw(2) << m.count() << "m:"
		<< std::setw(2) << s.count() << "s:";
	os.fill(fill);

	os << ms.count() << "ms";

	return os;
}

bool flt_cmp(const double & left_double, const double & right_double)
{
	return fabs(left_double - right_double) < EPSILON;
}

double SincFunction(double x)
{
    // Absolute value
    x = fabs(x);
    // If 0, return 0
    if (flt_cmp(x, 0.0))
        return 0.0;

    // sin of x divided by x
    return (sin(M_PI * x)) / M_PI * x;
}

double LanczosFunction(double x, double a)
{
    // a = filter size
    if ((-1 * a) < x && x < a)
        return SincFunction(M_PI * x) * SincFunction((M_PI * x) / a);
    else
        return 0.0;
}
