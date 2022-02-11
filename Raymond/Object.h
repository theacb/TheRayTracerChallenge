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

class PrimitiveBase;
class ObjectBase;

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

class ObjectBase : public std::enable_shared_from_this<ObjectBase>
{
public:
	ObjectBase();
	virtual ~ObjectBase();

	std::shared_ptr<ObjectBase> get_ptr();

	// Methods
	Tuple normal_at(const Tuple & world_space_point) const;
	std::vector<double> intersect_t(const Ray & r) const;
	Intersections intersect_i(const Ray & r);

	// Parenting
	// Parents objects to this
	void parent_children(std::vector<std::shared_ptr<ObjectBase>> & children);
	void parent_children(std::shared_ptr<ObjectBase> & child);

	// Applies the parent's transformation matrix to the children and then unparents
	void freeze_children();

	// Accessors
	std::shared_ptr<ObjectBase> get_parent() const;
	bool has_parent() const;
	std::vector<std::shared_ptr<ObjectBase>> get_children() const;
	size_t num_children() const;

	void set_name(std::string new_name);
	std::string get_name() const;

	std::shared_ptr<PrimitiveDefinition> get_definition();
	std::shared_ptr<PrimitiveDefinition> get_definition() const;

	void set_transform(Matrix4 m);
	Matrix4 get_transform() const;
	Matrix4 get_world_transform() const;
	Matrix4 get_inverse_transform() const;

	bool bounds_as_group() const;

	// Self Transformers
	Ray ray_to_object_space(const Ray & r) const;

	Tuple point_to_object_space(const Tuple & p) const;
	Tuple point_to_world_space(const Tuple & p) const;

	Tuple normal_vector_to_world_space(const Tuple & v) const;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream & os, const ObjectBase & obj);

	//properties
	ObjectType object_type;

//protected:
	// Should only be set from derived class
	void set_definition(std::shared_ptr<PrimitiveDefinition> def);
	void set_bounds_as_group(bool bound_as_group);

private:
	// Methods
	void o_set_parent_(std::shared_ptr<ObjectBase> & parent_children);
	// Unparents this but does not remove it from Parent vector
	void o_unparent_();

	// Properties
	std::string o_name_;
	std::shared_ptr<TransformController> o_transform_;
	std::shared_ptr<PrimitiveDefinition> o_definition_;
	std::shared_ptr<ObjectBase> o_parent_;
	std::vector<std::shared_ptr<ObjectBase>> o_children_;
	bool o_bounds_as_group_;
};

#endif
