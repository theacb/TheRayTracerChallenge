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
	Light(Color color, float multiplier);
	~Light();

	virtual std::vector<float> local_intersect_t(const Ray &) const = 0;
	virtual Tuple normal_at(const Tuple &) const = 0;

	//properties
	Color color;
	float multiplier;

	bool falloff;
	float cutoff;

	// Methods
	Tuple position() const;
};

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(Tuple position, Color color);
	PointLight(Tuple position, Color color, float multiplier);
	~PointLight();

	// Methods
	virtual std::vector<float> local_intersect_t(const Ray & ray) const override;
	virtual Tuple normal_at(const Tuple & point) const override;
};

//Overloaded Operators
bool operator==(const PointLight & left_light, const PointLight & right_light);
bool operator!=(const PointLight & left_light, const PointLight & right_light);

#endif
