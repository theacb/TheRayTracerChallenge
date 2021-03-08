#include "pch.h"
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
}

World::~World()
{
}

// Factory

World World::Default()
{
	World def = World();

	auto light = std::make_shared<PointLight>(Tuple::Point(-10.0f, 10.0f, -10.0f), Color(1.0f));

	def.add_object(light);

	auto s1 = std::make_shared<Sphere>();
	auto ms1 = std::dynamic_pointer_cast<PhongMaterial>(s1->material);
	ms1->color = Color(0.8f, 1.0f, 0.6f);
	ms1->diffuse = 0.7f;
	ms1->specular = 0.2f;

	def.add_object(s1);

	auto s2 = std::make_shared<Sphere>();
	s2->set_transform(Matrix4::Scaling(0.5f, 0.5f, 0.5f));

	def.add_object(s2);

	return def;
}

// ------------------------------------------------------------------------
// Intersector
// ------------------------------------------------------------------------

Intersections World::intersect_world(Ray & ray) const
{
	Intersections result;

	// Prereserve space for at least 3 intersections per scene object
	// This may be unwise...
	result.reserve(this->w_primitives_.size() * 3);

	for (std::shared_ptr<Primitive> obj: this->w_primitives_)
	{
		// generates an intersection for each object in the scene
		Intersections obj_xs = intersect(ray, obj);

		// Then concatenates them into a single vector
		for (Intersection& ix : obj_xs)
		{
			// Filter bad values
			if (ix.is_valid() && ix.t_value > -0.0f)
			{
				result.push_back(ix);
			}
		}
	}

	std::sort(result.begin(), result.end());

	return result;
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Color World::shade(IxComps & comps) const
{
	Color sample = Color(0.0f);
	
	// std::cout << "Shading " << comps.object->get_name() << " at ";
	// std::cout << comps.point << ", Normal: " << comps.normal_v << ", Offset Point: " << comps.over_point << "\n";

	for (std::shared_ptr<Light> lgt : this->w_lights_)
	{
		auto obj_prim = std::dynamic_pointer_cast<Primitive>(comps.object);

		bool shd = this->is_shadowed(lgt, comps.over_point);

		comps.shadow_multiplier = shd ? 0.0f : 1.0f;

		if (lgt->falloff)
		{
			// Calculate quadratic intensity using formula I = 1/d^2
			float distance = Tuple::distance(lgt->position(), comps.point);
			float quad_multiplier = 1.0f / (distance * distance);
			float intensity = lgt->color.magnitude() * quad_multiplier * lgt->multiplier;
			
			// If value is less than cutoff value, do not calculate sample
			if (intensity > lgt->cutoff)
			{
				sample = sample + (obj_prim->material->lighting(lgt, comps) * intensity);
			}
		}
		else
		{
			sample = sample + obj_prim->material->lighting(lgt, comps);
		}
	}
	return sample;
}

Color World::color_at(Ray & ray) const
{
	Intersections ix = this->intersect_world(ray);
	if (ix.size() > 0)
	{
		Intersection hit = ix.hit();
		IxComps comps = IxComps(hit, ray);

		return this->shade(comps);
	}
	else
	{
		IxComps comps = IxComps::Background(ray);

		return this->background->shade(comps);
	}
}

bool World::is_shadowed(std::shared_ptr<Light> light, Tuple & point) const
{
	Tuple v = light->position() - point;
	float distance = v.magnitude();
	Tuple direction = v.normalize();

	Ray r = Ray(point, direction);
	Intersections ix = intersect_world(r);

	Intersection h = ix.hit();

	// std::cout << "Shadow Ray - Intersecting: " << h.object->get_name() << " at " << h.t_value << "\n";

	return (h.is_valid() && h.t_value < distance);
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

const std::vector<std::shared_ptr<Primitive>> & World::get_primitives()
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

void World::add_object(std::shared_ptr<Primitive> obj)
{
	this->w_primitives_.push_back(obj);
}

void World::add_object(std::shared_ptr<Light> obj)
{
	this->w_lights_.push_back(obj);
}

