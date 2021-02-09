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
	if (x >= 0 && x < width && y >= 0 && y < height)
		set_element_(x, y, color);
}

Color Canvas::pixel_at(int x, int y)
{
	if ((0 <= x < width) && (0 <= y < height))
		return get_element_(x, y);
	else 
		return Color(0.0f, 0.0f, 0.0f);
}

std::stringstream Canvas::to_ppm_lines(const bool convert_toSRGB)
{
	std::stringstream ss;

	// Tracks the line length
	int line_length = 0;

	for (Color c : pixels_)
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
		int length = converted_color.length();

		// The maximum line length is 70 characters, a newline character is inserted
		if (line_length + length >= 70)
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
	output_file << "P3\n" << canvas.width << " " << canvas.height << "\n255\n";

	output_file << canvas.to_ppm_lines(true).rdbuf();

	output_file << "\n";

	output_file.close();

}
