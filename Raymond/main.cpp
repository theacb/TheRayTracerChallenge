// Raymond.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <string>
#include <typeinfo>

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
	matte_gray_mtl->ambient = Color(0.01);

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
	shiny_yellow_mtl->ambient = Color(0.01);

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
	matte_blue_mtl->ambient = Color(0.01);

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

	auto glass_mtl = std::static_pointer_cast<PhongMaterial>(gs->material);
	glass_mtl->reflection.set_value(Color(1.0));
	glass_mtl->color.set_value(Color(0.0));

	auto air_mtl = std::static_pointer_cast<PhongMaterial>(gs2->material);
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
	purple_cone->set_closed(true);
	purple_cone->material = glass_mtl;
	purple_cone->set_transform(Matrix4::Translation(-1.0, 1.0, -2.0) * Matrix4::Rotation_Y(deg_to_rad(-90.0)) * Matrix4::Rotation_Z(deg_to_rad(90.0)) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(purple_cone);

	auto purple_cyl = std::make_shared<Cylinder>(-1.0, -0.95, "blue cylinder 000");
	purple_cyl->set_closed(true);
	purple_cyl->material = glass_mtl;
	purple_cyl->set_transform(Matrix4::Translation(0.0, 2.0, -2.0) * Matrix4::Rotation_Y(deg_to_rad(-45.0)) * Matrix4::Rotation_Z(deg_to_rad(90.0)) * Matrix4::Scaling(0.5, 0.5, 0.5));
	w.add_object(purple_cyl);

	int num_blue_cubes = 7;

	for (int i = 0; i < num_blue_cubes; i++)
	{
		auto sx = double(num_blue_cubes - 1);

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
	std::string folder = R"(I:\projects\Raymond\frames)";
	int version = 2;

	// 560 x 315

	int width = 72;
	int height = 36;
	double fov = 90.0;

	std::cout << "Executing Render " << chapter << " v" << pad_num(version, 2) << std::endl << std::endl;

	// Start time
	auto start = std::chrono::steady_clock::now();

	// Build World
	std::cout << "Building World...\n";
	World w = render_ch13_world();

    w.sample_min = 4;

	// Execution
	SampleBuffer image;

	Camera c = Camera(width, height, deg_to_rad(fov));

	Tuple from = Tuple::Point(0.0, 0.8, -5.0);
	Tuple to = Tuple::Point(0.0, 1.0, 0.0);
	Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

	c.set_transform(Matrix4::ViewTransform(from, to, up));

	try
	{
		std::cout << "Tracing...\n";


		image = c.multi_sample_threaded_render(w);
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

	canvas_to_ppm(image.to_canvas(rgb), file_path);
	std::cout << "Complete" << std::endl;

	return 0;
}

int render_animation()
{
	std::string file_name_root = "frame_";
	std::string folder = R"(I:\projects\Raymond\frames\ani_DATE_01)";

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
		std::string file_path = folder + "\\" + file_name_root + pad_num(int(i), 4) + ".ppm"; // NOLINT(performance-inefficient-string-concatenation)

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
