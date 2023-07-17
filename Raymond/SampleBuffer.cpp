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

SampledPixel::SampledPixel(const Tuple& top_left, const Tuple& bottom_right) : SampledPixel(AABB2D(top_left, bottom_right))
{
}

SampledPixel::SampledPixel(const AABB2D& range)
{
	this->sp_subdivs_ = 1;
	this->sp_sub_samples_ = QuadBranch<Sample>(range, 4);

	this->sp_average_ = Sample();
}

SampledPixel::~SampledPixel()
= default;

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

void SampledPixel::write_sample(const Sample& sample)
{
	auto qn = std::make_shared<QuadNode<Sample>>(sample.CanvasOrigin, sample);

	this->sp_sub_samples_.insert(qn);
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
	return SampledPixel::nodes_to_samples(this->sp_sub_samples_.get_all_nodes());
}

std::vector<Sample> SampledPixel::query_range(const AABB2D &range) const
{
	return SampledPixel::nodes_to_samples(this->sp_sub_samples_.query_range(range));
}

void SampledPixel::quick_average()
{
    this->quick_average(this->get_samples());
}

AABB2D SampledPixel::get_bounds() const {
    return this->sp_sub_samples_.get_bounds();
}

void SampledPixel::quick_average(const std::vector<Sample> & sample_group)
{
	// Because this is a quick average, it only averages the RGB color channel.
	// This is then placed into a copy of the first sample in the list
	// Use for noise threshold calculation
	Sample base = Sample(sample_group[0]);

    if (! sample_group.empty()) {
        Color mean = base.get_current_rgb();

        for (int i = 1; i < sample_group.size(); ++i) {
            mean = mean + sample_group[i].get_current_rgb();
        }

        mean = mean / static_cast<double>(sample_group.size());

        base.set_rgb(mean);

        this->sp_average_ = base;
    }
}

void SampledPixel::full_average()
{
    this->full_average(this->get_samples());
}

void SampledPixel::full_average(const std::vector<Sample>& sample_group)
{
	// This averages every channel, so it is much slower than quick average
	// Use for final channel calculation

    if (! sample_group.empty())
    {
        Sample mean = sample_group[0];

        // Add up samples
        for (int i = 1; i < sample_group.size(); ++i) {
            mean = mean + sample_group[i];
        }

        mean = mean / static_cast<double>(sample_group.size());

        this->sp_average_ = mean;
    }
}

