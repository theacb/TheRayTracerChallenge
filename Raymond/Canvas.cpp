#include "pch.h"
#include "Canvas.h"

// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Canvas::Canvas(int canvas_width, int canvas_height)
{
	c_width_ = canvas_width;
	c_height_ = canvas_height;
	c_total_size_ = canvas_width * canvas_height;

	c_pixels_ = std::vector<Color>(c_total_size_);
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
	// Check if the index is within the bounds of the matrix
	// Out of bounds is ignored
	if (x >= 0 && x < c_width_ && y >= 0 && y < c_height_)
		c_set_element_(x, y, color);
}

Color Canvas::pixel_at(int x, int y)
{
	// Check if the index is within the bounds of the matrix
	if (x >= 0 && x < c_width_ && y >= 0 && y < c_height_)
		return c_get_element_(x, y);
	else 
		// Out of bounds throws an error
		throw std::out_of_range(
			"The requested pixel, (" + 
			std::to_string(x) + 
			", " + 
			std::to_string(y) +
			"), is not within the bounds of the canvas."
		);
}

std::stringstream Canvas::to_ppm_lines(const bool convert_toSRGB)
{
	std::stringstream ss;

	// Tracks the line length
	int line_length = 0;

	for (Color c : c_pixels_)
	{
		Color cc;
		if (convert_toSRGB)
		{
			cc = c.convert_linear_to_srgb();
		}
		else
		{
			cc = c;
		}
		// The color is first converetd to sRGB, then to 8Bit, then to a string.
		std::string converted_color = Color8Bit(cc).output();
		size_t length = converted_color.length();

		// The maximum line length is 70 characters (68 + \n)
		// A newline character is inserted.
		if (line_length + length >= 68)
		{
			// Insert newline
			ss << "\n";
			line_length = 0;
		}

		// Output the color
		ss << converted_color;
		line_length += length;
	}

	return ss;
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

int Canvas::width()
{
	return c_width_;
}

int Canvas::height()
{
	return c_height_;
}

// iterators
// Allows iteration access to the underlying vector
Color * Canvas::begin()
{
	return c_pixels_.data();
}

Color * Canvas::end()
{
	return c_pixels_.data() + c_pixels_.size();
}



// ------------------------------------------------------------------------
// Private Methods
// ------------------------------------------------------------------------
Color Canvas::c_get_element_(int x, int y)
{
	return c_pixels_.at(c_index_from_coordinates_(x, y));
}

void Canvas::c_set_element_(int x, int y, Color color)
{
	c_pixels_.at(c_index_from_coordinates_(x, y)) = color;
}

// Converts x,y cartesian coords to an index in the internal vector
int Canvas::c_index_from_coordinates_(int x, int y)
{
	return (y * c_width_) + x;
}

// ------------------------------------------------------------------------
// Helper Functions
// ------------------------------------------------------------------------

void canvas_to_ppm(Canvas canvas, std::string file_path)
{
	// File stream
	std::ofstream output_file;

	// Open stream to provided path
	output_file.open(file_path, std::ios::out);

	// Write header
	output_file << "P3\n" << canvas.width() << " " << canvas.height() << "\n255\n";

	output_file << canvas.to_ppm_lines(true).rdbuf();

	output_file << "\n";

	output_file.close();

}
