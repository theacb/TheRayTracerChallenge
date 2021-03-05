#pragma once

#include <vector>

#include "Object.h"
#include "IxComps.h"
#include "Primitive.h"
#include "Tuple.h"
#include "Color.h"
#include "Background.h"


static const float LIGHT_CUTOFF = 0.001f;
static const bool FALLOFF = true;

class World
{
public:
	World();
	~World();

	// METHODS
	// static
	static World Default();

	// Intersector
	Intersections intersect_world(Ray &) const;

	// Shade
	Color shade(IxComps&) const;
	Color color_at(Ray &) const;

	// accessors
	const std::vector<std::shared_ptr<Primitive>> & get_primitives();
	const std::vector<std::shared_ptr<Light>> & get_lights();

	void remove_primitive(int);
	void remove_light(int);

	void add_object(std::shared_ptr<Primitive>);
	void add_object(std::shared_ptr<Light>);

	// Public Properties
	std::shared_ptr<Background> background;

private:
	// private properties
	std::vector<std::shared_ptr<Primitive>> w_primitives_;
	std::vector<std::shared_ptr<Light>> w_lights_;
};

