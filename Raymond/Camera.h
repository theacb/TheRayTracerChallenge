#ifndef H_RAYMOND_CAMERA
#define H_RAYMOND_CAMERA

#include <thread>
#include <future>

#include "Tuple.h"
#include "Object.h"
#include "Ray.h"
#include "World.h"
#include "Canvas.h"

class Camera :
	public ObjectBase
{
public:
	Camera();
	Camera(int, int, float);
	~Camera();

	// Methods
	// Virtual
	virtual std::vector<float> local_intersect_t(const Ray & r) const override;
	virtual Tuple normal_at(const Tuple & p) const override;

	// Rays
	Ray ray_from_pixel(int, int) const;

	// Render
	Canvas render(const World & w) const;
	Canvas threaded_render(const World & w) const;
	Canvas render_scanline(const World & w, int line) const;

	// Accessors
	int get_horizontal_size() const;
	int get_vertical_size() const;
	float get_fov() const;
	float get_pixel_size() const;

private:
	// Private Properties
	int c_h_size_, c_v_size_;
	float c_fov_, c_pixel_size_, c_half_width_, c_half_height_;

	void pixel_size_();
};

#endif
