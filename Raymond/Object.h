#pragma once

#include <vector>
#include <algorithm> 
#include <string>
#include <memory> // Shared Pointers

#include "Ray.h"
#include "Tuple.h"

enum ObjectType { primitive, light };

class ObjectBase
{
public:
	ObjectBase();
	~ObjectBase();

	// Methods
	virtual std::vector<float> local_intersect_t(const Ray &) const = 0;
	virtual Tuple normal_at(const Tuple &) const = 0;

	// Accessors
	void set_name(std::string);
	std::string get_name() const;

	void set_transform(Matrix4);
	Matrix4 get_transform() const;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream &, const ObjectBase &);

	//properties
	ObjectType object_type;

private:
	//properties
	Matrix4 transform_;
	std::string name_;

};

class Intersection
{
public:
	Intersection();
	Intersection(float, std::shared_ptr<ObjectBase>);
	~Intersection();

	// Methods
	bool is_valid() const;

	// Properties
	float t_value; //Depth
	std::shared_ptr<ObjectBase> object;

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
	Intersections(std::vector<float>, std::shared_ptr<ObjectBase>);

	// Methods
	Intersection hit() const;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream &, const Intersections &);
};

Intersections intersect(Ray &, std::shared_ptr<ObjectBase>);
