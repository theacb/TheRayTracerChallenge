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

	virtual std::vector<double> local_intersect_t(const Ray &) const = 0;
	virtual Tuple local_normal_at(const Tuple &) const = 0;

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

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & ray) const override;
	virtual Tuple local_normal_at(const Tuple & point) const override;
};

//Overloaded Operators
bool operator==(const PointLight & left_light, const PointLight & right_light);
bool operator!=(const PointLight & left_light, const PointLight & right_light);

#endif
