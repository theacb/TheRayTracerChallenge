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

Sample::Sample(const Tuple & canvas_origin)
{
	this->name = "default sample";

	this->CanvasOrigin = canvas_origin;
    Tuple::Point(0.0, 0.0, 0.0);

	this->s_rgb_ = Color(0.0);
	this->Alpha = 0.0;
	this->Background = Color(0.0);

	this->Depth = 0.0;
	this->Normal = Color(0.0);
	this->Position = Color(0.0);
    this->BucketID = 0;

	this->Diffuse = Color(0.0);
	this->Specular = Color(0.0);
	this->Lighting = Color(0.0);
	this->GlobalIllumination = Color(0.0);

	this->Reflection = Color(0.0);
	this->ReflectionFilter = 0.0;
	this->Refraction = Color(0.0);
	this->RefractionFilter = 0.0;

    this->s_calculated_ = false;
}

Sample::Sample(const Sample & src)
{
	this->name = src.name;

	this->CanvasOrigin = src.CanvasOrigin;

	this->s_rgb_ = src.get_current_rgb();
	this->Alpha = src.Alpha;
	this->Background = src.Background;

	this->Depth = src.Depth;
	this->Normal = src.Normal;
	this->Position = src.Position;
    this->BucketID = src.BucketID;

	this->Diffuse = src.Diffuse;
	this->Specular = src.Specular;
	this->Lighting = src.Lighting;
	this->GlobalIllumination = src.GlobalIllumination;

	this->Reflection = src.Reflection;
	this->ReflectionFilter = src.ReflectionFilter;
	this->Refraction = src.Refraction;
	this->RefractionFilter = src.RefractionFilter;

    this->s_calculated_ = src.has_been_calculated();
}

Sample::~Sample()
= default;

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

Color Sample::get_channel(RE channel) const
{
	switch (channel)
	{
	case rgb:
		return this->s_rgb_;
	case alpha:
		return Color(this->Alpha);
	case background:
		return this->Background;
	case depth:
		return Color(this->Depth);
	case normal:
		return this->Normal;
	case position:
		return this->Position;
	case diffuse:
		return this->Diffuse;
	case specular:
		return this->Specular;
	case lighting:
		return this->Lighting;
	case globalillumination:
		return this->GlobalIllumination;
	case reflection:
		return this->Reflection;
	case reflectionfilter:
		return Color(this->ReflectionFilter);
	case refraction:
		return this->Refraction;
	case refractionfilter:
		return Color(this->RefractionFilter);
    case bucketid:
        return Sample::id_to_color(this->BucketID);
	default:
		return this->s_rgb_;
	}
}

Color Sample::get_calculated_rgb()
{
    if (! this->s_calculated_)
    {
        this->calculate_sample();
    }

	return this->s_rgb_;
}

Color Sample::get_current_rgb() const
{
    return this->s_rgb_;
}

bool Sample::has_been_calculated() const
{
    return this->s_calculated_;
}

void Sample::set_rgb(const Color & col)
{
    this->s_calculated_ = true;
	this->s_rgb_ = col;
}

void Sample::calculate_sample()
{
	Color rgb = Color(0.0);
	rgb = rgb + (this->Lighting * this->Diffuse);
	rgb = rgb + (this->GlobalIllumination * this->Diffuse);
	rgb = rgb + this->Background;
	rgb = rgb + (this->Reflection * this->ReflectionFilter);
	rgb = rgb + (this->Refraction * this->RefractionFilter);
	rgb = rgb + this->Specular;
	this->s_rgb_ = rgb;

    this->s_calculated_ = true;
}

Sample Sample::operator*(const Sample & right_sample) const
{
	Sample result = Sample(this->CanvasOrigin);

    result.BucketID = this->BucketID;

	result.set_rgb(this->s_rgb_ * right_sample.get_current_rgb());
	result.Alpha = this->Alpha * right_sample.Alpha;
	result.Background = this->Background * right_sample.Background;

	result.Depth = this->Depth * right_sample.Depth;
	result.Normal = this->Normal * right_sample.Normal;
	result.Position = this->Position * right_sample.Position;

	result.Diffuse = this->Diffuse * right_sample.Diffuse;
	result.Specular = this->Specular * right_sample.Specular;
	result.Lighting = this->Lighting * right_sample.Lighting;
	result.GlobalIllumination = this->GlobalIllumination * right_sample.GlobalIllumination;

	result.Reflection = this->Reflection * right_sample.Reflection;
	result.ReflectionFilter = this->ReflectionFilter * right_sample.ReflectionFilter;
	result.Refraction = this->Refraction * right_sample.Refraction;
	result.RefractionFilter = this->RefractionFilter * right_sample.RefractionFilter;

	return result;
}

