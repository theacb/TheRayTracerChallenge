#include "pch.h"
#include "SamplerGrid.h"

// ------------------------------------------------------------------------
//
// SampledPixel
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

SampledPixel::SampledPixel() : SampledPixel(Tuple::Point(0.0, 0.0, 0.0))
{
}

SampledPixel::SampledPixel(const Tuple origin)
{
	this->sp_center_pt_ = origin;

	this->sp_subdivs_ = 1;
	this->sp_sub_samples_ = std::vector<Sample>(this->sp_subdivs_);

	this->sp_east_ = nullptr;
	this->sp_southeast_ = nullptr;
	this->sp_south_ = nullptr;
	this->sp_southwest_ = nullptr;
	this->sp_west_ = nullptr;
	this->sp_northwest_ = nullptr;
	this->sp_north_ = nullptr;
	this->sp_northeast_ = nullptr;
}

SampledPixel::~SampledPixel()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void SampledPixel::write_sample(int ind, Sample sample)
{
	if (ind > (this->sp_subdivs_ * this->sp_subdivs_))
	{
		this->sp_sub_samples_[ind] = sample;
	}
	else
	{
		throw std::out_of_range(
			"The requested index, [" +
			std::to_string(ind) +
			"], is not within the bounds of the sample."
		);
	}
}

bool SampledPixel::sample_threshold(const double & threshold) const
{
	std::vector<Sample> p = std::vector<Sample>(this->sp_sub_samples_);

	std::vector<Sample> east = this->get_edge_samples(East);
	std::vector<Sample> southeast = this->get_edge_samples(Southeast);
	std::vector<Sample> south = this->get_edge_samples(South);

	p.resize(p.size() + east.size() + southeast.size() + south.size());
	p.insert(p.end(), east.begin(), east.end());
	p.insert(p.end(), southeast.begin(), southeast.end());
	p.insert(p.end(), south.begin(), south.end());

	return false;
}

