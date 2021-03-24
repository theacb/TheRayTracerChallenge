#ifndef H_RAYMOND_MATERIAL
#define H_RAYMOND_MATERIAL

#include "string.h" // Material Names
#include <memory> // Shared Pointers

#include "Tuple.h"
#include "Color.h"
#include "Light.h"
#include "IxComps.h"
#include "Texmap.h"

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

	virtual Color lighting(std::shared_ptr<Light>, IxComps & comps) const override;
};

class PhongMaterial :
	public BaseMaterial
{
public:
	PhongMaterial();
	~PhongMaterial();

	virtual Color lighting(std::shared_ptr<Light> lgt, IxComps & comps) const override;
	Color lighting(std::shared_ptr<Light> lgt, const Tuple & point, const Tuple & eye_v, const Tuple & normal_v) const;
	Color lighting(std::shared_ptr<Light> lgt, const Tuple & point, const Tuple & eye_v, const Tuple & normal_v, bool shadowed) const;

	// Properties
	Color color;
	double ambient;
	double diffuse;
	double specular;
	double shininess;
	std::shared_ptr<TexMap> color_tex;
};

//Overloaded Operators
bool operator==(const PhongMaterial & left_mat, const PhongMaterial & right_mat);
bool operator!=(const PhongMaterial & left_mat, const PhongMaterial & right_mat);

#endif
