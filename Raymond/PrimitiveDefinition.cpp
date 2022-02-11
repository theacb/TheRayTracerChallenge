#include "pch.h"
#include "PrimitiveDefinition.h"

// ------------------------------------------------------------------------
//
// Primitive
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

PrimitiveDefinition::PrimitiveDefinition()
{
}


PrimitiveDefinition::~PrimitiveDefinition()
{
}

// ------------------------------------------------------------------------
//
// Sphere
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

SphereDefinition::SphereDefinition() : PrimitiveDefinition()
{
}

SphereDefinition::~SphereDefinition()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> SphereDefinition::local_intersect_t(const Ray & r) const
{
	std::vector<double> result = std::vector<double>();

	// The vector from the sphere's center to the ray origin
	Tuple sphere_to_ray = r.origin - Tuple::Point(0.0, 0.0, 0.0);

	// Calculate discriminant
	double a = Tuple::dot(r.direction, r.direction);
	double b = 2.0 * Tuple::dot(r.direction, sphere_to_ray);
	double c = Tuple::dot(sphere_to_ray, sphere_to_ray) - 1;

	double discriminant = (b * b) - (4 * a * c);

	if (discriminant >= 0.0)
	{
		// resize vector
		result.resize(2);

		double sqrt_discriminant = sqrt(discriminant);

		// Calculate intersections
		result[0] = (-b - sqrt_discriminant) / (2 * a);
		result[1] = (-b + sqrt_discriminant) / (2 * a);

		return result;
	}
	else
	{
		return result;
	}
}

Tuple SphereDefinition::local_normal_at(const Tuple & object_space_point) const
{
	// Calculate surface normal
	return object_space_point - Tuple::Origin();
}

BoundingBox SphereDefinition::bounding_box() const
{
	return BoundingBox(Tuple::Point(-1.0, -1.0, -1.0), Tuple::Point(1.0, 1.0, 1.0));
}

// ------------------------------------------------------------------------
//
// Infinite Plane
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

InfinitePlaneDefinition::InfinitePlaneDefinition() : PrimitiveDefinition()
{
}

InfinitePlaneDefinition::~InfinitePlaneDefinition()
{
}

std::vector<double> InfinitePlaneDefinition::local_intersect_t(const Ray & r) const
{
	// Parallel or coplanar rays return empty vector
	if (abs(r.direction.y) < EPSILON)
	{
		return std::vector<double>();
	}
	// Assumes that the object space plane is on the X and Z axis
	else
	{
		return std::vector<double>({ (-r.origin.y) / r.direction.y });
	}
}

Tuple InfinitePlaneDefinition::local_normal_at(const Tuple & object_space_point) const
{
	// Assumes that the object space plane is on the X and Z axis, pointing in the positive y direction
	return Tuple::Vector(0.0, 1.0, 0.0);
}

BoundingBox InfinitePlaneDefinition::bounding_box() const
{
	return BoundingBox(
		Tuple::Point(
			-std::numeric_limits<double>::infinity(), 
			-EPSILON,
			-std::numeric_limits<double>::infinity()
		), 
		Tuple::Point(
			std::numeric_limits<double>::infinity(), 
			EPSILON, 
			std::numeric_limits<double>::infinity()
		)
	);
}

// ------------------------------------------------------------------------
//
// Cube
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

CubeDefinition::CubeDefinition() : PrimitiveDefinition()
{
}

CubeDefinition::~CubeDefinition()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> CubeDefinition::local_intersect_t(const Ray & r) const
{
	auto x_t_vec = this->check_axis_(r.origin.x, r.direction.x);
	auto y_t_vec = this->check_axis_(r.origin.y, r.direction.y);
	auto z_t_vec = this->check_axis_(r.origin.z, r.direction.z);

	// Get the smallest maximum and the largest minimum
	double tmin = std::max({ x_t_vec[0], y_t_vec[0], z_t_vec[0] });
	double tmax = std::min({ x_t_vec[1], y_t_vec[1], z_t_vec[1] });

	// If the ray misses, the minimum will be greater than the maximum
	if (tmin > tmax)
	{
		return std::vector<double>();
	}

	return std::vector<double>({ tmin, tmax });
}

