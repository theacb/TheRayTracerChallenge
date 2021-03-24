#include "pch.h"
#include "Texmap.h"

// ------------------------------------------------------------------------
//
// TexMap Base
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

TexMap::TexMap()
{
	this->transform_ = Matrix4::Identity();
	this->inverse_transform_ = Matrix4::Identity();
	this->mapping_space_ = WorldSpace;
}

TexMap::~TexMap()
{
}



// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void TexMap::set_transform(Matrix4 m)
{
	this->transform_ = m;
	this->inverse_transform_ = m.inverse();
}

Matrix4 TexMap::get_transform() const
{
	return this->transform_;
}

Matrix4 TexMap::get_inverse_transform() const
{
	return this->inverse_transform_;
}

void TexMap::set_mapping_space(MappingSpace space)
{
	this->mapping_space_ = space;
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Color TexMap::sample_at(const IxComps & comps) const
{
	// Make a copy to edit the point
	IxComps transformed_comps = IxComps(comps);

	// accomodate different mapping types
	switch (this->mapping_space_)
	{
	// Just transforms by the pattern's mapping matrix
	case WorldSpace:
		transformed_comps.texmap_point = this->inverse_transform_ * comps.point;
		break;

	// Also transforms by the object's transformation matrix
	case ObjectSpace:
		Matrix4 transform = this->inverse_transform_ * comps.object->get_inverse_transform();
		transformed_comps.texmap_point = transform * comps.point;
		break;
	}

	return this->local_sample_at(transformed_comps);
}

// Constructs an IxComps to execute shading function.
// Allows test of the shading function with only a point.
Color TexMap::sample_at_point(const Tuple & point) const
{
	IxComps dummy = IxComps();
	dummy.point = point;
	dummy.normal_v = Tuple::Vector(0.0, 1.0, 0.0);
	return this->sample_at(dummy);
}

// ------------------------------------------------------------------------
//
// Stripe Map
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

TestMap::TestMap() : TexMap()
{
}

TestMap::~TestMap()
{
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Color TestMap::local_sample_at(const IxComps & comps) const
{
	return Color(comps.texmap_point);
}

// ------------------------------------------------------------------------
//
// Stripe Map
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

StripeMap::StripeMap() : StripeMap(Color(0.0), Color(1.0))
{
}

StripeMap::StripeMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b)
{
	this->a = a;
	this->b = b;
}

StripeMap::StripeMap(Color a, Color b) : StripeMap(
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(a)),
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(b))
)
{
}

StripeMap::~StripeMap()
{
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Color StripeMap::local_sample_at(const IxComps & comps) const
{
	// returns color a when even on x axis and color b when odd
	if ((int(floor(comps.texmap_point.x)) % 2) == 0)
	{
		return this->a->sample_at(comps);
	}
	else
	{
		return this->b->sample_at(comps);
	}
}

// ------------------------------------------------------------------------
//
// Solid Color Map
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

SolidColorMap::SolidColorMap() : SolidColorMap(Color(0.0))
{
}

SolidColorMap::SolidColorMap(Color col) : TexMap()
{
	this->col = col;
}

SolidColorMap::~SolidColorMap()
{
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Color SolidColorMap::local_sample_at(const IxComps & comps) const
{
	return this->col;
}

// ------------------------------------------------------------------------
//
// Blend Map
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

GradientMap::GradientMap() : GradientMap(Color(0.0), Color(1.0))
{
}

GradientMap::GradientMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b) : TexMap()
{
	this->a = a;
	this->b = b;
	this->clamp_fraction = true;
}

GradientMap::GradientMap(Color a, Color b) : GradientMap(
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(a)),
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(b))
)
{
}

