#ifndef H_RAYMOND_PRIMITIVE
#define H_RAYMOND_PRIMITIVE

#include <memory> // Shared Pointers

#include "Object.h"
#include "Material.h"
#include "PrimitiveDefinition.h"

class PrimitiveBase :
	public ObjectBase
{
public:
	PrimitiveBase();
	~PrimitiveBase();

	//properties
	std::shared_ptr<BaseMaterial> material;

};

class Sphere :
	public PrimitiveBase
{
public:
	Sphere();
	Sphere(std::string name);
	~Sphere();

	// Static Methods
	static std::shared_ptr<Sphere> GlassSphere();
};

class InfinitePlane :
	public PrimitiveBase
{
public:
	InfinitePlane();
	InfinitePlane(std::string name);
	~InfinitePlane();

};

class Cube :
	public PrimitiveBase
{
public:
	Cube();
	Cube(std::string name);
	~Cube();

};

class Cylinder :
	public PrimitiveBase
{
public:
	Cylinder();
	Cylinder(std::string name);
	Cylinder(double minimum, double maximum);
	Cylinder(double minimum, double maximum, std::string name);
	~Cylinder();

	// Accessors
	bool get_closed() const;
	void set_closed(const bool & closed);

	double get_minimum() const;
	void set_minimum(const double & min);

	double get_maximum() const;
	void set_maximum(const double & max);
};

class DoubleNappedCone :
	public PrimitiveBase
{
public:
	DoubleNappedCone();
	DoubleNappedCone(std::string name);
	DoubleNappedCone(double minimum, double maximum);
	DoubleNappedCone(double minimum, double maximum, std::string name);
	~DoubleNappedCone();

	// Accessors
	bool get_closed() const;
	void set_closed(const bool & closed);

	double get_minimum() const;
	void set_minimum(const double & min);

	double get_maximum() const;
	void set_maximum(const double & max);
};

// Cone (Just a pre-truncated DoubleNappedCone)
class Cone :
	public DoubleNappedCone
{
public:
	Cone();
	Cone(std::string name);
	~Cone();
};

class TestShape :
	public PrimitiveBase
{
public:
	TestShape();
	TestShape(std::string name);
	~TestShape();
};

#endif
