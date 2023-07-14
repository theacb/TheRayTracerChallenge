#include "World.h"

// ------------------------------------------------------------------------
//
// World
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

World::World()
{
	this->background = std::make_shared<Background>();

    // Render Settings
    this->sample_min = 1;
    this->sample_max = 4;
    this->bucket_size = 32;

    this->shadow_subdivs = 4;
}

World::~World()
= default;

// Factory

World World::Default()
{
	World def = World();

	auto light = std::make_shared<PointLight>(Tuple::Point(-10.0, 10.0, -10.0), Color(1.0));

	def.add_object(light);

	auto s1 = std::make_shared<Sphere>();
	auto ms1 = std::static_pointer_cast<PhongMaterial>(s1->material);
	ms1->color = Color(0.8, 1.0, 0.6);
	ms1->diffuse = 0.7;
	ms1->specular = 0.2;

	def.add_object(s1);

	auto s2 = std::make_shared<Sphere>();
	s2->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));

	def.add_object(s2);

	return def;
}

// ------------------------------------------------------------------------
// Intersector
// ------------------------------------------------------------------------

Intersections World::intersect_world(const Ray & r) const
{
	Intersections result;

    // TODO: change this to first intersect bounding boxes. Then, later, it should intersect the BVH.

	for (const std::shared_ptr<PrimitiveBase> & obj: this->w_primitives_)
	{
		// generates an intersection for each object in the scene
		Intersections obj_xs = obj->intersect_i(r);

		// Then concatenates them into a single vector
		for (Intersection& ix : obj_xs)
		{
			// Filter bad values
			if (ix.is_valid() && ix.t_value > -0.0)
			{
				result.push_back(ix);
			}
		}
	}

	// Sorting is required to find the hit
	std::sort(result.begin(), result.end());
	result.calculate_hit();

	return result;
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Sample World::shade(IxComps & comps) const
{
    // TODO: convert this function to return a sample
    // TODO: Break up lighting into Diffuse, Specular, and Lighting
    // TODO: Determine Depth, Position, Normal, and Alpha
    Sample sample = Sample();
    sample.Position = Color(comps.point);
    sample.Normal = Color(comps.normal_v);
    sample.Depth = comps.ray_depth;

    sample.Diffuse = Color(1.0);
    sample.Alpha = 1.0;
    Color smp_lighting = Color(0.0);

	auto obj_prim = std::static_pointer_cast<PrimitiveBase>(comps.object);

	for (std::shared_ptr<Light> lgt : this->w_lights_) // NOLINT(performance-for-range-copy)
	{
        // Each Sample can have multiple shadow subdivs
        Color shadow_average = Color(0.0);
        for (int i = 0; i < this->shadow_subdivs; ++i)
        {
            shadow_average = shadow_average + this->shadowed(lgt, comps.over_point, comps.ray_depth);
        }
		comps.shadow_multiplier = (shadow_average / double(this->shadow_subdivs));

		if (lgt->falloff)
		{
			// Calculate quadratic intensity using formula I = 1/d^2
			double distance = Tuple::distance(lgt->position(), comps.point);
			double quad_multiplier = 1.0 / (distance * distance);
			double intensity = lgt->color.magnitude() * quad_multiplier * lgt->multiplier;
			
			// If value is less than cutoff value, do not calculate sample
			if (intensity > lgt->cutoff)
			{
                smp_lighting = smp_lighting + (obj_prim->material->lighting(lgt, comps) * intensity);
			}
		}
		else
		{
            smp_lighting = smp_lighting + obj_prim->material->lighting(lgt, comps);
		}
	}

    sample.Lighting = smp_lighting;

	// Reflection
	Color refl = (obj_prim->material->reflect(*this, comps));

	// Refraction
	Color rafr =  (obj_prim->material->refract(*this, comps));

	// Use Schlick approximation Effect
	if (obj_prim->material->use_schlick)
	{
		double reflectance = schlick(comps);

        sample.ReflectionFilter = reflectance;
        sample.RefractionFilter = 1.0 - reflectance;
	}
	else
	{
        sample.ReflectionFilter = 1.0;
        sample.RefractionFilter = 1.0;
	}

    sample.Reflection = refl;
    sample.Refraction = rafr;

	return sample;
}

Color World::color_at(const Ray & ray) const
{
    Sample sample = this->sample_at(ray);
    sample.calculate_sample();

	return sample.get_calculated_rgb();
}

Sample World::sample_at(const Ray &ray) const
{
    Intersections xs = this->intersect_world(ray);
    if (!xs.empty())
    {
        Intersection hit = xs.hit();
        IxComps comps = IxComps(hit, ray, xs);

        return this->shade(comps);
    }
    else
    {
        IxComps comps = IxComps::Background(ray);

        return this->background->sample_at(comps);
    }
}

bool World::is_shadowed(const std::shared_ptr<Light>& light, const Tuple & point) const
{
	Tuple v = light->position() - point;
	double distance = v.magnitude();
	Tuple direction = v.normalize();

	Ray r = Ray(point, direction);
	Intersections ix = this->intersect_world(r);

	Intersection h = ix.hit();

	return (h.is_valid() && h.t_value < distance);
}

Color World::shadowed(const std::shared_ptr<Light>& light, const Tuple & point, const int depth) const
{
    // Get vector between sample point and light
	Tuple v = light->area_position() - point;
    // Calculate distance and direction
	double distance = v.magnitude();
	Tuple direction = v.normalize();

    // Cast a ray between the point and the light
	Ray r = Ray(point, direction, depth);
	Intersections ix = this->intersect_world(r);

	Intersection h = ix.hit();

    // If there is a hit, return transmitted light (refracted shadows)
	if (h.is_valid() && h.t_value < distance)
	{
		IxComps comps = IxComps(h, r, ix);
		auto obj_prim = std::static_pointer_cast<PrimitiveBase>(h.object);

		return obj_prim->material->transmit(light, *this, comps, ix);
	}
	return {1.0};
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

const std::vector<std::shared_ptr<PrimitiveBase>> & World::get_primitives()
{
	return this->w_primitives_;
}

const std::vector<std::shared_ptr<Light>> & World::get_lights()
{
	return this->w_lights_;
}

void World::remove_primitive(int index)
{
	this->w_primitives_.erase(this->w_primitives_.begin() + index);
}

void World::remove_light(int index)
{
	this->w_lights_.erase(this->w_lights_.begin() + index);
}

void World::add_object(const std::shared_ptr<PrimitiveBase>& obj)
{
	this->w_primitives_.push_back(obj);
}

void World::add_object(const std::shared_ptr<Light>& obj)
{
	this->w_lights_.push_back(obj);
}

