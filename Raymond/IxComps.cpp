#include "pch.h"
#include "IxComps.h"

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

	this->point = Tuple::Point(0.0, 0.0, 0.0);
	this->texmap_point = this->point;
	this->eye_v = Tuple::Vector(0.0, 1.0, 0.0);
	this->normal_v = Tuple::Vector(0.0, 1.0, 0.0);
	this->over_point = this->point + (this->normal_v * EPSILON);

	this->shadow_multiplier = 1.0;

	this->inside = false;
}

IxComps::IxComps(const Intersection & ix, const Ray & ray)
{
	this->t_value = ix.t_value;
	this->object = ix.object;

	this->point = ray.position(this->t_value);
	this->texmap_point = this->point;
	this->eye_v = -(Tuple(ray.direction));
	this->normal_v = this->object->normal_at(this->point);

	this->shadow_multiplier = 1.0;

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
}

IxComps::IxComps(const IxComps & src)
{
	this->t_value = src.t_value;
	this->object = src.object;

	this->point = src.point;
	this->texmap_point = src.texmap_point;
	this->eye_v = src.eye_v;
	this->normal_v = src.normal_v;
	this->over_point = src.over_point;

	this->shadow_multiplier = src.shadow_multiplier;

	this->inside = src.inside;
}

IxComps::~IxComps()
{
}

// ------------------------------------------------------------------------
// Factories
// ------------------------------------------------------------------------

IxComps IxComps::Background(const Ray & r)
{
	IxComps comp = IxComps();
	comp.t_value = std::numeric_limits<double>::infinity();
	comp.object = nullptr;

	comp.point = Tuple::Point(0.0, 0.0, 0.0);
	comp.texmap_point = comp.point;
	comp.eye_v = -(Tuple(r.direction));
	comp.normal_v = comp.eye_v;

	comp.inside = false;

	return comp;
}

