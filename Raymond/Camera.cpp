#include "pch.h"
#include "Camera.h"

// ------------------------------------------------------------------------
//
// Base Object
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Camera::Camera() : Camera(560, 315, float(M_PI) / 2.0f)
{
}

Camera::Camera(int width, int height, float fov)
{
	this->c_h_size_ = width;
	this->c_v_size_ = height;
	this->c_fov_ = fov;
	this->pixel_size_();
}

Camera::~Camera()
{
}

// ------------------------------------------------------------------------
// Virtual
// ------------------------------------------------------------------------

std::vector<float> Camera::local_intersect_t(const Ray & r) const
{
	Tuple position = (this->get_transform()).position();
	Tuple ix_vector = position - r.origin;
	if (ix_vector.normalize() == r.direction.normalize())
	{
		return std::vector<float>({ Tuple::distance(position, r.origin) });
	}
	else
	{
		return std::vector<float>();
	}
}

Tuple Camera::normal_at(const Tuple & p) const
{
	return Tuple::Vector(0.0f, 1.0f, 0.0f);
}

// ------------------------------------------------------------------------
// Rays
// ------------------------------------------------------------------------

Ray Camera::ray_from_pixel(int x, int y) const
{
	// Offset from the edge of the canvas to the pixel's ceneter
	float x_offset = (float(x) + 0.5f) * this->c_pixel_size_;
	float y_offset = (float(y) + 0.5f) * this->c_pixel_size_;

	// The untransformed coordinates of the pixel in world space
	// Camera looks towards -z, so +x is to the left
	float world_x = this->c_half_width_ - x_offset;
	float world_y = this->c_half_height_ - y_offset;

	// Pre invert matrix so this is only done once
	Matrix4 inv_x_form = this->get_transform().inverse();

	// Using the camera's matrix, transform the canvas point and the origin 
	// and then compute the ray's direction vector
	// The camera is at z=-1.0f
	Tuple pixel = inv_x_form * Tuple::Point(world_x, world_y, -1.0f);
	Tuple origin = inv_x_form * Tuple::Point(0.0f, 0.0f, 0.0f);
	Tuple direction = (pixel - origin).normalize();

	return Ray(origin, direction);
}

// ------------------------------------------------------------------------
// Render
// ------------------------------------------------------------------------

Canvas Camera::render(const World & w) const
{
	Canvas image = Canvas(this->c_h_size_, this->c_v_size_);

	for (int y = 0; y < this->c_v_size_; y++)
	{
		std::cout << "Scanline: " << y + 1 << "/" << this->c_v_size_ << "\n";

		for (int x = 0; x < this->c_h_size_; x++)
		{
			// std::cout << "Pixel: (" << x << ", " << y << ")\n";
			Ray r = this->ray_from_pixel(x, y);
			Color color = w.color_at(r);
			image.write_pixel(x, y, color);
		}
	}

	return image;
}

Canvas Camera::threaded_render(const World & w) const
{
	Canvas image = Canvas(this->c_h_size_, this->c_v_size_);
	unsigned int c = std::thread::hardware_concurrency();

	auto line_results = std::vector<std::future<Canvas>>();

	auto f = [](const Camera * camera, const World & w, int line) {
		return camera->render_scanline(w, line);
	};

	for (int y = 0; y < this->c_v_size_; y++)
	{
		line_results.push_back(std::async(f, this, w, y));
	}

	for (size_t i = 0; i < line_results.size(); i++)
	{
		image.write_canvas_as_line(i, line_results[i].get());
	}

	return image;
}

Canvas Camera::render_scanline(const World & w, int line) const
{
	Canvas image_line = Canvas(this->c_h_size_, 1);

	std::cout << "Scanline: " << line + 1 << "/" << this->c_v_size_ << "\n";

	for (int x = 0; x < this->c_h_size_; x++)
	{
		// std::cout << "Pixel: (" << x << ", " << y << ")\n";
		Ray r = this->ray_from_pixel(x, line);
		Color color = w.color_at(r);
		image_line.write_pixel(x, 1, color);
	}

	return image_line;
}

// ------------------------------------------------------------------------
// Acssesors
// ------------------------------------------------------------------------

int Camera::get_horizontal_size() const
{
	return this->c_h_size_;
}

int Camera::get_vertical_size() const
{
	return this->c_v_size_;
}

float Camera::get_fov() const
{
	return this->c_fov_;
}

float Camera::get_pixel_size() const
{
	return this->c_pixel_size_;
}

// ------------------------------------------------------------------------
// Private Functions
// ------------------------------------------------------------------------

void Camera::pixel_size_()
{
	float half_view = tan(this->c_fov_ / 2.0f);
	float aspect_ratio = float(this->c_h_size_) / float(this->c_v_size_);

	if (aspect_ratio >= 1.0)
	{
		this->c_half_width_ = half_view;
		this->c_half_height_ = half_view / aspect_ratio;
	}
	else
	{
		this->c_half_width_ = half_view * aspect_ratio;
		this->c_half_height_ = half_view;
	}

	this->c_pixel_size_ = (this->c_half_width_ * 2.0f) / float(this->c_h_size_);
}
