#include "pch.h"
#include "math.h"

#include "../Raymond/Tuple.h"
#include "../Raymond/Color.h"
#include "../Raymond/Canvas.h"

// ------------------------------------------------------------------------
// Chapter 01 Tuples, Points, and Vectors
// ------------------------------------------------------------------------

TEST(Chapter01Tests, PointTuple) {
	float x = 4.3f;
	float y = -4.2f;
	float z = 3.1f;
	float w = 1.0f;

	Tuple a = Tuple(x, y, z, w);

	ASSERT_EQ(a.x, x);
	ASSERT_EQ(a.y, y);
	ASSERT_EQ(a.z, z);
	ASSERT_EQ(a.w, w);

	ASSERT_EQ(a.w, 1.0);
	ASSERT_NE(a.w, 0.0);
}

TEST(Chapter01Tests, VectporTuple) {
	float x = 4.3f;
	float y = -4.2f;
	float z = 3.1f;
	float w = 0.0f;

	Tuple a = Tuple(x, y, z, w);

	ASSERT_EQ(a.x, x);
	ASSERT_EQ(a.y, y);
	ASSERT_EQ(a.z, z);
	ASSERT_EQ(a.w, w);

	ASSERT_EQ(a.w, 0.0);
	ASSERT_NE(a.w, 1.0);
}

TEST(Chapter01Tests, PointFactory) {
	float x = 4.0f;
	float y = -4.0f;
	float z = 3.0f;
	float w = 1.0f;

	Tuple a = Tuple(x, y, z, w);
	Tuple p = Tuple::Point(x, y, z);

	ASSERT_EQ(a, p);
}

TEST(Chapter01Tests, VectorFactory) {
	float x = 4.0f;
	float y = -4.0f;
	float z = 3.0f;
	float w = 0.0f;

	Tuple a = Tuple(x, y, z, w);
	Tuple p = Tuple::Vector(x, y, z);

	ASSERT_EQ(a, p);
}

TEST(Chapter01Tests, AddingTuples) {
	Tuple a1 = Tuple(3.0f, -2.0f, 5.0f, 1.0f);
	Tuple a2 = Tuple(-2.0f, 3.0f, 1.0f, 0.0f);

	ASSERT_EQ(a1 + a2, Tuple(1.0f, 1.0f, 6.0f, 1.0f));
}

TEST(Chapter01Tests, SubtractingTwoPoints) {
	Tuple p1 = Tuple::Point(3.0f, 2.0f, 1.0f);
	Tuple p2 = Tuple::Point(5.0f, 6.0f, 7.0f);

	ASSERT_EQ(p1 - p2, Tuple::Vector(-2.0f, -4.0f, -6.0f));
}

TEST(Chapter01Tests, SubtractingAVectorFromAPoint) {
	Tuple p = Tuple::Point(3.0f, 2.0f, 1.0f);
	Tuple v = Tuple::Vector(5.0f, 6.0f, 7.0f);

	ASSERT_EQ(p - v, Tuple::Point(-2.0f, -4.0f, -6.0f));
}

TEST(Chapter01Tests, SubtractingTwoVectors) {
	Tuple v1 = Tuple::Vector(3.0f, 2.0f, 1.0f);
	Tuple v2 = Tuple::Vector(5.0f, 6.0f, 7.0f);

	ASSERT_EQ(v1 - v2, Tuple::Vector(-2.0f, -4.0f, -6.0f));
}

TEST(Chapter01Tests, SubtractingAZeroVector) {
	Tuple zero = Tuple::Vector(0.0f, 0.0f, 0.0f);
	Tuple v = Tuple::Vector(1.0f, -2.0f, 3.0f);

	ASSERT_EQ(zero - v, Tuple::Vector(-1.0f, 2.0f, -3.0f));
}

TEST(Chapter01Tests, NegatingATuple) {
	Tuple a = Tuple(1.0f, -2.0f, 3.0f, -4.0f);

	ASSERT_EQ(-a, Tuple(-1.0f, 2.0f, -3.0f, 4.0f));
}

TEST(Chapter01Tests, MultiplyingATupleByAScalar) {
	Tuple a = Tuple(1.0f, -2.0f, 3.0f, -4.0f);

	ASSERT_EQ(a * 3.5, Tuple(3.5f, -7.0f, 10.5f, -14.0f));
}

TEST(Chapter01Tests, MultiplyingATupleByAFraction) {
	Tuple a = Tuple(1.0f, -2.0f, 3.0f, -4.0f);

	ASSERT_EQ(a * 0.5, Tuple(0.5f, -1.0f, 1.5f, -2.0f));
}

TEST(Chapter01Tests, DividingATupleByAScalar) {
	Tuple a = Tuple(1.0f, -2.0f, 3.0f, -4.0f);

	ASSERT_EQ(a / 2, Tuple(0.5f, -1.0f, 1.5f, -2.0f));
}

