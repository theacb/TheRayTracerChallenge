#ifndef H_RAYMOND_IXCOMPS
#define H_RAYMOND_IXCOMPS

#include <memory> // Shared Pointers
#include <limits> // Infinity

#include "Object.h"
#include "Ray.h"
#include "Tuple.h"
#include "Constants.h"

class IxComps
{
public:
	IxComps();
	IxComps(const Intersection & ix, const Ray & ray);
	IxComps(const IxComps & src);
	~IxComps();

	// Factories
	static IxComps Background(const Ray &);


	// Properties
	std::shared_ptr<ObjectBase> object;
	Tuple point;
	Tuple over_point;
	Tuple eye_v;
	Tuple normal_v;
	bool inside;
	double t_value;
	double shadow_multiplier;
};

#endif
