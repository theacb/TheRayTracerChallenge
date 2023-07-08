#include "pch.h"
#include "SampleBuffer.h"

// ------------------------------------------------------------------------
//
// SampledPixel
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

SampledPixel::SampledPixel() : SampledPixel(AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0)))
{
}

SampledPixel::SampledPixel(const Tuple top_left, const Tuple bottom_right) : SampledPixel(AABB2D(top_left, bottom_right))
{
}

SampledPixel::SampledPixel(AABB2D range)
{
	this->sp_subdivs_ = 1;
	this->sp_sub_samples_ = std::make_shared<QuadBranch<Sample>>(range, 4);

	this->sp_average_ = Sample();
}

SampledPixel::~SampledPixel()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void SampledPixel::write_sample(Sample sample)
{
	auto qn = std::make_shared<QuadNode<Sample>>(sample.get_origin(), sample);
	this->sp_sub_samples_->insert(qn);
}


// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

Color SampledPixel::get_channel(RE channel) const
{
	return this->sp_average_.get_channel(channel);
}

Sample SampledPixel::get_calculated_average() const
{
	return this->sp_average_;
}

std::vector<Sample> SampledPixel::get_samples() const
{
	return SampledPixel::nodes_to_samples(this->sp_sub_samples_->get_all_nodes());
}

std::vector<Sample> SampledPixel::query_range(const AABB2D &range) const
{
	return SampledPixel::nodes_to_samples(this->sp_sub_samples_->query_range(range));
}

void SampledPixel::quick_average(std::vector<Sample> sample_group)
{
	// Because this is a quick average, it only averages the RGB color channel.
	// This is then placed into a copy of the first sample in the list
	// Use for noise threshold calculation
	Sample base = Sample(sample_group[0]);
	Color mean = Color(0.0);

	// Add up colors
	for (size_t i = 0; i < sample_group.size(); i++)
	{
		mean = mean + sample_group[i].get_rgb();
	}

	mean = mean / static_cast<double>(sample_group.size());

	base.set_rgb(mean);

	this->sp_average_ = base;
}

void SampledPixel::full_average(std::vector<Sample> sample_group)
{
	// This averages every channel, so it is much slower than quick average
	// Use for final channel calculation
	Sample mean;

	// Add up samples
	for (size_t i = 0; i < sample_group.size(); i++)
	{
		mean = mean + sample_group[i];
	}

	mean = mean / static_cast<double>(sample_group.size());

	this->sp_average_ = mean;
}

std::vector<Sample> SampledPixel::nodes_to_samples(const std::vector<std::shared_ptr<QuadNode<Sample>>> nodes)
{
	std::vector<Sample> sample_vec = std::vector<Sample>();
	sample_vec.reserve(nodes.size());

	for (size_t i = 0; i < nodes.size(); i++)
	{
		sample_vec.push_back(nodes[i]->data);
	}

	return sample_vec;
}


// ------------------------------------------------------------------------
//
// SampleBuffer
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

SampleBuffer::SampleBuffer() : SampleBuffer(1, 1, AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0)))
{
}

SampleBuffer::SampleBuffer(const SampleBuffer & src)
{
	this->sb_width_ = src.width();
	this->sb_height_ = src.height();

	this->sb_total_size_ = this->sb_width_ * this->sb_height_;
	this->sb_pixels_ = src.get_pixels();

	this->sb_extents_ = src.extents();
	this->sb_pixel_size_ = src.extents().width / static_cast<double>(src.width());
}

SampleBuffer::SampleBuffer(int grid_width, int grid_height, const AABB2D& extents)
{
	this->sb_width_ = grid_width;
	this->sb_height_ = grid_height;
	this->sb_total_size_ = grid_width * grid_height;

	this->sb_extents_ = extents;

	this->sb_pixel_size_ = extents.width / static_cast<double>(grid_width);

	this->sb_pixels_ = std::vector<std::shared_ptr<SampledPixel>>();
	this->sb_pixels_.reserve(this->sb_total_size_);

	for (int i = 0; i < this->sb_total_size_; i++)
	{
		Tuple center = this->sb_pixel_center_point_(this->sb_x_from_index_(i), sb_y_from_index_(i));
		AABB2D range = AABB2D(center, this->sb_pixel_size_ * 0.5);
		this->sb_pixels_.push_back(std::make_shared<SampledPixel>(range));
	}

	// TODO: Fill with new samples
	// TODO: Add function for noise threshold and allocating more samples
}

SampleBuffer::~SampleBuffer()
= default;

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void SampleBuffer::write_sample(int x, int y, const Sample& sample)
{
	std::shared_ptr<SampledPixel> sp = this->sb_get_element_(x, y);
	sp->write_sample(sample);
}

