#include "pch.h"
#include "Sample.h"

// ------------------------------------------------------------------------
//
// Sample
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Sample::Sample() : Sample(Tuple::Point(0.0, 0.0, 0.0))
{
}

Sample::Sample(const Tuple origin)
{
	this->name = "default sample";

	this->s_origin_pt_ = origin;

	this->s_rgb_ = Color(0.0);
	this->s_alpha_ = 0.0;
	this->s_background_ = Color(0.0);

	this->s_depth_ = 0.0;
	this->s_normal_ = Color(0.5, 0.5, 1.0);
	this->s_position_ = Color(0.0);

	this->s_diffuse_ = Color(1.0);
	this->s_specular_ = Color(0.0);
	this->s_lighting_ = Color(0.0);
	this->s_globalillumination_ = Color(0.0);

	this->s_reflection_ = Color(0.0);
	this->s_reflectionfilter_ = 1.0;
	this->s_refraction_ = Color(0.0);
	this->s_refractionfilter_ = 1.0;
}

Sample::Sample(const Sample & src)
{
	this->name = src.name;

	this->s_origin_pt_ = src.get_origin();
	this->s_rgb_ = src.get_rgb();
	this->s_alpha_ = src.get_alpha();
	this->s_background_ = src.get_background();

	this->s_depth_ = src.get_depth();
	this->s_normal_ = src.get_normal();
	this->s_position_ = src.get_position();

	this->s_diffuse_ = src.get_diffuse();
	this->s_specular_ = src.get_specular();
	this->s_lighting_ = src.get_lighting();
	this->s_globalillumination_ = src.get_globalillumination();

	this->s_reflection_ = src.get_reflection();
	this->s_reflectionfilter_ = src.get_reflectionfilter();
	this->s_refraction_ = src.get_refraction();
	this->s_refractionfilter_ = src.get_refractionfilter();
}

