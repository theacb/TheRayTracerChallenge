#ifndef H_RAYMOND_WORLD
#define H_RAYMOND_WORLD

#include <vector>

#include "Object.h"
#include "IxComps.h"
#include "Primitive.h"
#include "Tuple.h"
#include "Color.h"
#include "Background.h"

class World
{
public:
	World();
	~World();

	// METHODS
	// static
	static World Default();

	// Intersector
	Intersections intersect_world(const Ray & ray) const;

	// Shade
	Color shade(IxComps& comps) const;
	Color color_at(const Ray & ray) const;
	bool is_shadowed(const std::shared_ptr<Light> light, const Tuple & point) const;
	Color shadowed(const std::shared_ptr<Light> light, const Tuple & point, const int depth) const;

	// accessors
	const std::vector<std::shared_ptr<PrimitiveBase>> & get_primitives();
	const std::vector<std::shared_ptr<Light>> & get_lights();

	void remove_primitive(int index);
	void remove_light(int index);

	void add_object(std::shared_ptr<PrimitiveBase> obj);
	void add_object(std::shared_ptr<Light> obj);

	// Public Properties
	std::shared_ptr<Background> background;

private:
	// private properties
	std::vector<std::shared_ptr<PrimitiveBase>> w_primitives_;
	std::vector<std::shared_ptr<Light>> w_lights_;
};

#endif
