#ifndef H_RAYMOND_OBJECT
#define H_RAYMOND_OBJECT

#include <vector>
#include <algorithm> 
#include <string>
#include <memory> // Shared Pointers

#include "Ray.h"
#include "Tuple.h"
#include "PrimitiveDefinition.h"

enum ObjectType { primitive, light };

class TransformController
{
public:
	TransformController();
	TransformController(const Matrix4 & m);
	TransformController(const TransformController & src);
	~TransformController();

	// Methods
	void set_transform(const Matrix4 & m);
	Matrix4 get_transform() const;
	Matrix4 get_inverse_transform() const;

	// Self Transformers
	Ray ray_to_object_space(const Ray & r) const;

	Tuple point_to_object_space(const Tuple & p) const;
	Tuple point_to_world_space(const Tuple & p) const;

	Tuple normal_vector_to_world_space(const Tuple & v) const;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream & os, const TransformController & ctrl);

private:
	//properties
	Matrix4 x_transform_;
	Matrix4 x_inverse_transform_;
};

class ObjectBase
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	// Methods
	Tuple normal_at(const Tuple & world_space_point) const;
	std::vector<double> intersect_t(const Ray & r) const;

	// Accessors
	void set_name(std::string new_name);
	std::string get_name() const;

	void set_definition(std::shared_ptr<PrimitiveDefinition> def);
	std::shared_ptr<PrimitiveDefinition> get_definition();
	std::shared_ptr<PrimitiveDefinition> get_definition() const;

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
	std::string o_name_;
	std::shared_ptr<TransformController> o_transform_;
	std::shared_ptr<PrimitiveDefinition> o_definition_;
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
	int get_hit_index() const;
	void calculate_hit();

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream & os, const Intersections & ixs);

private:
	// Methods
	int hit_() const;
	// Properties
	int hit_index_;
};

// Single Functions
Intersections intersect(const Ray & r, const std::shared_ptr<ObjectBase> obj);

#endif
