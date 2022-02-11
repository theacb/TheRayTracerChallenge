#include "pch.h"
#include "Primitive.h"

// ------------------------------------------------------------------------
//
// Primitive
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

PrimitiveBase::PrimitiveBase() : ObjectBase()
{
	this->set_name("Default PrimitiveBase 000");
	this->material = std::make_shared<PhongMaterial>();
}


PrimitiveBase::~PrimitiveBase()
{
}

// ------------------------------------------------------------------------
//
// Sphere
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Sphere::Sphere() : Sphere("Default Sphere 000")
{
}

Sphere::Sphere(std::string name) : PrimitiveBase()
{
	this->set_definition(std::make_shared<SphereDefinition>());
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

	auto ms = std::static_pointer_cast<PhongMaterial>(s->material);
	ms->refraction.set_value(Color(1.0));
	ms->ior.set_value(1.5);

	return s;
}

// ------------------------------------------------------------------------
//
// Infinite Plane
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

InfinitePlane::InfinitePlane() : InfinitePlane("Default Sphere 000")
{
}

InfinitePlane::InfinitePlane(std::string name) : PrimitiveBase()
{
	this->set_definition(std::make_shared<InfinitePlaneDefinition>());
	this->set_name(name);
}

InfinitePlane::~InfinitePlane()
{
}

// ------------------------------------------------------------------------
//
// Cube
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Cube::Cube() : Cube("Default Cube 000")
{
}

Cube::Cube(std::string name) : PrimitiveBase()
{
	this->set_definition(std::make_shared<CubeDefinition>());
	this->set_name(name);
}

Cube::~Cube()
{
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

Cylinder::Cylinder(double minimum, double maximum, std::string name) : PrimitiveBase()
{
	this->set_definition(std::make_shared<CylinderDefinition>(minimum, maximum, false));
	this->set_name(name);
}

Cylinder::~Cylinder()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

bool Cylinder::get_closed() const
{
	auto def = std::static_pointer_cast<CylinderDefinition>(this->get_definition());
	return def->closed;
}

void Cylinder::set_closed(const bool & closed)
{
	auto def = std::static_pointer_cast<CylinderDefinition>(this->get_definition());
	def->closed = closed;
}

double Cylinder::get_minimum() const
{
	auto def = std::static_pointer_cast<CylinderDefinition>(this->get_definition());
	return def->minimum;
}

void Cylinder::set_minimum(const double & min)
{
	auto def = std::static_pointer_cast<CylinderDefinition>(this->get_definition());
	def->minimum = min;
}

double Cylinder::get_maximum() const
{
	auto def = std::static_pointer_cast<CylinderDefinition>(this->get_definition());
	return def->maximum;
}

void Cylinder::set_maximum(const double & max)
{
	auto def = std::static_pointer_cast<CylinderDefinition>(this->get_definition());
	def->minimum = max;
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

DoubleNappedCone::DoubleNappedCone(double minimum, double maximum, std::string name) : PrimitiveBase()
{
	this->set_definition(std::make_shared<DoubleNappedConeDefinition>(minimum, maximum, false));
	this->set_name(name);
}

DoubleNappedCone::~DoubleNappedCone()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

bool DoubleNappedCone::get_closed() const
{
	auto def = std::static_pointer_cast<DoubleNappedConeDefinition>(this->get_definition());
	return def->closed;
}

void DoubleNappedCone::set_closed(const bool & closed)
{
	auto def = std::static_pointer_cast<DoubleNappedConeDefinition>(this->get_definition());
	def->closed = closed;
}

double DoubleNappedCone::get_minimum() const
{
	auto def = std::static_pointer_cast<DoubleNappedConeDefinition>(this->get_definition());
	return def->minimum;
}

void DoubleNappedCone::set_minimum(const double & min)
{
	auto def = std::static_pointer_cast<DoubleNappedConeDefinition>(this->get_definition());
	def->minimum = min;
}

double DoubleNappedCone::get_maximum() const
{
	auto def = std::static_pointer_cast<DoubleNappedConeDefinition>(this->get_definition());
	return def->maximum;
}

void DoubleNappedCone::set_maximum(const double & max)
{
	auto def = std::static_pointer_cast<DoubleNappedConeDefinition>(this->get_definition());
	def->minimum = max;
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

TestShape::TestShape() : TestShape("Default NullShape 000")
{
}

TestShape::TestShape(std::string name) : PrimitiveBase()
{
	this->set_name(name);
}

TestShape::~TestShape()
{
}

// ------------------------------------------------------------------------
//
// Group
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Group::Group() : Group("Default Group 000")
{
}

Group::Group(std::string name) : PrimitiveBase()
{
	this->set_name(name);
	this->set_bounds_as_group(true);
}

Group::~Group()
{
}

