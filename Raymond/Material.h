#pragma once

#include "string.h"

#include "Tuple.h"
#include "Color.h"
#include "Light.h"

class Light;

class Material
{
public:
	Material();
	~Material();

	// Properties
	std::string name;
	Color color;
	float ambient;
	float diffuse;
	float specular;
	float shininess;

};

//Overloaded Operators
bool operator==(const Material &, const Material &);
bool operator!=(const Material &, const Material &);

// Shading Function
Color lighting(const Material &, const Light *, const Tuple &, const Tuple &, const Tuple &);
