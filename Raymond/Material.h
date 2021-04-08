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
	virtual Color lighting(const std::shared_ptr<Light>, const IxComps &) const = 0;
	virtual Color reflect(const World & world, const IxComps & comps) const = 0;
	
	// Properties
	std::string name;

};

class NormalsMaterial :
	public BaseMaterial
{
public:
	NormalsMaterial();
	~NormalsMaterial();

	virtual Color lighting(const std::shared_ptr<Light>, const IxComps & comps) const override;

	virtual Color reflect(const World & world, const IxComps & comps) const override;
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


	// Properties
	ColorMapSlot color;
	ColorMapSlot reflection;
	ColorMapSlot ambient;
	FloatMapSlot diffuse;
	FloatMapSlot specular;
	FloatMapSlot shininess;
	FloatMapSlot reflection_roughness;
};

//Overloaded Operators
bool operator==(const PhongMaterial & left_mat, const PhongMaterial & right_mat);
bool operator!=(const PhongMaterial & left_mat, const PhongMaterial & right_mat);

#endif
