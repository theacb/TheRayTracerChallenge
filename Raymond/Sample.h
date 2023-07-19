#ifndef H_RAYMOND_SAMPLE
#define H_RAYMOND_SAMPLE

#include "Color.h"
#include "Tuple.h"

enum RE { 
	rgb, alpha, background, depth, normal, position, diffuse, specular, lighting, 
	globalillumination, reflection, reflectionfilter, refraction, refractionfilter,
    bucketid
};

class Sample
{
public:
	Sample();
	explicit Sample(const Tuple& canvas_origin);
	Sample(const Sample & src);

	~Sample();

    [[nodiscard]] static Color id_to_color(const int & id);

	// Properties
	std::string name;

	// Accessors
	[[nodiscard]] Color get_channel(RE channel) const;
	[[nodiscard]] Color get_calculated_rgb();
    [[nodiscard]] Color get_current_rgb() const;
    [[nodiscard]] bool has_been_calculated() const;

	void set_rgb(const Color & col);

	void calculate_sample();

	Sample operator*(const Sample & right_sample) const;
	Sample operator/(const Sample & right_sample) const;
	Sample operator+(const Sample & right_sample) const;
	Sample operator-(const Sample & right_sample) const;

	Sample operator*(const double & scalar) const;
	Sample operator/(const double & scalar) const;
	Sample operator+(const double & scalar) const;
	Sample operator-(const double & scalar) const;

    double Alpha;
    Color Background;

    double Depth;
    Color Normal;
    Color Position;

    int BucketID;

    Color Diffuse;
    Color Specular;
    Color Lighting;
    Color GlobalIllumination;

    Color Reflection;
    double ReflectionFilter;
    Color Refraction;
    double RefractionFilter;

    Tuple CanvasOrigin;

private:
	// Properties
	Color s_rgb_;
    bool s_calculated_;

};

// Overloaded Operators
std::ostream & operator<<(std::ostream & os, const Sample & s);

#endif