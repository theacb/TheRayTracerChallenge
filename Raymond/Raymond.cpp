// Raymond.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "tuple.h"
#include "canvas.h"
#include "color.h"
#include "Matrix.h"

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

int main()
{
	Matrix3 m = Matrix3({
	1.0f, 5.0f, 0.0f,
	-3.0f, 2.0f, 7.0f,
	0.0f, 6.0f, -3.0f
		});

	Matrix2 expected_result = Matrix2({
		-3.0f, 2.0f,
		-0.0f, 6.0f,
		});

	Matrix2 result = m.sub_matrix3(0, 2);

	std::cout << "\n" << result << "\n";

	std::cout << (result == expected_result) << "\n";
}
