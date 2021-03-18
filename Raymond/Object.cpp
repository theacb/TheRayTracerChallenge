#include "pch.h"
#include "Object.h"

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
	this->inverse_transform_ = Matrix4::Identity();
	this->name_ = "Default Object 000";
}


ObjectBase::~ObjectBase()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

Tuple ObjectBase::normal_at(const Tuple & world_space_point) const
{
	// Multiply point by inverse xform matrix to bring into object space
	Tuple object_space_point = this->point_to_object_space(world_space_point);

	// Calculate surface normal
	Tuple object_normal_vector = this->local_normal_at(object_space_point);

	// Multiply object space normal by transpose of the inverted x form matrix
	Tuple world_normal_vector = this->normal_vector_to_world_space(object_normal_vector);

	// Enforce behavior as a vector by setting w to 0
	world_normal_vector.w = 0.0;

	// Normalize vector
	return world_normal_vector.normalize();
}

void ObjectBase::set_name(std::string new_name)
{
	this->name_ = new_name;
}

std::string ObjectBase::get_name() const
{
	return this->name_;
}

void ObjectBase::set_transform(Matrix4 m )
{
	this->transform_ = m;
	this->inverse_transform_ = m.inverse();
}

Matrix4 ObjectBase::get_transform() const
{
	return this->transform_;
}

Matrix4 ObjectBase::get_inverse_transform() const
{
	return this->inverse_transform_;
}

// ------------------------------------------------------------------------
// Transformers
// ------------------------------------------------------------------------

Ray ObjectBase::ray_to_object_space(const Ray & r) const
{
	return Ray(r).transform(this->inverse_transform_);
}

Tuple ObjectBase::point_to_object_space(const Tuple & p) const
{
	return this->inverse_transform_ * p;
}

Tuple ObjectBase::point_to_world_space(const Tuple & p) const
{
	return this->transform_ * p;
}

Tuple ObjectBase::normal_vector_to_world_space(const Tuple & v) const
{
	return this->get_inverse_transform().transpose() * v;
}

// ------------------------------------------------------------------------
// Operators
// ------------------------------------------------------------------------

std::ostream & operator<<(std::ostream & os, const ObjectBase & obj)
{
	return os << "(" << obj.name_ << ", " << obj.transform_ << ")";
}

// ------------------------------------------------------------------------
//
// Intersection
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Intersection::Intersection() : Intersection(0.0, nullptr)
{
}

Intersection::Intersection(double t, std::shared_ptr<ObjectBase> obj)
{
	this->t_value = t;
	this->object = obj;
}

Intersection::~Intersection()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

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

Intersections::Intersections(std::vector<double> t_values, std::shared_ptr<ObjectBase> obj) :
	std::vector<Intersection>()
{
	for (double t : t_values)
	{
		this->push_back(Intersection(t, obj));
	}

	std::sort(this->begin(), this->end());
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

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
			[](const Intersection & ix) -> bool { return ix.t_value > 0.0; }
		);


		// If find_if returns an intersection with a positive t value, dereference the 
		// iterator and return the result
		if ((*result).t_value > 0.0)
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
// Intersector
// ------------------------------------------------------------------------

Intersections intersect( const Ray & r, const std::shared_ptr<ObjectBase> obj)
{
	// Transform Ray by Inverse of Object transform Matrix
	Ray transformed_ray = obj->ray_to_object_space(r);
	std::vector<double> t_values = obj->local_intersect_t(transformed_ray);

	return Intersections(t_values, obj);
}
