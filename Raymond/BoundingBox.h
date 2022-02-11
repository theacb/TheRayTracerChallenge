#ifndef H_RAYMOND_BOUNDINGBOX
#define H_RAYMOND_BOUNDINGBOX

#include "Tuple.h"
#include "Ray.h"

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(Tuple min, Tuple max);
	~BoundingBox();

	// Properties
	Tuple minimum, maximum;

	// Methods
	void transform(const Matrix4 & m);

	bool intersect(const Ray & r) const;
};

class BoundingVolumeNode
{
public:
	BoundingVolumeNode();
	~BoundingVolumeNode();

private:
	// Properties

	BoundingBox bv_bbox;
};

#endif
