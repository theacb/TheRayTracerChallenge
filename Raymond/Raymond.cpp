// Raymond.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>

float deg_to_rad(float degrees)
{
	return (degrees * float(M_PI)) / 180.0f;
}

float rad_to_deg(float radians)
{
	return (radians * 180.0f) / float(M_PI);
}

std::string pad_num(int num, int pad)
{
	std::string ts = std::to_string(num);
	return std::string(pad - ts.length(), '0') + ts;
}

std::string generate_name(std::string name, std::string folder, int version)
{
	time_t rawtime = time(0);
	struct tm ltm;
	localtime_s(&ltm, &rawtime);

	std::string date_time_string = (
		pad_num(1900 + ltm.tm_year - 2000, 2) +
		pad_num(1 + ltm.tm_mon, 2) +
		pad_num(ltm.tm_mday, 2) +
		"-" +
		pad_num(ltm.tm_hour, 2) +
		pad_num(ltm.tm_min, 2) +
		pad_num(ltm.tm_sec, 2)
		);

	return (
		folder +
		"\\" +
		name +
		"_" +
		pad_num(version, 2) +
		"_" +
		date_time_string +
		".ppm"
		);
}

// Based on an answer on Stack Overflow by Howard Hinnant
// https://stackoverflow.com/a/22593250/15062519
std::ostream& clock_display(std::ostream& os, std::chrono::duration<double, std::nano> dur)
{
	char fill = os.fill();
	os.fill('0');

	auto h = std::chrono::duration_cast<std::chrono::hours>(dur);
	dur -= h;
	auto m = std::chrono::duration_cast<std::chrono::minutes>(dur);
	dur -= m;
	auto s = std::chrono::duration_cast<std::chrono::seconds>(dur);
	dur -= s;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);

	os  << std::setw(2) << h.count() << "h:"
		<< std::setw(2) << m.count() << "m:"
		<< std::setw(2) << s.count() << "s:";
	os.fill(fill);

	os << ms.count() << "ms";

	return os;
}


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
	int canvas_width = 560;
	int canvas_height = 315;

	float screen_width = 7.0f * float(canvas_width) / float(canvas_height);
	float screen_height = screen_width * float(canvas_height) / float(canvas_width);

	float pixel_size = screen_width / float(canvas_width);
	float half_screen_width = screen_width * 0.5f;
	float half_screen_height = screen_height * 0.5f;

	float wall_z = 10.0f;

	Canvas c = Canvas(canvas_width, canvas_height);

	auto s = std::make_shared<Sphere>(Sphere());

	s->set_transform(
		Matrix4::Rotation_Z(static_cast<float>(M_PI) / 2.0f) *
		Matrix4::Scaling(0.5f, 1.0f, 1.0f)
		
	);

	auto material = std::dynamic_pointer_cast<PhongMaterial>(s->material);
	material->color = Color(1.0f, 0.2f, 1.0f);

	auto lgt = std::make_shared<PointLight>(Tuple::Point(-10.0, 10.0, -10.0), Color(1.0f, 1.0f, 1.0f));

	Tuple ray_origin = Tuple::Point(0.0f, 0.0f, -5.0f);

	std::cout << "Tracing...\n";

	for (int y = 0; y < canvas_height; y++)
	{
		std::cout << "Scanline: " << y + 1 << "/" << canvas_height << "\n";

		float y_pos = half_screen_height - (float(y) * pixel_size);

		for (int x = 0; x < canvas_width; x++)
		{

			float x_pos = (-half_screen_width) + (float(x) * pixel_size);

			Tuple ray_target = Tuple::Point(x_pos, y_pos, wall_z);
			
			Ray r = Ray(ray_origin, (ray_target - ray_origin).normalize());


			Intersections xs = intersect(r, s);


			Intersection h = xs.hit();

			if (h.is_valid())
			{
				auto obj_ptr = std::dynamic_pointer_cast<Primitive>(h.object);
				Tuple hit_point = r.position(h.t_value);

				auto mat_ptr = std::dynamic_pointer_cast<PhongMaterial>(obj_ptr->material);

				Color col = mat_ptr->lighting(
					lgt, 
					hit_point, 
					-(r.direction), 
					obj_ptr->normal_at(hit_point)
				);

				c.write_pixel(x, y, col);
			}
			else
			{
				auto r = float(x) / (canvas_width - 1);
				auto g = float(canvas_height - y) / (canvas_height - 1);

				c.write_pixel(x, y, Color(r, g, 1.0f));
			}

		}
	}

	std::cout << "Complete\n";

	canvas_to_ppm(c, "E:\\dump\\projects\\Raymond\\frames\\LightAndShading_CH06_04.ppm", false);

	return 0;
}

