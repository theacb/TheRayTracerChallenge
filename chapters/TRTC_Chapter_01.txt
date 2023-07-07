#include "pch.h"
#include <iostream>
#include "tuple.h"
#include <math.h>

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

int main()
{
	Tuple point_00 = Tuple::Point(4.0f, 2.0f, 3.0f);
	Tuple point_01 = Tuple::Point(-1.0f, 1.0f, 30.0f);

	Tuple vector_00 = Tuple::Vector(2.0f, 3.0f, 7.0f);
	Tuple vector_01 = Tuple::Vector(2.0f, -10.0f, 5.0f);
	Tuple vector_02 = Tuple::Vector(1.0f, 2.0f, 3.0f);
	Tuple vector_03 = Tuple::Vector(2.0f, 3.0f, 4.0f);

	std::cout << "\nPoint:\n";
	std::cout << point_00 << "\n";

	std::cout << "\nVector:\n";
	std::cout << point_00 << "\n";

	std::cout << "\nAddition:\n";
	std::cout << point_00 << " + " << vector_01 << " = " << point_00 + vector_01 << "\n";
	std::cout << point_01 << " + " << vector_00 << " = " << point_01 + vector_00 << "\n";

	std::cout << "\nSubtraction:\n";
	std::cout << point_00 << " - " << vector_01 << " = " << point_00 - vector_01 << "\n";
	std::cout << point_01 << " - " << vector_00 << " = " << point_01 - vector_00 << "\n";
	std::cout << vector_01 << " - " << vector_00 << " = " << vector_01 - vector_00 << "\n";

	Tuple point_00_copy = Tuple::Point(4.0f, 2.0f, 3.0f);
	Tuple vector_00_copy = Tuple::Vector(2.0f, 3.0f, 7.0f);

	std::cout << "\nEquality:\n";
	std::cout << point_00 << " == " << point_00_copy << ": " << (point_00 == point_00_copy) << "\n";
	std::cout << vector_00 << " == " << vector_00_copy << ": " << (vector_00 == vector_00_copy) << "\n";
	std::cout << point_00 << " == " << point_01 << ": " << (point_00 == point_01) << "\n";
	std::cout << vector_00 << " == " << vector_01 << ": " << (vector_00 == vector_01) << "\n";

	std::cout << "\nNegation:\n";
	std::cout << "-( " << point_00 << " ) " << " = " << -point_00 << "\n";
	std::cout << "-( " << point_01 << " ) " << " = " << -point_01 << "\n";
	std::cout << "-( " << vector_00 << " ) " << " = " << -vector_00 << "\n";
	std::cout << "-( " << vector_01 << " ) " << " = " << -vector_01 << "\n";

	std::cout << "\nScalar Multiplication:\n";
	std::cout << point_00 << " * 3.0 = " << " = " << point_00 * 3.0f << "\n";
	std::cout << vector_01 << " * 3.0 = " << " = " << vector_01 * 3.0f << "\n";
	std::cout << point_00 << " * -2.0 = " << " = " << point_00 * -2.0f << "\n";
	std::cout << vector_01 << " * -2.0 = " << " = " << vector_01 * -2.0f << "\n";
	std::cout << point_01 << " * 0.5 = " << " = " << point_01 * 0.5f << "\n";
	std::cout << vector_00 << " * 0.5 = " << " = " << vector_00 * 0.5f << "\n";
	std::cout << "2.0 * " << vector_01 << " = " << " = " << 2.0 * vector_01 << "\n";

	std::cout << "\nScalar Division:\n";
	std::cout << point_00 << " / 3.0 = " << " = " << point_00 / 3.05f << "\n";
	std::cout << vector_01 << " / 3.0 = " << " = " << vector_01 / 3.0f << "\n";
	std::cout << point_00 << " / -2.0 = " << " = " << point_00 / -2.0f << "\n";
	std::cout << vector_01 << " / -2.0 = " << " = " << vector_01 / -2.0f << "\n";

	std::cout << "\nMagnitude:\n";
	std::cout << vector_00 << ".magnitude() = " << vector_00.magnitude() << "\n";
	std::cout << vector_01 << ".magnitude() = " << vector_01.magnitude() << "\n";
	std::cout << vector_02 << ".magnitude() = " << vector_02.magnitude() << " which should equal: " << sqrt(14) << "\n";

	std::cout << "\nNormalize:\n";
	std::cout << vector_00 << ".normalize() = " << vector_00.normalize() << "\n";
	std::cout << vector_01 << ".normalize() = " << vector_01.normalize() << "\n";
	std::cout << vector_02 << ".normalize() = " << vector_02.normalize() << "\n";

	std::cout << "\nDot Product:\n";
	std::cout << "dot( " << vector_00 << ", " << vector_01 << " ) = " << Tuple::dot(vector_00, vector_01) << "\n";
	std::cout << "dot( " << vector_00 << ".normalize(), " << vector_01 << ".normalize() ) = " << Tuple::dot(vector_00.normalize(), vector_01.normalize()) << "\n";
	std::cout << "dot( " << vector_02 << ", " << vector_03 << " ) = " << Tuple::dot(vector_02, vector_03) << "\n";
	std::cout << "dot( " << vector_02 << ".normalize(), " << vector_03 << ".normalize() ) = " << Tuple::dot(vector_02.normalize(), vector_03.normalize()) << "\n";

	std::cout << "\nCross Product:\n";
	std::cout << "cross( " << vector_00 << ", " << vector_01 << " ) = " << Tuple::cross(vector_00, vector_01) << "\n";
	std::cout << "cross( " << vector_00 << ".normalize(), " << vector_01 << ".normalize() ) = " << Tuple::cross(vector_00.normalize(), vector_01.normalize()) << "\n";
	std::cout << "cross( " << vector_02 << ", " << vector_03 << " ) = " << Tuple::cross(vector_02, vector_03) << "\n";
	std::cout << "cross( " << vector_02 << ".normalize(), " << vector_03 << ".normalize() ) = " << Tuple::cross(vector_02.normalize(), vector_03.normalize()) << "\n";

	std::cout << "\nProjectile:\n";
	Projectile p;
	Environment e;

	p.position = Tuple::Point(0.0f, 1.0f, 0.0f);
	p.velocity = Tuple::Vector(9.0f, 1.0f, 0.0f);

	e.gravity = Tuple::Vector(0.0f, -0.1f, 0.0f);
	e.wind = Tuple::Vector(-0.01f, 0.0f, 0.0f);

	while (p.position.y > 0.0)
	{
		std::cout << p.position << "\n";
		p = tick(p, e);
	}

	return 0;

}