std::vector<Sample> SampledPixel::nodes_to_samples(const std::vector<std::shared_ptr<QuadNode<Sample>>>& nodes)
{
	std::vector<Sample> sample_vec = std::vector<Sample>();
	sample_vec.reserve(nodes.size());

	for (const auto & node : nodes)
	{
		sample_vec.push_back(node->data);
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

    this->sb_x_pos_ = src.x_position();
    this->sb_y_pos_ = src.y_position();

	this->sb_total_size_ = this->sb_width_ * this->sb_height_;
    this->sb_longest_side_ = (this->sb_width_ > this->sb_height_) ? this->sb_width_ : this->sb_height_;

	this->sb_pixels_ = src.get_pixels();

	this->sb_extents_ = src.extents();
	this->sb_pixel_size_ = src.extents().width / static_cast<double>(src.width());
}

SampleBuffer::SampleBuffer(int grid_width, int grid_height, const AABB2D& extents) : SampleBuffer(0, 0, grid_width, grid_height, extents)
{
}

SampleBuffer::SampleBuffer(int x, int y, int grid_width, int grid_height, const AABB2D &extents) {
    this->sb_width_ = grid_width;
    this->sb_height_ = grid_height;
    this->sb_x_pos_ = x;
    this->sb_y_pos_ = y;

    this->sb_total_size_ = grid_width * grid_height;
    this->sb_longest_side_ = (this->sb_width_ > this->sb_height_) ? this->sb_width_ : this->sb_height_;

    this->sb_extents_ = extents;

    this->sb_pixel_size_ = extents.width / static_cast<double>(grid_width);

    this->sb_pixels_ = std::vector<std::shared_ptr<SampledPixel>>();
    this->sb_pixels_.resize(this->sb_total_size_);

    this->sb_initialize_elements_();
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
	Tuple ori = sample.CanvasOrigin;


	int x = this->sb_x_from_pos_(ori);
	int y = this->sb_y_from_pos_(ori);

	std::shared_ptr<SampledPixel> sp = this->sb_get_element_(x, y);

    AABB2D box = sp->get_bounds();

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

void SampleBuffer::write_portion(const SampleBuffer & grid)
{
    this->write_portion(grid.x_position(), grid.y_position(), grid);
}

void SampleBuffer::write_portion(int x, int y, const SampleBuffer & bucket)
{
    // TODO: Update the pixel bounds - yes, will need this for filtering samples
    // TODO: This is now working but still leaving un-stitched buckets on the edges of the buffer
    // if (y >= 0 && y + bucket.height() < this->sb_height_ && x >= 0 && x + bucket.width() < this->sb_width_)

    // Bucket Data
    std::vector<std::shared_ptr<SampledPixel>> pixels = bucket.get_pixels();

    // Iterate over the bucket, adding it line by line to this buffer
    for (int i = 0; i < bucket.height(); i++)
    {
        if ((i + y) < this->sb_height_)
        {
            // This Buffer indices
            // Starting point for the line
            auto this_result_index = this->sb_pixels_.begin() + (((i + y) * this->sb_width_) + x);
            // Calculate the end point of this line
            auto maximum_line_index = this->sb_pixels_.begin() + (((i + y) * this->sb_width_) + x + (this->sb_width_ - x));

            // Bucket indices
            auto bucket_start_index = pixels.begin() + (i * bucket.width());
            auto bucket_end_index = bucket_start_index + bucket.width();

            // Convert some indices to integers so that a little math can be done with them
            long long int result_index_int = std::distance(this->sb_pixels_.begin(), this_result_index);
            long long int maximum_line_index_int = std::distance(this->sb_pixels_.begin(), maximum_line_index);
            long long int bucket_length_index_int = std::distance(bucket_start_index, bucket_end_index);

            // Test if the bucket overflows the buffer on the y-axis
            // Overflows if the end index of the buffer line is less than
            // the result index plus the length of the bucket line
            bool overflow_y = maximum_line_index_int < (bucket_length_index_int + result_index_int);

            // If the bucket is partially outside the bounds of the buffer, truncate the line at the buffer's edge
            if (overflow_y)
            {
                bucket_end_index -= (maximum_line_index_int - result_index_int);
            }

//            // Debugging print (only uncomment in the case of a problem with this method)
//            std::cout << "X: " << x << " Y: " << y << " - Bucket size: (" << bucket.width() << ", " << bucket.height()
//                      << ") - Line Max: " << std::distance(this->sb_pixels_.begin(), maximum_line_index)
//                      << " - Truncating: " << (overflow_y)
//                      << " - Start Index: " << std::distance(pixels.begin(), bucket_start_index)
//                      << " - End Index: " << std::distance(pixels.begin(), bucket_end_index)
//                      << " - Bucket Length: " << bucket_length_index_int
//                      << " - Result index: " << std::distance(this->sb_pixels_.begin(), this_result_index)
//                      << std::endl;

            std::move(bucket_start_index, bucket_end_index, this_result_index);
        }
    }
}

void SampleBuffer::fill_solid(const Sample & sample)
{
    // Reset Buffer
    this->sb_initialize_elements_();

    // Fills the entire buffer with copies of one sample
    int i = 0;
    for (std::shared_ptr<SampledPixel> & p : this->sb_pixels_)
    {
        Sample smp = Sample(sample);
        smp.CanvasOrigin = this->coordinates_from_index(i);

        p->write_sample(smp);
        p->full_average();

        i++;
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

Tuple SampleBuffer::coordinates_from_pixel(const int &x, const int &y) const
{
    return this->sb_pixel_center_point_(x, y);
}

Tuple SampleBuffer::coordinates_from_pixel(const int &x, const int &y, const double &px_os_x, const double &px_os_y) const
{
    Tuple center = this->sb_pixel_center_point_(x, y);
    // Allows offsetting the position of the pixel using a value from 0.0 to 1.0 representing the size of the pixel
    return center + Tuple::Point2D(
            remap(px_os_x, 0.0, 1.0, -0.5, 0.5) * this->sb_pixel_size_,
            remap(px_os_y, 0.0, 1.0, -0.5, 0.5) * this->sb_pixel_size_
            );
}

Tuple SampleBuffer::coordinates_from_index(const int &i) const
{
    return this->sb_pixel_center_point_(this->sb_x_from_index_(i), sb_y_from_index_(i));
}

bool SampleBuffer::test_noise_threshold(const int &i, const double &noise_threshold) const
{
    // TODO: Implement a noise test function
    return false;
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

int SampleBuffer::x_position() const {
    return this->sb_x_pos_;
}

int SampleBuffer::y_position() const {
    return this->sb_y_pos_;
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
	Tuple raw = Tuple::Point2D((x + 0.5) * this->sb_pixel_size_, (y + 0.5) * sb_pixel_size_);
    return raw + this->extents().ne_corner;
}

int SampleBuffer::sb_x_from_pos_(const Tuple & position) const
{
	double pos_x = position.x;
	double left_x = this->sb_extents_.ne_corner.x;
	double right_x = this->sb_extents_.sw_corner.x;
    double result = ((pos_x - left_x) / (right_x - left_x)) * double(this->sb_longest_side_);
	return static_cast<int>(result);
}

int SampleBuffer::sb_y_from_pos_(const Tuple & position) const
{
	double pos_y = position.y;
	double left_y = this->sb_extents_.ne_corner.y;
	double right_y = this->sb_extents_.sw_corner.y;
    double result = ((pos_y - left_y) / (right_y - left_y)) * double(this->sb_longest_side_);
	return static_cast<int>(result);
}

void SampleBuffer::sb_set_element_(int x, int y, std::shared_ptr<SampledPixel> pixel)
{
	this->sb_pixels_.at(this->sb_index_from_coordinates_(x, y)) = pixel; // NOLINT(performance-unnecessary-value-param)
}

int SampleBuffer::sb_index_from_coordinates_(int x, int y) const
{
	return (y * this->sb_width_) + x;
}

int SampleBuffer::sb_x_from_index_(int i) const
{
    return i % this->sb_width_;
}

int SampleBuffer::sb_y_from_index_(int i) const
{
    return i / this->sb_width_;
}

void SampleBuffer::sb_initialize_elements_()
{
    // Fill the buffer with SamplePixels that have had their areas initialized to their pixel's AABB
    for (int i = 0; i < this->sb_total_size_; i++)
    {
        Tuple center = this->coordinates_from_index(i);
        AABB2D range = AABB2D(center, this->sb_pixel_size_ * 0.5);
        this->sb_pixels_[i] = std::make_shared<SampledPixel>(range);
    }
}

std::ostream & operator<<(std::ostream & os, const SampleBuffer & s) {
    os << "[ SampleBuffer: Width: " << s.width()
       << ", Height: " << s.height()
       << ", x position: " << s.x_position()
       << ", y position: " << s.y_position()
       << ", extents: " << s.extents()
       << " ]";
    return os;
}