Sample::~Sample()
{
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

const Color Sample::get_rgb() const
{
	return this->s_rgb_;
}

const Tuple Sample::get_origin() const
{
	return this->s_origin_pt_;
}

const double Sample::get_alpha() const
{
	return this->s_alpha_;
}

const Color Sample::get_background() const
{
	return this->s_background_;
}

const double Sample::get_depth() const
{
	return this->s_depth_;
}

const Color Sample::get_normal() const
{
	return this->s_normal_;
}

const Color Sample::get_position() const
{
	return this->s_position_;
}

const Color Sample::get_diffuse() const
{
	return this->s_diffuse_;
}

const Color Sample::get_specular() const
{
	return this->s_specular_;
}

const Color Sample::get_lighting() const
{
	return this->s_lighting_;
}

const Color Sample::get_globalillumination() const
{
	return this->s_globalillumination_;
}

const Color Sample::get_reflection() const
{
	return this->s_reflection_;
}

const double Sample::get_reflectionfilter() const
{
	return this->s_reflectionfilter_;
}

const Color Sample::get_refraction() const
{
	return this->s_refraction_;
}

const double Sample::get_refractionfilter() const
{
	return this->s_refractionfilter_;
}

void Sample::set_rgb(const Color col)
{
	this->s_rgb_ = col;
}

void Sample::set_alpha(const double val)
{
	this->s_alpha_ = val;
}

void Sample::set_background(const Color col)
{
	this->s_background_ = col;
}

void Sample::set_depth(const double val)
{
	this->s_depth_ = val;
}

void Sample::set_normal(const Color col)
{
	this->s_normal_ = col;
}

void Sample::set_position(const Color col)
{
	this->s_position_ = col;
}

void Sample::set_diffuse(const Color col)
{
	this->s_diffuse_ = col;
}

void Sample::set_specular(const Color col)
{
	this->s_specular_ = col;
}

void Sample::set_lighting(const Color col)
{
	this->s_lighting_ = col;
}

void Sample::set_globalillumination(const Color col)
{
	this->s_globalillumination_ = col;
}

void Sample::set_reflection(const Color col)
{
	this->s_reflection_ = col;
}

void Sample::set_reflectionfilter(const double val)
{
	this->s_reflectionfilter_ = val;
}

void Sample::set_refraction(const Color col)
{
	this->s_refraction_ = col;
}

void Sample::set_refractionfilter(const double val)
{
	this->s_refractionfilter_ = val;
}

void Sample::calculate_sample()
{
	Color rgb = Color(0.0);
	rgb = rgb + (this->s_lighting_ * this->s_diffuse_);
	rgb = rgb + (this->s_globalillumination_ * this->s_diffuse_);
	rgb = rgb + this->s_background_;
	rgb = rgb + (this->s_reflection_ * this->s_reflectionfilter_);
	rgb = rgb + (this->s_refraction_ * this->s_refractionfilter_);
	rgb = rgb + this->s_specular_;
	this->s_rgb_ = rgb;
}

Sample Sample::operator*(const Sample & right_sample) const
{
	Sample result = Sample();

	result.set_rgb(this->s_rgb_ * right_sample.get_rgb());
	result.set_alpha(this->s_alpha_ * right_sample.get_alpha());
	result.set_background(this->s_background_ * right_sample.get_background());

	result.set_depth(this->s_depth_ * right_sample.get_depth());
	result.set_normal(this->s_normal_ * right_sample.get_normal());
	result.set_position(this->s_position_ * right_sample.get_position());

	result.set_diffuse(this->s_diffuse_ * right_sample.get_diffuse());
	result.set_specular(this->s_specular_ * right_sample.get_specular());
	result.set_lighting(this->s_lighting_ * right_sample.get_lighting());
	result.set_globalillumination(this->s_globalillumination_ * right_sample.get_globalillumination());

	result.set_reflection(this->s_reflection_ * right_sample.get_reflection());
	result.set_reflectionfilter(this->s_reflectionfilter_ * right_sample.get_reflectionfilter());
	result.set_refraction(this->s_refraction_ * right_sample.get_refraction());
	result.set_refractionfilter(this->s_refractionfilter_ * right_sample.get_refractionfilter());

	return result;
}

Sample Sample::operator/(const Sample & right_sample) const
{
	Sample result = Sample();

	result.set_rgb(this->s_rgb_ / right_sample.get_rgb());
	result.set_alpha(safe_comp_divide(this->s_alpha_, right_sample.get_alpha()));
	result.set_background(this->s_background_ / right_sample.get_background());

	result.set_depth(safe_comp_divide(this->s_depth_, right_sample.get_depth()));
	result.set_normal(this->s_normal_ / right_sample.get_normal());
	result.set_position(this->s_position_ / right_sample.get_position());

	result.set_diffuse(this->s_diffuse_ / right_sample.get_diffuse());
	result.set_specular(this->s_specular_ / right_sample.get_specular());
	result.set_lighting(this->s_lighting_ / right_sample.get_lighting());
	result.set_globalillumination(this->s_globalillumination_ / right_sample.get_globalillumination());

	result.set_reflection(this->s_reflection_ / right_sample.get_reflection());
	result.set_reflectionfilter(safe_comp_divide(this->s_reflectionfilter_, right_sample.get_reflectionfilter()));
	result.set_refraction(this->s_refraction_ / right_sample.get_refraction());
	result.set_refractionfilter(safe_comp_divide(this->s_refractionfilter_, right_sample.get_refractionfilter()));

	return result;
}

Sample Sample::operator+(const Sample & right_sample) const
{
	Sample result = Sample();

	result.set_rgb(this->s_rgb_ + right_sample.get_rgb());
	result.set_alpha(this->s_alpha_ + right_sample.get_alpha());
	result.set_background(this->s_background_ + right_sample.get_background());

	result.set_depth(this->s_depth_ + right_sample.get_depth());
	result.set_normal(this->s_normal_ + right_sample.get_normal());
	result.set_position(this->s_position_ + right_sample.get_position());

	result.set_diffuse(this->s_diffuse_ + right_sample.get_diffuse());
	result.set_specular(this->s_specular_ + right_sample.get_specular());
	result.set_lighting(this->s_lighting_ + right_sample.get_lighting());
	result.set_globalillumination(this->s_globalillumination_ + right_sample.get_globalillumination());

	result.set_reflection(this->s_reflection_ + right_sample.get_reflection());
	result.set_reflectionfilter(this->s_reflectionfilter_ + right_sample.get_reflectionfilter());
	result.set_refraction(this->s_refraction_ + right_sample.get_refraction());
	result.set_refractionfilter(this->s_refractionfilter_ + right_sample.get_refractionfilter());

	return result;
}

Sample Sample::operator-(const Sample & right_sample) const
{
	Sample result = Sample();

	result.set_rgb(this->s_rgb_ - right_sample.get_rgb());
	result.set_alpha(this->s_alpha_ - right_sample.get_alpha());
	result.set_background(this->s_background_ - right_sample.get_background());

	result.set_depth(this->s_depth_ - right_sample.get_depth());
	result.set_normal(this->s_normal_ - right_sample.get_normal());
	result.set_position(this->s_position_ - right_sample.get_position());

	result.set_diffuse(this->s_diffuse_ - right_sample.get_diffuse());
	result.set_specular(this->s_specular_ - right_sample.get_specular());
	result.set_lighting(this->s_lighting_ - right_sample.get_lighting());
	result.set_globalillumination(this->s_globalillumination_ - right_sample.get_globalillumination());

	result.set_reflection(this->s_reflection_ - right_sample.get_reflection());
	result.set_reflectionfilter(this->s_reflectionfilter_ - right_sample.get_reflectionfilter());
	result.set_refraction(this->s_refraction_ - right_sample.get_refraction());
	result.set_refractionfilter(this->s_refractionfilter_ - right_sample.get_refractionfilter());

	return result;
}

Sample Sample::operator*(const double & scalar) const
{
	Sample result = Sample();

	result.set_rgb(this->s_rgb_ * scalar);
	result.set_alpha(this->s_alpha_ * scalar);
	result.set_background(this->s_background_ * scalar);

	result.set_depth(this->s_depth_ * scalar);
	result.set_normal(this->s_normal_ * scalar);
	result.set_position(this->s_position_ * scalar);

	result.set_diffuse(this->s_diffuse_ * scalar);
	result.set_specular(this->s_specular_ * scalar);
	result.set_lighting(this->s_lighting_ * scalar);
	result.set_globalillumination(this->s_globalillumination_ * scalar);

	result.set_reflection(this->s_reflection_ * scalar);
	result.set_reflectionfilter(this->s_reflectionfilter_ * scalar);
	result.set_refraction(this->s_refraction_ * scalar);
	result.set_refractionfilter(this->s_refractionfilter_ * scalar);

	return result;
}

Sample Sample::operator/(const double & scalar) const
{
	Sample result = Sample();

	result.set_rgb(this->s_rgb_ / scalar);
	result.set_alpha(safe_comp_divide(this->s_alpha_, scalar));
	result.set_background(this->s_background_ / scalar);

	result.set_depth(safe_comp_divide(this->s_depth_, scalar));
	result.set_normal(this->s_normal_ / scalar);
	result.set_position(this->s_position_ / scalar);

	result.set_diffuse(this->s_diffuse_ / scalar);
	result.set_specular(this->s_specular_ / scalar);
	result.set_lighting(this->s_lighting_ / scalar);
	result.set_globalillumination(this->s_globalillumination_ / scalar);

	result.set_reflection(this->s_reflection_ / scalar);
	result.set_reflectionfilter(safe_comp_divide(this->s_reflectionfilter_, scalar));
	result.set_refraction(this->s_refraction_ / scalar);
	result.set_refractionfilter(safe_comp_divide(this->s_refractionfilter_, scalar));

	return result;
}

Sample Sample::operator+(const double & scalar) const
{
	Sample result = Sample();

	result.set_rgb(this->s_rgb_ + scalar);
	result.set_alpha(this->s_alpha_ + scalar);
	result.set_background(this->s_background_ + scalar);

	result.set_depth(this->s_depth_ + scalar);
	result.set_normal(this->s_normal_ + scalar);
	result.set_position(this->s_position_ + scalar);

	result.set_diffuse(this->s_diffuse_ + scalar);
	result.set_specular(this->s_specular_ + scalar);
	result.set_lighting(this->s_lighting_ + scalar);
	result.set_globalillumination(this->s_globalillumination_ + scalar);

	result.set_reflection(this->s_reflection_ + scalar);
	result.set_reflectionfilter(this->s_reflectionfilter_ + scalar);
	result.set_refraction(this->s_refraction_ + scalar);
	result.set_refractionfilter(this->s_refractionfilter_ + scalar);

	return result;
}

Sample Sample::operator-(const double & scalar) const
{
	Sample result = Sample();

	result.set_rgb(this->s_rgb_ - scalar);
	result.set_alpha(this->s_alpha_ - scalar);
	result.set_background(this->s_background_ - scalar);

	result.set_depth(this->s_depth_ - scalar);
	result.set_normal(this->s_normal_ - scalar);
	result.set_position(this->s_position_ - scalar);

	result.set_diffuse(this->s_diffuse_ - scalar);
	result.set_specular(this->s_specular_ - scalar);
	result.set_lighting(this->s_lighting_ - scalar);
	result.set_globalillumination(this->s_globalillumination_ - scalar);

	result.set_reflection(this->s_reflection_ - scalar);
	result.set_reflectionfilter(this->s_reflectionfilter_ - scalar);
	result.set_refraction(this->s_refraction_ - scalar);
	result.set_refractionfilter(this->s_refractionfilter_ - scalar);

	return result;
}

std::ostream & operator<<(std::ostream & os, const Sample & s)
{
	os << "[ Sample: RGB: " << s.get_rgb()
		<< ", Alpha: " << s.get_alpha()
		<< ", Background: " << s.get_background()
		<< ", Depth: " << s.get_depth()
		<< ", Normal: " << s.get_normal()
		<< ", Position: " << s.get_position()
		<< ", Diffuse: " << s.get_diffuse()
		<< ", Specular: " << s.get_specular()
		<< ", Lighting: " << s.get_lighting()
		<< ", GI: " << s.get_globalillumination()
		<< ", Reflection: " << s.get_reflection()
		<< ", Refl Filter: " << s.get_reflectionfilter()
		<< ", Refraction: " << s.get_refraction()
		<< ", Rafr Filter: " << s.get_refractionfilter()
		<< " ]";
	return os;
}