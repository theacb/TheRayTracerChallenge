#ifndef H_RAYMOND_PRIMITIVE
#define H_RAYMOND_PRIMITIVE

#include <memory> // Shared Pointers

#include "Object.h"
#include "Material.h"



class Primitive :
	public ObjectBase
{
public:
	Primitive();
	~Primitive();

	//properties
	std::shared_ptr<BaseMaterial> material;

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const = 0;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const = 0;

};

class Sphere :
	public Primitive
{
public:
	Sphere();
	Sphere(std::string name);
	~Sphere();

	// Properties
	double radius;

	// Static Methods
	static std::shared_ptr<Sphere> GlassSphere();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;
};

class InfinitePlane :
	public Primitive
{
public:
	InfinitePlane();
	InfinitePlane(std::string name);
	~InfinitePlane();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;
};

class Cube :
	public Primitive
{
public:
	Cube();
	Cube(std::string name);
	~Cube();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;

private:
	std::vector<double> check_axis_(const double & origin, const double & direction) const;
};

class Cylinder :
	public Primitive
{
public:
	Cylinder();
	Cylinder(std::string name);
	Cylinder(double minimum, double maximum);
	Cylinder(double minimum, double maximum, std::string name);
	~Cylinder();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;

	// Properties
	double minimum;
	double maximum;
	bool closed;
private:
	void intersect_caps_(const Ray & r, std::vector<double> & xs) const;
	bool check_caps_(const Ray & r, const double & t) const;
};

class DoubleNappedCone :
	public Primitive
{
public:
	DoubleNappedCone();
	DoubleNappedCone(std::string name);
	DoubleNappedCone(double minimum, double maximum);
	DoubleNappedCone(double minimum, double maximum, std::string name);
	~DoubleNappedCone();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;

	// Properties
	double minimum;
	double maximum;
	bool closed;
private:
	void intersect_caps_(const Ray & r, std::vector<double> & xs) const;
	bool check_caps_(const Ray & r, const double & t, const double & radius) const;
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
	public Primitive
{
public:
	TestShape();
	TestShape(std::string name);
	~TestShape();

	// Properties
	Ray ray;

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;
};

#endif
