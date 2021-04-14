#ifndef H_RAYMOND_MATERIAL
#define H_RAYMOND_MATERIAL

#include "string.h" // Material Names
#include <memory> // Shared Pointers

#include "Constants.h"
#include "Tuple.h"
#include "Color.h"
#include "Light.h"
#include "IxComps.h"
#include "Texmap.h"

class World;

class BaseMaterial;
using MatPtr = BaseMaterial * ;
using ConstMatPtr = const BaseMaterial *;

class BaseMaterial
{
public:
	BaseMaterial();
	~BaseMaterial();

	// Methods
	virtual Color lighting(const std::shared_ptr<Light>, const IxComps &) const;
	virtual Color reflect(const World & world, const IxComps & comps) const;
	virtual Color refract(const World & world, const IxComps & comps) const;
	virtual Color transmit(const std::shared_ptr<Light> lgt, const World & world, const IxComps & comps, const Intersections & ix) const;
	
	// Properties
	std::string name;
	FloatMapSlot ior;
	bool use_schlick;

};

class NormalsMaterial :
	public BaseMaterial
{
public:
	NormalsMaterial();
	~NormalsMaterial();

	virtual Color lighting(const std::shared_ptr<Light>, const IxComps & comps) const override;
};

class PhongMaterial :
	public BaseMaterial
{
public:
	PhongMaterial();
	~PhongMaterial();

	virtual Color lighting(const std::shared_ptr<Light> lgt, const IxComps & comps) const override;
	Color lighting(const std::shared_ptr<Light> lgt, const Tuple & point, const Tuple & eye_v, const Tuple & normal_v) const;
	Color lighting(const std::shared_ptr<Light> lgt, const Tuple & point, const Tuple & eye_v, const Tuple & normal_v, bool shadowed) const;

	virtual Color reflect(const World & world, const IxComps & comps) const override;
	virtual Color refract(const World & world, const IxComps & comps) const override;
	virtual Color transmit(const std::shared_ptr<Light> lgt, const World & world, const IxComps & comps, const Intersections & ix) const override;


	// Properties
	ColorMapSlot color;
	ColorMapSlot reflection;
	ColorMapSlot refraction;
	ColorMapSlot ambient;

	FloatMapSlot diffuse;
	FloatMapSlot specular;
	FloatMapSlot shininess;
	FloatMapSlot reflection_roughness;
	FloatMapSlot refraction_roughness;

	bool transparent_shadows;

};

// Overloaded Operators
bool operator==(const PhongMaterial & left_mat, const PhongMaterial & right_mat);
bool operator!=(const PhongMaterial & left_mat, const PhongMaterial & right_mat);

// Shading Functions
double schlick(const IxComps & comps);

#endif
