// Raymond.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <string>
#include <exception>
#include <typeinfo>
#include <stdexcept>

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

	Color line = Color(0.0, 1.0, 1.0);
	Color fill = Color(0.5, 0.5, 1.0);

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

	p.position = Tuple::Point(0.1, 1.0, 0.0);
	p.velocity = Tuple::Vector(1.0, 1.8, 0.0).normalize() * 11.25;

	e.gravity = Tuple::Vector(0.0, -0.1, 0.0);
	e.wind = Tuple::Vector(-0.01, 0.0, 0.0);

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
	double clock_radius = static_cast<double>(width) * (3.0 / 8.0);
	double center = static_cast<double>(width) * 0.5;

	Canvas c = Canvas(width, width);
	Color pen_color = Color(1.0, 1.0, 1.0);

	Tuple hour_point = Tuple::Point(0.0, 0.0, clock_radius);

	int hours = 0;


	Matrix4 increment = Matrix4::Rotation_Y((2.0 * static_cast<double>(M_PI)) * (1.0 / 12.0));
	Matrix4 view_x_form = Matrix4::Translation(center, 0.0, center);

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

	double screen_width = 7.0 * double(canvas_width) / double(canvas_height);
	double screen_height = screen_width * double(canvas_height) / double(canvas_width);

	double pixel_size = screen_width / double(canvas_width);
	double half_screen_width = screen_width * 0.5;
	double half_screen_height = screen_height * 0.5;

	double wall_z = 10.0;

	Canvas c = Canvas(canvas_width, canvas_height);

	auto s = std::make_shared<Sphere>(Sphere());

	s->set_transform(
		Matrix4::Rotation_Z(static_cast<double>(M_PI) / 2.0) *
		Matrix4::Scaling(0.5, 1.0, 1.0)
		
	);

	auto material = std::dynamic_pointer_cast<PhongMaterial>(s->material);
	material->color = Color(1.0, 0.2, 1.0);

	auto lgt = std::make_shared<PointLight>(Tuple::Point(-10.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));

	Tuple ray_origin = Tuple::Point(0.0, 0.0, -5.0);

	std::cout << "Tracing...\n";

	for (int y = 0; y < canvas_height; y++)
	{
		std::cout << "Scanline: " << y + 1 << "/" << canvas_height << "\n";

		double y_pos = half_screen_height - (double(y) * pixel_size);

		for (int x = 0; x < canvas_width; x++)
		{

			double x_pos = (-half_screen_width) + (double(x) * pixel_size);

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
					obj_ptr->local_normal_at(hit_point)
				);

				c.write_pixel(x, y, col);
			}
			else
			{
				auto r = double(x) / (canvas_width - 1);
				auto g = double(canvas_height - y) / (canvas_height - 1);

				c.write_pixel(x, y, Color(r, g, 1.0));
			}

		}
	}

	std::cout << "Complete\n";

	canvas_to_ppm(c, "E:\\dump\\projects\\Raymond\\frames\\LightAndShading_CH06_04.ppm", false);

	return 0;
}

Canvas render_ch08_world(int width, int height, double fov)
{
	World w = World();

	// Lights

	auto light_000 = std::make_shared<PointLight>(Tuple::Point(-9.0, 9.0, -9.0), Color(1.0), 50.0);
	light_000->falloff = true;
	light_000->set_name("light 000");
	w.add_object(light_000);

	auto light_001 = std::make_shared<PointLight>(Tuple::Point(0.0, 6.0, 5.0), Color(1.0, 0.0, 0.0), 50.0);
	light_001->falloff = true;
	light_001->set_name("light 001");
	w.add_object(light_001);

	// Materials

	auto matte_gray_mtl = std::make_shared<PhongMaterial>();
	matte_gray_mtl->color = Color(0.18);
	matte_gray_mtl->specular = 0.0;
	matte_gray_mtl->ambient = 0.01;

	auto shiny_green_mtl = std::make_shared<PhongMaterial>();
	shiny_green_mtl->color = Color(Color8Bit(54, 163, 83)).convert_srgb_to_linear();
	shiny_green_mtl->specular = 0.9;
	shiny_green_mtl->ambient = 0.01;

	auto shiny_yellow_mtl = std::make_shared<PhongMaterial>();
	shiny_yellow_mtl->color = Color(Color8Bit(245, 209, 66)).convert_srgb_to_linear();
	shiny_yellow_mtl->specular = 0.9;
	shiny_yellow_mtl->ambient = 0.01;

	auto matte_blue_mtl = std::make_shared<PhongMaterial>();
	matte_blue_mtl->color = Color(Color8Bit(21, 80, 117)).convert_srgb_to_linear();
	matte_blue_mtl->specular = 0.0;
	matte_blue_mtl->ambient = 0.01;

	// Objects

	auto floor_000 = std::make_shared<Sphere>("floor 000");
	floor_000->material = matte_gray_mtl;
	floor_000->set_transform(Matrix4::Scaling(20.0, 0.01, 20.0));
	w.add_object(floor_000);

	auto wall_000 = std::make_shared<Sphere>("wall 000");
	wall_000->material = matte_gray_mtl;
	wall_000->set_transform(Matrix4::Translation(10.0, 0.0, 0.0) * Matrix4::Scaling(0.01, 20.0, 20.0));
	w.add_object(wall_000);

	auto wall_001 = std::make_shared<Sphere>("wall 001");
	wall_001->material = matte_gray_mtl;
	wall_001->set_transform(Matrix4::Translation(0.0, 0.0, 10.0) * Matrix4::Scaling(20.0, 20.0, 0.01));
	w.add_object(wall_001);

	auto wall_002 = std::make_shared<Sphere>("wall 002");
	wall_002->material = matte_gray_mtl;
	wall_002->set_transform(Matrix4::Translation(0.0, 0.0, -10.0) * Matrix4::Scaling(20.0, 20.0, 0.01));
	w.add_object(wall_002);

	auto wall_003 = std::make_shared<Sphere>("wall 003");
	wall_003->material = matte_gray_mtl;
	wall_003->set_transform(Matrix4::Translation(-10.0, 0.0, 0.0) * Matrix4::Scaling(0.01, 20.0, 20.0));
	w.add_object(wall_003);

	auto green_sphere = std::make_shared<Sphere>("green sphere");
	green_sphere->material = shiny_green_mtl;
	green_sphere->set_transform(Matrix4::Translation(0.0, 1.0, 0.0));
	w.add_object(green_sphere);

	auto yellow_sphere_000 = std::make_shared<Sphere>("yellow sphere 000");
	yellow_sphere_000->material = shiny_yellow_mtl;
	yellow_sphere_000->set_transform(Matrix4::Translation(-1.5, 0.5, -0.0) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_000);

	auto yellow_sphere_001 = std::make_shared<Sphere>("yellow sphere 001");
	yellow_sphere_001->material = shiny_yellow_mtl;
	yellow_sphere_001->set_transform(Matrix4::Translation(2.0, 0.5, -0.5) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_001);

	int num_blue_sphs = 7;

	for (int i = 0; i < num_blue_sphs; i++)
	{
		double sx = double(num_blue_sphs - 1);

		auto blue_sphere_inst = std::make_shared<Sphere>("yellow sphere 001");
		blue_sphere_inst->material = matte_blue_mtl;

		blue_sphere_inst->set_transform(
			Matrix4::Translation((i * 2.0) - sx, 2.0, 2.0) * 
			Matrix4::Rotation_Z((((M_PI / 2.0) / sx) * (num_blue_sphs - i)) - (M_PI / 4.0)) *
			Matrix4::Scaling(0.25, 2.0, 0.25)
		);

		w.add_object(blue_sphere_inst);
	}

	Camera c = Camera(width, height, deg_to_rad(fov));

	Tuple from = Tuple::Point(0.0, 1.0, -5.0);
	Tuple to = Tuple::Point(0.0, 1.0, 0.0);
	Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

	c.set_transform(Matrix4::ViewTransform(from, to, up));

	std::cout << "Tracing...\n";

	Canvas image = c.threaded_render(w);
	// Canvas image = c.render(w);

	std::cout << "Complete\n";

	return image;
}

