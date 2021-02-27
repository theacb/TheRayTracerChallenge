#include "pch.h"
#include "Object.h"

// ------------------------------------------------------------------------
//
// Intersection
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Intersection::Intersection() : Intersection(0.0f, nullptr)
{
}

Intersection::Intersection(float t, ConstObjectPtr obj)
{
	this->t_value = t;
	this->object = obj;
}

Intersection::~Intersection()
{
}

bool Intersection::is_valid() const
{
	return !(this->object == nullptr);
}

// ------------------------------------------------------------------------
// Overloaded Operators
// ------------------------------------------------------------------------

std::ostream & operator<<(std::ostream & os, const Intersection & ix)
{
	return os << "(" << ix.t_value << ", " << ix.object << ")";
}

bool operator==(const Intersection & l_ix, const Intersection & r_ix)
{
	return flt_cmp(l_ix.t_value, r_ix.t_value);
}

bool operator!=(const Intersection & l_ix, const Intersection & r_ix)
{
	return !(l_ix == r_ix);
}

bool operator<(const Intersection & l_ix, const Intersection & r_ix)
{
	return l_ix.t_value < r_ix.t_value;
}

// ------------------------------------------------------------------------
//
// Intersections
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Intersections::Intersections() : std::vector<Intersection>()
{
}

Intersections::Intersections(std::initializer_list<Intersection> il) : 
	std::vector<Intersection>(il)
{
	std::sort(this->begin(), this->end());
}

Intersections::Intersections(std::vector<float> t_values, ConstObjectPtr o) : 
	std::vector<Intersection>()
{
	for (float t : t_values)
	{
		this->push_back(Intersection(t, o));
	}

	std::sort(this->begin(), this->end());
}

Intersection Intersections::hit() const
{
	// Returns a null intersection if there are no valid intersections inside the vector
	if (this->size() < 1)
	{
		return Intersection();
	}
	else
	{
		// The vector is sorted from lowest to highest on construction
		// Finds the first intersection with a positive t_value
		std::vector<Intersection>::const_iterator result = std::find_if(
			this->cbegin(),
			this->cend() - 1,
			[](const Intersection & ix) -> bool { return ix.t_value > 0.0f; }
		);


		// If find_if returns an intersection with a positive t value, dereference the 
		// iterator and return the result
		if ((*result).t_value > 0.0f)
		{
			return *result;
		}
		else
		{
			return Intersection();
		}
	}
}

std::ostream & operator<<(std::ostream & os, const Intersections & ixs)
{
	if (ixs.size() > 0)
	{
		os << "< ";
		for (Intersection ix : ixs)
		{
			os << ix << ", ";
		}
		os << " >";
		return os;
	}
	else
	{
		return os << "<NONE>";
	}
}

// ------------------------------------------------------------------------
//
// Base Object
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

ObjectBase::ObjectBase()
{
	this->transform_ = Matrix4::Identity();
	this->name_ = "Default Object 000";
}


ObjectBase::~ObjectBase()
{
}

void ObjectBase::set_name(std::string new_name)
{
	this->name_ = name_;
}

std::string ObjectBase::get_name() const
{
	return this->name_;
}

void ObjectBase::set_transform(Matrix4 m )
{
	this->transform_ = m;
}

Matrix4 ObjectBase::get_transform() const
{
	return this->transform_;
}

std::ostream & operator<<(std::ostream & os, const ObjectBase & obj)
{
	return os << "(" << obj.name_ << ", " << obj.transform_ << ")";
}

// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Intersections intersect(Ray & r, ConstObjectPtr o)
{
	// Transform Ray by Inverse of Object transform Matrix
	Ray transformed_ray = r.transform((o->get_transform()).inverse());
	std::vector<float> t_values = o->intersect_t(transformed_ray);

	return Intersections(t_values, o);
}
