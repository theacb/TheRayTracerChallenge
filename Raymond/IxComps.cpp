#include "pch.h"
#include "IxComps.h"
#include "Primitive.h"

// ------------------------------------------------------------------------
//
// Intersection Precomputed Computations
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

IxComps::IxComps()
{
	this->t_value = 0.0;
	this->object = nullptr;
	this->ray_depth = 0;

	this->point = Tuple::Point(0.0, 0.0, 0.0);
	this->texmap_point = this->point;
	this->eye_v = Tuple::Vector(0.0, 1.0, 0.0);
	this->normal_v = Tuple::Vector(0.0, 1.0, 0.0);
	this->reflect_v = Tuple::reflect(-(this->eye_v), this->normal_v);

	this->shadow_multiplier = Color(1.0);

	this->inside = false;

	this->over_point = this->point + (this->normal_v * EPSILON);
	this->under_point = this->point - (this->normal_v * EPSILON);

	this->n1 = 1.0;
	this->n2 = 1.0;
}

IxComps::IxComps(const Intersection & ix, const Ray & ray)
{
	this->t_value = ix.t_value;
	this->object = ix.object;
	this->ray_depth = ray.depth;

	this->point = ray.position(this->t_value);
	this->texmap_point = this->point;
	this->eye_v = -(Tuple(ray.direction));
	this->normal_v = this->object->normal_at(this->point);
	this->reflect_v = Tuple::reflect(ray.direction, this->normal_v);

	this->shadow_multiplier = Color(1.0);

	if (Tuple::dot(this->normal_v, this->eye_v) < 0.0)
	{
		this->inside = true;
		this->normal_v = -(this->normal_v);
	}
	else
	{
		this->inside = false;
	}

	this->over_point = this->point + (this->normal_v * EPSILON);
	this->under_point = this->point - (this->normal_v * EPSILON);

	this->n1 = 1.0;
	this->n2 = 1.0;
}

IxComps::IxComps(const Intersection & ix, const Ray & ray, const Intersections & xs) : IxComps(ix, ray)
{
	// Find n1 and n2

	// Temp vector
	// Can be sized to xs.size() as that is its maximum possible size
	auto containers = std::vector<std::shared_ptr<ObjectBase>>();
	containers.reserve(xs.size());

	// Iterate over intersections
	for (const auto & x : xs)
	{
		// If the current object is the hit, set n1
		if (x == ix)
		{
			// If container is empty, assume medium is air and give IOR as 1.0
			// If container is not empty, give IOR as a sampled texture on the 
			// surface of the last intersection's object

			if (containers.empty())
			{
				this->n1 = 1.0;
			}
			else
			{
				this->n1 = std::static_pointer_cast<PrimitiveBase>(containers.back())->material->ior.sample_at(IxComps(x, ray));
			}
		}

		// Check Containers for the current object
		// Erase it if it exists already, append it if it doesn't

		int location = -1;

		for (int i = 0; i < containers.size(); i++)
		{
			if (containers[i] == x.object)
			{
				location = i;
			}
		}

		if (location >= 0)
		{
			containers.erase(containers.begin() + location);
		}
		else
		{
			containers.push_back(x.object);
		}

		// If the current object is the hit, set n2, then break
		if (x == ix)
		{
			// If container is empty, assume medium is air and give IOR as 1.0
			// If container is not empty, give IOR as a sampled texture on the 
			// surface of the last intersection's object
			if (containers.empty())
			{
				this->n2 = 1.0;
			}
			else
			{
				this->n2 = std::static_pointer_cast<PrimitiveBase>(containers.back())->material->ior.sample_at(IxComps(x, ray));
			}

			break;
		}
	}
}

IxComps::IxComps(const IxComps & src)
{
	this->t_value = src.t_value;
	this->object = src.object;
	this->ray_depth = src.ray_depth;

	this->point = src.point;
	this->texmap_point = src.texmap_point;
	this->eye_v = src.eye_v;
	this->normal_v = src.normal_v;
	this->reflect_v = src.reflect_v;
	this->over_point = src.over_point;
	this->under_point = src.under_point;

	this->shadow_multiplier = src.shadow_multiplier;

	this->inside = src.inside;

	this->n1 = src.n1;
	this->n2 = src.n2;
}

IxComps::~IxComps()
= default;

// ------------------------------------------------------------------------
// Factories
// ------------------------------------------------------------------------

IxComps IxComps::Background(const Ray & r)
{
	IxComps comp = IxComps();
	comp.t_value = std::numeric_limits<double>::infinity();
	comp.object = nullptr;
	comp.ray_depth = r.depth;

	comp.point = Tuple::Point(0.0, 0.0, 0.0);
	comp.texmap_point = comp.point;
	comp.eye_v = -(Tuple(r.direction));
	comp.normal_v = comp.eye_v;
	comp.reflect_v = Tuple::reflect(r.direction, comp.normal_v);

	comp.inside = false;

	comp.n1 = 1.0;
	comp.n2 = 1.0;

	return comp;
}

