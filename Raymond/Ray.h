#ifndef H_RAYMOND_RAY
#define H_RAYMOND_RAY

#include "Tuple.h"
#include "Matrix.h"

class Ray
{
public:
	Ray();
	Ray(const Ray & src);
	Ray(Tuple origin, Tuple direction);
	Ray(Tuple origin, Tuple direction, int depth);
	~Ray();

	// Methods
	Tuple position(double) const;

	Ray transform(Matrix4) const;

	// Properties
	Tuple origin;
	Tuple direction;
	Tuple dir_mult_inv;
	int depth;

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream &, const Ray &);
};

#endif
