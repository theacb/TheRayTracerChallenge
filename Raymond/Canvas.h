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

	// Properties
	int width;
	int height;
	int total_size;

	// methods
	void write_pixel(int, int, Color);
	Color pixel_at(int, int);
	std::stringstream to_ppm_lines(const bool);

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

// File Output
void canvas_to_ppm(Canvas, std::string);