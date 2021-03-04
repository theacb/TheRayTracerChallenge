#pragma once

#include "IxComps.h"
#include "Color.h"

class Background
{
public:
	Background();
	~Background();

	// Methods
	virtual Color shade(IxComps &) const;
};

class NormalGradientBackground :
	public Background
{
public:
	NormalGradientBackground();
	~NormalGradientBackground();

	// Methods
	virtual Color shade(IxComps &) const override;
};

