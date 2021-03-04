#pragma once

#include <memory> // Shared Pointers
#include <limits> // Infinity

#include "Object.h"
#include "Ray.h"
#include "Tuple.h"

class IxComps
{
public:
	IxComps();
	IxComps(Intersection &, Ray &);
	~IxComps();

	static IxComps Background(Ray &);


	// Properties
	std::shared_ptr<ObjectBase> object;
	Tuple point;
	Tuple eye_v;
	Tuple normal_v;
	bool inside;
	float t_value;
};