Canvas render_ch09_world(int width, int height, double fov)
{
	World w = World();

	w.background = std::make_shared<NormalGradientBackground>();

	// Lights

	auto light_000 = std::make_shared<PointLight>(Tuple::Point(-9.0, 9.0, -9.0), Color(1.0), 50.0);
	light_000->falloff = true;
	light_000->set_name("light 000");
	w.add_object(light_000);

	auto light_001 = std::make_shared<PointLight>(Tuple::Point(0.0, 6.0, 5.0), Color(1.0, 0.0, 0.0), 50.0);
	light_001->falloff = true;
	light_001->set_name("light 001");
	w.add_object(light_001);

	// Materials

	auto matte_gray_mtl = std::make_shared<PhongMaterial>();
	matte_gray_mtl->color = Color(0.18);
	matte_gray_mtl->specular = 0.0;
	matte_gray_mtl->ambient = 0.01;

	auto shiny_green_mtl = std::make_shared<PhongMaterial>();
	shiny_green_mtl->color = Color(Color8Bit(54, 163, 83)).convert_srgb_to_linear();
	shiny_green_mtl->specular = 0.9;
	shiny_green_mtl->ambient = 0.01;

	auto shiny_yellow_mtl = std::make_shared<PhongMaterial>();
	shiny_yellow_mtl->color = Color(Color8Bit(245, 209, 66)).convert_srgb_to_linear();
	shiny_yellow_mtl->specular = 0.9;
	shiny_yellow_mtl->ambient = 0.01;

	auto matte_blue_mtl = std::make_shared<PhongMaterial>();
	matte_blue_mtl->color = Color(Color8Bit(21, 80, 117)).convert_srgb_to_linear();
	matte_blue_mtl->specular = 0.0;
	matte_blue_mtl->ambient = 0.01;

	// Objects

	auto floor_000 = std::make_shared<InfinitePlane>("floor 000");
	floor_000->material = matte_gray_mtl;
	w.add_object(floor_000);

	auto green_sphere = std::make_shared<Sphere>("green sphere");
	green_sphere->material = shiny_green_mtl;
	green_sphere->set_transform(Matrix4::Translation(0.0, 1.0, 0.0));
	w.add_object(green_sphere);

	auto yellow_sphere_000 = std::make_shared<Sphere>("yellow sphere 000");
	yellow_sphere_000->material = shiny_yellow_mtl;
	yellow_sphere_000->set_transform(Matrix4::Translation(-1.5, 0.5, -0.0) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_000);

	auto yellow_sphere_001 = std::make_shared<Sphere>("yellow sphere 001");
	yellow_sphere_001->material = shiny_yellow_mtl;
	yellow_sphere_001->set_transform(Matrix4::Translation(2.0, 0.5, -0.5) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_001);

	int num_blue_sphs = 7;

	for (int i = 0; i < num_blue_sphs; i++)
	{
		double sx = double(num_blue_sphs - 1);

		auto blue_sphere_inst = std::make_shared<Sphere>("yellow sphere 001");
		blue_sphere_inst->material = matte_blue_mtl;

		blue_sphere_inst->set_transform(
			Matrix4::Translation((i * 2.0) - sx, 0.0, 2.0) *
			Matrix4::Rotation_Z((((M_PI / 2.0) / sx) * (num_blue_sphs - i)) - (M_PI / 4.0)) *
			Matrix4::Scaling(0.25, 2.0, 0.25)
		);

		w.add_object(blue_sphere_inst);
	}

	Camera c = Camera(width, height, deg_to_rad(fov));

	Tuple from = Tuple::Point(0.0, 0.8, -5.0);
	Tuple to = Tuple::Point(0.0, 1.0, 0.0);
	Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

	c.set_transform(Matrix4::ViewTransform(from, to, up));

	std::cout << "Tracing...\n";

	Canvas image = c.threaded_render(w);
	// Canvas image = c.render(w);

	std::cout << "Complete\n";

	return image;
}

