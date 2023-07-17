#ifndef H_RAYMOND_CAMERA
#define H_RAYMOND_CAMERA

#include <thread>
#include <future>

#include "Tuple.h"
#include "Object.h"
#include "Ray.h"
#include "World.h"
#include "Canvas.h"
#include "SampleBuffer.h"
#include "Utilities.h"

class Camera :
	public ObjectBase
{
public:
	Camera();
	Camera(int, int, double);
	~Camera(); // NOLINT(modernize-use-override)

	// Methods

	// Rays
	Ray ray_from_pixel(int x, int y) const;
    Ray ray_from_pixel(int x, int y, double px_os_x, double px_os_y) const;

	// Render
	Canvas render(const World & w) const;
	Canvas threaded_render(const World & w) const;
	Canvas render_scanline(const World & w, int line) const;

    SampleBuffer multi_sample_render_bucket(const World & w, int x, int y, int width, int height, int bucket_id) const;
    SampleBuffer multi_sample_threaded_render(const World & l_camera) const;

	// Accessors
	int get_horizontal_size() const;
	int get_vertical_size() const;
	double get_fov() const;
	double get_pixel_size() const;

private:
	// Private Properties
    // dimensions
	int c_h_size_, c_v_size_;

	double c_fov_, c_pixel_size_, c_half_width_, c_half_height_;

	void pixel_size_();
    AABB2D extent_from_bucket_(int x, int y, int w, int h) const;
};

#endif
