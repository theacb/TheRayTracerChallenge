#ifndef H_RAYMOND_WORLD
#define H_RAYMOND_WORLD

#include <vector>

#include "Object.h"
#include "IxComps.h"
#include "Primitive.h"
#include "Tuple.h"
#include "Color.h"
#include "Background.h"
#include "Sample.h"

class World
{
public:
	World();
	~World();

	// METHODS
	// static
	static World Default();

	// Intersector
	[[nodiscard]] Intersections intersect_world(const Ray & ray) const;

	// Shade
	Sample shade(IxComps& comps) const;
	[[nodiscard]] Color color_at(const Ray & ray) const;
    Sample sample_at(const Ray & ray) const;
	[[nodiscard]] bool is_shadowed(std::shared_ptr<Light> light, const Tuple & point) const;
	[[nodiscard]] Color shadowed(std::shared_ptr<Light> light, const Tuple & point, int depth) const;

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