Canvas render_ch10_world(int width, int height, double fov)
{
	World w = World();

	w.background = std::make_shared<NormalGradientBackground>();

	// Lights

	auto light_000 = std::make_shared<PointLight>(Tuple::Point(-9.0, 9.0, -9.0), Color(1.0), 50.0);
	light_000->falloff = true;
	light_000->set_name("light 000");
	w.add_object(light_000);

	auto light_001 = std::make_shared<PointLight>(Tuple::Point(0.0, 6.0, 5.0), Color(1.0, 0.0, 0.0), 50.0);
	light_001->falloff = true;
	light_001->set_name("light 001");
	w.add_object(light_001);

	// Materials

	auto matte_gray_mtl = std::make_shared<PhongMaterial>();
	matte_gray_mtl->color = Color(0.18);
	matte_gray_mtl->specular = 0.0;
	matte_gray_mtl->ambient = 0.01;

	auto gray_checkers_tex = std::make_shared<CheckerMap>(Color(0.18), Color(0.36));
	auto gray_ring_tex = std::make_shared<RingMap>(Color(0.18), Color(0.36));
	auto gray_gradient_tex = std::make_shared<GradientMap>(Color(0.18), Color(0.36));
	auto gray_stripe_tex = std::make_shared<StripeMap>(Color(0.18), Color(0.36));

	auto stripe1 = std::make_shared<StripeMap>(Color(1.0, 0.0, 0.0), Color(0.5, 0.0, 0.0));
	stripe1->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5) * Matrix4::Rotation_Y(deg_to_rad(90)));
	auto stripe2 = std::make_shared<StripeMap>(Color(0.0, 1.0, 0.0), Color(0.0, 0.5, 0.0));
	stripe2->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));

	auto c_map_1 = std::make_shared <CompositeMap>(stripe1, stripe2, CompAdd, 1.0);

	auto perlin_noise = std::make_shared<ColoredPerlin>(32255);
	perlin_noise->octaves = 6;

	matte_gray_mtl->color.connect(perlin_noise);

	auto shiny_green_mtl = std::make_shared<PhongMaterial>();
	Color green = Color(Color8Bit(57, 0, 214)).convert_srgb_to_linear();
	shiny_green_mtl->color = green;
	shiny_green_mtl->specular = 0.9;
	shiny_green_mtl->ambient = 0.01;
	auto green_stripe_tex = std::make_shared<StripeMap>(green, green * 0.6);
	green_stripe_tex->set_mapping_space(ObjectSpace);
	green_stripe_tex->transform->set_transform(Matrix4::Scaling(0.1, 0.1, 0.1));

	auto green_stripe_perturb = std::make_shared<PerturbMap>(green_stripe_tex, perlin_noise);
	green_stripe_perturb->displacement_remap = true;
	green_stripe_perturb->scale = 0.1;

	shiny_green_mtl->color.connect(green_stripe_perturb);

	auto shiny_yellow_mtl = std::make_shared<PhongMaterial>();
	shiny_yellow_mtl->color = Color(Color8Bit(245, 209, 66)).convert_srgb_to_linear();
	shiny_yellow_mtl->specular = 0.9;
	shiny_yellow_mtl->ambient = 0.01;

	auto matte_blue_mtl = std::make_shared<PhongMaterial>();
	matte_blue_mtl->color = Color(Color8Bit(21, 80, 117)).convert_srgb_to_linear();
	matte_blue_mtl->specular = 0.0;
	matte_blue_mtl->ambient = 0.01;

	// Objects

	auto floor_000 = std::make_shared<InfinitePlane>("floor 000");
	floor_000->material = matte_gray_mtl;
	w.add_object(floor_000);

	auto green_sphere = std::make_shared<Sphere>("green sphere");
	green_sphere->material = shiny_green_mtl;
	green_sphere->set_transform(Matrix4::Translation(0.0, 1.0, 0.0) * Matrix4::Rotation_Z(deg_to_rad(45.0)));
	w.add_object(green_sphere);

	auto yellow_sphere_000 = std::make_shared<Sphere>("yellow sphere 000");
	yellow_sphere_000->material = shiny_yellow_mtl;
	yellow_sphere_000->set_transform(Matrix4::Translation(-1.5, 0.5, -0.0) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_000);

	auto yellow_sphere_001 = std::make_shared<Sphere>("yellow sphere 001");
	yellow_sphere_001->material = shiny_yellow_mtl;
	yellow_sphere_001->set_transform(Matrix4::Translation(2.0, 0.5, -0.5) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_001);

	int num_blue_sphs = 7;

	for (int i = 0; i < num_blue_sphs; i++)
	{
		double sx = double(num_blue_sphs - 1);

		auto blue_sphere_inst = std::make_shared<Sphere>("yellow sphere 001");
		blue_sphere_inst->material = matte_blue_mtl;

		blue_sphere_inst->set_transform(
			Matrix4::Translation((i * 2.0) - sx, 0.0, 2.0) *
			Matrix4::Rotation_Z((((M_PI / 2.0) / sx) * (num_blue_sphs - i)) - (M_PI / 4.0)) *
			Matrix4::Scaling(0.25, 2.0, 0.25)
		);

		w.add_object(blue_sphere_inst);
	}

	Camera c = Camera(width, height, deg_to_rad(fov));

	Tuple from = Tuple::Point(0.0, 0.8, -5.0);
	Tuple to = Tuple::Point(0.0, 1.0, 0.0);
	Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

	c.set_transform(Matrix4::ViewTransform(from, to, up));

	std::cout << "Tracing...\n";

	Canvas image = c.threaded_render(w);
	// Canvas image = c.render(w);

	std::cout << "Complete\n";

	return image;
}

