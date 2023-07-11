#ifndef H_RAYMOND_SAMPLE
#define H_RAYMOND_SAMPLE

#include "Color.h"
#include "Tuple.h"

enum RE { 
	rgb, alpha, background, depth, normal, position, diffuse, specular, lighting, 
	globalillumination, reflection, reflectionfilter, refraction, refractionfilter
};

class Sample
{
public:
	Sample();
	explicit Sample(const Tuple& canvas_origin);
	Sample(const Sample & src);

	~Sample();

	// Properties
	std::string name;

	// Getters
	[[nodiscard]] Color get_channel(RE channel) const;

	[[nodiscard]] Color get_rgb() const;
	[[nodiscard]] Tuple get_canvas_origin() const;
    [[nodiscard]] Tuple get_world_origin() const;

	[[nodiscard]] double get_alpha() const;
	[[nodiscard]] Color get_background() const;
	[[nodiscard]] double get_depth() const;
	[[nodiscard]] Color get_normal() const;
	[[nodiscard]] Color get_position() const;
	[[nodiscard]] Color get_diffuse() const;
	[[nodiscard]] Color get_specular() const;
	[[nodiscard]] Color get_lighting() const;
	[[nodiscard]] Color get_globalillumination() const;
	[[nodiscard]] Color get_reflection() const;
	[[nodiscard]] double get_reflectionfilter() const;
	[[nodiscard]] Color get_refraction() const;
	[[nodiscard]] double get_refractionfilter() const;

	// Setters

	void set_rgb(const Color & col);
    void set_world_origin(const Tuple & pt);
    void set_canvas_origin(const Tuple & pt);

	void set_alpha(const double & val);
	void set_background(const Color & col);
	void set_depth(const double & val);
	void set_normal(const Color & col);
	void set_position(const Color & col);
	void set_diffuse(const Color & col);
	void set_specular(const Color & col);
	void set_lighting(const Color & col);
	void set_globalillumination(const Color & col);
	void set_reflection(const Color & col);
	void set_reflectionfilter(const double & val);
	void set_refraction(const Color & col);
	void set_refractionfilter(const double & val);

	void calculate_sample();

	Sample operator*(const Sample & right_sample) const;
	Sample operator/(const Sample & right_sample) const;
	Sample operator+(const Sample & right_sample) const;
	Sample operator-(const Sample & right_sample) const;

	Sample operator*(const double & scalar) const;
	Sample operator/(const double & scalar) const;
	Sample operator+(const double & scalar) const;
	Sample operator-(const double & scalar) const;

private:
	// Properties
	Color s_rgb_;
	double s_alpha_;
	Color s_background_;

	double s_depth_;
	Color s_normal_;
	Color s_position_;

	Color s_diffuse_;
	Color s_specular_;
	Color s_lighting_;
	Color s_globalillumination_;

	Color s_reflection_;
	double s_reflectionfilter_;
	Color s_refraction_;
	double s_refractionfilter_;

	Tuple s_world_origin_pt_;
    Tuple s_canvas_origin_pt_;
};

// Overloaded Operators
std::ostream & operator<<(std::ostream & os, const Sample & s);

#endif