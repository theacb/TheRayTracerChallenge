#pragma once

#include "Color.h"
#include <vector>
#include <fstream>
#include <sstream> 

class Canvas
{
public:
	// Constructors
	Canvas(int, int);

	// Destructor
	~Canvas();

	// methods
	void write_pixel(int, int, Color);
	Color pixel_at(int, int);
	std::stringstream to_ppm_lines(const bool);

	// Accessors

	int width();
	int height();

	// iterators
	Color * begin();
	Color * end();

private:

	// Properties
	std::vector<Color> c_pixels_;

	int c_width_;
	int c_height_;
	int c_total_size_;

	Color c_get_element_(int, int);
	void c_set_element_(int, int, Color);
	int c_index_from_coordinates_(int, int);
};

// File Output
void canvas_to_ppm(Canvas, std::string);