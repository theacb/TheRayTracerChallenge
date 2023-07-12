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
	this->ior = FloatMapSlot(1.0);
	this->use_schlick = true;
}


BaseMaterial::~BaseMaterial()
= default;

Color BaseMaterial::lighting(const std::shared_ptr<Light>, const IxComps &) const
{
	return Color(0.0);
}

Color BaseMaterial::reflect(const World & world, const IxComps & comps) const
{
	return Color(0.0);
}

Color BaseMaterial::refract(const World & world, const IxComps & comps) const
{
	return Color(0.0);
}

Color BaseMaterial::transmit(const std::shared_ptr<Light> lgt, const World & world, const IxComps & comps, const Intersections & ix) const
{
	return Color(0.0);
}

// ------------------------------------------------------------------------
//
// Normal Material
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

NormalsMaterial::NormalsMaterial() : BaseMaterial()
{
	this->name = "Default Normals Material 000";
	this->use_schlick = false;
}

NormalsMaterial::~NormalsMaterial()
= default;

Color NormalsMaterial::lighting(const std::shared_ptr<Light>, const IxComps & comps) const
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
	this->use_schlick = true;

	this->color = ColorMapSlot(Color(1.0, 1.0, 1.0));
	this->reflection = ColorMapSlot(Color(0.0));
	this->refraction = ColorMapSlot(Color(0.0));
	this->ambient = ColorMapSlot(0.1);

	this->diffuse = FloatMapSlot(0.9);
	this->specular = FloatMapSlot(0.9);
	this->shininess = FloatMapSlot(200.0);
	this->reflection_roughness = FloatMapSlot(0.0);
	this->refraction_roughness = FloatMapSlot(0.0);

	this->transparent_shadows = true;
}

PhongMaterial::~PhongMaterial()
= default;

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

	if (light_dot_normal < 0.0 || comps.shadow_multiplier.magnitude() < lgt->cutoff)
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
	comps.shadow_multiplier = shadowed ? Color(0.0) : Color(1.0);

	return this->lighting(lgt, comps);
}

Color PhongMaterial::reflect(const World & world, const IxComps & comps) const
{
	Color blk = Color(0.0);
	Color slt_reflection = this->reflection.sample_at(comps);

	// Black texture means no reflection && prevents ray from reflecting infinitely
	if (slt_reflection != blk && comps.ray_depth < RAY_DEPTH_LIMIT)
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

	return blk;
}

Color PhongMaterial::refract(const World & world, const IxComps & comps) const
{
	Color blk = Color(0.0);
	Color slt_refraction = this->refraction.sample_at(comps);

	// Black texture means no refraction && prevents ray from refracting infinitely
	if (slt_refraction != blk && comps.ray_depth < RAY_DEPTH_LIMIT)
	{
		// Calculations to determine if there is total internal reflection

		// Find the ratio of the first IOR to the second
		// Inverted from the definition of Snell's Law
		double n_ratio = comps.n1 / comps.n2;

		// cos(theta_i) is the same as the dot product of the two vectors
		double cos_i = Tuple::dot(comps.eye_v, comps.normal_v);

		// Find sin(theta_t)^2 via trigonometric identity
		double sin2_t = (n_ratio * n_ratio) * (1 - (cos_i * cos_i));

		// False if Total Internal Reflection applies
		if (sin2_t < 1.0)
		{
			// Refraction

			// Find cos(theta_t) via trigonometric identity
			double cos_t = sqrt(1.0 - sin2_t);

			// Compute the direction of the refracted ray
			Tuple direction = comps.normal_v * (n_ratio * cos_i - cos_t) - comps.eye_v * n_ratio;

			// Create the refracted Ray
			// Roughness code from Ray Tracing in One Weekend by Peter Shirley
			// https://raytracing.github.io/books/RayTracingInOneWeekend.html#metal/fuzzyreflection
			double slt_rafr_rough = this->refraction_roughness.sample_at(comps);
			Ray refract_ray = Ray(
				comps.under_point,
				direction + (slt_rafr_rough * Tuple::RandomInUnitSphere()),
				comps.ray_depth + 1
			);
			// Find the color of the refracted ray
			return world.color_at(refract_ray) * slt_refraction;
		}
	}

	return blk;
}

Color PhongMaterial::transmit(const std::shared_ptr<Light> lgt, const World & world, const IxComps & comps, const Intersections & ix) const
{
	Color transmittance = this->refraction.sample_at(comps);

	if (this->transparent_shadows && transmittance.magnitude() > Color(lgt->cutoff).magnitude() && comps.ray_depth < RAY_DEPTH_LIMIT / 2)
	{
		// light that is reflected casts shadows
		double cos_i = Tuple::dot(comps.eye_v, comps.normal_v);
		
		if (cos_i > 0.001)
		{
			// Copy Transmittance
			Color result = Color(transmittance);
			double linear_attenuation = 0.0;

			int hit_index = ix.get_hit_index();

			if (hit_index + 1 < ix.size())
			{
				Intersection next_ix = ix[hit_index + 1];

				double light_distance = (comps.point - lgt->position()).magnitude();
				double distance = next_ix.t_value - comps.t_value;

				if (light_distance > distance)
				{
					result = result * world.shadowed(lgt, comps.under_point, comps.ray_depth);
				}

				if (comps.n2 > 1.0 + EPSILON)
				{
					// Clip linear attenuation to [0,1];
					linear_attenuation = clip(1.0 / (fmin(distance, light_distance) * comps.n2 * 2.0), 0.0, 1.0);
					result = result * linear_attenuation;
				}
			}
			result = result * cos_i;

			return result;			
		}
	}
	return Color(0.0);
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

// ------------------------------------------------------------------------
//
// Helpful Shading Functions
//
// ------------------------------------------------------------------------

double schlick(const IxComps & comps)
{
	double cos, cos_t, n, sin2_t, r0, x, y;
	// find the cosine of the angle between the eye and normal vectors
	cos = Tuple::dot(comps.eye_v, comps.normal_v);

	// Total Internal reflection occurs if n1 > n2
	if (comps.n1 > comps.n2)
	{
		n = comps.n1 / comps.n2;
		sin2_t = (n * n) * (1.0 - (cos * cos));

		if (sin2_t > 1.0)
		{
			return 1.0;
		}

		// Compute cosine of theta using the trig identity
		cos_t = sqrt(1.0 - sin2_t);

		// When n1 > n2, use cos(theta_t) instead
		cos = cos_t;
	}

	// Store as variable to square
	x = ((comps.n1 - comps.n2) / (comps.n1 + comps.n2));

	r0 = x * x;

	// Store as variable to 5th power
	y = (1.0 - cos);

	return r0 + (1.0 - r0) * (y * y * y * y * y);
}
