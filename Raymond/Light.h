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
	Light(const Color& color);
	Light(const Color& color, double multiplier);
	~Light();

	// properties
	Color color;
	double multiplier;

	bool falloff;
	double cutoff;

     // Methods
     Tuple position() const;
     virtual Tuple area_position() const;
};

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(const Tuple& position, const Color& color);
	PointLight(const Tuple& position, const Color& color, double multiplier);
    PointLight(const Tuple& position, const Color& color, double multiplier, double radius);
	~PointLight();

    // Methods
    Tuple area_position() const override;

    // properties
    double radius;
};

//Overloaded Operators
bool operator==(const PointLight & left_light, const PointLight & right_light);
bool operator!=(const PointLight & left_light, const PointLight & right_light);

#endif
