#include "pch.h"
#include "Material.h"


Material::Material()
{
	this->name = "Default Phong Material 000";

	this->color = Color(1.0f, 1.0f, 1.0f);

	this->ambient = 0.1f;
	this->diffuse = 0.9f;
	this->specular = 0.9f;
	this->shininess = 200.0;
}


Material::~Material()
{
}

bool operator==(const Material & left_mat, const Material & right_mat)
{
	return (
		left_mat.color == right_mat.color &&
		flt_cmp(left_mat.ambient, right_mat.ambient) &&
		flt_cmp(left_mat.diffuse, right_mat.diffuse) &&
		flt_cmp(left_mat.specular, right_mat.specular) &&
		flt_cmp(left_mat.shininess, right_mat.shininess)
		);
}

bool operator!=(const Material & left_mat, const Material & right_mat)
{
	return !(left_mat == right_mat);
}

Color lighting(const Material & mat, const Light * lgt, const Tuple & point, const Tuple & eye_v, const Tuple & normal_v)
{
	Color effective_col, ambient_col, diffuse_col, specular_col;
	Tuple light_v, reflect_v;
	float light_dot_normal, reflect_dot_eye, factor;

	Color black = Color(0.0f, 0.0f, 0.0f);

	// Combine the surface color with the light's color and intensity
	effective_col = mat.color * lgt->color;
	// Compute ambient contribution
	ambient_col = effective_col * mat.ambient;

	// Find the Direction to the light source
	light_v = (lgt->position() - point).normalize();

	// light_dot_normal represents the cosine of the angle 
	// between the light vector and the normal vector. A negative number means 
	// the light is on the other side of the surface.
	light_dot_normal = Tuple::dot(light_v, normal_v);

	if (light_dot_normal < 0.0f)
	{
		diffuse_col = black;
		specular_col = black;
	}
	else
	{
		diffuse_col = effective_col * mat.diffuse * light_dot_normal;
		reflect_v = Tuple::reflect(-light_v, normal_v);

		reflect_dot_eye = Tuple::dot(reflect_v, eye_v);

		if (reflect_dot_eye <= 0.0f)
		{
			specular_col = black;
		}
		else
		{
			factor = pow(reflect_dot_eye, mat.shininess);
			specular_col = lgt->color * mat.specular * factor;
		}
	}
		

	return ambient_col + diffuse_col + specular_col;
}
