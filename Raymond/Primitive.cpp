#include "pch.h"
#include "Primitive.h"

// ------------------------------------------------------------------------
//
// Sphere
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Primitive::Primitive() : ObjectBase()
{
	this->set_name("Default Primitive 000");
	this->material = std::make_shared<PhongMaterial>();
}


Primitive::~Primitive()
{
}

// ------------------------------------------------------------------------
//
// Sphere
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Sphere::Sphere() : Primitive()
{
	this->radius = 1.0;
	this->set_name("Default Sphere 000");
}

Sphere::Sphere(std::string name)
{
	this->radius = 1.0;
	this->set_name(name);
}

Sphere::~Sphere()
{
}

// ------------------------------------------------------------------------
// Static Factory Methods
// ------------------------------------------------------------------------

std::shared_ptr<Sphere> Sphere::GlassSphere()
{
	auto s = std::make_shared<Sphere>();

	auto ms = std::dynamic_pointer_cast<PhongMaterial>(s->material);
	ms->refraction.set_value(Color(1.0));
	ms->ior.set_value(1.5);

	return s;
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> Sphere::local_intersect_t(const Ray & r) const
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

Tuple Sphere::local_normal_at(const Tuple & object_space_point) const
{
	// Calculate surface normal
	return object_space_point - Tuple::Origin();
}

// ------------------------------------------------------------------------
//
// Infinite Plane
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

InfinitePlane::InfinitePlane()
{
	this->set_name("Default Sphere 000");
}

InfinitePlane::InfinitePlane(std::string name)
{
	this->set_name(name);
}

InfinitePlane::~InfinitePlane()
{
}

std::vector<double> InfinitePlane::local_intersect_t(const Ray & r) const
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

Tuple InfinitePlane::local_normal_at(const Tuple & object_space_point) const
{
	// Assumes that the object space plane is on the X and Z axis, pointing in the positive y direction
	return Tuple::Vector(0.0, 1.0, 0.0);
}

// ------------------------------------------------------------------------
//
// Cube
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Cube::Cube()
{
	this->set_name("Default Cube 000");
}

Cube::Cube(std::string name)
{
	this->set_name(name);
}

Cube::~Cube()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> Cube::local_intersect_t(const Ray & r) const
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

Tuple Cube::local_normal_at(const Tuple & object_space_point) const
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

std::vector<double> Cube::check_axis_(const double & origin, const double & direction) const
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
		tmin = tmin_numerator * std::numeric_limits<double>::infinity();;
		tmax = tmax_numerator * std::numeric_limits<double>::infinity();;
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

Cylinder::Cylinder() : Cylinder("Default Cylinder 000")
{
}

Cylinder::Cylinder(std::string name) : Cylinder(
	-std::numeric_limits<double>::infinity(),
	std::numeric_limits<double>::infinity(),
	name
)
{
}

Cylinder::Cylinder(double minimum, double maximum) : Cylinder(
	minimum, 
	maximum, 
	"Default Cylinder 000"
)
{
}

Cylinder::Cylinder(double minimum, double maximum, std::string name)
{
	this->minimum = minimum;
	this->maximum = maximum;
	this->set_name(name);
	this->closed = false;
}

Cylinder::~Cylinder()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> Cylinder::local_intersect_t(const Ray & r) const
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

		// Check if the first intesection is within the minimum bounds
		double y0 = r.origin.y + (t0 * r.direction.y);
		if (this->minimum < y0 && y0 < this->maximum)
		{
			results.push_back(t0);
		}

		// Check if the second intesection is within the minimum bounds
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

Tuple Cylinder::local_normal_at(const Tuple & object_space_point) const
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

void Cylinder::intersect_caps_(const Ray & r, std::vector<double> & xs) const
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

bool Cylinder::check_caps_(const Ray & r, const double & t) const
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

DoubleNappedCone::DoubleNappedCone() : DoubleNappedCone("Default DoubleNappedCone 000")
{
}

DoubleNappedCone::DoubleNappedCone(std::string name) : DoubleNappedCone(
	-std::numeric_limits<double>::infinity(),
	std::numeric_limits<double>::infinity(),
	name
)
{
}

DoubleNappedCone::DoubleNappedCone(double minimum, double maximum) : DoubleNappedCone(
	minimum,
	maximum,
	"Default DoubleNappedCone 000"
)
{
}

DoubleNappedCone::DoubleNappedCone(double minimum, double maximum, std::string name)
{
	this->minimum = minimum;
	this->maximum = maximum;
	this->set_name(name);
	this->closed = false;
}

DoubleNappedCone::~DoubleNappedCone()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> DoubleNappedCone::local_intersect_t(const Ray & r) const
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

		// Check if the first intesection is within the minimum bounds
		double y0 = r.origin.y + (t0 * r.direction.y);
		if (this->minimum < y0 && y0 < this->maximum)
		{
			results.push_back(t0);
		}

		// Check if the second intesection is within the minimum bounds
		double y1 = r.origin.y + (t1 * r.direction.y);
		if (this->minimum < y1 && y1 < this->maximum)
		{
			results.push_back(t1);
		}
	}
	else if (abs(b) > std::numeric_limits<double>::epsilon())
	{
		// The Ray is parralel to one of the cone halves, but will hit the other
		double t = -(c / (2.0 * b));

		// Check if the intesection is within the minimum bounds
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

Tuple DoubleNappedCone::local_normal_at(const Tuple & object_space_point) const
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

void DoubleNappedCone::intersect_caps_(const Ray & r, std::vector<double>& xs) const
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

bool DoubleNappedCone::check_caps_(const Ray & r, const double & t, const double & radius) const
{
	// Checks to see if the intersection at 't' is within the radius of the Cone cap
	// Prevents duplications on a corner hit
	double x = r.origin.x + (t * r.direction.x);
	double z = r.origin.z + (t * r.direction.z);

	// The radius is equal to the truncation length for this cap (minuimum or maximum)
	// Because these could be negative, it's simpler to abs the radius here
	return ((x * x) + (z * z)) <= (radius * radius);
}


// ------------------------------------------------------------------------
//
// Cone (Just a pre-truncated DoubleNappedCone)
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Cone::Cone() : DoubleNappedCone(-1.0, 0.0, "Default Cone 000")
{
}

Cone::Cone(std::string name) : DoubleNappedCone(-1.0, 0.0, name)
{
}

Cone::~Cone()
{
}

// ------------------------------------------------------------------------
//
// Test Shape
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

TestShape::TestShape()
{
}

TestShape::TestShape(std::string)
{
}

TestShape::~TestShape()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<double> TestShape::local_intersect_t(const Ray & r) const
{
	return std::vector<double>();
}

Tuple TestShape::local_normal_at(const Tuple & object_space_point) const
{
	return Tuple::Vector(object_space_point.x, object_space_point.y, object_space_point.z);
}

