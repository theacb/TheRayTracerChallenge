#ifndef H_RAYMOND_SAMPLER
#define H_RAYMOND_SAMPLER

#include "Sample.h"
#include "BoundingBox.h"
#include "Color.h"
#include "Canvas.h"
#include "Tuple.h"
#include <vector>
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
	SampledPixel(const Tuple origin);
	~SampledPixel();

	// Methods
	void write_sample(int ind, Sample sample);
	bool sample_threshold(const double & threshold) const;
	std::vector<Sample> subdivide();

	// Accessors
	Color get_color() const;
	std::vector<Sample> get_edge_samples(Edge edge) const;
	std::vector<Sample> get_samples() const;
	std::shared_ptr<SampledPixel const> get_neighbor(Edge edge) const;
	void set_neighbor(Edge edge, std::shared_ptr<SampledPixel const> neighbor);

private:
	// Properties
	std::vector<Sample> sp_sub_samples_;
	int sp_subdivs_;

	Tuple sp_center_pt_;

	std::shared_ptr<SampledPixel const> sp_east_;
	std::shared_ptr<SampledPixel const> sp_southeast_;
	std::shared_ptr<SampledPixel const> sp_south_;
	std::shared_ptr<SampledPixel const> sp_southwest_;
	std::shared_ptr<SampledPixel const> sp_west_;
	std::shared_ptr<SampledPixel const> sp_northwest_;
	std::shared_ptr<SampledPixel const> sp_north_;
	std::shared_ptr<SampledPixel const> sp_northeast_;

	// Functions
	static Sample quick_average(std::vector<Sample> sample_group);
};

class SamplerGrid
{
public:
	// Constructors
	SamplerGrid();
	SamplerGrid(const SamplerGrid & src);
	SamplerGrid(int grid_width, int grid_height);

	// Destructor
	~SamplerGrid();

	// methods
	void write_pixel(int x, int y, std::shared_ptr<SampledPixel> pixel);
	void write_portion_as_line(int y, const SamplerGrid & line);
	void write_portion(int x, int y, const SamplerGrid & grid);
	std::shared_ptr<SampledPixel> pixel_at(int x, int y);
	Canvas to_canvas();
	std::vector<std::shared_ptr<SampledPixel>> get_pixels() const;

	// Accessors
	int width() const;
	int height() const;

	// iterators
	std::shared_ptr<SampledPixel> * begin();
	std::shared_ptr<SampledPixel> * end();

private:
	// Properties
	std::vector<std::shared_ptr<SampledPixel>> sg_pixels_;

	int sg_width_;
	int sg_height_;
	int sg_total_size_;

	std::shared_ptr<SampledPixel> sg_get_element_(int x, int y);
	void sg_set_element_(int x, int y, std::shared_ptr<SampledPixel> pixel);
	int sg_index_from_coordinates_(int x, int y);
};

#endif