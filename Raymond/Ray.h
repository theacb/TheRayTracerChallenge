#ifndef H_RAYMOND_RAY
#define H_RAYMOND_RAY

#include "Tuple.h"
#include "Matrix.h"

class Ray
{
public:
	Ray();
	Ray(const Ray &);
	Ray(Tuple, Tuple);
	~Ray();

	// Methods
	Tuple position(float) const;

	Ray transform(Matrix4) const;

	// Properties
	Tuple origin;
	Tuple direction;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream &, const Ray &);
};

#endif