void SampleBuffer::write_sample(const Sample& sample)
{
	// Hallowed are the Ori
	Tuple ori = sample.get_origin();
	// 
	int x = this->sb_x_coord_from_pos_(ori);
	int y = this->sb_y_coord_from_pos_(ori);

	std::shared_ptr<SampledPixel> sp = this->sb_get_element_(x, y);
	sp->write_sample(sample);
}

void SampleBuffer::write_pixel(int x, int y, std::shared_ptr<SampledPixel> pixel)
{
	// Check if the index is within the bounds of the grid
	// Out of bounds is ignored
	if (x >= 0 && x < sb_width_ && y >= 0 && y < sb_height_)
        sb_set_element_(x, y, pixel); // NOLINT(performance-unnecessary-value-param)
}

void SampleBuffer::write_portion_as_line(int y, const SampleBuffer & line)
{
	if (y >= 0 && y < this->sb_height_)
	{
		int start_index = (y * this->sb_width_);
		std::vector<std::shared_ptr<SampledPixel>> pixels = line.get_pixels();

		std::copy(pixels.begin(), pixels.end(), this->sb_pixels_.begin() + start_index);

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

void SampleBuffer::write_portion(int x, int y, const SampleBuffer & grid)
{
	if (y >= 0 && y + grid.height() < this->sb_height_ && x >= 0 && x + grid.width() < this->sb_width_)
	{
		std::vector<std::shared_ptr<SampledPixel>> pixels = grid.get_pixels();

		for (int i = 0; i < grid.height(); i++)
		{
            // Declaring std::vector<std::shared_ptr<SampledPixel>>::iterator with auto
			auto this_start_index = this->sb_pixels_.begin() + (((i + y) * this->sb_width_) + x);
			auto grid_start_index = pixels.begin() + i;

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

std::shared_ptr<SampledPixel> SampleBuffer::pixel_at(int x, int y)
{
	// Check if the index is within the bounds of the matrix
	if (x >= 0 && x < sb_width_ && y >= 0 && y < sb_height_)
		return sb_get_element_(x, y);
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

Canvas SampleBuffer::to_canvas(RE channel)
{
	// Create a canvas the same size as the sampler grid
	Canvas c = Canvas(this->sb_width_, this->sb_height_);

	// Iterate through grid
	for (int y = 0; y < this->sb_height_; y++)
	{
		for (int x = 0; x < this->sb_width_; x++)
		{
			// Assign SamplerPixel color to canvas pixels
			c.write_pixel(x, y, (this->pixel_at(x, y))->get_channel(channel));
		}
	}

	return c;
}

std::vector<std::shared_ptr<SampledPixel>> SampleBuffer::get_pixels() const
{
	return this->sb_pixels_;
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

int SampleBuffer::width() const
{
	return this->sb_width_;
}

int SampleBuffer::height() const
{
	return this->sb_height_;
}

AABB2D SampleBuffer::extents() const
{
	return this->sb_extents_;
}

// iterators
// Allows iteration access to the underlying vector
std::shared_ptr<SampledPixel>* SampleBuffer::begin()
{
	return this->sb_pixels_.data();
}

std::shared_ptr<SampledPixel>* SampleBuffer::end()
{
	return this->sb_pixels_.data() + sb_pixels_.size();
}

// ------------------------------------------------------------------------
// Private Methods
// ------------------------------------------------------------------------

std::shared_ptr<SampledPixel> SampleBuffer::sb_get_element_(int x, int y)
{
	return this->sb_pixels_.at(this->sb_index_from_coordinates_(x, y));
}

Tuple SampleBuffer::sb_pixel_center_point_(int x, int y) const
{
	return Tuple::Point2D(x * this->sb_pixel_size_, y * sb_pixel_size_);
}

int SampleBuffer::sb_x_coord_from_pos_(const Tuple & position) const
{
	double pos_x = position.x;
	double left_x = this->sb_extents_.ne_corner.x;
	double right_x = this->sb_extents_.sw_corner.x;
	return static_cast<int>((pos_x - left_x) / (right_x - left_x) * this->sb_width_);
}

int SampleBuffer::sb_y_coord_from_pos_(const Tuple & position) const
{
	double pos_y = position.y;
	double left_y = this->sb_extents_.ne_corner.y;
	double right_y = this->sb_extents_.sw_corner.y;
	return static_cast<int>((pos_y - left_y) / (right_y - left_y) * this->sb_width_);
}

void SampleBuffer::sb_set_element_(int x, int y, std::shared_ptr<SampledPixel> pixel)
{
	this->sb_pixels_.at(this->sb_index_from_coordinates_(x, y)) = pixel; // NOLINT(performance-unnecessary-value-param)
}

int SampleBuffer::sb_index_from_coordinates_(int x, int y) const
{
	return (y * this->sb_width_) + x;
}

int SampleBuffer::sb_x_from_index_(int i) const {
    return i % this->sb_width_;
}

int SampleBuffer::sb_y_from_index_(int i) const {
    return i / this->sb_width_;
}
