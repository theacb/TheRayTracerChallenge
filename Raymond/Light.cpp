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

Light::Light(Color color) : Light(color, 1.0)
{
}

Light::Light(Color color, double multiplier) : ObjectBase()
{
	this->color = color;
	this->multiplier = multiplier;

	this->falloff = false;
	this->cutoff = 0.0001;
}

Light::~Light()
{
}

Tuple Light::position() const
{
	return (this->get_transform()).position();
}

// ------------------------------------------------------------------------
//
// PointLight
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

PointLight::PointLight() : PointLight(Tuple::Point(0.0, 0.0, 0.0), Color(1.0), 1.0)
{
}

PointLight::PointLight(Tuple position, Color color) : PointLight(position, color, 1.0)
{
}

PointLight::PointLight(Tuple position, Color color, double multiplier) : Light(color, multiplier)
{
	this->set_transform(Matrix4::Translation(position));
}

PointLight::~PointLight()
{
}

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