void shadowed_test(World & w, const std::shared_ptr<Light> light, const Tuple & point, const int depth)
{
	// locater
	auto locator = std::make_shared<Sphere>("locator 001");
	auto locator_mat_phong = std::dynamic_pointer_cast<PhongMaterial>(locator->material);
	locator_mat_phong->ambient.set_value(Color(1.0));
	locator_mat_phong->color.set_value(Color(1.0, 1.0, 0.0) * (1.0 - (double(depth) / 30.0)));
	locator_mat_phong->specular.set_value(0.0);
	locator->set_transform(Matrix4::Translation(point.x, point.y, point.z) * Matrix4::Scaling(0.05, 0.05, 0.05));
	w.add_object(locator);

	std::cout << "Placing locater: " << point << "Number " << depth << std::endl;

	// Function
	Tuple v = light->position() - point;
	double light_distance = v.magnitude();
	Tuple direction = v.normalize();

	Ray r = Ray(point, direction, depth);
	Intersections ix = w.intersect_world(r);

	Intersection h = ix.hit();

	if (h.is_valid() && h.t_value < light_distance)
	{
		IxComps comps = IxComps(h, r, ix);
		auto obj_prim = std::dynamic_pointer_cast<Primitive>(h.object);
		auto mat_phong = std::dynamic_pointer_cast<PhongMaterial>(obj_prim->material);

		// Start transmit function
		Color transmittance = mat_phong->refraction.sample_at(comps);

		if (mat_phong->transparent_shadows && transmittance.magnitude() > Color(light->cutoff).magnitude() && comps.ray_depth < 30)
		{
			// light that is reflected casts shadows
			double cos_i = Tuple::dot(comps.eye_v, comps.normal_v);

			if (cos_i > 0.001)
			{
				// Copy Transmittance
				Color result = Color(transmittance);

				int hit_index = ix.get_hit_index();

				if (hit_index + 1 < ix.size())
				{
					Intersection next_ix = ix[hit_index + 1];

					double point_to_light_distance = (comps.point - light->position()).magnitude();
					double distance = next_ix.t_value - comps.t_value;

					if (point_to_light_distance > distance)
					{
						shadowed_test(w, light, comps.under_point, depth + 1);
					}

					// Clip linear attenuation to [0,1];
					double	linear_attenuation = clip(1.0 / (fmin(distance, point_to_light_distance) * comps.n2 * 5.0), 0.0, 1.0);
					result = result * linear_attenuation;
				}

				return;
			}
		}
	}
	return;
}

World render_ch11_world()
{
	World w = World();

	w.background = std::make_shared<NormalGradientBackground>();

	// Lights

	auto light_000 = std::make_shared<PointLight>(Tuple::Point(-9.0, 9.0, -9.0), Color(1.0), 50.0);
	light_000->falloff = true;
	light_000->set_name("light 000");
	w.add_object(light_000);

	auto light_001 = std::make_shared<PointLight>(Tuple::Point(0.0, 6.0, 5.0), Color(1.0, 0.0, 0.0), 50.0);
	light_001->falloff = true;
	light_001->set_name("light 001");
	w.add_object(light_001);

	// Materials

	auto matte_gray_mtl = std::make_shared<PhongMaterial>();
	matte_gray_mtl->color = Color(0.18);
	matte_gray_mtl->specular = 0.0;
	matte_gray_mtl->ambient = 0.01;

	auto gray_checkers_tex = std::make_shared<CheckerMap>(Color(0.18), Color(0.36));
	auto gray_ring_tex = std::make_shared<RingMap>(Color(0.18), Color(0.36));
	auto gray_gradient_tex = std::make_shared<GradientMap>(Color(0.18), Color(0.36));
	auto gray_stripe_tex = std::make_shared<StripeMap>(Color(0.18), Color(0.36));

	auto stripe1 = std::make_shared<StripeMap>(Color(1.0, 0.0, 0.0), Color(0.5, 0.0, 0.0));
	stripe1->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5) * Matrix4::Rotation_Y(deg_to_rad(90)));
	auto stripe2 = std::make_shared<StripeMap>(Color(0.0, 1.0, 0.0), Color(0.0, 0.5, 0.0));
	stripe2->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));

	auto c_map_1 = std::make_shared <CompositeMap>(stripe1, stripe2, CompAdd, 1.0);

	auto perlin_noise = std::make_shared<ColoredPerlin>(32255);
	perlin_noise->octaves = 6;

	//matte_gray_mtl->color.connect(perlin_noise);

	auto shiny_purple_mtl = std::make_shared<PhongMaterial>();
	Color purple = Color(Color8Bit(57, 0, 214)).convert_srgb_to_linear();
	shiny_purple_mtl->color.set_value(purple);
	shiny_purple_mtl->specular.set_value(0.9);
	shiny_purple_mtl->ambient.set_value(0.01);
	shiny_purple_mtl->reflection_roughness.set_value(0.05);
	shiny_purple_mtl->reflection.set_value(Color(1.0));
	shiny_purple_mtl->ior = 20.0;

	// Reflection Map
	auto purple_stripe_tex = std::make_shared<StripeMap>(purple, Color(0.1));
	purple_stripe_tex->set_mapping_space(ObjectSpace);
	purple_stripe_tex->transform->set_transform(Matrix4::Scaling(0.1, 0.1, 0.1));

	auto purple_stripe_perturb = std::make_shared<PerturbMap>(purple_stripe_tex, perlin_noise);
	purple_stripe_perturb->displacement_remap = true;
	purple_stripe_perturb->scale = 0.1;

	// Shininess Map
	auto bw_stripe_tex = std::make_shared<StripeMap>(Color(200.0), Color(10.0));
	bw_stripe_tex->set_mapping_space(ObjectSpace);
	bw_stripe_tex->transform->set_transform(Matrix4::Scaling(0.1, 0.1, 0.1));

	auto bw_stripe_perturb = std::make_shared<PerturbMap>(bw_stripe_tex, perlin_noise);
	bw_stripe_perturb->displacement_remap = true;
	bw_stripe_perturb->scale = 0.1;

	// Maps
	//shiny_purple_mtl->color.connect(purple_stripe_perturb);
	shiny_purple_mtl->reflection.connect(purple_stripe_perturb);
	shiny_purple_mtl->shininess.connect(bw_stripe_tex);

	auto shiny_yellow_mtl = std::make_shared<PhongMaterial>();
	shiny_yellow_mtl->color = Color(Color8Bit(245, 209, 66)).convert_srgb_to_linear();
	shiny_yellow_mtl->specular = 0.9;
	shiny_yellow_mtl->ambient = 0.01;

	auto matte_blue_mtl = std::make_shared<PhongMaterial>();
	matte_blue_mtl->color = Color(Color8Bit(21, 80, 117)).convert_srgb_to_linear();
	matte_blue_mtl->specular = 0.0;
	matte_blue_mtl->ambient = 0.01;

	// Objects

	auto gs = Sphere::GlassSphere();
	gs->set_name("Glass Sphere 001");
	gs->set_transform(Matrix4::Translation(-2.0, 0.5, -1.0) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(gs);

	auto gs2 = Sphere::GlassSphere();
	gs2->set_name("Glass Sphere 002");
	gs2->set_transform(Matrix4::Translation(-2.0, 0.5, -1.0) * Matrix4::Scaling(0.4, 0.4, 0.4));
	w.add_object(gs2);

	auto glass_mtl = std::dynamic_pointer_cast<PhongMaterial>(gs->material);
	glass_mtl->reflection.set_value(Color(1.0));
	glass_mtl->color.set_value(Color(0.0));

	auto air_mtl = std::dynamic_pointer_cast<PhongMaterial>(gs2->material);
	air_mtl->reflection.set_value(Color(1.0));
	air_mtl->color.set_value(Color(0.0));
	air_mtl->ior = 1.0;

	auto floor_000 = std::make_shared<InfinitePlane>("floor 000");
	floor_000->material = matte_gray_mtl;
	w.add_object(floor_000);

	auto green_sphere = std::make_shared<Sphere>("green sphere");
	green_sphere->material = shiny_purple_mtl;
	green_sphere->set_transform(Matrix4::Translation(0.0, 1.0, 0.0) * Matrix4::Rotation_Z(deg_to_rad(45.0)));
	w.add_object(green_sphere);

	auto yellow_sphere_000 = std::make_shared<Sphere>("yellow sphere 000");
	yellow_sphere_000->material = shiny_yellow_mtl;
	yellow_sphere_000->set_transform(Matrix4::Translation(-1.5, 0.5, -0.0) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_000);

	auto yellow_sphere_001 = std::make_shared<Sphere>("yellow sphere 001");
	yellow_sphere_001->material = shiny_yellow_mtl;
	yellow_sphere_001->set_transform(Matrix4::Translation(2.0, 0.5, -0.5) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_001);

	int num_blue_sphs = 7;

	for (int i = 0; i < num_blue_sphs; i++)
	{
		double sx = double(num_blue_sphs - 1);

		auto blue_sphere_inst = std::make_shared<Sphere>("yellow sphere 001");
		blue_sphere_inst->material = matte_blue_mtl;

		blue_sphere_inst->set_transform(
			Matrix4::Translation((i * 2.0) - sx, 0.0, 2.0) *
			Matrix4::Rotation_Z((((M_PI / 2.0) / sx) * (num_blue_sphs - i)) - (M_PI / 4.0)) *
			Matrix4::Scaling(0.25, 2.0, 0.25)
		);

		w.add_object(blue_sphere_inst);
	}

	// shadowed_test(w, light_001, Tuple::Point(-2.1586175486649597, 0.64386222869072685,-1.3379706235927262), 0);

	return w;
}