std::vector<Sample> SampledPixel::subdivide()
{
	return std::vector<Sample>();
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

Color SampledPixel::get_color() const
{
	return Color();
}

std::vector<Sample> SampledPixel::get_edge_samples(Edge edge) const
{
	return std::vector<Sample>();
}

std::vector<Sample> SampledPixel::get_samples() const
{
	return std::vector<Sample>();
}

std::shared_ptr<SampledPixel const> SampledPixel::get_neighbor(Edge edge) const
{
	return std::shared_ptr<SampledPixel const>();
}

void SampledPixel::set_neighbor(Edge edge, std::shared_ptr<SampledPixel const> neighbor)
{
}

Sample SampledPixel::quick_average(std::vector<Sample> sample_group)
{
	Color mean = Color(0.0);
	for (size_t i = 0; i < sample_group.size(); i++)
	{
		mean = mean + sample_group[i].get_rgb();
	}

	return mean / static_cast<double>(sample_group.size());
}


// ------------------------------------------------------------------------
//
// SamplerGrid
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

SamplerGrid::SamplerGrid() : SamplerGrid(1, 1)
{
}

SamplerGrid::SamplerGrid(const SamplerGrid & src)
{
	this->sg_width_ = src.width();
	this->sg_height_ = src.height();

	this->sg_total_size_ = this->sg_width_ * this->sg_height_;
	this->sg_pixels_ = src.get_pixels();
}

SamplerGrid::SamplerGrid(int grid_width, int grid_height)
{
	this->sg_width_ = grid_width;
	this->sg_height_ = grid_height;
	this->sg_total_size_ = grid_width * grid_height;

	this->sg_pixels_ = std::vector<std::shared_ptr<SampledPixel>>(this->sg_total_size_);
}

SamplerGrid::~SamplerGrid()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void SamplerGrid::write_pixel(int x, int y, std::shared_ptr<SampledPixel> pixel)
{
	// Check if the index is within the bounds of the grid
	// Out of bounds is ignored
	if (x >= 0 && x < sg_width_ && y >= 0 && y < sg_height_)
		sg_set_element_(x, y, pixel);
}

void SamplerGrid::write_portion_as_line(int y, const SamplerGrid & line)
{
	if (y >= 0 && y < this->sg_height_)
	{
		int start_index = (y * this->sg_width_);
		std::vector<std::shared_ptr<SampledPixel>> pixels = line.get_pixels();

		std::copy(pixels.begin(), pixels.end(), this->sg_pixels_.begin() + start_index);

	}
	else
	{
		throw std::out_of_range(
			"The requested line, (" +
			std::to_string(y) +
			"), is not within the bounds of the grid."
		);
	}
}

void SamplerGrid::write_portion(int x, int y, const SamplerGrid & grid)
{
	if (y >= 0 && y + grid.height() < this->sg_height_ && x >= 0 && x + grid.width() < this->sg_width_)
	{
		std::vector<std::shared_ptr<SampledPixel>> pixels = grid.get_pixels();

		for (int i = 0; i < grid.height(); i++)
		{
			std::vector<std::shared_ptr<SampledPixel>>::iterator this_start_index = this->sg_pixels_.begin() + (((i + y) * this->sg_width_) + x);
			std::vector<std::shared_ptr<SampledPixel>>::iterator grid_start_index = pixels.begin() + i;

			std::copy(grid_start_index, grid_start_index + grid.width(), this_start_index);
		}

	}
	else
	{
		throw std::out_of_range(
			"The requested sub-grid, ((" +
			std::to_string(x) + ", " + 
			std::to_string(y) + "), (" +
			std::to_string(x + grid.width()) + ", " +
			std::to_string(y + grid.height()) +
			")), is not within the bounds of the grid."
		);
	}
}

std::shared_ptr<SampledPixel> SamplerGrid::pixel_at(int x, int y)
{
	// Check if the index is within the bounds of the matrix
	if (x >= 0 && x < sg_width_ && y >= 0 && y < sg_height_)
		return sg_get_element_(x, y);
	else
		// Out of bounds throws an error
		throw std::out_of_range(
			"The requested pixel, (" +
			std::to_string(x) +
			", " +
			std::to_string(y) +
			"), is not within the bounds of the grid."
		);
}

Canvas SamplerGrid::to_canvas()
{
	// Create a canvas the same size as the sampler grid
	Canvas c = Canvas(this->sg_width_, this->sg_height_);

	// Iterate through grid
	for (int y = 0; y < this->sg_height_; y++)
	{
		for (int x = 0; x < this->sg_width_; x++)
		{
			// Assign SamplerPixel color to canvas pixels
			c.write_pixel(x, y, (this->pixel_at(x, y))->get_color());
		}
	}

	return c;
}

std::vector<std::shared_ptr<SampledPixel>> SamplerGrid::get_pixels() const
{
	return this->sg_pixels_;
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

int SamplerGrid::width() const
{
	return this->sg_width_;
}

int SamplerGrid::height() const
{
	return this->sg_height_;
}

// iterators
// Allows iteration access to the underlying vector
std::shared_ptr<SampledPixel>* SamplerGrid::begin()
{
	return this->sg_pixels_.data();
}

std::shared_ptr<SampledPixel>* SamplerGrid::end()
{
	return this->sg_pixels_.data() + sg_pixels_.size();
}

// ------------------------------------------------------------------------
// Private Methods
// ------------------------------------------------------------------------

std::shared_ptr<SampledPixel> SamplerGrid::sg_get_element_(int x, int y)
{
	return this->sg_pixels_.at(this->sg_index_from_coordinates_(x, y));
}

void SamplerGrid::sg_set_element_(int x, int y, std::shared_ptr<SampledPixel> pixel)
{
	this->sg_pixels_.at(this->sg_index_from_coordinates_(x, y)) = pixel;
}

int SamplerGrid::sg_index_from_coordinates_(int x, int y)
{
	return (y * this->sg_width_) + x;
}