Sample Sample::operator/(const Sample & right_sample) const
{
	Sample result = Sample(this->CanvasOrigin);

    result.BucketID = this->BucketID;

	result.set_rgb(this->s_rgb_ / right_sample.get_current_rgb());
	result.Alpha = safe_comp_divide(this->Alpha, right_sample.Alpha);
	result.Background = this->Background / right_sample.Background;

	result.Depth = safe_comp_divide(this->Depth, right_sample.Depth);
	result.Normal = this->Normal / right_sample.Normal;
	result.Position = this->Position / right_sample.Position;

	result.Diffuse = this->Diffuse / right_sample.Diffuse;
	result.Specular = this->Specular / right_sample.Specular;
	result.Lighting = this->Lighting / right_sample.Lighting;
	result.GlobalIllumination = this->GlobalIllumination / right_sample.GlobalIllumination;

	result.Reflection = this->Reflection / right_sample.Reflection;
	result.ReflectionFilter = safe_comp_divide(this->ReflectionFilter, right_sample.ReflectionFilter);
	result.Refraction = this->Refraction / right_sample.Refraction;
	result.RefractionFilter = safe_comp_divide(this->RefractionFilter, right_sample.RefractionFilter);

	return result;
}

Sample Sample::operator+(const Sample & right_sample) const
{
	Sample result = Sample(this->CanvasOrigin);

    result.BucketID = this->BucketID;

	result.set_rgb(this->s_rgb_ + right_sample.get_current_rgb());
	result.Alpha = this->Alpha + right_sample.Alpha;
	result.Background = this->Background + right_sample.Background;

	result.Depth = this->Depth + right_sample.Depth;
	result.Normal = this->Normal + right_sample.Normal;
	result.Position = this->Position + right_sample.Position;

	result.Diffuse = this->Diffuse + right_sample.Diffuse;
	result.Specular = this->Specular + right_sample.Specular;
	result.Lighting = this->Lighting + right_sample.Lighting;
	result.GlobalIllumination = this->GlobalIllumination + right_sample.GlobalIllumination;

	result.Reflection = this->Reflection + right_sample.Reflection;
	result.ReflectionFilter = this->ReflectionFilter + right_sample.ReflectionFilter;
	result.Refraction = this->Refraction + right_sample.Refraction;
	result.RefractionFilter = this->RefractionFilter + right_sample.RefractionFilter;

	return result;
}

Sample Sample::operator-(const Sample & right_sample) const
{
	Sample result = Sample(this->CanvasOrigin);

    result.BucketID = this->BucketID;

	result.set_rgb(this->s_rgb_ - right_sample.get_current_rgb());
	result.Alpha = this->Alpha - right_sample.Alpha;
	result.Background = this->Background - right_sample.Background;

	result.Depth = this->Depth - right_sample.Depth;
	result.Normal = this->Normal - right_sample.Normal;
	result.Position = this->Position - right_sample.Position;

	result.Diffuse = this->Diffuse - right_sample.Diffuse;
	result.Specular = this->Specular - right_sample.Specular;
	result.Lighting = this->Lighting - right_sample.Lighting;
	result.GlobalIllumination = this->GlobalIllumination - right_sample.GlobalIllumination;

	result.Reflection = this->Reflection - right_sample.Reflection;
	result.ReflectionFilter = this->ReflectionFilter - right_sample.ReflectionFilter;
	result.Refraction = this->Refraction - right_sample.Refraction;
	result.RefractionFilter = this->RefractionFilter - right_sample.RefractionFilter;

	return result;
}

Sample Sample::operator*(const double & scalar) const
{
	Sample result = Sample(this->CanvasOrigin);

    result.BucketID = this->BucketID;

	result.set_rgb(this->s_rgb_ * scalar);
	result.Alpha = this->Alpha * scalar;
	result.Background = this->Background * scalar;

	result.Depth = this->Depth * scalar;
	result.Normal = this->Normal * scalar;
	result.Position = this->Position * scalar;

	result.Diffuse = this->Diffuse * scalar;
	result.Specular = this->Specular * scalar;
	result.Lighting = this->Lighting * scalar;
	result.GlobalIllumination = this->GlobalIllumination * scalar;

	result.Reflection = this->Reflection * scalar;
	result.ReflectionFilter = this->ReflectionFilter * scalar;
	result.Refraction = this->Refraction * scalar;
	result.RefractionFilter = this->RefractionFilter * scalar;

	return result;
}

