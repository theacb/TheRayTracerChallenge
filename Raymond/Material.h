#pragma once

#include "string.h" // Material Names
#include <memory> // Shared Pointers

#include "Tuple.h"
#include "Color.h"
#include "Light.h"
#include "IxComps.h"

class BaseMaterial;
using MatPtr = BaseMaterial * ;
using ConstMatPtr = const BaseMaterial *;

class BaseMaterial
{
public:
	BaseMaterial();
	~BaseMaterial();

	// Methods
	virtual Color lighting(std::shared_ptr<Light>, IxComps &) const = 0;
	
	// Properties
	std::string name;

};

class NormalsMaterial :
	public BaseMaterial
{
public:
	NormalsMaterial();
	~NormalsMaterial();

	virtual Color lighting(std::shared_ptr<Light>, IxComps &) const override;
};

class PhongMaterial :
	public BaseMaterial
{
public:
	PhongMaterial();
	~PhongMaterial();

	virtual Color lighting(std::shared_ptr<Light>, IxComps &) const override;
	Color lighting(std::shared_ptr<Light>, const Tuple &, const Tuple &, const Tuple &) const;
	Color lighting(std::shared_ptr<Light>, const Tuple &, const Tuple &, const Tuple &, bool) const;

	// Properties
	Color color;
	double ambient;
	double diffuse;
	double specular;
	double shininess;
};

//Overloaded Operators
bool operator==(const PhongMaterial &, const PhongMaterial &);
bool operator!=(const PhongMaterial &, const PhongMaterial &);
