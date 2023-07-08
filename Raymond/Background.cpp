#include "Background.h"

// ------------------------------------------------------------------------
//
// Default Background
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Background::Background()
= default;

Background::~Background()
= default;

// ------------------------------------------------------------------------
// sample_at
// ------------------------------------------------------------------------

Sample Background::sample_at(IxComps & comps) const
{
    Sample sample = Sample();
    sample.set_background(0.0);
    sample.set_diffuse(0.0);
    sample.set_alpha(0.0);
	return sample;
}

// ------------------------------------------------------------------------
//
// Normal Gradient Background
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

NormalGradientBackground::NormalGradientBackground()
= default;

NormalGradientBackground::~NormalGradientBackground()
= default;

// ------------------------------------------------------------------------
// sample_at
// ------------------------------------------------------------------------

Sample NormalGradientBackground::sample_at(IxComps & comps) const
{
    Sample sample = Sample();

	Tuple n = comps.normal_v - Tuple::Vector(0.0, 0.0, -1.0);
	Color grad = Color(n.x + 1.0, n.y + 1.0, n.z + 1.0) * 0.5;

    sample.set_background(grad);
    sample.set_diffuse(0.0);
    sample.set_alpha(0.0);
    return sample;
}

// ------------------------------------------------------------------------
//
// Hosek Wilkie Background
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

HosekWilkieBackground::HosekWilkieBackground(const Tuple & sun_vector)
{
	this->sun_vector_ = sun_vector;
}

HosekWilkieBackground::~HosekWilkieBackground()
= default;

Sample HosekWilkieBackground::sample_at(IxComps & comps) const
{
    Sample sample = Sample();
    sample.set_background(0.0);
    sample.set_diffuse(0.0);
    sample.set_alpha(0.0);
    return sample;
}