Sample Sample::operator/(const double & scalar) const
{
	Sample result = Sample(this->CanvasOrigin);

    result.BucketID = this->BucketID;

	result.set_rgb(this->s_rgb_ / scalar);
	result.Alpha = safe_comp_divide(this->Alpha, scalar);
	result.Background = this->Background / scalar;

	result.Depth = safe_comp_divide(this->Depth, scalar);
	result.Normal = this->Normal / scalar;
	result.Position = this->Position / scalar;

	result.Diffuse = this->Diffuse / scalar;
	result.Specular = this->Specular / scalar;
	result.Lighting = this->Lighting / scalar;
	result.GlobalIllumination = this->GlobalIllumination / scalar;

	result.Reflection = this->Reflection / scalar;
	result.ReflectionFilter = safe_comp_divide(this->ReflectionFilter, scalar);
	result.Refraction = this->Refraction / scalar;
	result.RefractionFilter = safe_comp_divide(this->RefractionFilter, scalar);

	return result;
}

Sample Sample::operator+(const double & scalar) const
{
	Sample result = Sample(this->CanvasOrigin);

    result.BucketID = this->BucketID;

	result.set_rgb(this->s_rgb_ + scalar);
	result.Alpha = this->Alpha + scalar;
	result.Background = this->Background + scalar;

	result.Depth = this->Depth + scalar;
	result.Normal = this->Normal + scalar;
	result.Position = this->Position + scalar;

	result.Diffuse = this->Diffuse + scalar;
	result.Specular = this->Specular + scalar;
	result.Lighting = this->Lighting + scalar;
	result.GlobalIllumination = this->GlobalIllumination + scalar;

	result.Reflection = this->Reflection + scalar;
	result.ReflectionFilter = this->ReflectionFilter + scalar;
	result.Refraction = this->Refraction + scalar;
	result.RefractionFilter = this->RefractionFilter + scalar;

	return result;
}

Sample Sample::operator-(const double & scalar) const
{
	Sample result = Sample(this->CanvasOrigin);

    result.BucketID = this->BucketID;

	result.set_rgb(this->s_rgb_ - scalar);
	result.Alpha = this->Alpha - scalar;
	result.Background = this->Background - scalar;

	result.Depth = this->Depth - scalar;
	result.Normal = this->Normal - scalar;
	result.Position = this->Position - scalar;

	result.Diffuse = this->Diffuse - scalar;
	result.Specular = this->Specular - scalar;
	result.Lighting = this->Lighting - scalar;
	result.GlobalIllumination = this->GlobalIllumination - scalar;

	result.Reflection = this->Reflection - scalar;
	result.ReflectionFilter = this->ReflectionFilter - scalar;
	result.Refraction = this->Refraction - scalar;
	result.RefractionFilter = this->RefractionFilter - scalar;

	return result;
}

Color Sample::id_to_color(const int &id)
{

    int r = ((int) id & 0x000000FF) >> 0;
    int g = ((int) id & 0x0000FF00) >> 8;
    int b = ((int) id & 0x00FF0000) >> 16;

    Color col = Color(Color8Bit(r, g, b));

    return col;
}

std::ostream & operator<<(std::ostream & os, const Sample & s)
{
	os << "[ Sample: RGB: " << s.get_current_rgb()
		<< ", Alpha: " << s.Alpha
		<< ", Background: " << s.Background
		<< ", Depth: " << s.Depth
		<< ", Normal: " << s.Normal
		<< ", Position: " << s.Position
		<< ", Diffuse: " << s.Diffuse
		<< ", Specular: " << s.Specular
		<< ", Lighting: " << s.Lighting
		<< ", GI: " << s.GlobalIllumination
		<< ", Reflection: " << s.Reflection
		<< ", Refl Filter: " << s.ReflectionFilter
		<< ", Refraction: " << s.Refraction
		<< ", Rafr Filter: " << s.RefractionFilter
        << ", Bucket ID: " << s.BucketID
        << ", Canvas Origin: " << s.CanvasOrigin
		<< " ]";
	return os;
}