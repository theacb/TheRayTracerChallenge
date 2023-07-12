#include "Camera.h"

// ------------------------------------------------------------------------
//
// Base Object
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Camera::Camera() : Camera(560, 315, M_PI / 4.0)
{
}

Camera::Camera(int width, int height, double fov)
{
	this->c_h_size_ = width;
	this->c_v_size_ = height;
	this->c_fov_ = fov;

    this->c_pixel_size_= 0.0;
    this->c_half_width_ = 0.0;
    this->c_half_height_ = 0.0;

	this->pixel_size_();
}

Camera::~Camera()
= default;

// ------------------------------------------------------------------------
// Rays
// ------------------------------------------------------------------------

Ray Camera::ray_from_pixel(int x, int y) const
{
    // Offset from the edge of the canvas to the pixel's center
    return this->ray_from_pixel(x, y, 0.5, 0.5);
}

Ray Camera::ray_from_pixel(int x, int y, double px_os_x, double px_os_y) const
{
    // px_os: pixel offset: A value between 0.0 and 1.0 that controls where in the pixel the ray is generated
    // Offset from the edge of the canvas to the pixel's center
    double x_offset = (double(x) + px_os_x) * this->c_pixel_size_;
    double y_offset = (double(y) + px_os_y) * this->c_pixel_size_;

    // The untransformed coordinates of the pixel in world space
    // Camera looks towards -z, so +x is to the left
    double world_x = this->c_half_width_ - x_offset;
    double world_y = this->c_half_height_ - y_offset;

    // Pre invert matrix so this is only done once
    Matrix4 inv_x_form = this->get_transform().inverse();

    // Using the camera's matrix, transform the canvas point and the origin
    // and then compute the ray's direction vector
    // The camera is at z=-1.0
    Tuple pixel = inv_x_form * Tuple::Point(world_x, world_y, -1.0);
    Tuple origin = inv_x_form * Tuple::Point(0.0, 0.0, 0.0);
    Tuple direction = (pixel - origin).normalize();

    return {origin, direction};
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

	// lambda to execute rendering of the line
	auto f = [](const Camera * camera, const World & w, int line) {
		return camera->render_scanline(w, line);
	};

	// Queue up all the lines
	for (int y = 0; y < this->c_v_size_; y++)
	{
		line_results.push_back(std::async(f, this, w, y));
	}

	// stitch them back together
	for (int i = 0; i < line_results.size(); i++)
	{
		image.write_canvas_as_line(i, line_results[i].get());
	}

	return image;
}

SampleBuffer Camera::multi_sample_threaded_render(const World &w) const
{
    int horizontal_buckets = std::ceil(double(this->c_h_size_) / double(w.bucket_size));
    int vertical_buckets = std::ceil(double(this->c_v_size_) / double(w.bucket_size));

    int num_buckets = horizontal_buckets * vertical_buckets;

    unsigned int c = std::thread::hardware_concurrency();

    auto bucket_results = std::vector<std::future<SampleBuffer>>();

    // lambda to execute rendering of the line
    auto f = [](const Camera * camera, const World & w, int x, int y, int width, int height, int bucket_id, int total_buckets)
            {
                std::ostringstream oss;
                oss << "Starting Bucket: " << bucket_id << "/" << total_buckets
                << " - Start Coordinates: (" << x << ", "<< y << ") - Dimensions ["
                << width << ", " << height << "]" << std::endl;

                std::cout << oss.str();
                return camera->multi_sample_render_bucket(w, x, y, width, height, bucket_id);
            };

    // Queue up all the lines
    for (int x = 0; x < horizontal_buckets; x++)
    {
        for (int y = 0; y < vertical_buckets; y++)
        {
            // Default Bucket Size
            int width = w.bucket_size;
            int height = w.bucket_size;

            // On the edges of the image, the buckets may not be square.
            // This ensures that the overflow is not rendered, as that may result in thousands of discarded pixels
            if (x == horizontal_buckets - 1)
            {
                width = int(fmod(double(this->c_h_size_), double(w.bucket_size))) - 1;
            }
            if (y == vertical_buckets - 1)
            {
                height = int(fmod(double(this->c_v_size_), double(w.bucket_size))) - 1;
            }

            if (width >= 0 && height >= 0)
            {
                // Offset of the bucket from top left (0, 0)
                int x_offset = x * w.bucket_size;
                int y_offset = y * w.bucket_size;

                int bucket_id = (x * vertical_buckets) + y;

                // Adds bucket to the queue
                bucket_results.push_back(std::async(f, this, w, x_offset, y_offset, width, height, bucket_id, num_buckets));
            }
        }
    }

    std::ostringstream oss;
    oss << "Stitching final image from " << bucket_results.size() << " buckets.\n";
    std::cout << oss.str();

    // Create bounding box and final SampleBuffer
    AABB2D extents = this->extent_from_bucket_(0, 0, this->c_h_size_, this->c_v_size_);
    SampleBuffer image = SampleBuffer(0, 0, this->c_h_size_, this->c_v_size_, extents);

    std::cout << image << std::endl;

    std::cout << "Pixel size: " << this->c_pixel_size_ << std::endl;
    std::string folder = R"(I:\projects\Raymond\frames\dump\)";
    int i = 0;
    for (auto & br : bucket_results)
    {
        SampleBuffer bucket = br.get();

        std::cout << "Bucket " << i << " - " << bucket << " - [";

        for (const auto & px: bucket)
        {
            std::cout << px->get_samples().size() << ", ";
        }

        std::cout << "]" << std::endl;

        canvas_to_ppm(bucket.to_canvas(rgb), folder + std::to_string(i) + ".ppm", true);
        image.write_portion(bucket);

        i++;
    }

    // stitch them back together
//    for (auto & br : bucket_results)
//    {
//        image.write_portion(br.get());
//    }

    std::cout << "Imaged stitched!\n";

    return image;
}

