#include "pch.h"
#include "Noise.h"

// ------------------------------------------------------------------------
//
// Perlin Noise
//
// ------------------------------------------------------------------------
// Taken from Paul Silisteanu's C++ implementation
//
// https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
//
// With some suggestions from Adrian Biagioli
//
// https://adrianb.io/2014/08/09/perlinnoise.html
//
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN
//
// ------------------------------------------------------------------------
// Static Members
// ------------------------------------------------------------------------

// Hash lookup table as defined by Ken Perlin.  This is a randomly
// arranged array of all numbers from 0-255 inclusive.

// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

PerlinMap::PerlinMap()
{
	// Intialize with the refernce values
	this->p = { 151, 160, 137, 91, 90, 15,131, 13, 201, 95, 96, 53, 194, 233, 7, 225,
		140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62,
		94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168,
		68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
		220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
		187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52,
		217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58,
		17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167,
		43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
		228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
		49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236,
		205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
	};

	// Duplicate the vector
	this->p.insert(this->p.end(), this->p.begin(), this->p.end());

	this->octaves = 3;
	this->persistence = 1.0;
}

// Generate a new permutation vector based on the value of seed
PerlinMap::PerlinMap(int seed) {
	this->p.resize(256);

	// Fill p with values from 0 to 255
	std::iota(this->p.begin(), this->p.end(), 0);

	// Initialize a random engine with seed
	std::default_random_engine engine(seed);

	// Suffle using the above random engine
	std::shuffle(this->p.begin(), this->p.end(), engine);

	// Duplicate the permutation vector
	this->p.insert(this->p.end(), this->p.begin(), this->p.end());

	this->octaves = 3;
	this->persistence = 1.0;
}

PerlinMap::~PerlinMap()
{
}

Color PerlinMap::local_sample_at(const IxComps & comps) const
{
	return Color(this->p_octave_perlin_(comps.texmap_point.x, comps.texmap_point.y, comps.texmap_point.z));
}


double PerlinMap::p_fade_(const double & t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

// Source: http://riven8192.blogspot.com/2010/08/calculate-perlinnoise-twice-as-fast.html
double PerlinMap::p_grad_(int hash, double x, double y, double z)
{
	switch (hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0; // never happens
	}
}

double PerlinMap::p_perlin_(double x, double y, double z) const
{
	// Find the unit cube that contains the point
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	double u = p_fade_(x);
	double v = p_fade_(y);
	double w = p_fade_(z);

	// Hash coordinates of the 8 cube corners
	int A = this->p[X] + Y;
	int AA = this->p[A] + Z;
	int AB = this->p[A + 1] + Z;
	int B = this->p[X + 1] + Y;
	int BA = this->p[B] + Z;
	int BB = this->p[B + 1] + Z;

	// Add blended results from 8 corners of cube
	double res = PerlinMap::p_lerp_(
		w, 
		PerlinMap::p_lerp_(
			v, 
			PerlinMap::p_lerp_(
				u, 
				p_grad_(this->p[AA], x, y, z),
				p_grad_(this->p[BA], x - 1, y, z)
			), 
			PerlinMap::p_lerp_(
				u, p_grad_(this->p[AB], x, y - 1, z),
				p_grad_(this->p[BB], x - 1, y - 1, z)
			)
		), 
		PerlinMap::p_lerp_(
			v, 
			PerlinMap::p_lerp_(
				u, 
				p_grad_(this->p[AA + 1], x, y, z - 1),
				p_grad_(this->p[BA + 1], x - 1, y, z - 1)
			), 
			PerlinMap::p_lerp_(
				u, 
				p_grad_(this->p[AB + 1], x, y - 1, z - 1),
				p_grad_(this->p[BB + 1], x - 1, y - 1, z - 1)
			)
		)
	);
	return (res + 1.0) / 2.0;
}

// Sourced from Adrian Biagioli
// https://adrianb.io/2014/08/09/perlinnoise.html
double PerlinMap::p_octave_perlin_(double x, double y, double z) const {
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	double maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
	for (int i = 0; i < this->octaves; i++) {
		total += this->p_perlin_(x * frequency, y * frequency, z * frequency) * amplitude;

		maxValue += amplitude;

		amplitude *= this->persistence;
		frequency *= 2;
	}

	return total / maxValue;
}

double PerlinMap::p_lerp_(double t, double a, double b)
{
	return a + t * (b - a);
}

// ------------------------------------------------------------------------
//
// Colored Perlin Map
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

ColoredPerlin::ColoredPerlin() : ColoredPerlin(12345)
{
}

ColoredPerlin::ColoredPerlin(int seed)
{
	this->r_ = std::make_shared<PerlinMap>(seed);
	this->g_ = std::make_shared<PerlinMap>(seed + 1);
	this->b_ = std::make_shared<PerlinMap>(seed + 2);

}

ColoredPerlin::~ColoredPerlin()
{
}

Color ColoredPerlin::local_sample_at(const IxComps & comps) const
{
	return Color(
		this->r_->local_sample_at(comps).luminosity(),
		this->g_->local_sample_at(comps).luminosity(),
		this->b_->local_sample_at(comps).luminosity()
	);
}
