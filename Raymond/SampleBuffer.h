#ifndef H_RAYMOND_SAMPLER
#define H_RAYMOND_SAMPLER

#include "Sample.h"
#include "BoundingBox.h"
#include "Color.h"
#include "Canvas.h"
#include "Tuple.h"
#include "Quadtree.h"
#include "Utilities.h"

#include <vector>
#include <cstddef>
#include <fstream>
#include <sstream> 

enum Edge { East, Southeast, South, Southwest, West, Northwest, North, Northeast};

// Sampled Pixel begins by containing the top Left corner of each pixel in the virtual film back
// As rendering progresses, the sampled pixel is subdivided into smaller pieces in order to 
// super sample the pixel
class SampledPixel
{
public:
	SampledPixel();
	SampledPixel(const Tuple& top_left, const Tuple& bottom_right);
	explicit SampledPixel(const AABB2D& range);
	~SampledPixel();

	// Methods
	void write_sample(const Sample& sample);
	void quick_average();
    void quick_average(const std::vector<Sample>& sample_group);
    void full_average();
	void full_average(const std::vector<Sample>& sample_group);

	// Accessors
	[[nodiscard]] Color get_channel(RE channel) const;
	[[nodiscard]] Sample get_calculated_average() const;
	[[nodiscard]] std::vector<Sample> get_samples() const;
	[[nodiscard]] std::vector<Sample> query_range(const AABB2D &range) const;
    [[nodiscard]] AABB2D get_bounds() const;

	// Static
	static std::vector<Sample> nodes_to_samples(const std::vector<std::shared_ptr<QuadNode<Sample>>>& nodes);
	
private:
	// Properties
	QuadBranch<Sample> sp_sub_samples_;
	int sp_subdivs_;

	Sample sp_average_;

};

class SampleBuffer
{
public:
	// Constructors
	SampleBuffer();
	SampleBuffer(const SampleBuffer & src);
	SampleBuffer(int grid_width, int grid_height, const AABB2D & extents);
    SampleBuffer(int x, int y, int grid_width, int grid_height, const AABB2D & extents);

	// Destructor
	~SampleBuffer();

	// methods
	void write_sample(int x, int y, const Sample& sample);
	void write_sample(const Sample& sample);
	void write_pixel(int x, int y, std::shared_ptr<SampledPixel> pixel);
	void write_portion_as_line(int y, const SampleBuffer & line);
	void write_portion(int x, int y, const SampleBuffer & grid);
	std::shared_ptr<SampledPixel> pixel_at(int x, int y);
    [[nodiscard]] Tuple coordinates_from_pixel(const int & x, const int & y) const;
    [[nodiscard]] Tuple coordinates_from_pixel(const int & x, const int & y, const double & px_os_x, const double & px_os_y) const;
    [[nodiscard]] Tuple coordinates_from_index(const int & i) const;
	Canvas to_canvas(RE channel);
	[[nodiscard]] std::vector<std::shared_ptr<SampledPixel>> get_pixels() const;

    [[nodiscard]] bool test_noise_threshold(const int & i, const double & noise_threshold) const;
    [[nodiscard]] Tuple request_new_sample_point(const int & i) const;

	// Accessors
	[[nodiscard]] int width() const;
	[[nodiscard]] int height() const;
    [[nodiscard]] int x_position() const;
    [[nodiscard]] int y_position() const;
	[[nodiscard]] AABB2D extents() const;

	// iterators
	std::shared_ptr<SampledPixel> * begin();
	std::shared_ptr<SampledPixel> * end();

private:
	// Properties
	std::vector<std::shared_ptr<SampledPixel>> sb_pixels_;

	int sb_width_;
	int sb_height_;
    int sb_x_pos_;
    int sb_y_pos_;
	int sb_total_size_;
	AABB2D sb_extents_;
	double sb_pixel_size_;

	std::shared_ptr<SampledPixel> sb_get_element_(int x, int y);
	[[nodiscard]] Tuple sb_pixel_center_point_(int x, int y) const;
	[[nodiscard]] int sb_x_coord_from_pos_(const Tuple &position) const;
	[[nodiscard]] int sb_y_coord_from_pos_(const Tuple &position) const;
	void sb_set_element_(int x, int y, std::shared_ptr<SampledPixel> pixel);
	[[nodiscard]] int sb_index_from_coordinates_(int x, int y) const;
    [[nodiscard]] int sb_x_from_index_(int i) const;
    [[nodiscard]] int sb_y_from_index_(int i) const;
};

#endif