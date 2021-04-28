#include "pch.h"
#include "Object.h"

// ------------------------------------------------------------------------
//
// Transform Controller
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

TransformController::TransformController()
{
	this->x_transform_ = Matrix4::Identity();
	this->x_inverse_transform_ = Matrix4::Identity();
}

TransformController::TransformController(const Matrix4 & m)
{
	this->x_transform_ = m;
	this->x_inverse_transform_ = m.inverse();
}

TransformController::TransformController(const TransformController & src)
{
	this->x_transform_ = src.get_transform();
	this->x_inverse_transform_ = src.get_transform().inverse();
}

TransformController::~TransformController()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void TransformController::set_transform(const Matrix4 & m)
{
	this->x_transform_ = m;
	this->x_inverse_transform_ = m.inverse();
}

Matrix4 TransformController::get_transform() const
{
	return this->x_transform_;
}

Matrix4 TransformController::get_inverse_transform() const
{
	return this->x_inverse_transform_;
}

// ------------------------------------------------------------------------
// Transformers
// ------------------------------------------------------------------------

Ray TransformController::ray_to_object_space(const Ray & r) const
{
	return Ray(r).transform(this->x_inverse_transform_);
}

Tuple TransformController::point_to_object_space(const Tuple & p) const
{
	return this->x_inverse_transform_ * p;
}

Tuple TransformController::point_to_world_space(const Tuple & p) const
{
	return this->x_transform_ * p;
}

Tuple TransformController::normal_vector_to_world_space(const Tuple & v) const
{
	return this->x_inverse_transform_.transpose() * v;
}

// ------------------------------------------------------------------------
// Operators
// ------------------------------------------------------------------------

std::ostream & operator<<(std::ostream & os, const TransformController & ctrl)
{
	return os << ctrl.x_transform_;
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
	this->o_transform_ = std::make_shared<TransformController>();
	this->o_name_ = "Default Object 000";
	this->o_definition_ = std::make_shared<NullShapeDefinition>();
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
	Tuple object_space_point = this->o_transform_->point_to_object_space(world_space_point);

	// Calculate surface normal
	Tuple object_normal_vector = this->o_definition_->local_normal_at(object_space_point);

	// Multiply object space normal by transpose of the inverted x form matrix
	Tuple world_normal_vector = this->o_transform_->normal_vector_to_world_space(object_normal_vector);

	// Enforce behavior as a vector by setting w to 0
	world_normal_vector.w = 0.0;

	// Normalize vector
	return world_normal_vector.normalize();
}

std::vector<double> ObjectBase::intersect_t(const Ray & r) const
{
	// Transform ray to object space
	Ray transformed_ray = this->ray_to_object_space(r);
	return this->o_definition_->local_intersect_t(transformed_ray);
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

void ObjectBase::set_name(std::string new_name)
{
	this->o_name_ = new_name;
}

std::string ObjectBase::get_name() const
{
	return this->o_name_;
}

void ObjectBase::set_definition(std::shared_ptr<PrimitiveDefinition> def)
{
	this->o_definition_ = def;
}

std::shared_ptr<PrimitiveDefinition> ObjectBase::get_definition()
{
	return this->o_definition_;
}

std::shared_ptr<PrimitiveDefinition> ObjectBase::get_definition() const
{
	return this->o_definition_;
}

void ObjectBase::set_transform(Matrix4 m )
{
	this->o_transform_->set_transform(m);
}

Matrix4 ObjectBase::get_transform() const
{
	return this->o_transform_->get_transform();
}

Matrix4 ObjectBase::get_inverse_transform() const
{
	return this->o_transform_->get_inverse_transform();
}

// ------------------------------------------------------------------------
// Transformers
// ------------------------------------------------------------------------

Ray ObjectBase::ray_to_object_space(const Ray & r) const
{
	return this->o_transform_->ray_to_object_space(r);
}

Tuple ObjectBase::point_to_object_space(const Tuple & p) const
{
	return this->o_transform_->point_to_object_space(p);
}

Tuple ObjectBase::point_to_world_space(const Tuple & p) const
{
	return this->o_transform_->point_to_world_space(p);
}

Tuple ObjectBase::normal_vector_to_world_space(const Tuple & v) const
{
	return this->o_transform_->normal_vector_to_world_space(v);
}

// ------------------------------------------------------------------------
// Operators
// ------------------------------------------------------------------------

std::ostream & operator<<(std::ostream & os, const ObjectBase & obj)
{
	return os << "(" << obj.o_name_ << ", " << obj.o_transform_ << ")";
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
	this->hit_index_ = -1;
}

Intersections::Intersections(std::initializer_list<Intersection> il) :
	std::vector<Intersection>(il)
{
	this->hit_index_ = -1;

	std::sort(this->begin(), this->end());

	this->hit_index_ = this->hit_();
}

Intersections::Intersections(std::vector<double> t_values, std::shared_ptr<ObjectBase> obj) :
	std::vector<Intersection>()
{
	this->hit_index_ = -1;

	for (double t : t_values)
	{
		this->push_back(Intersection(t, obj));
	}

	std::sort(this->begin(), this->end());

	this->hit_index_ = this->hit_();
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

Intersection Intersections::hit() const
{
	if (this->hit_index_ > -1)
	{ 
		Intersection hit = this->at(this->hit_index_);
		return hit;
	}
	else
	{
		return Intersection();
	}
}

int Intersections::get_hit_index() const
{
	return this->hit_index_;
}

void Intersections::calculate_hit()
{
	this->hit_index_ = this->hit_();
}

// Private

int Intersections::hit_() const
{
	// Returns a null intersection if there are no valid intersections inside the vector
	if (this->size() > 0)
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
			return static_cast<int>(result - this->begin());
		}
	}

	// default to returning -1
	return -1;
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
	std::vector<double> t_values = obj->intersect_t(r);

	return Intersections(t_values, obj);
}