World render_ch12_world()
{
	World w = World();

	w.background = std::make_shared<NormalGradientBackground>();

	// Lights

	auto light_000 = std::make_shared<PointLight>(Tuple::Point(-9.0, 9.0, -9.0), Color(1.0), 50.0);
	light_000->falloff = true;
	light_000->set_name("light 000");
	w.add_object(light_000);

	auto light_001 = std::make_shared<PointLight>(Tuple::Point(0.0, 6.0, 5.0), Color(1.0, 0.0, 0.0), 50.0);
	light_001->falloff = true;
	light_001->set_name("light 001");
	w.add_object(light_001);

	// Materials

	auto matte_gray_mtl = std::make_shared<PhongMaterial>();
	matte_gray_mtl->color = Color(0.18);
	matte_gray_mtl->specular = 0.0;
	matte_gray_mtl->ambient = 0.01;

	auto gray_checkers_tex = std::make_shared<CheckerMap>(Color(0.18), Color(0.36));
	auto gray_ring_tex = std::make_shared<RingMap>(Color(0.18), Color(0.36));
	auto gray_gradient_tex = std::make_shared<GradientMap>(Color(0.18), Color(0.36));
	auto gray_stripe_tex = std::make_shared<StripeMap>(Color(0.18), Color(0.36));

	auto stripe1 = std::make_shared<StripeMap>(Color(1.0, 0.0, 0.0), Color(0.5, 0.0, 0.0));
	stripe1->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5) * Matrix4::Rotation_Y(deg_to_rad(90)));
	auto stripe2 = std::make_shared<StripeMap>(Color(0.0, 1.0, 0.0), Color(0.0, 0.5, 0.0));
	stripe2->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));

	auto c_map_1 = std::make_shared <CompositeMap>(stripe1, stripe2, CompAdd, 1.0);

	auto perlin_noise = std::make_shared<ColoredPerlin>(32255);
	perlin_noise->octaves = 6;

	//matte_gray_mtl->color.connect(perlin_noise);

	auto shiny_purple_mtl = std::make_shared<PhongMaterial>();
	Color purple = Color(Color8Bit(57, 0, 214)).convert_srgb_to_linear();
	shiny_purple_mtl->color.set_value(purple);
	shiny_purple_mtl->specular.set_value(0.9);
	shiny_purple_mtl->ambient.set_value(0.01);
	shiny_purple_mtl->reflection_roughness.set_value(0.05);
	shiny_purple_mtl->reflection.set_value(Color(1.0));
	shiny_purple_mtl->ior = 20.0;

	// Reflection Map
	auto purple_stripe_tex = std::make_shared<StripeMap>(purple, Color(0.1));
	purple_stripe_tex->set_mapping_space(ObjectSpace);
	purple_stripe_tex->transform->set_transform(Matrix4::Scaling(0.1, 0.1, 0.1));

	auto purple_stripe_perturb = std::make_shared<PerturbMap>(purple_stripe_tex, perlin_noise);
	purple_stripe_perturb->displacement_remap = true;
	purple_stripe_perturb->scale = 0.1;

	// Shininess Map
	auto bw_stripe_tex = std::make_shared<StripeMap>(Color(200.0), Color(10.0));
	bw_stripe_tex->set_mapping_space(ObjectSpace);
	bw_stripe_tex->transform->set_transform(Matrix4::Scaling(0.1, 0.1, 0.1));

	auto bw_stripe_perturb = std::make_shared<PerturbMap>(bw_stripe_tex, perlin_noise);
	bw_stripe_perturb->displacement_remap = true;
	bw_stripe_perturb->scale = 0.1;

	// Maps
	//shiny_purple_mtl->color.connect(purple_stripe_perturb);
	shiny_purple_mtl->reflection.connect(purple_stripe_perturb);
	shiny_purple_mtl->shininess.connect(bw_stripe_tex);

	auto shiny_yellow_mtl = std::make_shared<PhongMaterial>();
	shiny_yellow_mtl->color = Color(Color8Bit(245, 209, 66)).convert_srgb_to_linear();
	shiny_yellow_mtl->specular = 0.9;
	shiny_yellow_mtl->ambient = 0.01;

	auto matte_blue_mtl = std::make_shared<PhongMaterial>();
	matte_blue_mtl->color = Color(Color8Bit(21, 80, 117)).convert_srgb_to_linear();
	matte_blue_mtl->specular = 0.0;
	matte_blue_mtl->ambient = 0.01;

	// Objects

	auto gs = Sphere::GlassSphere();
	gs->set_name("Glass Sphere 001");
	gs->set_transform(Matrix4::Translation(-2.0, 0.5, -1.0) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(gs);

	auto gs2 = Sphere::GlassSphere();
	gs2->set_name("Glass Sphere 002");
	gs2->set_transform(Matrix4::Translation(-2.0, 0.5, -1.0) * Matrix4::Scaling(0.1, 0.1, 0.1));
	w.add_object(gs2);

	auto glass_mtl = std::dynamic_pointer_cast<PhongMaterial>(gs->material);
	glass_mtl->reflection.set_value(Color(1.0));
	glass_mtl->color.set_value(Color(0.0));

	auto air_mtl = std::dynamic_pointer_cast<PhongMaterial>(gs2->material);
	air_mtl->reflection.set_value(Color(1.0));
	air_mtl->color.set_value(Color(0.0));
	air_mtl->ior = 1.0;

	auto floor_000 = std::make_shared<InfinitePlane>("floor 000");
	floor_000->material = matte_gray_mtl;
	w.add_object(floor_000);

	auto green_sphere = std::make_shared<Sphere>("green sphere");
	green_sphere->material = shiny_purple_mtl;
	green_sphere->set_transform(Matrix4::Translation(0.0, 1.0, 0.0) * Matrix4::Rotation_Z(deg_to_rad(45.0)));
	w.add_object(green_sphere);

	auto yellow_sphere_000 = std::make_shared<Sphere>("yellow sphere 000");
	yellow_sphere_000->material = shiny_yellow_mtl;
	yellow_sphere_000->set_transform(Matrix4::Translation(-1.5, 0.5, -0.0) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_000);

	auto yellow_sphere_001 = std::make_shared<Sphere>("yellow sphere 001");
	yellow_sphere_001->material = shiny_yellow_mtl;
	yellow_sphere_001->set_transform(Matrix4::Translation(2.0, 0.5, -0.5) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_001);

	int num_blue_cubes = 7;

	for (int i = 0; i < num_blue_cubes; i++)
	{
		double sx = double(num_blue_cubes - 1);

		auto blue_cube_inst = std::make_shared<Cube>("Blue Cube " + pad_num(i, 3));
		blue_cube_inst->material = matte_blue_mtl;

		blue_cube_inst->set_transform(
			Matrix4::Translation((i * 2.0) - sx, 0.0, 2.0) *
			Matrix4::Rotation_Z((((M_PI / 2.0) / sx) * (num_blue_cubes - i)) - (M_PI / 4.0)) *
			Matrix4::Scaling(0.25, 2.0, 0.25)
		);

		w.add_object(blue_cube_inst);
	}
	return w;
}

