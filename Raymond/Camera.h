#pragma once

#include "Tuple.h"
#include "Object.h"
#include "Ray.h"

class Camera :
	public ObjectBase
{
public:
	Camera();
	Camera(int, int, float);
	~Camera();

	// Methods
	// Virtual
	virtual std::vector<float> intersect_t(Ray &) const override;
	virtual Tuple normal_at(Tuple &) const override;

	// Rays
	Ray ray_from_pixel(int, int);

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