Tuple CubeDefinition::local_normal_at(const Tuple & object_space_point) const
{
	double maxc = std::max({ abs(object_space_point.x), abs(object_space_point.y), abs(object_space_point.z) });

	// Compare which axis has the largest point
	if (flt_cmp(maxc, abs(object_space_point.x)))
	{
		return Tuple::Vector(object_space_point.x, 0.0, 0.0);
	}
	else if (flt_cmp(maxc, abs(object_space_point.y)))
	{
		return Tuple::Vector(0.0, object_space_point.y, 0.0);
	}

	return Tuple::Vector(0.0, 0.0, object_space_point.z);
}

BoundingBox CubeDefinition::bounding_box() const
{
	return BoundingBox(Tuple::Point(-1.0, -1.0, -1.0), Tuple::Point(1.0, 1.0, 1.0));
}

std::vector<double> CubeDefinition::check_axis_(const double & origin, const double & direction) const
{
	// Intersect with an infinite plane
	double tmax, tmin;

	double tmin_numerator = (-1.0 - origin);
	double tmax_numerator = (1.0 - origin);

	// If the ray does not intersect with the plane, the t value is infinity
	if (abs(direction) >= EPSILON)
	{
		tmin = tmin_numerator / direction;
		tmax = tmax_numerator / direction;
	}
	else
	{
		tmin = tmin_numerator * std::numeric_limits<double>::infinity();
		tmax = tmax_numerator * std::numeric_limits<double>::infinity();
	}

	// Swap so that the first value is always the minimum
	if (tmin > tmax)
	{
		return std::vector<double>({ tmax, tmin });
	}
	else
	{
		return std::vector<double>({ tmin, tmax });
	}
}

// ------------------------------------------------------------------------
//
// Cylinder
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

CylinderDefinition::CylinderDefinition() : CylinderDefinition(
	-std::numeric_limits<double>::infinity(),
	std::numeric_limits<double>::infinity(),
	false
)
{
}

CylinderDefinition::CylinderDefinition(double minimum, double maximum, bool closed) : PrimitiveDefinition()
{
	this->minimum = minimum;
	this->maximum = maximum;
	this->closed = closed;
}

CylinderDefinition::~CylinderDefinition()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> CylinderDefinition::local_intersect_t(const Ray & r) const
{
	auto results = std::vector<double>();
	results.reserve(2);

	double a = (r.direction.x * r.direction.x) + (r.direction.z * r.direction.z);

	// return a miss if a is 0
	if (abs(a) > std::numeric_limits<double>::epsilon())
	{
		double b = (2.0 * r.origin.x * r.direction.x) + (2.0 * r.origin.z * r.direction.z);

		double c = (r.origin.x * r.origin.x) + (r.origin.z * r.origin.z) - 1.0;

		double disc = (b * b) - 4.0 * a * c;

		// Return a miss if the discriminant is negative
		if (disc < 0.0)
		{
			return results;
		}

		double sq_disc = sqrt(disc);

		double t0 = (-b - sq_disc) / (2.0 * a);
		double t1 = (-b + sq_disc) / (2.0 * a);

		// swap if t0 is larger than t1
		if (t0 > t1)
		{
			double temp = t1;
			t1 = t0;
			t0 = temp;
		}

		// Check if the first intersection is within the minimum bounds
		double y0 = r.origin.y + (t0 * r.direction.y);
		if (this->minimum < y0 && y0 < this->maximum)
		{
			results.push_back(t0);
		}

		// Check if the second intersection is within the minimum bounds
		double y1 = r.origin.y + (t1 * r.direction.y);
		if (this->minimum < y1 && y1 < this->maximum)
		{
			results.push_back(t1);
		}
	}

	// Caps only matter if the cylinder is closed, and might possibly be 
	// intersected by the ray
	if (this->closed && abs(r.direction.y) > 0.0)
	{
		this->intersect_caps_(r, results);
	}

	return results;
}

