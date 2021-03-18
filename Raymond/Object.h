#ifndef H_RAYMOND_OBJECT
#define H_RAYMOND_OBJECT

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
	virtual std::vector<double> local_intersect_t(const Ray &) const = 0;
	virtual Tuple local_normal_at(const Tuple &) const = 0;
	Tuple normal_at(const Tuple & world_space_point) const;

	// Accessors
	void set_name(std::string new_name);
	std::string get_name() const;

	void set_transform(Matrix4 m);
	Matrix4 get_transform() const;
	Matrix4 get_inverse_transform() const;

	// Self Transformers
	Ray ray_to_object_space(const Ray & r) const;

	Tuple point_to_object_space(const Tuple & p) const;
	Tuple point_to_world_space(const Tuple & p) const;

	Tuple normal_vector_to_world_space(const Tuple & v) const;


	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream & os, const ObjectBase & obj);

	//properties
	ObjectType object_type;

private:
	//properties
	Matrix4 transform_;
	Matrix4 inverse_transform_;
	std::string name_;

};

class Intersection
{
public:
	Intersection();
	Intersection(double t, std::shared_ptr<ObjectBase> obj);
	~Intersection();

	// Methods
	bool is_valid() const;

	// Properties
	double t_value; //Depth
	std::shared_ptr<ObjectBase> object;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream & os, const Intersection & ix);

	friend bool operator==(const Intersection & l_ix, const Intersection & r_ix);
	friend bool operator!=(const Intersection & l_ix, const Intersection & r_ix);
	friend bool operator<(const Intersection & l_ix, const Intersection & r_ix);

};

class Intersections :
	public std::vector<Intersection>
{
public:
	Intersections();
	Intersections(std::initializer_list<Intersection> il);
	Intersections(std::vector<double> t_values, std::shared_ptr<ObjectBase> objs);

	// Methods
	Intersection hit() const;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream & os, const Intersections & ixs);
};

Intersections intersect(const Ray & r, const std::shared_ptr<ObjectBase> obj);

#endif
