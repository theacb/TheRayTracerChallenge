#include "pch.h"
#include "Quadtree.h"

// ------------------------------------------------------------------------
//
// AABB
//
// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------

AABB2D::AABB2D() : AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0))
{
}

AABB2D::AABB2D(const Tuple northeast, const Tuple southwest)
{
	this->ne_corner = northeast;
	this->sw_corner = southwest;

	this->half = Tuple::Point2D(
		fabs((northeast.x - southwest.x) * 0.5), 
		fabs((northeast.y - southwest.y) * 0.5)
	);

	this->center = Tuple::Point2D(
		northeast.x + this->half.x,
		southwest.y + this->half.y
	);

	this->height = fabs(this->ne_corner.y - this->sw_corner.y);
	this->width = fabs(this->ne_corner.x - this->sw_corner.x);
}

AABB2D::AABB2D(const Tuple center, const double radius)
{
	this->half = Tuple::Point2D(radius, radius);
	this->center = center;

	double dim = radius * 2;

	this->height = dim;
	this->width = dim;

	this->ne_corner = this->center + this->half;
	this->sw_corner = this->center - this->half;
}

AABB2D::AABB2D(const AABB2D & src) : AABB2D(src.ne_corner, src.sw_corner)
{
}

AABB2D::~AABB2D()
{
}

bool AABB2D::intersects_aabb(const AABB2D box) const
{
	double dx = box.center.x - this->center.x;
	double px = (box.half.x + this->half.x) - fabs(dx);

	if (px <= 0.0)
	{
		return false;
	}

	double dy = box.center.y - this->center.y;
	double py = (box.half.y + this->half.y) - fabs(dy);

	if (py <= 0.0)
	{
		return false;
	}

	return true;
}

bool AABB2D::contains_point(const Tuple point) const
{
	// Greater and less than are used on each dimension to determine if the point is within the quadtree
	return (
		point.x >= this->ne_corner.x &&
		point.x <= this->sw_corner.x &&
		point.y >= this->ne_corner.y &&
		point.y <= this->sw_corner.y
		);
}
