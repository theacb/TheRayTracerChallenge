#include "pch.h"
#include "Material.h"
#include "World.h"

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

Color NormalsMaterial::lighting(const std::shared_ptr<Light>, const IxComps & comps) const
{
	// returns the normal as a color, mapping from (-1.0, 1.0) to (0.0, 1.0)
	// Assumes a normalized vector
	Tuple n = comps.normal_v - Tuple::Vector(0.0, 0.0, -1.0);
	return Color(n.x + 1.0, n.y + 1.0, n.z + 1.0) * 0.5;
}

Color NormalsMaterial::reflect(const World & world, const IxComps & comps) const
{
	return Color(0.0);
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

	this->color = ColorMapSlot(Color(1.0, 1.0, 1.0));
	this->reflection = ColorMapSlot(Color(0.0));
	this->ambient = ColorMapSlot(0.1);

	this->diffuse = FloatMapSlot(0.9);
	this->specular = FloatMapSlot(0.9);
	this->shininess = FloatMapSlot(200.0);
	this->reflection_roughness = FloatMapSlot(0.0);
}

PhongMaterial::~PhongMaterial()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

Color PhongMaterial::lighting(const std::shared_ptr<Light> lgt, const IxComps & comps) const
{
	Color slt_color, slt_ambient;
	double slt_diffuse, slt_specular, slt_shininess;

	Color effective_col, ambient_col, diffuse_col, specular_col;
	Tuple light_v, reflect_v;
	double light_dot_normal, reflect_dot_eye, factor;

	Color black = Color(0.0, 0.0, 0.0);

	// Sample Textures
	slt_color = this->color.sample_at(comps);
	slt_ambient = this->ambient.sample_at(comps);

	slt_diffuse = this->diffuse.sample_at(comps);
	slt_specular = this->specular.sample_at(comps);
	slt_shininess = this->shininess.sample_at(comps);

	// Combine the surface color with the light's color and intensity
	effective_col = slt_color * lgt->color;
	// Compute ambient contribution
	ambient_col = effective_col * slt_ambient;

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
		diffuse_col = effective_col * slt_diffuse * light_dot_normal * comps.shadow_multiplier;
		reflect_v = Tuple::reflect(-light_v, comps.normal_v);

		reflect_dot_eye = Tuple::dot(reflect_v, comps.eye_v);

		if (reflect_dot_eye <= 0.0)
		{
			specular_col = black;
		}
		else
		{
			factor = pow(reflect_dot_eye, slt_shininess);
			specular_col = lgt->color * slt_specular * factor;
		}
	}


	return ambient_col + diffuse_col + specular_col;
}

Color PhongMaterial::lighting(
	const std::shared_ptr<Light> lgt, 
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
	const std::shared_ptr<Light> lgt, 
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

Color PhongMaterial::reflect(const World & world, const IxComps & comps) const
{
	Color blk = Color(0.0);
	Color slt_reflection = this->reflection.sample_at(comps);

	// Black texture means no reflection
	if (slt_reflection != blk)
	{
		// prevents ray from reflecting infinitely
		if (comps.ray_depth < RAY_DEPTH_LIMIT)
		{
			// Roughness code from Ray Tracing in One Weekend by Peter Shirley
			// https://raytracing.github.io/books/RayTracingInOneWeekend.html#metal/fuzzyreflection
			double slt_refl_rough = this->reflection_roughness.sample_at(comps);
			Ray reflect_ray = Ray(
				comps.over_point, 
				comps.reflect_v + (slt_refl_rough * Tuple::RandomInUnitSphere()), 
				comps.ray_depth + 1
			);
			return world.color_at(reflect_ray) * slt_reflection;
		}
	}

	return blk;
}

// ------------------------------------------------------------------------
// Comparison Operators
// ------------------------------------------------------------------------

bool operator==(const PhongMaterial & left_mat, const PhongMaterial & right_mat)
{
	return (
		left_mat.color == right_mat.color &&
		left_mat.ambient == right_mat.ambient &&
		left_mat.diffuse == right_mat.diffuse &&
		left_mat.specular == right_mat.specular &&
		left_mat.shininess == right_mat.shininess
		);
}

bool operator!=(const PhongMaterial & left_mat, const PhongMaterial & right_mat)
{
	return !(left_mat == right_mat);
}