Tuple CylinderDefinition::local_normal_at(const Tuple & object_space_point) const
{
	// Compute the square of the distance from the Y axis
	double dist = (object_space_point.x * object_space_point.x) + (object_space_point.z * object_space_point.z);

	if (dist < 1.0 && object_space_point.y >= this->maximum - EPSILON)
	{
		return Tuple::Vector(0.0, 1.0, 0.0);
	}
	else if (dist < 1.0 && object_space_point.y <= this->minimum + EPSILON)
	{
		return Tuple::Vector(0.0, -1.0, 0.0);
	}
	else
	{
		return Tuple::Vector(object_space_point.x, 0.0, object_space_point.z);
	}
}

BoundingBox CylinderDefinition::bounding_box() const
{
	return BoundingBox(Tuple::Point(-1.0, this->minimum, -1.0), Tuple::Point(1.0, this->maximum, 1.0));
}

void CylinderDefinition::intersect_caps_(const Ray & r, std::vector<double> & xs) const
{
	// Pass xs by reference, and update it rather than merge a return vector

	// Check for an intersection with the lower end cap by intersecting
	// the ray with the plane at y=cyl.minimum
	double t_min = (this->minimum - r.origin.y) / r.direction.y;
	if (this->check_caps_(r, t_min))
		xs.push_back(t_min);

	// Check for an intersection with the upper end cap by intersecting
	// the ray with the plane at y.cyl.maximum
	double t_max = (this->maximum - r.origin.y) / r.direction.y;
	if (this->check_caps_(r, t_max))
		xs.push_back(t_max);
}

bool CylinderDefinition::check_caps_(const Ray & r, const double & t) const
{
	// Checks to see if the intersection at 't' is within the radius of the cylinder
	// Prevents duplications on a corner hit
	double x = r.origin.x + (t * r.direction.x);
	double z = r.origin.z + (t * r.direction.z);

	return ((x * x) + (z * z)) <= 1.0;
}

// ------------------------------------------------------------------------
//
// DoubleNappedCone
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

DoubleNappedConeDefinition::DoubleNappedConeDefinition() : DoubleNappedConeDefinition(
	-std::numeric_limits<double>::infinity(),
	std::numeric_limits<double>::infinity(),
	false
)
{
}

DoubleNappedConeDefinition::DoubleNappedConeDefinition(double minimum, double maximum, bool closed) : PrimitiveDefinition()
{
	this->minimum = minimum;
	this->maximum = maximum;
	this->closed = closed;
}

DoubleNappedConeDefinition::~DoubleNappedConeDefinition()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> DoubleNappedConeDefinition::local_intersect_t(const Ray & r) const
{
	auto results = std::vector<double>();
	results.reserve(4);

	double a = (r.direction.x * r.direction.x) - (r.direction.y * r.direction.y) + (r.direction.z * r.direction.z);
	double b = (2.0 * r.origin.x * r.direction.x) - (2.0 * r.origin.y * r.direction.y) + (2.0 * r.origin.z * r.direction.z);
	double c = (r.origin.x * r.origin.x) - (r.origin.y * r.origin.y) + (r.origin.z * r.origin.z);

	// return a miss if a is 0
	if (abs(a) > std::numeric_limits<double>::epsilon())
	{
		double disc = (b * b) - 4.0 * a * c;

		// Return a miss if the discriminant is negative
		if (disc < 0.0)
		{
			return results;
		}

		double sq_disc = sqrt(disc);

		double t0 = (-b - sq_disc) / (2.0 * a);
		double t1 = (-b + sq_disc) / (2.0 * a);

		// swap if t0 is larger than t1
		if (t0 > t1)
		{
			double temp = t1;
			t1 = t0;
			t0 = temp;
		}

		// Check if the first intersection is within the minimum bounds
		double y0 = r.origin.y + (t0 * r.direction.y);
		if (this->minimum < y0 && y0 < this->maximum)
		{
			results.push_back(t0);
		}

		// Check if the second intersection is within the minimum bounds
		double y1 = r.origin.y + (t1 * r.direction.y);
		if (this->minimum < y1 && y1 < this->maximum)
		{
			results.push_back(t1);
		}
	}
	else if (abs(b) > std::numeric_limits<double>::epsilon())
	{
		// The Ray is parallel to one of the cone halves, but will hit the other
		double t = -(c / (2.0 * b));

		// Check if the intersection is within the minimum bounds
		double y = r.origin.y + (t * r.direction.y);
		if (this->minimum < y && y < this->maximum)
		{
			results.push_back(t);
		}
	}

	// Caps only matter if the cylinder is closed, and might possibly be 
	// intersected by the ray
	if (this->closed && abs(r.direction.y) > 0.0)
	{
		this->intersect_caps_(r, results);
	}

	return results;
}

