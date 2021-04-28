#ifndef H_RAYMOND_PRIMITIVEDEFINITION
#define H_RAYMOND_PRIMITIVEDEFINITION

#include <vector>

#include "Tuple.h"
#include "Ray.h"

class PrimitiveDefinition
{
public:
	PrimitiveDefinition();
	~PrimitiveDefinition();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const = 0;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const = 0;
};

class SphereDefinition :
	public PrimitiveDefinition
{
public:
	SphereDefinition();
	~SphereDefinition();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;
};

class InfinitePlaneDefinition :
	public PrimitiveDefinition
{
public:
	InfinitePlaneDefinition();
	~InfinitePlaneDefinition();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;
};

class CubeDefinition :
	public PrimitiveDefinition
{
public:
	CubeDefinition();
	~CubeDefinition();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;

private:
	std::vector<double> check_axis_(const double & origin, const double & direction) const;
};

class CylinderDefinition :
	public PrimitiveDefinition
{
public:
	CylinderDefinition();
	CylinderDefinition(double minimum, double maximum, bool closed);
	~CylinderDefinition();

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

class DoubleNappedConeDefinition :
	public PrimitiveDefinition
{
public:
	DoubleNappedConeDefinition();
	DoubleNappedConeDefinition(double minimum, double maximum, bool closed);
	~DoubleNappedConeDefinition();

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

class NullShapeDefinition :
	public PrimitiveDefinition
{
public:
	NullShapeDefinition();
	~NullShapeDefinition();

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;
};

#endif
