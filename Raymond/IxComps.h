#ifndef H_RAYMOND_IXCOMPS
#define H_RAYMOND_IXCOMPS

#include <memory> // Shared Pointers
#include <limits> // Infinity

#include "Object.h"
#include "Ray.h"
#include "Tuple.h"
#include "Color.h"
#include "Constants.h"

class PrimitiveBase;

class IxComps
{
public:
	IxComps();
	IxComps(const Intersection & ix, const Ray & ray);
	IxComps(const Intersection & ix, const Ray & ray, const Intersections & xs);
	IxComps(const IxComps & src);
	~IxComps();

	// Factories
	static IxComps Background(const Ray &);


	// Properties
	std::shared_ptr<ObjectBase> object;
	Tuple point;
	Tuple texmap_point;
	Tuple over_point;
	Tuple under_point;
	Tuple eye_v;
	Tuple normal_v;
	Tuple reflect_v;
	int ray_depth;
	bool inside;
	double t_value;
	Color shadow_multiplier;
	double n1, n2;
};

#endif