TEST(Chapter01Tests, ComputingMagnitudeOfAVector) {
	Tuple v1 = Tuple::Vector(1.0f, 0.0f, 0.0f);
	ASSERT_TRUE(flt_cmp(v1.magnitude(), 1.0));

	Tuple v2 = Tuple::Vector(0.0f, 1.0f, 0.0f);
	ASSERT_TRUE(flt_cmp(v2.magnitude(), 1.0));

	Tuple v3 = Tuple::Vector(0.0f, 0.0f, 1.0f);
	ASSERT_TRUE(flt_cmp(v3.magnitude(), 1.0));

	Tuple v4 = Tuple::Vector(1.0f, 2.0f, 3.0f);
	ASSERT_TRUE(flt_cmp(v4.magnitude(), sqrt(14.0)));

	Tuple v5 = Tuple::Vector(-1.0f, -2.0f, -3.0f);
	ASSERT_TRUE(flt_cmp(v5.magnitude(), sqrt(14.0)));
}

TEST(Chapter01Tests, NormalizingAVector) {
	Tuple v1 = Tuple::Vector(4.0f, 0.0f, 0.0f);
	ASSERT_EQ(v1.normalize(), Tuple::Vector(1.0f, 0.0f, 0.0f));

	Tuple v2 = Tuple::Vector(1.0f, 2.0f, 3.0f);
	ASSERT_EQ(v2.normalize(), Tuple::Vector(0.26726f, 0.53452f, 0.80178f));
}

TEST(Chapter01Tests, ComputingMagnitudeOfANormalizedVector) {
	Tuple v = Tuple::Vector(1.0f, 2.0f, 3.0f);
	Tuple norm = v.normalize();
	ASSERT_TRUE(flt_cmp(norm.magnitude(), 1.0));
}

TEST(Chapter01Tests, DotProductOfTwoVectors) {
	Tuple v1 = Tuple::Vector(1.0f, 2.0f, 3.0f);
	Tuple v2 = Tuple::Vector(2.0f, 3.0f, 4.0f);

	ASSERT_TRUE(flt_cmp(Tuple::dot(v1, v2), 20.0));
}

TEST(Chapter01Tests, CrossProductOfTwoVectors) {
	Tuple a = Tuple::Vector(1.0f, 2.0f, 3.0f);
	Tuple b = Tuple::Vector(2.0f, 3.0f, 4.0f);

	ASSERT_EQ(Tuple::cross(a, b), Tuple::Vector(-1.0f, 2.0f, -1.0f));
	ASSERT_EQ(Tuple::cross(b, a), Tuple::Vector(1.0f, -2.0f, 1.0f));
}

// ------------------------------------------------------------------------
// Chapter 02 Drawing on a Canvas
// ------------------------------------------------------------------------

TEST(Chapter02Tests, CallingColorChannels) {
	float red = -0.5f;
	float green = 0.4f;
	float blue = 1.7f;

	Color c = Color(red, green, blue);

	ASSERT_EQ(c.r(), red);
	ASSERT_EQ(c.g(), green);
	ASSERT_EQ(c.b(), blue);
}

TEST(Chapter02Tests, AssigningColorChannels) {
	float red = -0.5f;
	float green = 0.4f;
	float blue = 1.7f;

	Color c = Color(0.0f, 0.0f, 0.0f);

	c.r() = red;
	c.g() = green;
	c.b() = blue;

	ASSERT_EQ(c, Color(red, green, blue));
}

TEST(Chapter02Tests, AddingColors) {
	Color c1 = Color(0.9f, 0.6f, 0.75f);
	Color c2 = Color(0.7f, 0.1f, 0.25f);

	ASSERT_EQ(c1 + c2, Color(1.6f, 0.7f, 1.0f));
}

TEST(Chapter02Tests, SubtractingColors) {
	Color c1 = Color(0.9f, 0.6f, 0.75f);
	Color c2 = Color(0.7f, 0.1f, 0.25f);

	ASSERT_EQ(c1 - c2, Color(0.2f, 0.5f, 0.5f));
}

TEST(Chapter02Tests, MultiplyingColorByAScalar) {
	Color c = Color(0.2f, 0.3f, 0.4f);

	ASSERT_EQ(c * 2.0f, Color(0.4f, 0.6f, 0.8f));
}

TEST(Chapter02Tests, MultiplyingColors) {
	Color c1 = Color(1.0f, 0.2f, 0.4f);
	Color c2 = Color(0.9f, 1.0f, 0.1f);

	ASSERT_EQ(c1.multiply(c2), Color(0.9f, 0.2f, 0.04f));
}

TEST(Chapter02Tests, CreatingACanvas) {
	int width = 10;
	int height = 20;
	Canvas c = Canvas(width, height);

	ASSERT_EQ(c.width, width);
	ASSERT_EQ(c.height, height);

	Color black = Color();

	for (Color i : c)
		ASSERT_EQ(i, black);
}

TEST(Chapter02Tests, WritingPixelsToACanvas) {
	int x = 2;
	int y = 3;

	Canvas c = Canvas(10, 20);

	Color red = Color(1.0f, 0.0f, 0.0f);

	c.write_pixel(x, y, red);

	ASSERT_EQ(c.pixel_at(x, y), red);
}