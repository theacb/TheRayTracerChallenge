#ifndef H_RAYMOND_BACKGROUND
#define H_RAYMOND_BACKGROUND

#include "IxComps.h"
#include "Color.h"

class Background
{
public:
	Background();
	~Background();

	// Methods
	virtual Color shade(IxComps & comps) const;
};

class NormalGradientBackground :
	public Background
{
public:
	NormalGradientBackground();
	~NormalGradientBackground();

	// Methods
	virtual Color shade(IxComps & comps) const override;
};

class HosekWilkietBackground :
	public Background
{
public:
	HosekWilkietBackground(const Tuple & sun_vector);
	~HosekWilkietBackground();

	// Methods
	virtual Color shade(IxComps & comps) const override;

private:
	// Properties
	Tuple sun_vector_;
};

#endif
