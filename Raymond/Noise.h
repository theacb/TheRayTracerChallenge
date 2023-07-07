#ifndef H_RAYMOND_NOISE
#define H_RAYMOND_NOISE

#include <algorithm>
#include <numeric>
#include <random>

#include "Utilities.h"
#include "Tuple.h"
#include "Object.h"
#include "Texmap.h"

class PerlinMap :
	public TexMap
{
	std::vector<int> p;
public:
	PerlinMap();
	PerlinMap(int seed);
	~PerlinMap();

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const override;

	// Properties
	int octaves;
	double persistence;

private:
	// Perlin Noise 
	// static
	static double p_fade_(const double & t);
	static double p_grad_(int hash, double x, double y, double z);

	double p_perlin_(double x, double y, double z) const;
	double p_octave_perlin_(double x, double y, double z) const;

	static double p_lerp_(double t, double a, double b);
};

class ColoredPerlin :
	public TexMap
{
public:
	ColoredPerlin();
	ColoredPerlin(int seed);
	~ColoredPerlin();

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const override;

	// Properties
	int octaves;
	double persistence;
private:
	std::shared_ptr<PerlinMap> r_, g_, b_;
};

#endif
