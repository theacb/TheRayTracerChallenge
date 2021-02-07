#include "pch.h"
#include "Canvas.h"

// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Canvas::Canvas(int canvas_width, int canvas_height)
{
	width = canvas_width;
	height = canvas_height;
	total_size = canvas_width * canvas_height;

	pixels_ = std::vector<Color>(total_size);
}

// Destructor
Canvas::~Canvas()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void Canvas::write_pixel(int x, int y, Color color)
{
	set_element_(x, y, color);
}

Color Canvas::pixel_at(int x, int y)
{
	return get_element_(x, y);
}

// iterators
Color * Canvas::begin()
{
	return pixels_.data();
}

Color * Canvas::end()
{
	return pixels_.data() + pixels_.size();
}



// ------------------------------------------------------------------------
// Private Methods
// ------------------------------------------------------------------------
Color Canvas::get_element_(int x, int y)
{

	return pixels_.at(index_from_coordinates_(x, y));
}

void Canvas::set_element_(int x, int y, Color color)
{
	pixels_.at(index_from_coordinates_(x, y)) = color;
}

int Canvas::index_from_coordinates_(int x, int y)
{
	return (y * width) + x;
}
