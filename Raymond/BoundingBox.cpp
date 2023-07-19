#include "BoundingBox.h"

// ------------------------------------------------------------------------
//
// BoundingBox
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

BoundingBox::BoundingBox() : BoundingBox(Tuple::Point(-1.0, -1.0, -1.0), Tuple::Point(1.0, 1.0, 1.0))
{
}

BoundingBox::BoundingBox(const Tuple& min, const Tuple& max)
{
	this->minimum = min;
	this->maximum = max;
}

BoundingBox::~BoundingBox()
= default;

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void BoundingBox::transform(const Matrix4 & m)
{
    this->minimum = m * this->minimum;
    this->maximum = m * this->maximum;
}

// Code by Tavian Barnes from Fast, Branchless Ray/Bounding Box Intersections, Part 2: NaNs
// https://tavianator.com/2015/ray_box_nan.html

bool BoundingBox::intersect(const Ray & r) const
{
	double t1 = (this->minimum[0] - r.origin[0]) * r.dir_mult_inv[0];
	double t2 = (this->maximum[0] - r.origin[0]) * r.dir_mult_inv[0];

	double tmin = std::min(t1, t2);
	double tmax = std::max(t1, t2);

	for (int i = 1; i < 3; ++i) {
		t1 = (this->minimum[i] - r.origin[i]) * r.dir_mult_inv[i];
		t2 = (this->maximum[i] - r.origin[i]) * r.dir_mult_inv[i];

		tmin = std::max(tmin, std::min(std::min(t1, t2), tmax));
		tmax = std::min(tmax, std::max(std::max(t1, t2), tmin));
	}

	return tmax > std::max(tmin, 0.0);
}

// ------------------------------------------------------------------------
//
// BoundingVolumeNode
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

BoundingVolumeNode::BoundingVolumeNode()
{
	this->bv_bbox = BoundingBox(Tuple::Point(0.0, 0.0, 0.0), Tuple::Point(0.0, 0.0, 0.0));
}

BoundingVolumeNode::~BoundingVolumeNode()
= default;
