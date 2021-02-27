// Raymond.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>

struct Projectile
{
	Tuple position, velocity;
};

struct Environment
{
	Tuple gravity, wind;
};

Projectile tick(const Projectile & proj, const Environment & env)
{
	Projectile new_proj;
	new_proj.position = proj.position + proj.velocity;
	new_proj.velocity = proj.velocity + env.gravity + env.wind;
	return new_proj;
}

void projectile_to_square(Canvas & canvas, const Projectile proj)
{
	int center_x, center_y;

	Color line = Color(0.0f, 1.0f, 1.0f);
	Color fill = Color(0.5f, 0.5f, 1.0f);

	center_x = static_cast<int>(round(proj.position.x));
	center_y = canvas.height() - static_cast<int>(round(proj.position.y));

	canvas.write_pixel(center_x, center_y, fill);
	canvas.write_pixel(center_x + 1, center_y, line);
	canvas.write_pixel(center_x, center_y + 1, line);
	canvas.write_pixel(center_x + 1, center_y + 1, line);
	canvas.write_pixel(center_x - 1, center_y, line);
	canvas.write_pixel(center_x, center_y - 1, line);
	canvas.write_pixel(center_x - 1, center_y - 1, line);
	canvas.write_pixel(center_x + 1, center_y - 1, line);
	canvas.write_pixel(center_x - 1, center_y + 1, line);
}

int simulate_projectile()
{
	int width = 900;
	Canvas c = Canvas(width, width);

	std::cout << "\nProjectile:\n";
	Projectile p;
	Environment e;

	p.position = Tuple::Point(0.1f, 1.0f, 0.0f);
	p.velocity = Tuple::Vector(1.0f, 1.8f, 0.0f).normalize() * 11.25f;

	e.gravity = Tuple::Vector(0.0f, -0.1f, 0.0f);
	e.wind = Tuple::Vector(-0.01f, 0.0f, 0.0f);

	int this_tick = 0;
	int step = 10;

	while (p.position.y > 0.0)
	{
		if (this_tick % step == 0)
		{
			std::cout << this_tick << ": " << p.position << "\n";
			projectile_to_square(c, p);
		}
		p = tick(p, e);
		++this_tick;
	}

	//std::cout << c.to_ppm_lines(false).rdbuf();

	canvas_to_ppm(c, "E:\\dump\\projects\\Raymond\\frames\\ProjectileDotsCh02_tenth_02.ppm");

	return 0;
}

int render_clock()
{
	int width = 500;
	float clock_radius = static_cast<float>(width) * (3.0f / 8.0f);
	float center = static_cast<float>(width) * 0.5f;

	Canvas c = Canvas(width, width);
	Color pen_color = Color(1.0f, 1.0f, 1.0f);

	Tuple hour_point = Tuple::Point(0.0f, 0.0f, clock_radius);

	int hours = 0;


	Matrix4 increment = Matrix4::Rotation_Y((2.0f * static_cast<float>(M_PI)) * (1.0f / 12.0f));
	Matrix4 view_x_form = Matrix4::Translation(center, 0.0f, center);

	while (hours <= 12)
	{
		hour_point = increment * hour_point;

		Tuple pen = view_x_form * hour_point;

		std::cout << "Hour Point: " << hour_point << "\nPen point: " << pen << "\n";

		c.write_pixel(static_cast<int>(pen.x), static_cast<int>(pen.z), pen_color);

		++hours;
	}

	canvas_to_ppm(c, "E:\\dump\\projects\\Raymond\\frames\\ClockCh04_01.ppm");

	return 0;
}

int render_sphere()
{
	int canvas_width = 400;
	float ortho_width = 7.0f;
	float pixel_size = ortho_width / static_cast<float>(canvas_width);
	float half_ortho_size = ortho_width * 0.5f;
	float wall_z = 10.0f;

	Canvas c = Canvas(canvas_width, canvas_width);

	Color brush_color = Color(0.0f, 0.5f, 1.0f);
	Color bkg_color = Color(0.5f, 0.5f, 1.0f);

	Sphere s = Sphere();

	s.set_transform(Matrix4::Shear(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f) * Matrix4::Scaling(0.5f, 1.0f, 1.0f));

	Tuple ray_origin = Tuple::Point(0.0f, 0.0f, -5.0f);

	std::cout << "Tracing...\n";

	for (int y = 0; y < canvas_width; y++)
	{
		float y_pos = half_ortho_size - (static_cast<float>(y) * pixel_size);

		for (int x = 0; x < canvas_width; x++)
		{

			float x_pos = -half_ortho_size + (static_cast<float>(x) * pixel_size);

			Tuple ray_target = Tuple::Point(y_pos, x_pos, wall_z);
			
			Ray r = Ray(ray_origin, (ray_target - ray_origin).normalize());


			Intersections xs = intersect(r, &s);


			Intersection h = xs.hit();

			if (h.is_valid())
			{
				c.write_pixel(x, y, brush_color);
			}
			else
			{
				c.write_pixel(x, y, bkg_color);
			}

		}
	}

	std::cout << "Complete\n";

	canvas_to_ppm(c, "E:\\dump\\projects\\Raymond\\frames\\RaySphereIntersect_CH05_06.ppm");

	return 0;
}

int main()
{
	render_sphere();
}
