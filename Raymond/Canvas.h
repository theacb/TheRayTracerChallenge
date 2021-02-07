#pragma once

#include "Color.h"
#include <vector>

class Canvas
{
public:
	// Constructors
	Canvas(int, int);

	// Destructor
	~Canvas();

	// Properties
	int width;
	int height;
	int total_size;

	// methods
	void write_pixel(int, int, Color);
	Color pixel_at(int, int);

	// iterators
	Color * begin();
	Color * end();

private:

	// Properties
	std::vector<Color> pixels_;

	Color get_element_(int, int);
	void set_element_(int, int, Color);
	int index_from_coordinates_(int, int);

};

