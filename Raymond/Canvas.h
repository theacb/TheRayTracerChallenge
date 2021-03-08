#ifndef H_RAYMOND_CANVAS
#define H_RAYMOND_CANVAS

#include "Color.h"
#include <vector>
#include <fstream>
#include <sstream> 

class Canvas
{
public:
	// Constructors
	Canvas();
	Canvas(const Canvas & src);
	Canvas(int canvas_width, int canvas_height);

	// Destructor
	~Canvas();

	// methods
	void write_pixel(int x, int y, Color color);
	void write_canvas_as_line(int y, const Canvas & canvas);
	Color pixel_at(int x, int y);
	std::stringstream to_ppm_lines(const bool convert_toSRGB);
	std::vector<Color> get_pixels() const;

	// Accessors

	int width() const;
	int height() const;

	// iterators
	Color * begin();
	Color * end();

private:

	// Properties
	std::vector<Color> c_pixels_;

	int c_width_;
	int c_height_;
	int c_total_size_;

	Color c_get_element_(int x, int y);
	void c_set_element_(int x, int y, Color color);
	int c_index_from_coordinates_(int x, int y);
};

// File Output
void canvas_to_ppm(Canvas canvas, std::string file_path);
void canvas_to_ppm(Canvas canvas, std::string file_path, bool convert_to_sRGB);

#endif