World render_ch13_world()
{
	World w = World();

	w.background = std::make_shared<NormalGradientBackground>();

	// Lights

	auto light_000 = std::make_shared<PointLight>(Tuple::Point(-20.0, 5.0, -4.0), Color(1.0), 100.0);
	light_000->falloff = true;
	light_000->set_name("light 000");
	w.add_object(light_000);

	auto light_001 = std::make_shared<PointLight>(Tuple::Point(0.0, 1.0, 5.0), Color(1.0, 0.0, 0.0), 500.0);
	light_001->falloff = true;
	light_001->set_name("light 001");
	w.add_object(light_001);

	// Materials

	auto matte_gray_mtl = std::make_shared<PhongMaterial>();
	matte_gray_mtl->color = Color(0.18);
	matte_gray_mtl->specular = 0.0;
	matte_gray_mtl->ambient = 0.01;

	auto gray_checkers_tex = std::make_shared<CheckerMap>(Color(0.18), Color(0.36));
	auto gray_ring_tex = std::make_shared<RingMap>(Color(0.18), Color(0.36));
	auto gray_gradient_tex = std::make_shared<GradientMap>(Color(0.18), Color(0.36));
	auto gray_stripe_tex = std::make_shared<StripeMap>(Color(0.18), Color(0.36));

	auto stripe1 = std::make_shared<StripeMap>(Color(1.0, 0.0, 0.0), Color(0.5, 0.0, 0.0));
	stripe1->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5) * Matrix4::Rotation_Y(deg_to_rad(90)));
	auto stripe2 = std::make_shared<StripeMap>(Color(0.0, 1.0, 0.0), Color(0.0, 0.5, 0.0));
	stripe2->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));

	auto c_map_1 = std::make_shared <CompositeMap>(stripe1, stripe2, CompAdd, 1.0);

	auto perlin_noise = std::make_shared<ColoredPerlin>(32255);
	perlin_noise->octaves = 6;

	//matte_gray_mtl->color.connect(perlin_noise);

	auto shiny_purple_mtl = std::make_shared<PhongMaterial>();
	Color purple = Color(Color8Bit(57, 0, 214)).convert_srgb_to_linear();
	shiny_purple_mtl->color.set_value(purple);
	shiny_purple_mtl->specular.set_value(0.9);
	shiny_purple_mtl->ambient.set_value(0.01);
	shiny_purple_mtl->reflection.set_value(Color(1.0));
	shiny_purple_mtl->ior = 20.0;

	// Reflection Map
	auto purple_stripe_tex = std::make_shared<StripeMap>(purple, Color(0.1));
	purple_stripe_tex->set_mapping_space(ObjectSpace);
	purple_stripe_tex->transform->set_transform(Matrix4::Scaling(0.1, 0.1, 0.1));

	auto purple_stripe_perturb = std::make_shared<PerturbMap>(purple_stripe_tex, perlin_noise);
	purple_stripe_perturb->displacement_remap = true;
	purple_stripe_perturb->scale = 0.1;

	// Shininess Map
	auto bw_stripe_tex = std::make_shared<StripeMap>(Color(200.0), Color(10.0));
	bw_stripe_tex->set_mapping_space(ObjectSpace);
	bw_stripe_tex->transform->set_transform(Matrix4::Scaling(0.1, 0.1, 0.1));

	auto bw_stripe_perturb = std::make_shared<PerturbMap>(bw_stripe_tex, perlin_noise);
	bw_stripe_perturb->displacement_remap = true;
	bw_stripe_perturb->scale = 0.1;

	// Maps
	//shiny_purple_mtl->color.connect(purple_stripe_perturb);
	shiny_purple_mtl->reflection.connect(purple_stripe_perturb);
	shiny_purple_mtl->shininess.connect(bw_stripe_tex);

	auto shiny_yellow_mtl = std::make_shared<PhongMaterial>();
	shiny_yellow_mtl->color = Color(Color8Bit(245, 209, 66)).convert_srgb_to_linear();
	shiny_yellow_mtl->specular = 0.9;
	shiny_yellow_mtl->ambient = 0.01;

	auto gold_metal_mtl = std::make_shared<PhongMaterial>();
	gold_metal_mtl->ior.set_value(12.0);
	gold_metal_mtl->color.set_value(Color(0.0));
	gold_metal_mtl->reflection.set_value(Color(Color8Bit(212, 175, 55)).convert_srgb_to_linear());
	gold_metal_mtl->specular.set_value(0.9);
	gold_metal_mtl->shininess.set_value(600.0);
	gold_metal_mtl->ambient.set_value(0.0);

	auto matte_blue_mtl = std::make_shared<PhongMaterial>();
	matte_blue_mtl->color = Color(Color8Bit(21, 80, 117)).convert_srgb_to_linear();
	matte_blue_mtl->specular = 0.0;
	matte_blue_mtl->ambient = 0.01;

	auto nor_mtl = std::make_shared<NormalsMaterial>();

	// Objects

	auto gs = Sphere::GlassSphere();
	gs->set_name("Glass Sphere 001");
	gs->set_transform(Matrix4::Translation(-2.0, 0.5, -1.0) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(gs);

	auto gs2 = Sphere::GlassSphere();
	gs2->set_name("Glass Sphere 002");
	gs2->set_transform(Matrix4::Translation(-2.0, 0.5, -1.0) * Matrix4::Scaling(0.45, 0.45, 0.45));
	w.add_object(gs2);

	auto glass_mtl = std::dynamic_pointer_cast<PhongMaterial>(gs->material);
	glass_mtl->reflection.set_value(Color(1.0));
	glass_mtl->color.set_value(Color(0.0));

	auto air_mtl = std::dynamic_pointer_cast<PhongMaterial>(gs2->material);
	air_mtl->reflection.set_value(Color(1.0));
	air_mtl->color.set_value(Color(0.0));
	air_mtl->ior = 1.0;

	auto floor_000 = std::make_shared<InfinitePlane>("floor 000");
	floor_000->material = matte_gray_mtl;
	w.add_object(floor_000);

	auto purple_sphere = std::make_shared<Sphere>("purple sphere");
	purple_sphere->material = shiny_purple_mtl;
	purple_sphere->set_transform(Matrix4::Translation(0.0, 1.0, 0.0) * Matrix4::Rotation_Z(deg_to_rad(45.0)));
	w.add_object(purple_sphere);

	auto yellow_sphere_000 = std::make_shared<Sphere>("yellow sphere 000");
	yellow_sphere_000->material = shiny_yellow_mtl;
	yellow_sphere_000->set_transform(Matrix4::Translation(-1.5, 0.5, -0.0) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_000);

	auto yellow_sphere_001 = std::make_shared<Sphere>("yellow sphere 001");
	yellow_sphere_001->material = shiny_yellow_mtl;
	yellow_sphere_001->set_transform(Matrix4::Translation(2.0, 0.5, -0.5) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(yellow_sphere_001);

	auto gold_inf_cylinder_000 = std::make_shared<Cylinder>("Gold Infinite Cylinder 000");
	gold_inf_cylinder_000->material = gold_metal_mtl;
	gold_inf_cylinder_000->set_transform(Matrix4::Rotation_X(deg_to_rad(-45.0)) * Matrix4::Translation(0.0, 0.0, 10.0) * Matrix4::Scaling(4.0, 4.0, 4.0));
	w.add_object(gold_inf_cylinder_000);

	auto gold_inf_cylinder_001 = std::make_shared<Cylinder>("Gold Infinite Cylinder 001");
	gold_inf_cylinder_001->material = gold_metal_mtl;
	gold_inf_cylinder_001->set_transform(Matrix4::Rotation_X(deg_to_rad(45.0)) * Matrix4::Translation(-10.0, 0.0, 10.0) * Matrix4::Scaling(4.0, 1.0, 4.0));
	w.add_object(gold_inf_cylinder_001);

	auto gold_inf_cylinder_002 = std::make_shared<Cylinder>("Gold Infinite Cylinder 002");
	gold_inf_cylinder_002->material = gold_metal_mtl;
	gold_inf_cylinder_002->set_transform(Matrix4::Rotation_X(deg_to_rad(45.0)) * Matrix4::Translation(10.0, 0.0, 10.0) * Matrix4::Scaling(4.0, 1.0, 4.0));
	w.add_object(gold_inf_cylinder_002);

	auto gold_inf_cylinder_003 = std::make_shared<Cylinder>("Gold Infinite Cylinder 003");
	gold_inf_cylinder_003->material = gold_metal_mtl;
	gold_inf_cylinder_003->set_transform(Matrix4::Rotation_X(deg_to_rad(-45.0)) * Matrix4::Translation(0.0, 0.0, -10.0) * Matrix4::Scaling(4.0, 1.0, 4.0));
	w.add_object(gold_inf_cylinder_003);

	auto gold_inf_cylinder_004 = std::make_shared<Cylinder>("Gold Infinite Cylinder 004");
	gold_inf_cylinder_004->material = gold_metal_mtl;
	gold_inf_cylinder_004->set_transform(Matrix4::Rotation_X(deg_to_rad(45.0)) * Matrix4::Translation(-10.0, 0.0, -10.0) * Matrix4::Scaling(4.0, 1.0, 4.0));
	w.add_object(gold_inf_cylinder_004);

	auto gold_inf_cylinder_005 = std::make_shared<Cylinder>("Gold Infinite Cylinder 005");
	gold_inf_cylinder_005->material = gold_metal_mtl;
	gold_inf_cylinder_005->set_transform(Matrix4::Rotation_X(deg_to_rad(45.0)) * Matrix4::Translation(10.0, 0.0, -10.0) * Matrix4::Scaling(4.0, 1.0, 4.0));
	w.add_object(gold_inf_cylinder_005);

	auto purple_cone = std::make_shared<DoubleNappedCone>(-1.0, -0.8, "blue cone 000");
	purple_cone->closed = true;
	purple_cone->material = glass_mtl;
	purple_cone->set_transform(Matrix4::Translation(-1.0, 1.0, -2.0) * Matrix4::Rotation_Y(deg_to_rad(-90.0)) * Matrix4::Rotation_Z(deg_to_rad(90.0)) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(purple_cone);

	auto purple_cyl = std::make_shared<Cylinder>(-1.0, -0.95, "blue cylinder 000");
	purple_cyl->closed = true;
	purple_cyl->material = glass_mtl;
	purple_cyl->set_transform(Matrix4::Translation(0.0, 2.0, -2.0) * Matrix4::Rotation_Y(deg_to_rad(-45.0)) * Matrix4::Rotation_Z(deg_to_rad(90.0)) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(purple_cyl);

	int num_blue_cubes = 7;

	for (int i = 0; i < num_blue_cubes; i++)
	{
		double sx = double(num_blue_cubes - 1);

		auto blue_cube_inst = std::make_shared<Cube>("Blue Cube " + pad_num(i, 3));
		blue_cube_inst->material = matte_blue_mtl;

		blue_cube_inst->set_transform(
			Matrix4::Translation((i * 2.0) - sx, 0.0, 2.0) *
			Matrix4::Rotation_Z((((M_PI / 2.0) / sx) * (num_blue_cubes - i)) - (M_PI / 4.0)) *
			Matrix4::Scaling(0.25, 2.0, 0.25)
		);

		w.add_object(blue_cube_inst);
	}
	return w;
}

int render_still()
{
	std::string chapter = "Cylinders_CH13";
	std::string folder = "E:\\dump\\projects\\Raymond\\frames";
	int version = 2;

	int width = 1920;
	int height = 1080;
	double fov = 90.0;

	std::cout << "Executing Render " << chapter << " v" << pad_num(version, 2) << std::endl << std::endl;

	// Start time
	auto start = std::chrono::steady_clock::now();

	// Build World
	std::cout << "Building World...\n";
	World w = render_ch13_world();

	// Execution
	Canvas image;

	Camera c = Camera(width, height, deg_to_rad(fov));

	Tuple from = Tuple::Point(0.0, 0.8, -5.0);
	Tuple to = Tuple::Point(0.0, 1.0, 0.0);
	Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

	c.set_transform(Matrix4::ViewTransform(from, to, up));

	try
	{
		std::cout << "Tracing...\n";


		image = c.threaded_render(w);
		//image = c.render(w);

		//image = c.render_scanline(w, 160);

		std::cout << "Complete\n";

	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return 10;
	}
	catch (...)
	{
		std::cout << "unknown exception\n";
		return 100;
	}
	

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

int render_animation()
{
	std::string file_name_root = "frame_";
	std::string folder = "E:\\dump\\projects\\Raymond\\frames\\ani_DATE_01";

	int width = 1920;
	int height = 1080;
	double fov = 90.0;
	size_t frames = 240;

	std::cout << std::string(50, '*') << std::endl << "Executing Animation Render" << std::endl << frames << " Frames" << std::endl << std::string(50, '*') << std::endl << std::endl;

	auto total_start = std::chrono::steady_clock::now();

	// Build World
	std::cout << "Building World...\n";
	World w = render_ch13_world();
	std::cout << "Complete\n\n";

	for (size_t i = 0; i < frames; i++)
	{
		std::cout << std::string(50, '*') << std::endl << "Frame: " << pad_num(int(i) + 1, 3) << "/" << frames << std::endl << std::string(50, '*') << std::endl << std::endl;

		// Start time
		auto frame_start = std::chrono::steady_clock::now();

		// Execution
		Canvas image;

		Camera c = Camera(width, height, deg_to_rad(fov));

		double perc = (3.0 / double(frames) * double(i)) - 1.5;

		Tuple from = Tuple::Point(perc, 0.8, -5.0);
		Tuple to = Tuple::Point(0.0, 1.0, 0.0);
		Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

		c.set_transform(Matrix4::ViewTransform(from, to, up));

		try
		{
			std::cout << "Tracing...\n";


			image = c.threaded_render(w);
			//image = c.render(w);

			//image = c.render_scanline(w, 160);

			std::cout << "Complete\n";

		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
			return 10;
		}
		catch (...)
		{
			std::cout << "unknown exception\n";
			return 100;
		}


		// End time
		auto frame_end = std::chrono::steady_clock::now();

		auto frame_diff = frame_end - frame_start;

		std::cout << std::endl << "Time: ";
		clock_display(std::cout, frame_diff);
		std::cout << std::endl << std::endl;

		// File Name
		std::string file_path = folder + "\\" + file_name_root + pad_num(int(i), 4) + ".ppm";

		std::cout << "Writing file: " << file_path << std::endl;

		canvas_to_ppm(image, file_path);
		std::cout << "Complete" << std::endl;
	}

	auto total_end = std::chrono::steady_clock::now();

	auto total_diff = total_end - total_start;

	std::cout << std::endl << "Total Time: ";
	clock_display(std::cout, total_diff);
	std::cout << std::endl << std::endl;

	return 0;
}

int main()
{
	return render_still();
}
