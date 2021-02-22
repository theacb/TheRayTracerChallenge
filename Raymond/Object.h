#pragma once

#include <vector>
#include <algorithm> 
#include "Ray.h"
#include "Tuple.h"
#include <string>

class ObjectBase;
using ObjectPtr = ObjectBase * ;
using ConstObjectPtr = const ObjectBase *;

class Intersection
{
public:
	Intersection();
	Intersection(float, ConstObjectPtr);
	~Intersection();

	// Methods
	bool is_valid() const;

	// Properties
	float t_value; //Depth
	ConstObjectPtr object;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream &, const Intersection &);

	friend bool operator==(const Intersection &, const Intersection &);
	friend bool operator!=(const Intersection &, const Intersection &);
	friend bool operator<(const Intersection &, const Intersection &);

};

class Intersections :
	public std::vector<Intersection>
{
public:
	Intersections();
	Intersections(std::initializer_list<Intersection>);
	Intersections(std::vector<float>, ConstObjectPtr);

	// Methods
	Intersection hit() const;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream &, const Intersections &);
};

class ObjectBase
{
public:
	ObjectBase();
	~ObjectBase();

	// Methods
	virtual std::vector<float> intersect_t(Ray) const = 0;

	// Accessors
	void set_name(std::string);
	std::string get_name() const;

	void set_transform(Matrix4);
	Matrix4 get_transform() const;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream &, const ObjectBase &);

private:
	//properties
	Matrix4 transform_;
	std::string name_;

};

class Sphere :
	public ObjectBase
{
public:
	Sphere();
	~Sphere();

	//properties
	float radius;

	// Methods
	std::vector<float> intersect_t(Ray) const;
};

Intersections intersect(Ray &, ConstObjectPtr);
