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
}

IxComps::IxComps(Intersection & ix, Ray & r)
{
	this->t_value = ix.t_value;
	this->object = ix.object;

	this->point = r.position(this->t_value);
	this->eye_v = -(r.direction);
	this->normal_v = this->object->normal_at(this->point);

	if (Tuple::dot(this->normal_v, this->eye_v) < 0.0f)
	{
		this->inside = true;
		this->normal_v = -(this->normal_v);
	}
	else
	{
		this->inside = false;
	}
}

IxComps::~IxComps()
{
}

IxComps IxComps::Background(Ray & r)
{
	IxComps comp = IxComps();
	comp.t_value = std::numeric_limits<float>::infinity();
	comp.object = nullptr;

	comp.point = Tuple::Point(0.0f, 0.0f, 0.0f);
	comp.eye_v = -(r.direction);
	comp.normal_v = comp.eye_v;

	comp.inside = false;

	return comp;
}