GradientMap::~GradientMap()
{
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Color GradientMap::local_sample_at(const IxComps & comps) const
{
	double fraction = comps.texmap_point.x;

	// Clamps the gradient between 0.0 and 1.0
	if (this->clamp_fraction)
	{
		fraction = clip(fraction, 0.0, 0.999999);
	}

	return lerp(fraction, a->sample_at(comps), b->sample_at(comps));
}

// ------------------------------------------------------------------------
//
// Ring Map
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

RingMap::RingMap() : RingMap(Color(0.0), Color(1.0))
{
}

RingMap::RingMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b) : TexMap()
{
	this->a = a;
	this->b = b;
}

RingMap::RingMap(Color a, Color b) : RingMap(
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(a)),
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(b))
)
{
}

RingMap::~RingMap()
{
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Color RingMap::local_sample_at(const IxComps & comps) const
{
	double x = comps.texmap_point.x;
	double z = comps.texmap_point.z;
	// returns color a when even on x and z axis and color b when odd
	if ((int(floor(sqrt((x * x) + (z * z)))) % 2) == 0)
	{
		return this->a->sample_at(comps);
	}
	else
	{
		return this->b->sample_at(comps);
	}
}

// ------------------------------------------------------------------------
//
// Checker Map
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

CheckerMap::CheckerMap() : CheckerMap(Color(0.0), Color(1.0))
{
}

CheckerMap::CheckerMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b) : TexMap()
{
	this->a = a;
	this->b = b;
}

CheckerMap::CheckerMap(Color a, Color b) : CheckerMap(
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(a)),
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(b))
)
{
}

CheckerMap::~CheckerMap()
{
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Color CheckerMap::local_sample_at(const IxComps & comps) const
{
	Tuple offset_point = comps.texmap_point + (comps.normal_v * EPSILON);
	double x = offset_point.x;
	double y = offset_point.y;
	double z = offset_point.z;
	// returns color a in a 3d checker pattern
	if (((int(floor(x)) + int(floor(y)) + int(floor(z))) % 2) == 0)
	{
		return this->a->sample_at(comps);
	}
	else
	{
		return this->b->sample_at(comps);
	}
}

// ------------------------------------------------------------------------
//
// Composite Map
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

CompositeMap::CompositeMap() : CompositeMap(Color(0.0), Color(1.0))
{
}

CompositeMap::CompositeMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b) : CompositeMap(a, b, CompAdd, 1.0)
{
}

CompositeMap::CompositeMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b, CompositeMode mode) : CompositeMap(a, b, mode, 1.0)
{
}

CompositeMap::CompositeMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b, CompositeMode mode, double factor) : TexMap()
{
	this->composite_mode = mode;
	this->a = a;
	this->b = b;
	this->factor = factor;
}

CompositeMap::CompositeMap(Color a, Color b) : CompositeMap(
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(a)),
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(b)),
	CompAdd,
	1.0
)
{
}

CompositeMap::CompositeMap(Color a, Color b, CompositeMode mode) : CompositeMap(
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(a)),
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(b)),
	mode,
	1.0
)
{
}

CompositeMap::CompositeMap(Color a, Color b, CompositeMode mode, double factor) : CompositeMap(
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(a)),
	std::dynamic_pointer_cast<TexMap>(std::make_shared<SolidColorMap>(b)),
	mode,
	factor
)
{
}

CompositeMap::~CompositeMap()
{
}

// ------------------------------------------------------------------------
// Shade
// ------------------------------------------------------------------------

Color CompositeMap::local_sample_at(const IxComps & comps) const
{
	Color result;
	Color a = this->a->sample_at(comps);
	Color b = this->b->sample_at(comps);

	switch (this->composite_mode)
	{
	case CompBlend:
		result = lerp(this->factor, a, b);
		break;
	case CompAdd:
		result = a.add(b, this->factor);
		break;
	case CompMultiply:
		result = a.multiply(b, this->factor);
		break;
	case CompDivide:
		result = a.divide(b, this->factor);
		break;
	case CompSubtract:
		result = a.subtract(b, this->factor);
		break;
	case CompOverlay:
		result = a.overlay(b, this->factor);
		break;
	case CompScreen:
		result = a.screen(b, this->factor);
		break;
	default:
		result = b;
		break;
	}

	return result;
}
