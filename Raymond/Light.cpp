#include "pch.h"
#include "Light.h"

// ------------------------------------------------------------------------
//
// Light
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Light::Light() : Light(Color(1.0, 1.0, 1.0), 1.0)
{
}

Light::Light(const Color& color) : Light(color, 1.0)
{
}

Light::Light(const Color& color, double multiplier) : ObjectBase()
{
	this->color = color;
	this->multiplier = multiplier;

	this->falloff = false;
	this->cutoff = 0.0001;
}

Light::~Light()
= default;

Tuple Light::position() const
{
	return (this->get_transform()).position();
}

Tuple Light::area_position() const
{
    return this->position();
}

// ------------------------------------------------------------------------
//
// PointLight
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

PointLight::PointLight() : PointLight(Tuple::Point(0.0, 0.0, 0.0), Color(1.0), 1.0, 0.013)
{
}

PointLight::PointLight(const Tuple& position, const Color& color) : PointLight(position, color, 1.0, 0.013)
{
}

PointLight::PointLight(const Tuple& position, const Color& color, double multiplier) : PointLight(position, color, multiplier, 0.013)
{
	this->set_transform(Matrix4::Translation(position));
}

PointLight::PointLight(const Tuple& position, const Color& color, double multiplier, double radius) : Light(color, multiplier)
{
    this->radius = radius;
    this->set_transform(Matrix4::Translation(position));
}

PointLight::~PointLight()
= default;

bool operator==(const PointLight & left_light, const PointLight & right_light)
{
	return (
		left_light.color == right_light.color &&
		left_light.position() == right_light.position()
		);
}

bool operator!=(const PointLight & left_light, const PointLight & right_light)
{
	return !(left_light == right_light);
}

Tuple PointLight::area_position() const
{
    Tuple area_point = Tuple::RandomInUnitSphere() * this->radius;
    return (this->get_transform()).position() + area_point;
}