Canvas Camera::render_scanline(const World & w, int line) const
{
	// Create a canvas with the full image width but only 1 height
	Canvas image_line = Canvas(this->c_h_size_, 1);

    std::ostringstream oss;
	oss << "Scanline: " << line + 1 << "/" << this->c_v_size_ << std::endl;
    std::cout << oss.str();


	for (int x = 0; x < this->c_h_size_; x++)
	{
		// Cast Rays
		Ray r = this->ray_from_pixel(x, line);
		Color color = w.color_at(r);
		// Always write to the first line (index 0)
		image_line.write_pixel(x, 0, color);
	}

	return image_line;
}

SampleBuffer Camera::multi_sample_render_bucket(const World & w, int x, int y, int width, int height, int bucket_id) const
{
    // Generate extents
    AABB2D extents = this->extent_from_bucket_(x, y, width, height);
    // Create new sample buffer to place buckets into
    SampleBuffer bucket = SampleBuffer(x, y, width, height, extents);

    // Iterate through the pixels of the sample buffer
    for (int ys = 0; ys < height; ys++)
    {

        for (int xs = 0; xs < width; xs++)
        {
            // Multi Sample
            for (int s = 0; s < w.sample_min; s++)
            {
                // Random between 0.0 and 1.0 that translates to a pixel offset
                double px_os_x = random_double();
                double px_os_y = random_double();

                // Casts ray to a random point within the pixel
                // TODO: This is not delivering the correct offsets per bucket and seems to be rendering the same tile over and over again
                Ray r = this->ray_from_pixel(xs, ys, px_os_x, px_os_y);
                // Samples at the Ray
                Sample sample = w.sample_at(r);
                // Assign origin coordinate
                sample.CanvasOrigin = bucket.coordinates_from_pixel(xs, ys, px_os_x, px_os_y);
                sample.WorldOrigin = r.origin;
                sample.BucketID = bucket_id;
                sample.calculate_sample();

                // Write to bucket
                bucket.write_sample(xs, ys, sample);
            }
        }
    }

    for (std::shared_ptr<SampledPixel> & s: bucket)
    {
        s->full_average();
    }

    return bucket;
}

// ------------------------------------------------------------------------
// Accessors
// ------------------------------------------------------------------------

int Camera::get_horizontal_size() const
{
	return this->c_h_size_;
}

int Camera::get_vertical_size() const
{
	return this->c_v_size_;
}

double Camera::get_fov() const
{
	return this->c_fov_;
}

double Camera::get_pixel_size() const
{
	return this->c_pixel_size_;
}

// ------------------------------------------------------------------------
// Private Functions
// ------------------------------------------------------------------------

void Camera::pixel_size_()
{
	double half_view = tan(this->c_fov_ / 2.0);
	double aspect_ratio = double(this->c_h_size_) / double(this->c_v_size_);

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

	this->c_pixel_size_ = (this->c_half_width_ * 2.0) / double(this->c_h_size_);
}

AABB2D Camera::extent_from_bucket_(const int x, const int y, const int w, const int h) const {
    // AABB2Ds are square, so the largest dimension sets the square size
    int size = (w > h) ? w : h;

    // Offset from the edge of the canvas to the pixel's NE (Northeast) corner
    double ne_x_offset = (double(x)) * this->c_pixel_size_;
    double ne_y_offset = (double(y)) * this->c_pixel_size_;

    // SW (Southwest) corner
    double sw_x_offset = (double(x + size)) * this->c_pixel_size_;
    double sw_y_offset = (double(y + size)) * this->c_pixel_size_;

    return {Tuple::Point2D(ne_x_offset, ne_y_offset), Tuple::Point2D(sw_x_offset, sw_y_offset)};
}




