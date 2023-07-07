#ifndef H_RAYMOND_LIGHT
#define H_RAYMOND_LIGHT


#include "Object.h"
#include "Color.h"
#include "Matrix.h"
#include "Ray.h"

class Light :
	public ObjectBase
{
public:
	Light();
	Light(Color color);
	Light(Color color, double multiplier);
	~Light();

	//properties
	Color color;
	double multiplier;

	bool falloff;
	double cutoff;

	// Methods
	Tuple position() const;
};

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(Tuple position, Color color);
	PointLight(Tuple position, Color color, double multiplier);
	~PointLight();
};

//Overloaded Operators
bool operator==(const PointLight & left_light, const PointLight & right_light);
bool operator!=(const PointLight & left_light, const PointLight & right_light);

#endif