Canvas render_ch08_world(int width, int height, float fov)
{
	World w = World();

	// Lights

	auto light_000 = std::make_shared<PointLight>(Tuple::Point(-9.0f, 9.0f, -9.0f), Color(1.0f), 50.0f);
	light_000->falloff = true;
	light_000->set_name("light 000");
	w.add_object(light_000);

	auto light_001 = std::make_shared<PointLight>(Tuple::Point(0.0f, 6.0f, 5.0f), Color(1.0f, 0.0f, 0.0f), 50.0f);
	light_001->falloff = true;
	light_001->set_name("light 001");
	w.add_object(light_001);

	// Materials

	auto matte_gray_mtl = std::make_shared<PhongMaterial>();
	matte_gray_mtl->color = Color(0.18f);
	matte_gray_mtl->specular = 0.0f;
	matte_gray_mtl->ambient = 0.01f;

	auto shiny_green_mtl = std::make_shared<PhongMaterial>();
	shiny_green_mtl->color = Color(Color8Bit(54, 163, 83)).convert_srgb_to_linear();
	shiny_green_mtl->specular = 0.9f;
	shiny_green_mtl->ambient = 0.01f;

	auto shiny_yellow_mtl = std::make_shared<PhongMaterial>();
	shiny_yellow_mtl->color = Color(Color8Bit(245, 209, 66)).convert_srgb_to_linear();
	shiny_yellow_mtl->specular = 0.9f;
	shiny_yellow_mtl->ambient = 0.01f;

	auto matte_blue_mtl = std::make_shared<PhongMaterial>();
	matte_blue_mtl->color = Color(Color8Bit(21, 80, 117)).convert_srgb_to_linear();
	matte_blue_mtl->specular = 0.0f;
	matte_blue_mtl->ambient = 0.01f;

	// Objects

	auto floor_000 = std::make_shared<Sphere>("floor 000");
	floor_000->material = matte_gray_mtl;
	floor_000->set_transform(Matrix4::Scaling(20.0f, 0.01f, 20.0f));
	w.add_object(floor_000);

	auto wall_000 = std::make_shared<Sphere>("wall 000");
	wall_000->material = matte_gray_mtl;
	wall_000->set_transform(Matrix4::Translation(10.0f, 0.0f, 0.0f) * Matrix4::Scaling(0.01f, 20.0f, 20.0f));
	w.add_object(wall_000);

	auto wall_001 = std::make_shared<Sphere>("wall 001");
	wall_001->material = matte_gray_mtl;
	wall_001->set_transform(Matrix4::Translation(0.0f, 0.0f, 10.0f) * Matrix4::Scaling(20.0f, 20.0f, 0.01f));
	w.add_object(wall_001);

	auto wall_002 = std::make_shared<Sphere>("wall 002");
	wall_002->material = matte_gray_mtl;
	wall_002->set_transform(Matrix4::Translation(0.0f, 0.0f, -10.0f) * Matrix4::Scaling(20.0f, 20.0f, 0.01f));
	w.add_object(wall_002);

	auto wall_003 = std::make_shared<Sphere>("wall 003");
	wall_003->material = matte_gray_mtl;
	wall_003->set_transform(Matrix4::Translation(-10.0f, 0.0f, 0.0f) * Matrix4::Scaling(0.01f, 20.0f, 20.0f));
	w.add_object(wall_003);

	auto green_sphere = std::make_shared<Sphere>("green sphere");
	green_sphere->material = shiny_green_mtl;
	green_sphere->set_transform(Matrix4::Translation(0.0f, 1.0f, 0.0f));
	w.add_object(green_sphere);

	auto yellow_sphere_000 = std::make_shared<Sphere>("yellow sphere 000");
	yellow_sphere_000->material = shiny_yellow_mtl;
	yellow_sphere_000->set_transform(Matrix4::Translation(-1.5f, 0.5f, -0.0f) * Matrix4::Scaling(0.5f, 0.5f, 0.5f));
	w.add_object(yellow_sphere_000);

	auto yellow_sphere_001 = std::make_shared<Sphere>("yellow sphere 001");
	yellow_sphere_001->material = shiny_yellow_mtl;
	yellow_sphere_001->set_transform(Matrix4::Translation(2.0f, 0.5f, -0.5f) * Matrix4::Scaling(0.5f, 0.5f, 0.5f));
	w.add_object(yellow_sphere_001);

	int num_blue_sphs = 7;

	for (int i = 0; i < num_blue_sphs; i++)
	{
		float sx = float(num_blue_sphs - 1);

		auto blue_sphere_inst = std::make_shared<Sphere>("yellow sphere 001");
		blue_sphere_inst->material = matte_blue_mtl;

		blue_sphere_inst->set_transform(
			Matrix4::Translation((i * 2.0f) - sx, 2.0f, 2.0f) * 
			Matrix4::Rotation_Z((((float(M_PI) / 2.0f) / sx) * (num_blue_sphs - i)) - (float(M_PI) / 4.0f)) *
			Matrix4::Scaling(0.25f, 2.0f, 0.25f)
		);

		w.add_object(blue_sphere_inst);
	}

	Camera c = Camera(width, height, deg_to_rad(fov));

	Tuple from = Tuple::Point(0.0f, 1.0f, -5.0f);
	Tuple to = Tuple::Point(0.0f, 1.0f, 0.0f);
	Tuple up = Tuple::Vector(0.0f, 1.0f, 0.0f);

	c.set_transform(Matrix4::ViewTransform(from, to, up));

	std::cout << "Tracing...\n";

	// Canvas image = c.render_scanline(w, 0);
	Canvas image = c.render(w);

	std::cout << "Complete\n";

	return image;
}

int main()
{
	std::string chapter = "Shadows_CH08";
	std::string folder = "E:\\dump\\projects\\Raymond\\frames";
	int version = 6;

	int width = 10;
	int height = 5;
	float fov = 45.0;

	std::cout << "Executing Render " << chapter << " v" << pad_num(version, 2) << std::endl << std::endl;

	// Start time
	auto start = std::chrono::steady_clock::now();

	// Execution
	Canvas image = render_ch08_world(width, height, fov);

	// End time
	auto end = std::chrono::steady_clock::now();

	auto diff = end - start;

	std::cout << std::endl << "Time: ";
	clock_display(std::cout, diff);
	std::cout << std::endl << std::endl;

	// File Name
	std::string file_path = generate_name(chapter, folder, version);

	std::cout << "Writing file: " << file_path << std::endl;

	canvas_to_ppm(image, file_path);
	std::cout << "Complete" << std::endl;

	return 0;
}
