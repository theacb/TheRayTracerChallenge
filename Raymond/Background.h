#ifndef H_RAYMOND_BACKGROUND
#define H_RAYMOND_BACKGROUND

#include "IxComps.h"
#include "Sample.h"

class Background
{
public:
	Background();
	~Background();

	// Methods
	virtual Sample sample_at(IxComps & comps) const;
};

class NormalGradientBackground :
	public Background
{
public:
	NormalGradientBackground();
	~NormalGradientBackground();

	// Methods
	Sample sample_at(IxComps & comps) const override;
};

class HosekWilkieBackground :
	public Background
{
public:
	explicit HosekWilkieBackground(const Tuple & sun_vector);
	~HosekWilkieBackground();

	// Methods
	Sample sample_at(IxComps & comps) const override;

private:
	// Properties
	Tuple sun_vector_;
};

#endif
