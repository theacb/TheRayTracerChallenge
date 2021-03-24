#include "pch.h"
#include "Material.h"

// ------------------------------------------------------------------------
//
// Material Base
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

BaseMaterial::BaseMaterial()
{
	this->name = "Default Material 000";
}


BaseMaterial::~BaseMaterial()
{
}

// ------------------------------------------------------------------------
//
// Normal Material
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

NormalsMaterial::NormalsMaterial()
{
	this->name = "Default Normals Material Material 000";
}

NormalsMaterial::~NormalsMaterial()
{
}

Color NormalsMaterial::lighting(std::shared_ptr<Light>, IxComps & comps) const
{
	// returns the normal as a color, mapping from (-1.0, 1.0) to (0.0, 1.0)
	// Assumes a normalized vector
	Tuple n = comps.normal_v - Tuple::Vector(0.0, 0.0, -1.0);
	return Color(n.x + 1.0, n.y + 1.0, n.z + 1.0) * 0.5;
}


// ------------------------------------------------------------------------
//
// Phong Material
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

PhongMaterial::PhongMaterial() : BaseMaterial()
{
	this->name = "Default Phong Material 000";

	this->color = Color(1.0, 1.0, 1.0);

	this->ambient = 0.1;
	this->diffuse = 0.9;
	this->specular = 0.9;
	this->shininess = 200.0;
	this->color_tex = nullptr;
}

PhongMaterial::~PhongMaterial()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

Color PhongMaterial::lighting(std::shared_ptr<Light> lgt, IxComps & comps) const
{
	Color texmap_col, effective_col, ambient_col, diffuse_col, specular_col;
	Tuple light_v, reflect_v;
	double light_dot_normal, reflect_dot_eye, factor;

	Color black = Color(0.0, 0.0, 0.0);

	texmap_col = (this->color_tex != nullptr) ? this->color_tex->sample_at(comps) : this->color;

	// Combine the surface color with the light's color and intensity
	effective_col = texmap_col * lgt->color;
	// Compute ambient contribution
	ambient_col = effective_col * this->ambient;

	// Find the Direction to the light source
	light_v = (lgt->position() - comps.point).normalize();

	// light_dot_normal represents the cosine of the angle 
	// between the light vector and the normal vector. A negative number means 
	// the light is on the other side of the surface.
	light_dot_normal = Tuple::dot(light_v, comps.normal_v);

	if (light_dot_normal < 0.0 || comps.shadow_multiplier < lgt->cutoff)
	{
		diffuse_col = black;
		specular_col = black;
	}
	else
	{
		diffuse_col = effective_col * this->diffuse * light_dot_normal * comps.shadow_multiplier;
		reflect_v = Tuple::reflect(-light_v, comps.normal_v);

		reflect_dot_eye = Tuple::dot(reflect_v, comps.eye_v);

		if (reflect_dot_eye <= 0.0)
		{
			specular_col = black;
		}
		else
		{
			factor = pow(reflect_dot_eye, this->shininess);
			specular_col = lgt->color * this->specular * factor;
		}
	}


	return ambient_col + diffuse_col + specular_col;
}

Color PhongMaterial::lighting(
	std::shared_ptr<Light> lgt, 
	const Tuple & point, 
	const Tuple & eye_v, 
	const Tuple & normal_v
) const
{
	return lighting(
		lgt,
		point,
		eye_v,
		normal_v,
		false
	);
}

Color PhongMaterial::lighting(
	std::shared_ptr<Light> lgt, 
	const Tuple & point, 
	const Tuple & eye_v, 
	const Tuple & normal_v, 
	bool shadowed
) const
{
	IxComps comps = IxComps();
	comps.point = point;
	comps.eye_v = eye_v;
	comps.normal_v = normal_v;
	comps.shadow_multiplier = shadowed ? 0.0 : 1.0;

	return this->lighting(lgt, comps);
}

// ------------------------------------------------------------------------
// Comparison Operators
// ------------------------------------------------------------------------

bool operator==(const PhongMaterial & left_mat, const PhongMaterial & right_mat)
{
	return (
		left_mat.color == right_mat.color &&
		flt_cmp(left_mat.ambient, right_mat.ambient) &&
		flt_cmp(left_mat.diffuse, right_mat.diffuse) &&
		flt_cmp(left_mat.specular, right_mat.specular) &&
		flt_cmp(left_mat.shininess, right_mat.shininess)
		);
}

bool operator!=(const PhongMaterial & left_mat, const PhongMaterial & right_mat)
{
	return !(left_mat == right_mat);
}