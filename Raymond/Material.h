#pragma once

#include "string.h" // Material Names
#include <memory> // Shared Pointers

#include "Tuple.h"
#include "Color.h"
#include "Light.h"

class BaseMaterial;
using MatPtr = BaseMaterial * ;
using ConstMatPtr = const BaseMaterial *;

class BaseMaterial
{
public:
	BaseMaterial();
	~BaseMaterial();

	// Methods
	virtual Color shade(const Light *, const Tuple &, const Tuple &, const Tuple &) const = 0;
	
	// Properties
	std::string name;

};

class NormalsMaterial :
	public BaseMaterial
{
public:
	NormalsMaterial();
	~NormalsMaterial();

	virtual Color shade(const Light *, const Tuple &, const Tuple &, const Tuple &) const override;
};

class PhongMaterial :
	public BaseMaterial
{
public:
	PhongMaterial();
	~PhongMaterial();

	virtual Color shade(const Light *, const Tuple &, const Tuple &, const Tuple &) const override;

	// Properties
	Color color;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

//Overloaded Operators
bool operator==(const PhongMaterial &, const PhongMaterial &);
bool operator!=(const PhongMaterial &, const PhongMaterial &);