Tuple DoubleNappedConeDefinition::local_normal_at(const Tuple & object_space_point) const
{
	// Compute the square of the distance from the Y axis
	double dist = (object_space_point.x * object_space_point.x) + (object_space_point.z * object_space_point.z);

	if (dist < abs(this->maximum) && object_space_point.y >= this->maximum - EPSILON)
	{
		return Tuple::Vector(0.0, 1.0, 0.0);
	}
	else if (dist < abs(this->minimum) && object_space_point.y <= this->minimum + EPSILON)
	{
		return Tuple::Vector(0.0, -1.0, 0.0);
	}
	else
	{
		double y = sqrt(dist);
		// Invert y for the upper cone
		if (object_space_point.y > 0.0)
		{
			y = -y;
		}
		return Tuple::Vector(object_space_point.x, y, object_space_point.z);
	}
}

BoundingBox DoubleNappedConeDefinition::bounding_box() const
{
	return BoundingBox(Tuple::Point(
		this->minimum, 
		this->minimum, 
		this->minimum
	), Tuple::Point(
		this->maximum,
		this->maximum,
		this->maximum
	));
}

void DoubleNappedConeDefinition::intersect_caps_(const Ray & r, std::vector<double>& xs) const
{
	// Pass xs by reference, and update it rather than merge a return vector

	// Check for an intersection with the lower end cap by intersecting
	// the ray with the plane at y=cyl.minimum
	double t_min = (this->minimum - r.origin.y) / r.direction.y;
	if (this->check_caps_(r, t_min, this->minimum))
		xs.push_back(t_min);

	// Check for an intersection with the upper end cap by intersecting
	// the ray with the plane at y.cyl.maximum
	double t_max = (this->maximum - r.origin.y) / r.direction.y;
	if (this->check_caps_(r, t_max, this->maximum))
		xs.push_back(t_max);
}

bool DoubleNappedConeDefinition::check_caps_(const Ray & r, const double & t, const double & radius) const
{
	// Checks to see if the intersection at 't' is within the radius of the Cone cap
	// Prevents duplications on a corner hit
	double x = r.origin.x + (t * r.direction.x);
	double z = r.origin.z + (t * r.direction.z);

	// The radius is equal to the truncation length for this cap (minimum or maximum)
	// Because these could be negative, it's simpler to abs the radius here
	return ((x * x) + (z * z)) <= (radius * radius);
}

// ------------------------------------------------------------------------
//
// Test Shape
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

NullShapeDefinition::NullShapeDefinition()
{
}

NullShapeDefinition::~NullShapeDefinition()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> NullShapeDefinition::local_intersect_t(const Ray & r) const
{
	return std::vector<double>();
}

Tuple NullShapeDefinition::local_normal_at(const Tuple & object_space_point) const
{
	return Tuple::Vector(object_space_point.x, object_space_point.y, object_space_point.z);
}

BoundingBox NullShapeDefinition::bounding_box() const
{
	return BoundingBox(Tuple::Point(0.0, 0.0, 0.0), Tuple::Point(0.0, 0.0, 0.0));;
}
