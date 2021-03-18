#include "pch.h"
#include "Background.h"

// ------------------------------------------------------------------------
//
// Default Background
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Background::Background()
{
}

Background::~Background()
{
}

// ------------------------------------------------------------------------
// shade
// ------------------------------------------------------------------------

Color Background::shade(IxComps & comps) const
{
	return Color(0.0, 0.0, 0.0);
}

// ------------------------------------------------------------------------
//
// Normal Gradient Background
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

NormalGradientBackground::NormalGradientBackground()
{
}

NormalGradientBackground::~NormalGradientBackground()
{
}

// ------------------------------------------------------------------------
// shade
// ------------------------------------------------------------------------

Color NormalGradientBackground::shade(IxComps & comps) const
{
	Tuple n = comps.normal_v - Tuple::Vector(0.0, 0.0, -1.0);
	return Color(n.x + 1.0, n.y + 1.0, n.z + 1.0) * 0.5;
}

// ------------------------------------------------------------------------
//
// Hosek Wilkie Background
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

HosekWilkietBackground::HosekWilkietBackground(const Tuple & sun_vector)
{
	this->sun_vector_ = sun_vector;
}

HosekWilkietBackground::~HosekWilkietBackground()
{
}

Color HosekWilkietBackground::shade(IxComps & comps) const
{
	// TODO: Implement this maybe someday lol
	return Color(1.0);
}
