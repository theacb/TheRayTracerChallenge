#include "pch.h"
#include "Canvas.h"

// ------------------------------------------------------------------------
//
// Canvas
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Canvas::Canvas() : Canvas(1, 1)
{
}

Canvas::Canvas(const Canvas & src)
{
	this->c_width_ = src.width();
	this->c_height_ = src.height();

	this->c_total_size_ = this->c_width_ * this->c_height_;
	this->c_pixels_ = src.get_pixels();
}

Canvas::Canvas(int canvas_width, int canvas_height)
{
	this->c_width_ = canvas_width;
	this->c_height_ = canvas_height;
	this->c_total_size_ = canvas_width * canvas_height;

	this->c_pixels_ = std::vector<Color>(this->c_total_size_);
}

// Destructor
Canvas::~Canvas()
= default;

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void Canvas::write_pixel(int x, int y, const Color & color)
{
	// Check if the index is within the bounds of the matrix
	// Out of bounds is ignored
	if (x >= 0 && x < c_width_ && y >= 0 && y < c_height_)
		c_set_element_(x, y, color);
}

void Canvas::write_canvas_as_line(int y, const Canvas & canvas)
{
	if (y < this->c_height_)
	{
		int start_index = (y * c_width_);
		std::vector<Color> pixels = canvas.get_pixels();

		std::copy(pixels.begin(), pixels.end(), this->c_pixels_.begin() + start_index);

	}
	else
	{
		throw std::out_of_range(
			"The requested line, (" +
			std::to_string(y) +
			"), is not within the bounds of the canvas."
		);
	}
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

	for (Color c : this->c_pixels_)
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
		// The color is first converted to sRGB, then to 8Bit, then to a string.
		std::string converted_color = Color8Bit(cc).output();
		int length = static_cast<int>(converted_color.length());

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

std::vector<Color> Canvas::get_pixels() const
{
	return this->c_pixels_;
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

int Canvas::width() const
{
	return this->c_width_;
}

int Canvas::height() const
{
	return this->c_height_;
}

// iterators
// Allows iteration access to the underlying vector
Color * Canvas::begin()
{
	return this->c_pixels_.data();
}

Color * Canvas::end()
{
	return this->c_pixels_.data() + c_pixels_.size();
}



// ------------------------------------------------------------------------
// Private Methods
// ------------------------------------------------------------------------
Color Canvas::c_get_element_(int x, int y)
{
	return this->c_pixels_.at(this->c_index_from_coordinates_(x, y));
}

void Canvas::c_set_element_(int x, int y, const Color & color)
{
	this->c_pixels_.at(this->c_index_from_coordinates_(x, y)) = color;
}

// Converts x,y cartesian coords to an index in the internal vector
int Canvas::c_index_from_coordinates_(int x, int y)
{
	return (y * this->c_width_) + x;
}

// ------------------------------------------------------------------------
// Helper Functions
// ------------------------------------------------------------------------

void canvas_to_ppm(Canvas canvas, std::string file_path)
{
	canvas_to_ppm(canvas, file_path, true);
}

void canvas_to_ppm(Canvas canvas, std::string file_path, bool convert_to_sRGB)
{
	// File stream
	std::ofstream output_file;

	// Open stream to provided path
	output_file.open(file_path, std::ios::out);

	// Write header
	output_file << "P3\n" << canvas.width() << " " << canvas.height() << "\n255\n";

	output_file << canvas.to_ppm_lines(convert_to_sRGB).rdbuf();

	output_file << "\n";

	output_file.close();
}
