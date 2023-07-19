#include <gtest/gtest.h>

#include <cmath>
#include <sstream>
#include <filesystem>

#include "../Raymond/Tuple.h"
#include "../Raymond/Canvas.h"
#include "../Raymond/Matrix.h"
#include "../Raymond/Ray.h"
#include "../Raymond/PrimitiveDefinition.h"
#include "../Raymond/Object.h"
#include "../Raymond/Primitive.h"
#include "../Raymond/World.h"
#include "../Raymond/Camera.h"

// ------------------------------------------------------------------------
// Constants
// ------------------------------------------------------------------------

static const Color BLACK = Color(0.0, 0.0, 0.0);
static const Color WHITE = Color(1.0, 1.0, 1.0);

// ------------------------------------------------------------------------
// Chapter 01 Tuples, Points, and Vectors
// ------------------------------------------------------------------------

TEST(Chapter01Tests, PointTuple) 
{
	double x = 4.3;
	double y = -4.2;
	double z = 3.1;
	double w = 1.0;

	Tuple a = Tuple(x, y, z, w);

	ASSERT_EQ(a.x, x);
	ASSERT_EQ(a.y, y);
	ASSERT_EQ(a.z, z);
	ASSERT_EQ(a.w, w);

	ASSERT_EQ(a.w, 1.0);
	ASSERT_NE(a.w, 0.0);
}

TEST(Chapter01Tests, VectporTuple) 
{
	double x = 4.3;
	double y = -4.2;
	double z = 3.1;
	double w = 0.0;

	Tuple a = Tuple(x, y, z, w);

	ASSERT_EQ(a.x, x);
	ASSERT_EQ(a.y, y);
	ASSERT_EQ(a.z, z);
	ASSERT_EQ(a.w, w);

	ASSERT_EQ(a.w, 0.0);
	ASSERT_NE(a.w, 1.0);
}

TEST(Chapter01Tests, PointFactory) 
{
	double x = 4.0;
	double y = -4.0;
	double z = 3.0;
	double w = 1.0;

	Tuple a = Tuple(x, y, z, w);
	Tuple p = Tuple::Point(x, y, z);

	ASSERT_EQ(a, p);
}

TEST(Chapter01Tests, VectorFactory) 
{
	double x = 4.0;
	double y = -4.0;
	double z = 3.0;
	double w = 0.0;

	Tuple a = Tuple(x, y, z, w);
	Tuple p = Tuple::Vector(x, y, z);

	ASSERT_EQ(a, p);
}

TEST(Chapter01Tests, AddingTuples) 
{
	Tuple a1 = Tuple(3.0, -2.0, 5.0, 1.0);
	Tuple a2 = Tuple(-2.0, 3.0, 1.0, 0.0);

	ASSERT_EQ(a1 + a2, Tuple(1.0, 1.0, 6.0, 1.0));
}

TEST(Chapter01Tests, SubtractingTwoPoints) 
{
	Tuple p1 = Tuple::Point(3.0, 2.0, 1.0);
	Tuple p2 = Tuple::Point(5.0, 6.0, 7.0);

	ASSERT_EQ(p1 - p2, Tuple::Vector(-2.0, -4.0, -6.0));
}

TEST(Chapter01Tests, SubtractingAVectorFromAPoint) 
{
	Tuple p = Tuple::Point(3.0, 2.0, 1.0);
	Tuple v = Tuple::Vector(5.0, 6.0, 7.0);

	ASSERT_EQ(p - v, Tuple::Point(-2.0, -4.0, -6.0));
}

TEST(Chapter01Tests, SubtractingTwoVectors) 
{
	Tuple v1 = Tuple::Vector(3.0, 2.0, 1.0);
	Tuple v2 = Tuple::Vector(5.0, 6.0, 7.0);

	ASSERT_EQ(v1 - v2, Tuple::Vector(-2.0, -4.0, -6.0));
}

TEST(Chapter01Tests, SubtractingAZeroVector) 
{
	Tuple zero = Tuple::Vector(0.0, 0.0, 0.0);
	Tuple v = Tuple::Vector(1.0, -2.0, 3.0);

	ASSERT_EQ(zero - v, Tuple::Vector(-1.0, 2.0, -3.0));
}

TEST(Chapter01Tests, NegatingATuple) 
{
	Tuple a = Tuple(1.0, -2.0, 3.0, -4.0);

	ASSERT_EQ(-a, Tuple(-1.0, 2.0, -3.0, 4.0));
}

TEST(Chapter01Tests, MultiplyingATupleByAScalar) 
{
	Tuple a = Tuple(1.0, -2.0, 3.0, -4.0);

	ASSERT_EQ(a * 3.5, Tuple(3.5, -7.0, 10.5, -14.0));
}

TEST(Chapter01Tests, MultiplyingATupleByAFraction) 
{
	Tuple a = Tuple(1.0, -2.0, 3.0, -4.0);

	ASSERT_EQ(a * 0.5, Tuple(0.5, -1.0, 1.5, -2.0));
}

TEST(Chapter01Tests, DividingATupleByAScalar) 
{
	Tuple a = Tuple(1.0, -2.0, 3.0, -4.0);

	ASSERT_EQ(a / 2, Tuple(0.5, -1.0, 1.5, -2.0));
}

TEST(Chapter01Tests, ComputingMagnitudeOfAVector) 
{
	Tuple v1 = Tuple::Vector(1.0, 0.0, 0.0);
	ASSERT_TRUE(flt_cmp(v1.magnitude(), 1.0));

	Tuple v2 = Tuple::Vector(0.0, 1.0, 0.0);
	ASSERT_TRUE(flt_cmp(v2.magnitude(), 1.0));

	Tuple v3 = Tuple::Vector(0.0, 0.0, 1.0);
	ASSERT_TRUE(flt_cmp(v3.magnitude(), 1.0));

	Tuple v4 = Tuple::Vector(1.0, 2.0, 3.0);
	ASSERT_TRUE(flt_cmp(v4.magnitude(), sqrt(14.0)));

	Tuple v5 = Tuple::Vector(-1.0, -2.0, -3.0);
	ASSERT_TRUE(flt_cmp(v5.magnitude(), sqrt(14.0)));
}

TEST(Chapter01Tests, NormalizingAVector) 
{
	Tuple v1 = Tuple::Vector(4.0, 0.0, 0.0);
	ASSERT_EQ(v1.normalize(), Tuple::Vector(1.0, 0.0, 0.0));

	Tuple v2 = Tuple::Vector(1.0, 2.0, 3.0);
	ASSERT_EQ(v2.normalize(), Tuple::Vector(0.26726, 0.53452, 0.80178));
}

TEST(Chapter01Tests, ComputingMagnitudeOfANormalizedVector) 
{
	Tuple v = Tuple::Vector(1.0, 2.0, 3.0);
	Tuple norm = v.normalize();
	ASSERT_TRUE(flt_cmp(norm.magnitude(), 1.0));
}

TEST(Chapter01Tests, DotProductOfTwoVectors) 
{
	Tuple v1 = Tuple::Vector(1.0, 2.0, 3.0);
	Tuple v2 = Tuple::Vector(2.0, 3.0, 4.0);

	ASSERT_TRUE(flt_cmp(Tuple::dot(v1, v2), 20.0));
}

TEST(Chapter01Tests, CrossProductOfTwoVectors) 
{
	Tuple a = Tuple::Vector(1.0, 2.0, 3.0);
	Tuple b = Tuple::Vector(2.0, 3.0, 4.0);

	ASSERT_EQ(Tuple::cross(a, b), Tuple::Vector(-1.0, 2.0, -1.0));
	ASSERT_EQ(Tuple::cross(b, a), Tuple::Vector(1.0, -2.0, 1.0));
}

// ------------------------------------------------------------------------
// Chapter 02 Drawing on a Canvas
// ------------------------------------------------------------------------

TEST(Chapter02Tests, CallingColorChannels) 
{
	double red = -0.5;
	double green = 0.4;
	double blue = 1.7;

	Color c = Color(red, green, blue);

	ASSERT_EQ(c.r(), red);
	ASSERT_EQ(c.g(), green);
	ASSERT_EQ(c.b(), blue);
}

TEST(Chapter02Tests, AssigningColorChannels) 
{
	double red = -0.5;
	double green = 0.4;
	double blue = 1.7;

	Color c = Color(0.0, 0.0, 0.0);

	c.r() = red;
	c.g() = green;
	c.b() = blue;

	ASSERT_EQ(c, Color(red, green, blue));
}

TEST(Chapter02Tests, AddingColors) 
{
	Color c1 = Color(0.9, 0.6, 0.75);
	Color c2 = Color(0.7, 0.1, 0.25);

	ASSERT_EQ(c1 + c2, Color(1.6, 0.7, 1.0));
}

TEST(Chapter02Tests, SubtractingColors) 
{
	Color c1 = Color(0.9, 0.6, 0.75);
	Color c2 = Color(0.7, 0.1, 0.25);

	ASSERT_EQ(c1 - c2, Color(0.2, 0.5, 0.5));
}

TEST(Chapter02Tests, MultiplyingColorByAScalar) 
{
	Color c = Color(0.2, 0.3, 0.4);

	ASSERT_EQ(c * 2.0, Color(0.4, 0.6, 0.8));
}

TEST(Chapter02Tests, MultiplyingColors) 
{
	Color c1 = Color(1.0, 0.2, 0.4);
	Color c2 = Color(0.9, 1.0, 0.1);

	ASSERT_EQ(c1 * c2, Color(0.9, 0.2, 0.04));
}

TEST(Chapter02Tests, ConvertingColorToInteger) 
{
	Color c = Color(1.0, 0.2, 0.4);

	ASSERT_EQ(Color8Bit(c), Color8Bit(255, 51, 102));
}

TEST(Chapter02Tests, ConvertingOutOfRangeColorToInteger) 
{
	Color c = Color(-1.0, 2.2, 10.4);

	ASSERT_EQ(Color8Bit(c), Color8Bit(0, 255, 255));
}

TEST(Chapter02Tests, ConvertingLinearColorToSRGB) 
{
	Color c = Color(0.5, 0.5, 1.0);
	Color con = c.convert_linear_to_srgb();

	ASSERT_GT(con.magnitude(), Color(0.7, 0.7, 0.99).magnitude());
}

TEST(Chapter02Tests, ConvertingSRGBToLinear) 
{
	Color c = Color(0.5, 0.5, 1.0);
	Color con = c.convert_srgb_to_linear();

	ASSERT_LT(con.magnitude(), Color(0.3, 0.3, 1.0).magnitude());
}

TEST(Chapter02Tests, CreatingACanvas) 
{
	int width = 10;
	int height = 20;
	Canvas c = Canvas(width, height);

	ASSERT_EQ(c.width(), width);
	ASSERT_EQ(c.height(), height);

	Color black = Color();

	for (const Color& i : c)
		ASSERT_EQ(i, black);
}

TEST(Chapter02Tests, WritingPixelsToACanvas) 
{
	int x = 2;
	int y = 3;

	Canvas c = Canvas(10, 20);

	Color red = Color(1.0, 0.0, 0.0);

	c.write_pixel(x, y, red);

	ASSERT_EQ(c.pixel_at(x, y), red);
}

TEST(Chapter02Tests, ConstructingThePPMPixelData) 
{
	Canvas c = Canvas(5, 3);

	Color c1 = Color(1.5, 0.0, 0.0);
	Color c2 = Color(0.0, 0.5, 0.0);
	Color c3 = Color(-0.5, 0.0, 1.0);

	c.write_pixel(0, 0, c1);
	c.write_pixel(2, 1, c2);
	c.write_pixel(4, 2, c3);

	std::stringstream result = c.to_ppm_lines(false);

	ASSERT_EQ(result.str(), (
		"255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 127 "
		"0 0 0 0 0 0 0 \n0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 "
		));
}

TEST(Chapter02Tests, SplittingLongLinesInPPMFiles) 
{
	int width = 10;
	int height = 2;

	Canvas c = Canvas(width, height);

	Color c1 = Color(1.0, 0.8, 0.6);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			c.write_pixel(x, y, c1);
		}
	}
	std::string line;

	std::stringstream result = c.to_ppm_lines(false);

	while (!result.eof())
	{
		std::getline(result, line);
		ASSERT_LT(line.size(), 68);
	}
}

TEST(Chapter02Tests, PPMFilesTerminateWithNewline) 
{
	Canvas c = Canvas(10, 10);

	std::filesystem::path file_path = std::filesystem::temp_directory_path();
    file_path /= R"(Raymond\frames\_x\)";
    std::filesystem::create_directories(file_path);

    file_path /= R"(PPMFilesTerminateWithNewline.ppm)";

	canvas_to_ppm(c, file_path.string());

	std::ifstream input_file;

	input_file.open(file_path);

	char last = 'x';
	char x;

	while (!input_file.eof())
	{
		input_file.get(x);
		last = x;
	}

	ASSERT_EQ(last, '\n');
}

// ------------------------------------------------------------------------
// Chapter 03 Matrices
// ------------------------------------------------------------------------

TEST(Chapter03Tests, ConstructingAndInspectingAFourByFourMatrix) 
{
	Matrix m = Matrix(4);
	m.set_multiple({
		1.0, 2.0, 3.0, 4.0,
		5.5, 6.5, 7.5, 8.5,
		9.0, 10.0, 11.0, 12.0,
		13.5, 14.5, 15.5, 16.5
		});

	ASSERT_TRUE(flt_cmp(m.get(0, 0), 1.0));
	ASSERT_TRUE(flt_cmp(m.get(0, 3), 4.0));
	ASSERT_TRUE(flt_cmp(m.get(1, 0), 5.5));
	ASSERT_TRUE(flt_cmp(m.get(1, 2), 7.5));
	ASSERT_TRUE(flt_cmp(m.get(2, 2), 11.0));
	ASSERT_TRUE(flt_cmp(m.get(3, 0), 13.5));
	ASSERT_TRUE(flt_cmp(m.get(3, 2), 15.5));
}

TEST(Chapter03Tests, ConstructingAndInspectingATwoByTwoMatrix) 
{
	Matrix m = Matrix(2);
	m.set_multiple({
		-3.0, 5.0,
		1.0, -2.0
		});

	ASSERT_TRUE(flt_cmp(m.get(0, 0), -3.0));
	ASSERT_TRUE(flt_cmp(m.get(0, 1), 5.0));
	ASSERT_TRUE(flt_cmp(m.get(1, 0), 1.0));
	ASSERT_TRUE(flt_cmp(m.get(1, 1), -2.0));
}

TEST(Chapter03Tests, ConstructingAndInspectingAThreeByThreeMatrix) 
{
	Matrix m = Matrix(3);
	m.set_multiple({
		-3.0, 5.0, 0.0,
		1.0, -2.0, -7.0,
		0.0, 1.0, 1.0
		});

	ASSERT_TRUE(flt_cmp(m.get(0, 0), -3.0));
	ASSERT_TRUE(flt_cmp(m.get(1, 1), -2.0));
	ASSERT_TRUE(flt_cmp(m.get(2, 2), 1.0));
}

TEST(Chapter03Tests, MatrixSizeLimitations) 
{
	bool too_large_error_thrown = false;
	bool too_small_error_thrown = false;

	Matrix m = Matrix(2);
	try
	{
		m.set_multiple({
		-3.0, 5.0, 0.0,
		1.0, -2.0, -7.0,
		0.0, 1.0, 1.0
			});
	}
	catch (const std::out_of_range& e)
	{
		too_large_error_thrown = true;
	}

	ASSERT_TRUE(too_large_error_thrown);

	try
	{
		m.set_multiple({
		-3.0, 5.0
			});
	}
	catch (const std::out_of_range& e)
	{
		too_small_error_thrown = true;
	}

	ASSERT_TRUE(too_small_error_thrown);
}


TEST(Chapter03Tests, MatrixOutOfBoundsIndices) 
{
	bool x_error_thrown = false;
	bool y_error_thrown = false;

	Matrix m = Matrix(2);
	try
	{
		m.get(0, 10);
	}
	catch (const std::out_of_range& e)
	{
		x_error_thrown = true;
	}

	ASSERT_TRUE(x_error_thrown);

	try
	{
		m.get(10, 0);
	}
	catch (const std::out_of_range& e)
	{
		y_error_thrown = true;
	}

	ASSERT_TRUE(y_error_thrown);
}

TEST(Chapter03Tests, MatrixEqualityWithIdenticalMatrices) 
{
	Matrix m1 = Matrix(4);
	Matrix m2 = Matrix(4);
	m1.set_multiple({
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0,
		9.0, 8.0, 7.0, 6.0,
		5.0, 4.0, 3.0, 2.0
		});

	m2.set_multiple({
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0,
		9.0, 8.0, 7.0, 6.0,
		5.0, 4.0, 3.0, 2.0
		});

	
	ASSERT_EQ(m1, m2);
}

TEST(Chapter03Tests, MatrixInequalityWithDifferentMatrices) 
{
	Matrix m1 = Matrix(4);
	Matrix m2 = Matrix(4);
	m1.set_multiple({
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0,
		9.0, 8.0, 7.0, 6.0,
		5.0, 4.0, 3.0, 2.0
		});

	m2.set_multiple({
		5.0, 4.0, 3.0, 2.0,
		9.0, 8.0, 7.0, 6.0,
		5.0, 6.0, 7.0, 8.0,
		1.0, 2.0, 3.0, 4.0
		});


	ASSERT_NE(m1, m2);
}

TEST(Chapter03Tests, MatrixInequalityWithDifferentSizedMatrices) 
{
	Matrix m1 = Matrix(4);
	Matrix m2 = Matrix(2);
	m1.set_multiple({
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0,
		9.0, 8.0, 7.0, 6.0,
		5.0, 4.0, 3.0, 2.0
		});

	m2.set_multiple({
		1.0, 2.0,
		3.0, 4.0
		});


	ASSERT_NE(m1, m2);
}

TEST(Chapter03Tests, Matrix4Multiplication) 
{
	Matrix4 m1 = Matrix4();
	Matrix4 m2 = Matrix4();
	Matrix4 result = Matrix4();

	m1.set_multiple({
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0,
		9.0, 8.0, 7.0, 6.0,
		5.0, 4.0, 3.0, 2.0
		});

	m2.set_multiple({
		-2.0, 1.0, 2.0, 3.0,
		3.0, 2.0, 1.0, -1.0,
		4.0, 3.0, 6.0, 5.0,
		1.0, 2.0, 7.0, 8.0
		});

	result.set_multiple({
		20.0, 22.0, 50.0, 48.0,
		44.0, 54.0, 114.0, 108.0,
		40.0, 58.0, 110.0, 102.0,
		16.0, 26.0, 46.0, 42.0
		});


	ASSERT_EQ((m1 * m2), result) << (m1 * m2);
}

TEST(Chapter03Tests, Matrix4MultupliedWithATuple) 
{
	Matrix4 m = Matrix4();

	m.set_multiple({
		1.0, 2.0, 3.0, 4.0,
		2.0, 4.0, 4.0, 2.0,
		8.0, 6.0, 4.0, 1.0,
		0.0, 0.0, 0.0, 1.0
		});

	Tuple t = Tuple(1.0, 2.0, 3.0, 1.0);

	ASSERT_EQ((m * t), Tuple(18.0, 24.0, 33.0, 1.0));
}

TEST(Chapter03Tests, MultiplyingAMatrix4ByTheIdentityMatrix) 
{
	Matrix4 m = Matrix4();

	m.set_multiple({
		0.0, 1.0, 2.0, 4.0,
		1.0, 2.0, 4.0, 8.0,
		2.0, 4.0, 8.0, 16.0,
		4.0, 8.0, 16.0, 32.0
		});

	Matrix4 identity = Matrix4::Identity();

	ASSERT_EQ((m * identity), m);
}

TEST(Chapter03Tests, MultiplyingATupleByTheIdentityMatrix) 
{
	Tuple t = Tuple(1, 2, 3, 4);

	Matrix4 identity = Matrix4::Identity();

	ASSERT_EQ((identity * t), t);
}

TEST(Chapter03Tests, TransposingAMatrix4) 
{
	Matrix4 m = Matrix4();

	m.set_multiple({
		0.0, 9.0, 3.0, 0.0,
		9.0, 8.0, 0.0, 8.0,
		1.0, 8.0, 5.0, 3.0,
		0.0, 0.0, 5.0, 8.0
		});

	Matrix4 result = Matrix4();

	result.set_multiple({
		0.0, 9.0, 1.0, 0.0,
		9.0, 8.0, 8.0, 0.0,
		3.0, 0.0, 5.0, 5.0,
		0.0, 8.0, 3.0, 8.0
		});

	ASSERT_EQ(m.transpose(), result);
}

TEST(Chapter03Tests, DeterminateOfAMatrix2) 
{
	Matrix2 m = Matrix2({
		1.0, 5.0,
		-3.0, 2.0
		});

	ASSERT_TRUE(flt_cmp(m.determinant(), 17.0));
}

TEST(Chapter03Tests, ASubMatrixOfAMatrix4IsAMatrix3) 
{
	Matrix4 m = Matrix4({
		-6.0, 1.0, 1.0, 6.0,
		-8.0, 5.0, 8.0, 6.0,
		-1.0, 0.0, 8.0, 2.0,
		-7.0, 1.0, -1.0, 1.0
		});

	Matrix3 expected_result = Matrix3({
		-6.0, 1.0, 6.0,
		-8.0, 8.0, 6.0,
		-7.0, -1.0, 1.0
		});

	Matrix3 result = m.sub_matrix4(2, 1);

	ASSERT_EQ(result, expected_result) << result << " != " << expected_result;
}

TEST(Chapter03Tests, ASubMatrixOfAMatrix3IsAMatrix2) 
{
	Matrix3 m = Matrix3({
		1.0, 5.0, 0.0,
		-3.0, 2.0, 7.0,
		0.0, 6.0, -3.0
		});

	Matrix2 expected_result = Matrix2({
		-3.0, 2.0,
		-0.0, 6.0,
		});

	Matrix2 result = m.sub_matrix3(0, 2);

	ASSERT_EQ(result, expected_result) << result << " != " << expected_result;
}

TEST(Chapter03Tests, ASubMatrixOfAMatrixIsASmallerMatrix) 
{
	Matrix m = Matrix(5, {
		1.0, 5.0, 0.0, 1.0, 2.0,
		-3.0, 2.0, 7.0, 3.0, 5.0,
		0.0, 6.0, -3.0, 4.0, 6.0,
		-1.0, -2.0, -3.0, -4.0, -5.0,
		6.0, 7.0, 8.0, 9.0, 10.0
		});

	Matrix4 expected_result = Matrix4({
		1.0, 0.0, 1.0, 2.0,
		-3.0, 7.0, 3.0, 5.0,
		-1.0, -3.0, -4.0, -5.0,
		6.0, 8.0, 9.0, 10.0
		});

	Matrix result = m.sub_matrix(2, 1);

	ASSERT_EQ(result, expected_result) << result << " != " << expected_result;
}

TEST(Chapter03Tests, MinorOfAMatrix3) 
{
	Matrix3 m = Matrix3({
		3.0, 5.0, 0.0,
		2.0, -1.0, -7.0,
		6.0, -1.0, 5.0
		});

	double det = (m.sub_matrix3(1, 0)).determinant();
	double min = m.minor(1, 0);

	ASSERT_TRUE(flt_cmp(det, min));
}

TEST(Chapter03Tests, CofactorOfAMatrix3) 
{
	Matrix3 m = Matrix3({
		3.0, 5.0, 0.0,
		2.0, -1.0, -7.0,
		6.0, -1.0, 5.0
		});

	ASSERT_TRUE(flt_cmp(m.minor(0, 0), -12.0));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 0), -12.0));
	ASSERT_TRUE(flt_cmp(m.minor(1, 0), 25.0));
	ASSERT_TRUE(flt_cmp(m.cofactor(1, 0), -25.0));

	ASSERT_TRUE(flt_cmp(m.minor(0, 0), m.cofactor(0, 0)));
	ASSERT_FALSE(flt_cmp(m.minor(1, 0), m.cofactor(1, 0)));
}

TEST(Chapter03Tests, DeterminantOfAMatrix3) 
{
	Matrix3 m = Matrix3({
		1.0, 2.0, 6.0,
		-5.0, 8.0, -4.0,
		2.0, 6.0, 4.0
		});

	ASSERT_TRUE(flt_cmp(m.cofactor(0, 0), 56.0));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 1), 12.0));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 2), -46.0));
	ASSERT_TRUE(flt_cmp(m.determinant(), -196.0));
}

TEST(Chapter03Tests, DeterminantOfAMatrix4) 
{
	Matrix4 m = Matrix4({
		-2.0, -8.0, 3.0, 5.0,
		-3.0, 1.0, 7.0, 3.0,
		1.0, 2.0, -9.0, 6.0,
		-6.0, 7.0, 7.0, -9.0
		});

	ASSERT_TRUE(flt_cmp(m.cofactor(0, 0), 690.0));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 1), 447.0));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 2), 210.0));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 3), 51.0));
	ASSERT_TRUE(flt_cmp(m.determinant(), -4071.0));
}

TEST(Chapter03Tests, DeterminantOfAnArbitrarySizedMatrix) 
{
	Matrix m = Matrix(5, {
		1.0, 5.0, 0.0, 1.0, 2.0,
		-3.0, 2.0, 7.0, 3.0, 5.0,
		0.0, 6.0, -3.0, 4.0, 6.0,
		-1.0, -2.0, -3.0, -4.0, -5.0,
		6.0, 7.0, 8.0, 9.0, 10.0
		});

	ASSERT_EQ(m.determinant(), -405.0) << m.determinant();
}

TEST(Chapter03Tests, TestingInvertibleMatrixForInvertability) 
{
	Matrix4 m = Matrix4({
		6.0, 4.0, 4.0, 4.0,
		5.0, 5.0, 7.0, 6.0,
		4.0, -9.0, 3.0, -7.0,
		9.0, 1.0, 7.0, -6.0
		});

	ASSERT_TRUE(m.is_invertable());
}

TEST(Chapter03Tests, TestingNoninvertibleMatrixForInvertability) 
{
	Matrix4 m = Matrix4({
		-4.0, 2.0, -2.0, -3.0,
		9.0, 6.0, 2.0, 6.0,
		0.0, -5.0, 1.0, -5.0,
		0.0, 0.0, 0.0, 0.0
		});

	ASSERT_FALSE(m.is_invertable());
}

TEST(Chapter03Tests, CalculatingTheInverseOfAMatrix) 
{
	Matrix m = Matrix(4, {
		-5.0, 2.0, 6.0, -8.0,
		1.0, -5.0, 1.0, 8.0,
		7.0, 7.0, -6.0, -7.0,
		1.0, -3.0, 7.0, 4.0
		});

	Matrix result = m.inverse();

	Matrix expected_result = Matrix(4, {
		0.21805, 0.45113, 0.24060, -0.04511,
		-0.80827, -1.45677, -0.44361, 0.52068,
		-0.07895, -0.22368, -0.05263, 0.19737,
		-0.52256, -0.81391, -0.30075, 0.30639
	});

	ASSERT_TRUE(flt_cmp(m.determinant(), 532));

	ASSERT_TRUE(flt_cmp(m.cofactor(2, 3), -160.0));
	ASSERT_TRUE(flt_cmp(result.get(3, 2), -160.0 / 532));

	ASSERT_TRUE(flt_cmp(m.cofactor(3, 2), 105.0));
	ASSERT_TRUE(flt_cmp(result.get(2, 3), 105.0 / 532));

	ASSERT_EQ(result, expected_result);
}

TEST(Chapter03Tests, CalculatingTheInverseOfAnotherMatrix) 
{
	Matrix m = Matrix(4, {
		8.0, -5.0, 9.0, 2.0,
		7.0, 5.0, 6.0, 1.0,
		-6.0, 0.0, 9.0, 6.0,
		-3.0, 0.0, -9.0, -4.0
		});

	Matrix result = m.inverse();

	Matrix expected_result = Matrix(4, {
		-0.15385, -0.15385, -0.28205, -0.53846,
		-0.07692, 0.12308, 0.02564, 0.03077,
		0.35897, 0.35897, 0.43590, 0.92308,
		-0.69231, -0.69231, -0.76923, -1.92308
		});

	ASSERT_EQ(result, expected_result);
}

TEST(Chapter03Tests, CalculatingTheInverseOfAThirdMatrix) 
{
	Matrix m = Matrix(4, {
		9.0, 3.0, 0.0, 9.0,
		-5.0, -2.0, -6.0, -3.0,
		-4.0, 9.0, 6.0, 4.0,
		-7.0, 6.0, 6.0, 2.0
		});

	Matrix result = m.inverse();

	Matrix expected_result = Matrix(4, {
		-0.04074, -0.07778, 0.14444, -0.22222,
		-0.07778, 0.03333, 0.36667, -0.33333,
		-0.02901, -0.14630, -0.10926, 0.12963,
		0.17778, 0.06667, -0.26667, 0.33333
		});

	ASSERT_EQ(result, expected_result);
}

TEST(Chapter03Tests, MultiplyingAProductbyItsInverse) 
{
	Matrix4 m1 = Matrix4({
		3.0, -9.0, 7.0, 3.0,
		3.0, -8.0, 2.0, -9.0,
		-4.0, 4.0, 4.0, 1.0,
		-6.0, 5.0, -1.0, 1.0
		});

	Matrix4 m2 = Matrix4({
		8.0, 2.0, 2.0, 2.0,
		3.0, -1.0, 7.0, 0.0,
		7.0, 0.0, 5.0, 4.0,
		6.0, -2.0, 0.0, 5.0
		});

	Matrix4 m3 = m1 * m2;

	ASSERT_EQ(m3 * m2.inverse(), m1);
}

// ------------------------------------------------------------------------
// Chapter 04 Matrix Transformations
// ------------------------------------------------------------------------

TEST(Chapter04Tests, MultiplyingByATranslationMatrix) 
{
	Matrix4 tm = Matrix4::Translation(5.0, -3.0, 2.0);
	Tuple p = Tuple::Point(-3.0, 4.0, 5.0);

	ASSERT_EQ(tm * p, Tuple::Point(2.0, 1.0, 7.0));
}

TEST(Chapter04Tests, MultiplyingByATheInverseOfATranslationMatrix) 
{
	Matrix4 tm = Matrix4::Translation(5.0, -3.0, 2.0);
	Tuple p = Tuple::Point(-3.0, 4.0, 5.0);

	ASSERT_EQ(tm.inverse() * p, Tuple::Point(-8.0, 7.0, 3.0));
}

TEST(Chapter04Tests, TranslationDoesNotAffectVectors) 
{
	Matrix4 tm = Matrix4::Translation(5.0, -3.0, 2.0);
	Tuple v = Tuple::Vector(-3.0, 4.0, 5.0);

	ASSERT_EQ(tm * v, v);
}

TEST(Chapter04Tests, ScalingMatrixAppliedToAPoint) 
{
	Matrix4 tm = Matrix4::Scaling(2.0, 3.0, 4.0);
	Tuple p = Tuple::Point(-4.0, 6.0, 8.0);

	ASSERT_EQ(tm * p, Tuple::Point(-8.0, 18.0, 32.0));
}

TEST(Chapter04Tests, ScalingMatrixAppliedToAVector) 
{
	Matrix4 tm = Matrix4::Scaling(2.0, 3.0, 4.0);
	Tuple v = Tuple::Vector(-4.0, 6.0, 8.0);

	ASSERT_EQ(tm * v, Tuple::Vector(-8.0, 18.0, 32.0));
}

TEST(Chapter04Tests, MultiplyingByTheInverseOfTheScalingMatrix) 
{
	Matrix4 tm = Matrix4::Scaling(2.0, 3.0, 4.0);
	Tuple p = Tuple::Vector(-4.0, 6.0, 8.0);

	ASSERT_EQ(tm.inverse() * p, Tuple::Vector(-2.0, 2.0, 2.0));
}

TEST(Chapter04Tests, ReflectionIsScalingByANegativeValue) 
{
	Matrix4 tm = Matrix4::Scaling(-1.0, 1.0, 1.0);
	Tuple p = Tuple::Point(2.0, 3.0, 4.0);

	ASSERT_EQ(tm * p, Tuple::Point(-2.0, 3.0, 4.0));
}

TEST(Chapter04Tests, RotatingAPointAroundTheXAxis) 
{
	Tuple p = Tuple::Point(0.0, 1.0, 0.0);

	Matrix4 eighth = Matrix4::Rotation_X(static_cast<double>(M_PI) / 4.0);
	Matrix4 quarter = Matrix4::Rotation_X(static_cast<double>(M_PI) / 2.0);

	ASSERT_EQ(eighth * p, Tuple::Point(0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));
	ASSERT_EQ(quarter * p, Tuple::Point(0.0, 0.0, 1.0));
}

TEST(Chapter04Tests, TheInverseOfAnXRotationRotatesInTheOpositeDirection) 
{
	Tuple p = Tuple::Point(0.0, 1.0, 0.0);

	Matrix4 eighth = Matrix4::Rotation_X(static_cast<double>(M_PI) / 4.0);

	ASSERT_EQ(eighth.inverse() * p, Tuple::Point(0.0, sqrt(2.0) / 2.0, -(sqrt(2.0) / 2.0)));
}

TEST(Chapter04Tests, RotatingAPointAroundTheYAxis) 
{
	Tuple p = Tuple::Point(0.0, 0.0, 1.0);

	Matrix4 eighth = Matrix4::Rotation_Y(static_cast<double>(M_PI) / 4.0);
	Matrix4 quarter = Matrix4::Rotation_Y(static_cast<double>(M_PI) / 2.0);

	ASSERT_EQ(eighth * p, Tuple::Point(sqrt(2.0) / 2.0, 0.0, sqrt(2.0) / 2.0));
	ASSERT_EQ(quarter * p, Tuple::Point(1.0, 0.0, 0.0));
}

TEST(Chapter04Tests, RotatingAPointAroundTheZAxis) 
{
	Tuple p = Tuple::Point(0.0, 1.0, 0.0);

	Matrix4 eighth = Matrix4::Rotation_Z(static_cast<double>(M_PI) / 4.0);
	Matrix4 quarter = Matrix4::Rotation_Z(static_cast<double>(M_PI) / 2.0);

	ASSERT_EQ(eighth * p, Tuple::Point(-(sqrt(2.0) / 2.0), sqrt(2.0) / 2.0, 0.0));
	ASSERT_EQ(quarter * p, Tuple::Point(-1.0, 0.0, 0.0));
}

TEST(Chapter04Tests, AShearingTransformationMovesXInProportionToY) 
{
	Matrix4 shear = Matrix4::Shear(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	Tuple p = Tuple::Point(2.0, 3.0, 4.0);

	ASSERT_EQ(shear * p, Tuple::Point(5.0, 3.0, 4.0));
}

TEST(Chapter04Tests, AShearingTransformationMovesXInProportionToZ) 
{
	Matrix4 shear = Matrix4::Shear(0.0, 1.0, 0.0, 0.0, 0.0, 0.0);
	Tuple p = Tuple::Point(2.0, 3.0, 4.0);

	ASSERT_EQ(shear * p, Tuple::Point(6.0, 3.0, 4.0));
}

TEST(Chapter04Tests, AShearingTransformationMovesYInProportionToX)
{
	Matrix4 shear = Matrix4::Shear(0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
	Tuple p = Tuple::Point(2.0, 3.0, 4.0);

	ASSERT_EQ(shear * p, Tuple::Point(2.0, 5.0, 4.0));
}

TEST(Chapter04Tests, AShearingTransformationMovesYInProportionToZ)
{
	Matrix4 shear = Matrix4::Shear(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
	Tuple p = Tuple::Point(2.0, 3.0, 4.0);

	ASSERT_EQ(shear * p, Tuple::Point(2.0, 7.0, 4.0));
}

TEST(Chapter04Tests, AShearingTransformationMovesZInProportionToX)
{
	Matrix4 shear = Matrix4::Shear(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	Tuple p = Tuple::Point(2.0, 3.0, 4.0);

	ASSERT_EQ(shear * p, Tuple::Point(2.0, 3.0, 6.0));
}

TEST(Chapter04Tests, AShearingTransformationMovesZInProportionToY)
{
	Matrix4 shear = Matrix4::Shear(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	Tuple p = Tuple::Point(2.0, 3.0, 4.0);

	ASSERT_EQ(shear * p, Tuple::Point(2.0, 3.0, 7.0));
}

TEST(Chapter04Tests, IndividualTransfromsAreAppliedInSequence)
{
	Tuple p = Tuple::Point(1.0, 0.0, 1.0);
	Matrix4 A = Matrix4::Rotation_X(static_cast<double>(M_PI) / 2.0);
	Matrix4 B = Matrix4::Scaling(5.0, 5.0, 5.0);
	Matrix4 C = Matrix4::Translation(10.0, 5.0, 7.0);

	Tuple p2 = A * p;

	ASSERT_EQ(p2, Tuple::Point(1.0, -1.0, 0.0));

	Tuple p3 = B * p2;

	ASSERT_EQ(p3, Tuple::Point(5.0, -5.0, 0.0));

	Tuple p4 = C * p3;

	ASSERT_EQ(p4, Tuple::Point(15.0, 0.0, 7.0));
}

TEST(Chapter04Tests, ChainedTransfromationsMustBeAppliedInReverseOrder)
{
	Tuple p = Tuple::Point(1.0, 0.0, 1.0);
	Matrix4 A = Matrix4::Rotation_X(static_cast<double>(M_PI) / 2.0);
	Matrix4 B = Matrix4::Scaling(5.0, 5.0, 5.0);
	Matrix4 C = Matrix4::Translation(10.0, 5.0, 7.0);

	Matrix4 T = C * B * A;

	ASSERT_EQ(T * p, Tuple::Point(15.0, 0.0, 7.0));
}

// ------------------------------------------------------------------------
// Chapter 05 Ray-Sphere Intersections
// ------------------------------------------------------------------------

TEST(Chapter05Tests, CreatingAndQueryingARay)
{
	Tuple origin = Tuple::Point(1.0, 2.0, 3.0);
	Tuple direction = Tuple::Vector(4.0, 5.0, 6.0);
	Ray r = Ray(origin, direction);

	ASSERT_EQ(r.origin, origin);
	ASSERT_EQ(r.direction, direction);
}

TEST(Chapter05Tests, ComputingAPointFromADistance)
{
	Ray r = Ray(Tuple::Point(2.0, 3.0, 4.0), Tuple::Vector(1.0, 0.0, 0.0));

	ASSERT_EQ(r.position(0.0), Tuple::Point(2.0, 3.0, 4.0));
	ASSERT_EQ(r.position(1.0), Tuple::Point(3.0, 3.0, 4.0));
	ASSERT_EQ(r.position(-1.0), Tuple::Point(1.0, 3.0, 4.0));
	ASSERT_EQ(r.position(2.5), Tuple::Point(4.5, 3.0, 4.0));
}

TEST(Chapter05Tests, ARayIntersectsASphereAtTwoPoints)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Sphere S = Sphere();

	std::vector<double> xs = S.intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], 4.0));
	ASSERT_TRUE(flt_cmp(xs[1], 6.0));
}

TEST(Chapter05Tests, ARayIntersectsASphereAtATangent)
{
	Ray r = Ray(Tuple::Point(0.0, 1.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Sphere S = Sphere();

	std::vector<double> xs = S.intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], 5.0)) << xs[0];
	ASSERT_TRUE(flt_cmp(xs[1], 5.0)) << xs[1];
}

TEST(Chapter05Tests, ARayMissesASphere)
{
	Ray r = Ray(Tuple::Point(0.0, 2.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Sphere S = Sphere();

	std::vector<double> xs = S.intersect_t(r);

	ASSERT_EQ(xs.size(), 0);
}

TEST(Chapter05Tests, ARayOriginatesInsideASphere)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 0.0, 1.0));

	Sphere S = Sphere();

	std::vector<double> xs = S.intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], -1.0));
	ASSERT_TRUE(flt_cmp(xs[1], 1.0));
}

TEST(Chapter05Tests, ASphereIsBehindARay)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, 5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Sphere s = Sphere();

	std::vector<double> xs = s.intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], -6.0));
	ASSERT_TRUE(flt_cmp(xs[1], -4.0));
}

TEST(Chapter05Tests, AnIntersectionEncapsulatesTAndAnObject)
{
	auto s = std::make_shared<Sphere>(Sphere());

	Intersection i = Intersection(3.5, s);

	ASSERT_TRUE(flt_cmp(i.t_value, 3.5));
	ASSERT_EQ(i.object, s);
}

TEST(Chapter05Tests, AggregatingIntersections)
{
	auto s = std::make_shared<Sphere>(Sphere());

	Intersection i1 = Intersection(1.0, s);
	Intersection i2 = Intersection(2.0, s);

	Intersections xs = Intersections({ i1, i2 });

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0].t_value, 1.0));
	ASSERT_TRUE(flt_cmp(xs[1].t_value, 2.0));
}

TEST(Chapter05Tests, IntersectSetsTheObjectOnTheIntersection)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	auto s = std::make_shared<Sphere>(Sphere());

	Intersections xs = intersect(r, s);

	ASSERT_EQ(xs.size(), 2) << xs;
	ASSERT_EQ(xs[0].object, s);
	ASSERT_EQ(xs[1].object, s);
}

TEST(Chapter05Tests, TheHitWhenAllIntersectionsHavePositiveT)
{
	auto s = std::make_shared<Sphere>(Sphere());

	Intersection i1 = Intersection(1.0, s);
	Intersection i2 = Intersection(2.0, s);

	Intersections xs = Intersections({ i1, i2 });

	Intersection hit = xs.hit();
	ASSERT_EQ(hit, i1) << xs;
}

TEST(Chapter05Tests, TheHitWhenSomeIntersectionsHaveNegativeT)
{
	auto s = std::make_shared<Sphere>(Sphere());

	Intersection i1 = Intersection(-1.0, s);
	Intersection i2 = Intersection(1.0, s);

	Intersections xs = Intersections({ i1, i2 });

	Intersection hit = xs.hit();
	ASSERT_EQ(hit, i2);
}

TEST(Chapter05Tests, TheHitWhenAllIntersectionsHaveNegativeT)
{
	auto s = std::make_shared<Sphere>(Sphere());

	Intersection i1 = Intersection(-2.0, s);
	Intersection i2 = Intersection(-1.0, s);

	Intersections xs = Intersections({ i1, i2 });

	Intersection hit = xs.hit();
	ASSERT_FALSE(hit.is_valid());
}

TEST(Chapter05Tests, TheHitIsAlwaysTheLowestNonNegativeIntersection)
{
	auto s = std::make_shared<Sphere>(Sphere());

	Intersection i1 = Intersection(5.0, s);
	Intersection i2 = Intersection(7.0, s);
	Intersection i3 = Intersection(-3.0, s);
	Intersection i4 = Intersection(2.0, s);

	Intersections xs = Intersections({ i1, i2, i3, i4 });

	Intersection hit = xs.hit();
	ASSERT_EQ(hit, i4);
}

TEST(Chapter05Tests, TranslatingARay)
{
	Ray r = Ray(Tuple::Point(1.0, 2.0, 3.0), Tuple::Vector(0.0, 1.0, 0.0));
	Matrix4 m = Matrix4::Translation(3.0, 4.0, 5.0);

	Ray r2 = r.transform(m);
	ASSERT_EQ(r2.origin, Tuple::Point(4.0, 6.0, 8.0));
	ASSERT_EQ(r2.direction, Tuple::Vector(0.0, 1.0, 0.0));
}

TEST(Chapter05Tests, ScalingARay)
{
	Ray r = Ray(Tuple::Point(1.0, 2.0, 3.0), Tuple::Vector(0.0, 1.0, 0.0));
	Matrix4 m = Matrix4::Scaling(2.0, 3.0, 4.0);

	Ray r2 = r.transform(m);
	ASSERT_EQ(r2.origin, Tuple::Point(2.0, 6.0, 12.0));
	ASSERT_EQ(r2.direction, Tuple::Vector(0.0, 3.0, 0.0));
}

TEST(Chapter05Tests, ASpheresDefaultTransformation)
{
	Sphere s = Sphere();

	ASSERT_EQ(s.get_transform(), Matrix4::Identity());
}

TEST(Chapter05Tests, ChangingASpheresTransformation)
{
	Sphere s = Sphere();
	Matrix4 t = Matrix4::Translation(2.0, 3.0, 4.0);

	s.set_transform(t);

	ASSERT_EQ(s.get_transform(), t);
}

TEST(Chapter05Tests, IntersectingAScaledSphereWithARay)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));
	auto s = std::make_shared<Sphere>(Sphere());
	s->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));
	Intersections xs = intersect(r, s);

	ASSERT_EQ(xs.size(), 2);

	ASSERT_TRUE(flt_cmp(xs[0].t_value, 3.0)) << xs[0].t_value;
	ASSERT_TRUE(flt_cmp(xs[1].t_value, 7.0));
}

TEST(Chapter05Tests, IntersectingATranslatedSphereWithARay)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));
	auto s = std::make_shared<Sphere>(Sphere());
	s->set_transform(Matrix4::Translation(5.0, 0.0, 0.0));
	Intersections xs = intersect(r, s);

	ASSERT_EQ(xs.size(), 0);
}

// ------------------------------------------------------------------------
// Chapter 06 Light and Shading
// ------------------------------------------------------------------------

TEST(Chapter06Tests, TheNormalOnASphereAtAPointOnTheXAxis)
{
	Sphere s = Sphere();
	
	Tuple n = s.normal_at(Tuple::Point(1.0, 0.0, 0.0));

	ASSERT_EQ(n, Tuple::Vector(1.0, 0.0, 0.0));
}

TEST(Chapter06Tests, TheNormalOnASphereAtAPointOnTheYAxis)
{
	Sphere s = Sphere();

	Tuple n = s.normal_at(Tuple::Point(0.0, 1.0, 0.0));

	ASSERT_EQ(n, Tuple::Vector(0.0, 1.0, 0.0));
}

TEST(Chapter06Tests, TheNormalOnASphereAtAPointOnTheZAxis)
{
	Sphere s = Sphere();

	Tuple n = s.normal_at(Tuple::Point(0.0, 0.0, 1.0));

	ASSERT_EQ(n, Tuple::Vector(0.0, 0.0, 1.0));
}

TEST(Chapter06Tests, TheNormalOnASphereAtANonAxialPoint)
{
	Sphere s = Sphere();

	double sq_3 = sqrt(3.0) / 3.0;

	Tuple n = s.normal_at(Tuple::Point(sq_3, sq_3, sq_3));

	ASSERT_EQ(n, Tuple::Vector(sq_3, sq_3, sq_3)) << s.get_transform();
}

TEST(Chapter06Tests, TheNormalIsANormalizedVector)
{
	Sphere s = Sphere();

	double sq_3 = sqrt(3.0) / 3.0;

	Tuple n = s.normal_at(Tuple::Point(sq_3, sq_3, sq_3));

	ASSERT_EQ(n, n.normalize());
}

TEST(Chapter06Tests, ComputingTheNormalOnATranslatedSphere)
{
	Sphere s = Sphere();
	s.set_transform(Matrix4::Translation(0.0, 1.0, 0.0));

	Tuple n = s.normal_at(Tuple::Point(0.0, 1.70711, -0.70711));

	ASSERT_EQ(n, Tuple::Vector(0.0, 0.70711, -0.70711));
}

TEST(Chapter06Tests, ComputingTheNormalOnATransformedSphere)
{
	Sphere s = Sphere();
	Matrix4 m = Matrix4::Scaling(1.0, 0.5, 1.0) * Matrix4::Rotation_Z(static_cast<double>(M_PI) / 5.0);

	s.set_transform(m);

	Tuple n = s.normal_at(Tuple::Point(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));

	ASSERT_EQ(n, Tuple::Vector(0.0, 0.97014, -0.24254));
}

TEST(Chapter06Tests, ReflectingAVectorApproachingAt45Degrees)
{
	Tuple v = Tuple::Vector(1.0, -1.0, 0.0);
	Tuple n = Tuple::Vector(0.0, 1.0, 0.0);

	Tuple r = Tuple::reflect(v, n);

	ASSERT_EQ(r, Tuple::Vector(1.0, 1.0, 0.0));
}

TEST(Chapter06Tests, ReflectingAVectorOffOfASlantedSurface)
{
	Tuple v = Tuple::Vector(0.0, -1.0, 0.0);
	Tuple n = Tuple::Vector(sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0);

	Tuple r = Tuple::reflect(v, n);

	ASSERT_EQ(r, Tuple::Vector(1.0, 0.0, 0.0));
}

TEST(Chapter06Tests, APointLightHasPositionAndIntensity)
{
	Color intensity = Color(1.0, 1.0, 1.0);
	Tuple position = Tuple::Point(0.0, 0.0, 0.0);

	PointLight light = PointLight(position, intensity);

	ASSERT_EQ(light.position(), position);
	ASSERT_EQ(light.color, intensity);
}

TEST(Chapter06Tests, TheDefaultMaterial)
{
	PhongMaterial m = PhongMaterial();
	
	ASSERT_EQ(m.color, Color(1.0, 1.0, 1.0));
	ASSERT_TRUE(flt_cmp(m.ambient.value().luminosity(), 0.1));
	ASSERT_TRUE(flt_cmp(m.diffuse.value(), 0.9));
	ASSERT_TRUE(flt_cmp(m.specular.value(), 0.9));
	ASSERT_TRUE(flt_cmp(m.shininess.value(), 200.0));
}

TEST(Chapter06Tests, ASphereHasADefaulMaterial)
{
	Sphere s = Sphere();
	auto m = std::make_shared<PhongMaterial>();

	auto sm = std::dynamic_pointer_cast<PhongMaterial>(s.material);

	ASSERT_EQ(*m, *sm);
}

TEST(Chapter06Tests, ASphereMayBeAssignedAMaterial)
{
	Sphere s = Sphere();
	auto m = std::make_shared<PhongMaterial>();
	m->ambient = Color(1.0);

	s.material = m;

	auto sm = std::dynamic_pointer_cast<PhongMaterial>(s.material);

	ASSERT_EQ(*m, *sm);
}

TEST(Chapter06Tests, LightingWithTheEyeBewteenTheLightAndTheSurface)
{
	Tuple position = Tuple::Point(0.0, 0.0, 0.0);
	PhongMaterial m = PhongMaterial();

	Tuple eye_v = Tuple::Vector(0.0, 0.0, -1.0);
	Tuple normal_v = Tuple::Vector(0.0, 0.0, -1.0);

	auto light = std::make_shared<PointLight>(Tuple::Point(0.0, 0.0, -10.0), Color(1.0, 1.0, 1.0));
	Color result = m.lighting(light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(1.9, 1.9, 1.9));
}

TEST(Chapter06Tests, LightingWithTheEyeBewteenTheLightAndTheSurfaceEyeOffset45Degrees)
{
	Tuple position = Tuple::Point(0.0, 0.0, 0.0);
	PhongMaterial m = PhongMaterial();

	Tuple eye_v = Tuple::Vector(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
	Tuple normal_v = Tuple::Vector(0.0, 0.0, -1.0);

	auto light = std::make_shared<PointLight>(Tuple::Point(0.0, 0.0, -10.0), Color(1.0, 1.0, 1.0));
	Color result = m.lighting(light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(1.0, 1.0, 1.0));
}

TEST(Chapter06Tests, LightingWithEyeOppositeSurfaceLightOffset45Degrees)
{
	Tuple position = Tuple::Point(0.0, 0.0, 0.0);
	PhongMaterial m = PhongMaterial();

	Tuple eye_v = Tuple::Vector(0.0, 0.0, -1.0);
	Tuple normal_v = Tuple::Vector(0.0, 0.0, -1.0);

	auto light = std::make_shared<PointLight>(Tuple::Point(0.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));
	Color result = m.lighting(light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(0.7364, 0.7364, 0.7364));
}

TEST(Chapter06Tests, LightingWithEyeInThePathOfTheReflectionVector)
{
	Tuple position = Tuple::Point(0.0, 0.0, 0.0);
	PhongMaterial m = PhongMaterial();

	Tuple eye_v = Tuple::Vector(0.0, -sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
	Tuple normal_v = Tuple::Vector(0.0, 0.0, -1.0);

	auto light = std::make_shared<PointLight>(Tuple::Point(0.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));
	Color result = m.lighting(light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(1.63639, 1.63639, 1.63639));
}

TEST(Chapter06Tests, LightingWithTheLightBehindTheSurface)
{
	Tuple position = Tuple::Point(0.0, 0.0, 0.0);
	PhongMaterial m = PhongMaterial();

	Tuple eye_v = Tuple::Vector(0.0, 0.0, -1.0);
	Tuple normal_v = Tuple::Vector(0.0, 0.0, -1.0);

	auto light = std::make_shared<PointLight>(Tuple::Point(0.0, 0.0, 10.0), Color(1.0, 1.0, 1.0));
	Color result = m.lighting(light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(0.1, 0.1, 0.1));
}

// ------------------------------------------------------------------------
// Chapter 07 Making a Scene
// ------------------------------------------------------------------------

TEST(Chapter07Tests, CreatingAWorld)
{
	World w = World();

	ASSERT_EQ(w.get_lights().size(), 0);
	ASSERT_EQ(w.get_primitives().size(), 0);
}

TEST(Chapter07Tests, TheDefaulWorld)
{
	World w = World::Default();

	PointLight light = PointLight(Tuple::Point(-10.0, 10.0, -10.0), Color(1.0));

	Sphere s1 = Sphere();
	auto ms1 = std::dynamic_pointer_cast<PhongMaterial>(s1.material);
	ms1->color = Color(0.8, 1.0, 0.6);
	ms1->diffuse = 0.7;
	ms1->specular = 0.2;

	Sphere s2 = Sphere();
	s2.set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));

	ASSERT_EQ(*std::dynamic_pointer_cast<PointLight>(w.get_lights()[0]), light);
	ASSERT_EQ(
		*std::dynamic_pointer_cast<PhongMaterial>((w.get_primitives()[0])->material), 
		*std::dynamic_pointer_cast<PhongMaterial>(s1.material)
	);
	ASSERT_EQ((w.get_primitives()[1])->get_transform(), s2.get_transform());
}

TEST(Chapter07Tests, IntersectAWorldWithARay)
{
	World w = World::Default();

	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Intersections xs = w.intersect_world(r);

	ASSERT_EQ(xs.size(), 4);
	ASSERT_TRUE(flt_cmp(xs[0].t_value, 4.0));
	ASSERT_TRUE(flt_cmp(xs[1].t_value, 4.5));
	ASSERT_TRUE(flt_cmp(xs[2].t_value, 5.5));
	ASSERT_TRUE(flt_cmp(xs[3].t_value, 6.0));
}

TEST(Chapter07Tests, PrecomputingTheStateOfAnIntersection)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	auto s = std::make_shared<Sphere>();
	Intersection i = Intersection(4.0, s);

	IxComps comps = IxComps(i, r);

	ASSERT_EQ(comps.object, s);
	ASSERT_TRUE(flt_cmp(comps.t_value, i.t_value));
	ASSERT_EQ(comps.point, Tuple::Point(0.0, 0.0, -1.0));
	ASSERT_EQ(comps.eye_v, Tuple::Vector(0.0, 0.0, -1.0));
	ASSERT_EQ(comps.normal_v, Tuple::Vector(0.0, 0.0, -1.0));
}

TEST(Chapter07Tests, TheHitWhenAnIntersectionOccursOnTheOutside)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	auto s = std::make_shared<Sphere>();
	Intersection i = Intersection(4.0, s);

	IxComps comps = IxComps(i, r);

	ASSERT_FALSE(comps.inside);
}

TEST(Chapter07Tests, TheHitWhenAnIntersectionOccursOnTheInside)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 0.0, 1.0));

	auto s = std::make_shared<Sphere>();
	Intersection i = Intersection(1.0, s);

	IxComps comps = IxComps(i, r);

	ASSERT_EQ(comps.point, Tuple::Point(0.0, 0.0, 1.0));
	ASSERT_EQ(comps.eye_v, Tuple::Vector(0.0, 0.0, -1.0));
	ASSERT_TRUE(comps.inside);
	ASSERT_EQ(comps.normal_v, Tuple::Vector(0.0, 0.0, -1.0));
}

TEST(Chapter07Tests, ShadingAnIntersection)
{
	World w = World::Default();

	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	std::shared_ptr<PrimitiveBase> s = w.get_primitives()[0];
	Intersection i = Intersection(4.0, s);

	IxComps comps = IxComps(i, r);
	Color sample = w.shade(comps).get_calculated_rgb();

	ASSERT_EQ(sample, Color(0.38066, 0.47583, 0.2855));
}

TEST(Chapter07Tests, ShadingAnIntersectionFromTheInside)
{
	World w = World::Default();

	w.add_object(std::make_shared<PointLight>(Tuple::Point(0.0, 0.25, 0.0), Color(1.0, 1.0, 1.0)));

	w.remove_light(0);

	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 0.0, 1.0));

	std::shared_ptr<PrimitiveBase> s = w.get_primitives()[1];
	Intersection i = Intersection(0.5, s);

	IxComps comps = IxComps(i, r);
	Color sample = w.shade(comps).get_calculated_rgb();

	ASSERT_EQ(sample, Color(0.90498, 0.90498, 0.90498));
}

TEST(Chapter07Tests, TheColorWhenARayMisses)
{
	World w = World::Default();

	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 1.0, 0.0));

	Color sample = w.color_at(r);

	ASSERT_EQ(sample, Color(0.0, 0.0, 0.0));
}

TEST(Chapter07Tests, TheColorWhenARayHits)
{
	World w = World::Default();

	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Color sample = w.color_at(r);

	ASSERT_EQ(sample, Color(0.38066, 0.47583, 0.2855));
}

TEST(Chapter07Tests, TheColorWithAnIntersectionBehindTheRay)
{
	World w = World::Default();

	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.75), Tuple::Vector(0.0, 0.0, -1.0));

	std::shared_ptr<PrimitiveBase> outer = w.get_primitives()[0];
	auto outer_mat = std::dynamic_pointer_cast<PhongMaterial>(outer->material);
	outer_mat->ambient = Color(1.0);

	std::shared_ptr<PrimitiveBase> inner = w.get_primitives()[1];
	auto inner_mat = std::dynamic_pointer_cast<PhongMaterial>(inner->material);
	inner_mat->ambient = Color(1.0);

	Color sample = w.color_at(r);

	ASSERT_EQ(sample, inner_mat->color);
}

TEST(Chapter07Tests, TheTransformationMatrixForTheDefaultOrientation)
{
	Tuple from = Tuple::Point(0.0, 0.0, 0.0);
	Tuple to = Tuple::Point(0.0, 0.0, -1.0);
	Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

	Matrix4 vt = Matrix4::ViewTransform(from, to, up);

	ASSERT_EQ(vt, Matrix4::Identity());
}

TEST(Chapter07Tests, AViewTransformMatrixLookingInPositiveZDirection)
{
	Tuple from = Tuple::Point(0.0, 0.0, 0.0);
	Tuple to = Tuple::Point(0.0, 0.0, 1.0);
	Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

	Matrix4 vt = Matrix4::ViewTransform(from, to, up);

	ASSERT_EQ(vt, Matrix4::Scaling(-1.0, 1.0, -1.0));
}

TEST(Chapter07Tests, TheViewTransformMovesTheWorld)
{
	Tuple from = Tuple::Point(0.0, 0.0, 8.0);
	Tuple to = Tuple::Point(0.0, 0.0, 0.0);
	Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

	Matrix4 vt = Matrix4::ViewTransform(from, to, up);

	ASSERT_EQ(vt, Matrix4::Translation(0.0, 0.0, -8.0)) << vt << Matrix4::Translation(0.0, 0.0, -8.0);
}

TEST(Chapter07Tests, AnArbitraryViewTransform)
{
	Tuple from = Tuple::Point(1.0, 3.0, 2.0);
	Tuple to = Tuple::Point(4.0, -2.0, 8.0);
	Tuple up = Tuple::Vector(1.0, 1.0, 0.0);

	Matrix4 vt = Matrix4::ViewTransform(from, to, up);

	Matrix4 expected_result = Matrix4({
		-0.50709, 0.50709, 0.67612, -2.36643,
		0.76772, 0.60609, 0.12122, -2.82843,
		-0.35857, 0.59761, -0.71714, 0.0,
		0.0, 0.0, 0.0, 1.0
		});

	ASSERT_EQ(vt, expected_result);
}

TEST(Chapter07Tests, ConstructingACamera)
{
	int h_size = 160;
	int v_size = 120;
	double fov = M_PI / 2.0;

	Camera c = Camera(h_size, v_size, fov);

	ASSERT_EQ(c.get_horizontal_size(), h_size);
	ASSERT_EQ(c.get_vertical_size(), v_size);
	ASSERT_EQ(c.get_fov(), fov);
	ASSERT_EQ(c.get_transform(), Matrix4::Identity());
}

TEST(Chapter07Tests, ThePixelSizeForAHorizontalCanvas)
{
	Camera c = Camera(200, 125, M_PI / 2.0);

	ASSERT_TRUE(flt_cmp(c.get_pixel_size(), 0.01));
}

TEST(Chapter07Tests, ThePixelSizeForAVerticalCanvas)
{
	Camera c = Camera(125, 200, M_PI / 2.0);

	ASSERT_TRUE(flt_cmp(c.get_pixel_size(), 0.01));
}

TEST(Chapter07Tests, ConstructingARayThroughTheCenterOfTheCanvas)
{
	Camera c = Camera(201, 101, M_PI / 2.0);

	Ray r = c.ray_from_pixel(100, 50);

	ASSERT_EQ(r.origin, Tuple::Point(0.0, 0.0, 0.0));
	ASSERT_EQ(r.direction, Tuple::Vector(0.0, 0.0, -1.0));
}

TEST(Chapter07Tests, ConstructingARayThroughACornerOfTheCanvas)
{
	Camera c = Camera(201, 101, M_PI / 2.0);

	Ray r = c.ray_from_pixel(0, 0);

	ASSERT_EQ(r.origin, Tuple::Point(0.0, 0.0, 0.0));
	ASSERT_EQ(r.direction, Tuple::Vector(0.66519, 0.33259, -0.66851));
}

TEST(Chapter07Tests, ConstructingARayWhenTheCameraIsTransformed)
{
	Camera c = Camera(201, 101, M_PI / 2.0);
	c.set_transform(
		Matrix4::Rotation_Y(M_PI / 4.0) *
		Matrix4::Translation(0.0, -2.0, 5.0)
	);

	Ray r = c.ray_from_pixel(100, 50);

	double angle = sqrt(2.0) / 2.0;

	ASSERT_EQ(r.origin, Tuple::Point(0.0, 2.0, -5.0));
	ASSERT_EQ(r.direction, Tuple::Vector(angle, 0.0, -angle));
}

TEST(Chapter07Tests, RenderingAWorldWithACamera)
{
	World w = World::Default();
	Camera c = Camera(11, 11, M_PI / 2.0);

	Tuple from = Tuple::Point(0.0, 0.0, -5.0);
	Tuple to = Tuple::Point(0.0, 0.0, 0.0);
	Tuple up = Tuple::Vector(0.0, 1.0, 0.0);

	c.set_transform(Matrix4::ViewTransform(from, to, up));

	Canvas image = c.render(w);

	ASSERT_EQ(image.pixel_at(5, 5), Color(0.38066, 0.47583, 0.2855));
}

TEST(Chapter07Tests, DegreesConvertToRadians)
{
	ASSERT_TRUE(flt_cmp(M_PI / 4.0, deg_to_rad(45.0)));
}

TEST(Chapter07Tests, RadiansConvertToDegrees)
{
	ASSERT_TRUE(flt_cmp(rad_to_deg(M_PI / 4.0), 45.0));
}

// ------------------------------------------------------------------------
// Chapter 08 Shadows
// ------------------------------------------------------------------------

TEST(Chapter08Tests, LightingWithTheSurfaceInShadow)
{
	Tuple eye_v = Tuple::Vector(0.0, 0.0, -1.0);
	Tuple normal_v = Tuple::Vector(0.0, 0.0, -1.0);
	auto light = std::make_shared<PointLight>(Tuple::Point(0.0, 0.0, -10.0), Color(1.0));
	bool is_shadowed = true;

	PhongMaterial m = PhongMaterial();
	Tuple position = Tuple::Point(0.0, 0.0, 0.0);

	Color result = m.lighting(light, position, eye_v, normal_v, is_shadowed);

	ASSERT_EQ(result, Color(0.1));
}

TEST(Chapter08Tests, ThereIsNoShadowWhenNothingIsCollinearWithPointAndLight)
{
	World w = World::Default();
	Tuple p = Tuple::Point(0.0, 10.0, 10.0);

	ASSERT_FALSE(w.is_shadowed(w.get_lights()[0], p));
}

TEST(Chapter08Tests, TheShadowWhenTheObjectIsBetweenThePointAndTheLight)
{
	World w = World::Default();
	Tuple p = Tuple::Point(10.0, -10.0, 10.0);

	ASSERT_TRUE(w.is_shadowed(w.get_lights()[0], p));
}

TEST(Chapter08Tests, ThereIsNoShadowWhenTheObjectIsBehindTheLight)
{
	World w = World::Default();
	Tuple p = Tuple::Point(-20.0, 20.0, -20.0);

	ASSERT_FALSE(w.is_shadowed(w.get_lights()[0], p));
}

TEST(Chapter08Tests, ThereIsNoShadowWhenTheObjectIsBehindThePoint)
{
	World w = World::Default();
	Tuple p = Tuple::Point(-2.0, 2.0, 2.0);

	ASSERT_FALSE(w.is_shadowed(w.get_lights()[0], p));
}

TEST(Chapter08Tests, ShadeIsGivenAnIntersectionInShadow)
{
	World w = World();
	auto light = std::make_shared<PointLight>(Tuple::Point(0.0, 0.0, -10.0), Color(1.0));
	w.add_object(light);

	auto s1 = std::make_shared<Sphere>();
	w.add_object(s1);
	auto s2 = std::make_shared<Sphere>();
	w.add_object(s2);
	s2->set_transform(Matrix4::Translation(0.0, 0.0, 10.0));

	Ray r = Ray(Tuple::Point(0.0, 0.0, 5.0), Tuple::Vector(0.0, 0.0, 1.0));
	Intersection i = Intersection(4.0, s2);

	IxComps comps = IxComps(i, r);

	Color c = w.shade(comps).get_calculated_rgb();

	ASSERT_EQ(c, Color(0.1));
}

TEST(Chapter08Tests, TheHitShouldOffsetThePoint)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	auto shape = std::make_shared<Sphere>();
	shape->set_transform(Matrix4::Translation(0.0, 0.0, 1.0));

	Intersection i = Intersection(5.0, shape);

	IxComps comps = IxComps(i, r);

	ASSERT_LT(comps.over_point.z, -(EPSILON / 2.0)) << comps.normal_v;
	ASSERT_GT(comps.point.z, comps.over_point.z);
}

// ------------------------------------------------------------------------
// Chapter 09 Planes
// ------------------------------------------------------------------------

TEST(Chapter09Tests, TheDefaultTransformation)
{
	TestShape s = TestShape();

	ASSERT_EQ(s.get_transform(), Matrix4::Identity());
}

TEST(Chapter09Tests, AssigningATransformation)
{
	TestShape s = TestShape();
	Matrix4 t = Matrix4::Translation(2.0, 3.0, 4.0);

	s.set_transform(t);

	ASSERT_EQ(s.get_transform(), t);
}

TEST(Chapter09Tests, TheDefaultMaterial)
{
	TestShape s = TestShape();
	auto m = std::make_shared<PhongMaterial>();

	auto sm = std::dynamic_pointer_cast<PhongMaterial>(s.material);

	ASSERT_EQ(*m, *sm);
}

TEST(Chapter09Tests, AssigningAMaterial)
{
	TestShape s = TestShape();
	auto m = std::make_shared<PhongMaterial>();
	m->ambient = Color(1.0);

	s.material = m;

	auto sm = std::dynamic_pointer_cast<PhongMaterial>(s.material);

	ASSERT_EQ(*m, *sm);
}

TEST(Chapter09Tests, IntersectingAScaledShapeWithAnArray)
{
	TestShape s = TestShape();
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	s.set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));

	Ray xr = s.ray_to_object_space(r);

	ASSERT_EQ(xr.origin, Tuple::Point(0.0, 0.0, -2.5));
	ASSERT_EQ(xr.direction, Tuple::Vector(0.0, 0.0, 0.5));
}

TEST(Chapter09Tests, IntersectingATranslatedShapeWithARay)
{
	TestShape s = TestShape();
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	s.set_transform(Matrix4::Translation(5.0, 0.0, 0.0));

	Ray xr = s.ray_to_object_space(r);

	ASSERT_EQ(xr.origin, Tuple::Point(-5.0, 0.0, -5.0));
	ASSERT_EQ(xr.direction, Tuple::Vector(0.0, 0.0, 1.0));
}

TEST(Chapter09Tests, ComputingTheNormalOnATranslatedShape)
{
	TestShape s = TestShape();

	s.set_transform(Matrix4::Translation(0.0, 1.0, 0.0));

	Tuple n = s.normal_at(Tuple::Point(0, 1.70711, -0.70711));

	ASSERT_EQ(n, Tuple::Vector(0.0, 0.70711, -0.70711));
}

TEST(Chapter09Tests, ComputingTheNormalOnATransformedShape)
{
	TestShape s = TestShape();

	Matrix4 m = Matrix4::Scaling(1.0, 0.5, 1.0) * Matrix4::Rotation_Z(M_PI / 5.0);

	s.set_transform(m);

	double sq_2 = sqrt(2.0) / 2;

	Tuple n = s.normal_at(Tuple::Point(0, sq_2, -sq_2));

	ASSERT_EQ(n, Tuple::Vector(0.0, 0.97014, -0.24254));
}

TEST(Chapter09Tests, TheNormalOfAPlaneIsConstantEverywhere)
{
	auto p = InfinitePlane();

	Tuple n1 = p.get_definition()->local_normal_at(Tuple::Point(0.0, 0.0, 0.0));
	Tuple n2 = p.get_definition()->local_normal_at(Tuple::Point(10.0, 0.0, -10.0));
	Tuple n3 = p.get_definition()->local_normal_at(Tuple::Point(-50.0, 0.0, 150.0));

	Tuple expected_result = Tuple::Vector(0.0, 1.0, 0.0);

	ASSERT_EQ(n1, expected_result);
	ASSERT_EQ(n2, expected_result);
	ASSERT_EQ(n3, expected_result);
}

TEST(Chapter09Tests, IntersectWithARayParallelToThePlane)
{
	auto p = std::make_shared<InfinitePlane>();

	Ray r = Ray(Tuple::Point(0.0, 10.0, 0.0), Tuple::Vector(0.0, 0.0, 1.0));

	Intersections xs = intersect(r, p);

	ASSERT_EQ(xs.size(), 0);
}

TEST(Chapter09Tests, IntersectWithACoplanarRay)
{
	auto p = std::make_shared<InfinitePlane>();

	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 0.0, 1.0));

	Intersections xs = intersect(r, p);

	ASSERT_EQ(xs.size(), 0);
}

TEST(Chapter09Tests, ARayIntersectingAPlaneFromAbove)
{
	auto p = std::make_shared<InfinitePlane>();

	Ray r = Ray(Tuple::Point(0.0, 1.0, 0.0), Tuple::Vector(0.0, -1.0, 0.0));

	Intersections xs = intersect(r, p);

	ASSERT_EQ(xs.size(), 1);
	ASSERT_TRUE(flt_cmp(xs[0].t_value, 1.0));
	ASSERT_EQ(xs[0].object, p);
}

TEST(Chapter09Tests, ARayIntersectingAPlaneFromBelow)
{
	auto p = std::make_shared<InfinitePlane>();

	Ray r = Ray(Tuple::Point(0.0, -1.0, 0.0), Tuple::Vector(0.0, 1.0, 0.0));

	Intersections xs = intersect(r, p);

	ASSERT_EQ(xs.size(), 1);
	ASSERT_TRUE(flt_cmp(xs[0].t_value, 1.0));
	ASSERT_EQ(xs[0].object, p);
}

// ------------------------------------------------------------------------
// Chapter 10 Patterns
// ------------------------------------------------------------------------

TEST(Chapter10Tests, CreatingAStripePattern)
{
	StripeMap pattern = StripeMap(WHITE, BLACK);

	auto a = std::dynamic_pointer_cast<SolidColorMap>(pattern.a);
	auto b = std::dynamic_pointer_cast<SolidColorMap>(pattern.b);

	ASSERT_EQ(a->col, WHITE);
	ASSERT_EQ(b->col, BLACK);
}

TEST(Chapter10Tests, AStripePatternIsConstantInY)
{
	StripeMap pattern = StripeMap(WHITE, BLACK);

	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 1.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 2.0, 0.0)), WHITE);
}

TEST(Chapter10Tests, AStripePatternIsConstantInZ)
{
	StripeMap pattern = StripeMap(WHITE, BLACK);

	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 1.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 2.0)), WHITE);
}

TEST(Chapter10Tests, AStripePatternAlternatesInX)
{
	StripeMap pattern = StripeMap(WHITE, BLACK);

	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.9, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(1.0, 0.0, 0.0)), BLACK);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(-0.1, 0.0, 0.0)), BLACK);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(-1.0, 0.0, 0.0)), BLACK);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(-1.1, 0.0, 0.0)), WHITE);
}

TEST(Chapter10Tests, LightingWithAPatternApplied)
{
	auto pattern = std::make_shared<StripeMap>(WHITE, BLACK);
	auto light = std::make_shared<PointLight>(Tuple::Point(0.0, 0.0, -10), Color(1.0));

	PhongMaterial m = PhongMaterial();
	m.color.connect(pattern);
	m.ambient = Color(1.0);
	m.diffuse = 0.0;
	m.specular = 0.0;

	Tuple eye_v = Tuple::Vector(0.0, 0.0, -1.0);
	Tuple normal_v = Tuple::Vector(0.0, 0.0, -1.0);

	Color c1 = m.lighting(light, Tuple::Point(0.9, 0.0, 0.0), eye_v, normal_v);
	Color c2 = m.lighting(light, Tuple::Point(1.1, 0.0, 0.0), eye_v, normal_v);

	ASSERT_EQ(c1, WHITE);
	ASSERT_EQ(c2, BLACK);
}

TEST(Chapter10Tests, StripesWithAnObjectTransformation)
{
	StripeMap pattern = StripeMap(WHITE, BLACK);
	pattern.set_mapping_space(ObjectSpace);

	auto s = std::make_shared<Sphere>();
	s->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));

	IxComps comps = IxComps();
	comps.point = Tuple::Point(1.5, 0.0, 0.0);
	comps.object = s;

	ASSERT_EQ(pattern.sample_at(comps), WHITE);
}

TEST(Chapter10Tests, StripesWithAPatternTransformation)
{
	StripeMap pattern = StripeMap(WHITE, BLACK);
	pattern.transform->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));

	auto s = std::make_shared<Sphere>();

	IxComps comps = IxComps();
	comps.point = Tuple::Point(1.5, 0.0, 0.0);
	comps.object = s;

	ASSERT_EQ(pattern.sample_at(comps), WHITE);
}

TEST(Chapter10Tests, StripesWithAnObjectAndAPatternTransformation)
{
	StripeMap pattern = StripeMap(WHITE, BLACK);
	pattern.set_mapping_space(ObjectSpace);
	pattern.transform->set_transform(Matrix4::Translation(0.5, 0.0, 0.0));

	auto s = std::make_shared<Sphere>();
	s->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));

	IxComps comps = IxComps();
	comps.point = Tuple::Point(2.5, 0.0, 0.0);
	comps.object = s;

	ASSERT_EQ(pattern.sample_at(comps), WHITE);
}

TEST(Chapter10Tests, TheDefaultPatternTransformation)
{
	TestMap pattern = TestMap();

	ASSERT_EQ(pattern.transform->get_transform(), Matrix4::Identity());
}

TEST(Chapter10Tests, AssigningATransformation)
{
	TestMap pattern = TestMap();
	pattern.transform->set_transform(Matrix4::Translation(1.0, 2.0, 3.0));

	ASSERT_EQ(pattern.transform->get_transform(), Matrix4::Translation(1.0, 2.0, 3.0));
}

TEST(Chapter10Tests, APatternWithAnObjectTransformation)
{
	TestMap pattern = TestMap();
	pattern.set_mapping_space(ObjectSpace);

	auto s = std::make_shared<Sphere>();
	s->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));

	IxComps comps = IxComps();
	comps.point = Tuple::Point(2.0, 3.0, 4.0);
	comps.object = s;

	Color result = pattern.sample_at(comps);
	result.w = 0.0;

	ASSERT_EQ(result, Color(1.0, 1.5, 2.0));
}

TEST(Chapter10Tests, APatternWithAPatternTransformation)
{
	TestMap pattern = TestMap();
	pattern.transform->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));

	auto s = std::make_shared<Sphere>();

	IxComps comps = IxComps();
	comps.point = Tuple::Point(2.0, 3.0, 4.0);
	comps.object = s;

	Color result = pattern.sample_at(comps);
	result.w = 0.0;

	ASSERT_EQ(result, Color(1.0, 1.5, 2.0));
}

TEST(Chapter10Tests, APatternWithAnObjectAndAPatternTransformation)
{
	TestMap pattern = TestMap();
	pattern.set_mapping_space(ObjectSpace);
	pattern.transform->set_transform(Matrix4::Translation(0.5, 1.0, 1.5));

	auto s = std::make_shared<Sphere>();
	s->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));

	IxComps comps = IxComps();
	comps.point = Tuple::Point(2.5, 3.0, 3.5);
	comps.object = s;

	Color result = pattern.sample_at(comps);
	result.w = 0.0;

	ASSERT_EQ(result, Color(0.75, 0.5, 0.25));
}

TEST(Chapter10Tests, AGradientInterpolatesBetweenColors)
{
	GradientMap pattern = GradientMap(WHITE, BLACK);

	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.25, 0.0, 0.0)), Color(0.75));
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.5, 0.0, 0.0)), Color(0.5));
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.75, 0.0, 0.0)), Color(0.25));
}

TEST(Chapter10Tests, ARingShouldExtendInBothXAndZ)
{
	RingMap pattern = RingMap(WHITE, BLACK);

	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(1, 0.0, 0.0)), BLACK);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0, 0.0, 1.0)), BLACK);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.708, 0.0, 0.708)), BLACK);
}

TEST(Chapter10Tests, CheckersShouldRepeatInX)
{
	CheckerMap pattern = CheckerMap(WHITE, BLACK);

	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.99, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(1.01, 0.0, 0.0)), BLACK);
}

TEST(Chapter10Tests, CheckersShouldRepeatInY)
{
	CheckerMap pattern = CheckerMap(WHITE, BLACK);

	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.99, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 1.01, 0.0)), BLACK);
}

TEST(Chapter10Tests, CheckersShouldRepeatInZ)
{
	CheckerMap pattern = CheckerMap(WHITE, BLACK);

	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.0)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.99)), WHITE);
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 1.01)), BLACK);
}

TEST(Chapter10Tests, OverlayBlendModeSingleChannel)
{
	Color base1 = Color(0.5, 0.0, 0.0);
	Color c1 = Color(0.5);
	Color c2 = Color(0.25);
	Color c3 = Color(0.0);
	Color c4 = Color(0.75);
	Color c5 = Color(1.0);

	ASSERT_EQ(base1.overlay(c1, 1.0), base1);
	ASSERT_EQ(base1.overlay(c1, 0.5), base1);
	ASSERT_EQ(base1.overlay(c2, 1.0), Color(0.25, 0.0, 0.0));
	ASSERT_EQ(base1.overlay(c2, 0.5), Color(0.375, 0.0, 0.0));
	ASSERT_EQ(base1.overlay(c3, 1.0), Color(0.0, 0.0, 0.0));
	ASSERT_EQ(base1.overlay(c3, 0.5), Color(0.25, 0.0, 0.0));
	ASSERT_EQ(base1.overlay(c4, 1.0), Color(0.75, 0.0, 0.0));
	ASSERT_EQ(base1.overlay(c4, 0.5), Color(0.625, 0.0, 0.0));
	ASSERT_EQ(base1.overlay(c5, 1.0), Color(1.0, 0.0, 0.0));
	ASSERT_EQ(base1.overlay(c5, 0.5), Color(0.75, 0.0, 0.0));
}

TEST(Chapter10Tests, OverlayBlendModeLuminosity)
{
	Color base1 = Color(0.5);
	Color c1 = Color(0.5);
	Color c2 = Color(0.25);
	Color c3 = Color(0.0);
	Color c4 = Color(0.75);
	Color c5 = Color(1.0);

	ASSERT_EQ(base1.overlay(c1, 1.0), base1);
	ASSERT_EQ(base1.overlay(c1, 0.5), base1);
	ASSERT_EQ(base1.overlay(c2, 1.0), Color(0.25));
	ASSERT_EQ(base1.overlay(c2, 0.5), Color(0.375));
	ASSERT_EQ(base1.overlay(c3, 1.0), Color(0.0));
	ASSERT_EQ(base1.overlay(c3, 0.5), Color(0.25));
	ASSERT_EQ(base1.overlay(c4, 1.0), Color(0.75));
	ASSERT_EQ(base1.overlay(c4, 0.5), Color(0.625));
	ASSERT_EQ(base1.overlay(c5, 1.0), Color(1.0));
	ASSERT_EQ(base1.overlay(c5, 0.5), Color(0.75));
}

TEST(Chapter10Tests, OverlayBlendModeChroma)
{
	Color base1 = Color(0.5, 0.5, 1.0);
	Color c1 = Color(0.5, 0.25, 0.0);
	Color c2 = Color(0.75, 1.0, 0,25);

	ASSERT_EQ(base1.overlay(c1, 1.0), Color(0.5, 0.25, 1.0));
	ASSERT_EQ(base1.overlay(c1, 0.5), Color(0.5, 0.375, 1.0));
	ASSERT_EQ(base1.overlay(c2, 1.0), Color(0.75, 1.0, 1.0));
	ASSERT_EQ(base1.overlay(c2, 0.5), Color(0.625, 0.75, 1.0));
}

TEST(Chapter10Tests, NestedPatterns)
{
	auto stripe1 = std::make_shared<StripeMap>(Color(1.0, 0.0, 0.0), Color(0.5, 0.0, 0.0));
	stripe1->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));
	auto stripe2 = std::make_shared<StripeMap>(Color(0.0, 1.0, 0.0), Color(0.0, 0.5, 0.0));
	stripe2->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));
	CheckerMap pattern = CheckerMap(stripe1, stripe2);

	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.0, 0.0, 0.0)), Color(1.0, 0.0, 0.0));
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(0.99, 0.0, 0.0)), Color(0.5, 0.0, 0.0));
	ASSERT_EQ(pattern.sample_at_point(Tuple::Point(1.01, 0.0, 0.0)), Color(0.0, 1.0, 0.0));
}

TEST(Chapter10Tests, LerpInterpolatesDoubles)
{
	double a = 14.0;
	double b = -12.0;

	ASSERT_TRUE(flt_cmp(lerp(0.0, a, b), 14.0));
	ASSERT_TRUE(flt_cmp(lerp(0.1, a, b), 11.4));
	ASSERT_TRUE(flt_cmp(lerp(0.5, a, b), 1.0));
	ASSERT_TRUE(flt_cmp(lerp(0.9, a, b), -9.4));
	ASSERT_TRUE(flt_cmp(lerp(1.0, a, b), -12.0));
}

TEST(Chapter10Tests, LerpInterpolatesTuples)
{
	Tuple a = Tuple::Point(-10.0, 40.0, -10.0);
	Tuple b = Tuple::Point(40.0, -10.0, 40.0);

	ASSERT_EQ(lerp(0.0, a, b), Tuple::Point(-10.0, 40.0, -10.0));
	ASSERT_EQ(lerp(0.1, a, b), Tuple::Point(-5.0, 35.0, -5.0));
	ASSERT_EQ(lerp(0.5, a, b), Tuple::Point(15.0, 15.0, 15.0));
	ASSERT_EQ(lerp(0.9, a, b), Tuple::Point(35.0, -5.0, 35.0));
	ASSERT_EQ(lerp(1.0, a, b), Tuple::Point(40.0, -10.0, 40.0));

}

TEST(Chapter10Tests, CompositeMapAddMode)
{
	// [1a,2a][1a,2b]
	// [1b,2a][1b,2b]
	auto stripe1 = std::make_shared<StripeMap>(Color(1.0, 0.0, 0.0), Color(0.5, 0.0, 0.0));
	stripe1->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5) * Matrix4::Rotation_Y(deg_to_rad(90.0)));
	auto stripe2 = std::make_shared<StripeMap>(Color(0.0, 1.0, 0.0), Color(0.0, 0.5, 0.0));
	stripe2->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));

	CompositeMap c_map_1 = CompositeMap(stripe1, stripe2, CompAdd, 1.0);

	ASSERT_EQ(c_map_1.sample_at_point(Tuple::Point(0.25, 0.0, 0.25)), Color(0.5, 1.0, 0.0)); // 1b, 2a
	ASSERT_EQ(c_map_1.sample_at_point(Tuple::Point(0.75, 0.0, 0.25)), Color(0.5, 0.5, 0.0)); // 1b, 2b
	ASSERT_EQ(c_map_1.sample_at_point(Tuple::Point(0.75, 0.0, 0.75)), Color(1.0, 0.5, 0.0)); // 1a, 2b

	CompositeMap c_map_2 = CompositeMap(stripe1, stripe2, CompAdd, 0.5);

	ASSERT_EQ(c_map_2.sample_at_point(Tuple::Point(0.25, 0.0, 0.25)), Color(0.5, 0.5, 0.0)); // 1b, 2a
	ASSERT_EQ(c_map_2.sample_at_point(Tuple::Point(0.75, 0.0, 0.25)), Color(0.5, 0.25, 0.0)); // 1b, 2b
	ASSERT_EQ(c_map_2.sample_at_point(Tuple::Point(0.75, 0.0, 0.75)), Color(1.0, 0.25, 0.0)); // 1a, 2b

	CompositeMap c_map_3 = CompositeMap(stripe1, stripe2, CompAdd, 0.0);

	ASSERT_EQ(c_map_3.sample_at_point(Tuple::Point(0.25, 0.0, 0.25)), Color(0.5, 0.0, 0.0)); // 1b, 2a
	ASSERT_EQ(c_map_3.sample_at_point(Tuple::Point(0.75, 0.0, 0.25)), Color(0.5, 0.0, 0.0)); // 1b, 2b
	ASSERT_EQ(c_map_3.sample_at_point(Tuple::Point(0.75, 0.0, 0.75)), Color(1.0, 0.0, 0.0)); // 1a, 2b
}

TEST(Chapter10Tests, CompositeMapBlendMode)
{
	// [1a,2a][1a,2b]
	// [1b,2a][1b,2b]
	Color c_1a = Color(1.0, 0.5, 0.5);
	Color c_1b = Color(0.5, 0.25, 0.25);
	Color c_2a = Color(0.25, 0.5, 0.25);
	Color c_2b = Color(0.5, 1.0, 0.5);

	auto stripe1 = std::make_shared<StripeMap>(c_1a, c_1b);
	stripe1->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5) * Matrix4::Rotation_Y(deg_to_rad(90.0)));
	auto stripe2 = std::make_shared<StripeMap>(c_2a, c_2b);
	stripe2->transform->set_transform(Matrix4::Scaling(0.5, 0.5, 0.5));

	CompositeMap c_map_1 = CompositeMap(stripe1, stripe2, CompBlend, 1.0);

	ASSERT_EQ(c_map_1.sample_at_point(Tuple::Point(0.25, 0.0, 0.25)), c_2a); // 1b, 2a
	ASSERT_EQ(c_map_1.sample_at_point(Tuple::Point(0.75, 0.0, 0.25)), c_2b); // 1b, 2b
	ASSERT_EQ(c_map_1.sample_at_point(Tuple::Point(0.75, 0.0, 0.75)), c_2b); // 1a, 2b

	CompositeMap c_map_2 = CompositeMap(stripe1, stripe2, CompBlend, 0.5);

	ASSERT_EQ(c_map_2.sample_at_point(Tuple::Point(0.25, 0.0, 0.25)), Color(0.375, 0.375, 0.25)); // 1b, 2a
	ASSERT_EQ(c_map_2.sample_at_point(Tuple::Point(0.75, 0.0, 0.25)), Color(0.5, 0.625, 0.375)); // 1b, 2b
	ASSERT_EQ(c_map_2.sample_at_point(Tuple::Point(0.75, 0.0, 0.75)), Color(0.75, 0.75, 0.5)); // 1a, 2b

	CompositeMap c_map_3 = CompositeMap(stripe1, stripe2, CompBlend, 0.0);

	ASSERT_EQ(c_map_3.sample_at_point(Tuple::Point(0.25, 0.0, 0.25)), c_1b); // 1b, 2a
	ASSERT_EQ(c_map_3.sample_at_point(Tuple::Point(0.75, 0.0, 0.25)), c_1b); // 1b, 2b
	ASSERT_EQ(c_map_3.sample_at_point(Tuple::Point(0.75, 0.0, 0.75)), c_1a); // 1a, 2b
}

// ------------------------------------------------------------------------
// Chapter 11 Reflection and Refraction
// ------------------------------------------------------------------------

TEST(Chapter11Tests, ReflectivityForTheDefaultMaterial)
{
	PhongMaterial m = PhongMaterial();
	ASSERT_EQ(m.reflection.value(), Color(0.0));
	ASSERT_EQ(m.reflection_roughness.value(), 0.0);
}

TEST(Chapter11Tests, PrecomputingTheReflectionVector)
{
	auto shape = std::make_shared<InfinitePlane>();
	Ray r = Ray(Tuple::Point(0.0, 1.0, -1.0), Tuple::Vector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));
	Intersection i = Intersection(sqrt(2.0), shape);

	IxComps comps = IxComps(i, r);

	ASSERT_EQ(comps.reflect_v, Tuple::Vector(0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));
}

TEST(Chapter11Tests, TheReflectedColorForANonreflectiveMaterial)
{
	World w = World::Default();

	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 0.0, 1.0));

	auto shape = w.get_primitives()[1];
	auto ms1 = std::dynamic_pointer_cast<PhongMaterial>(shape->material);
	ms1->ambient.set_value(Color(1.0));

	Intersection i = Intersection(1.0, shape);
	IxComps comps = IxComps(i, r);
	Color ref = ms1->reflect(w, comps);

	ASSERT_EQ(ref, Color(0.0));
}

TEST(Chapter11Tests, TheReflectedColorForAReflectiveMaterial)
{
	World w = World::Default();

	Ray r = Ray(Tuple::Point(0.0, 0.0, -3.0), Tuple::Vector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));

	auto shape = std::make_shared<InfinitePlane>();
	auto ms1 = std::dynamic_pointer_cast<PhongMaterial>(shape->material);
	ms1->reflection.set_value(Color(0.5));
	shape->set_transform(Matrix4::Translation(0.0, -1.0, 0.0));
	w.add_object(shape);

	Intersection i = Intersection(sqrt(2.0), shape);
	IxComps comps = IxComps(i, r);
	Color ref = ms1->reflect(w, comps);

	ASSERT_EQ(ref, Color(0.19032, 0.2379, 0.14274));
}

TEST(Chapter11Tests, shade_hit_WithAReflectiveMaterial)
{
	World w = World::Default();

	Ray r = Ray(Tuple::Point(0.0, 0.0, -3.0), Tuple::Vector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));

	auto shape = std::make_shared<InfinitePlane>();
	shape->set_transform(Matrix4::Translation(0.0, -1.0, 0.0));
	w.add_object(shape);

	auto ms1 = std::dynamic_pointer_cast<PhongMaterial>(shape->material);
	ms1->reflection.set_value(Color(0.5));
	ms1->use_schlick = false;

	Intersection i = Intersection(sqrt(2.0), shape);
	IxComps comps = IxComps(i, r);
	Color ref = w.shade(comps).get_calculated_rgb();

	ASSERT_EQ(ref, Color(0.87677, 0.92436, 0.82918));
}

TEST(Chapter11Tests, color_atFunctionWithMutuallyReflectiveSurfaces)
{
	World w = World();

	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 1.0, 0.0));

	w.add_object(std::make_shared<PointLight>(Tuple::Point(0.0, 0.0, 0.0), Color(1.0)));

	auto lower = std::make_shared<InfinitePlane>();
	lower->set_transform(Matrix4::Translation(0.0, -1.0, 0.0));
	w.add_object(lower);

	auto upper = std::make_shared<InfinitePlane>();
	upper->set_transform(Matrix4::Translation(0.0, 1.0, 0.0));
	w.add_object(upper);

	auto ms1 = std::dynamic_pointer_cast<PhongMaterial>(lower->material);
	ms1->reflection.set_value(Color(1.0));
	upper->material = ms1;

	auto start = std::chrono::steady_clock::now();

	Color col = w.color_at(r);

	auto end = std::chrono::steady_clock::now();

	auto diff = end - start;

	ASSERT_LT(diff, std::chrono::duration<int>(10));
}

TEST(Chapter11Tests, TheReflectedColorAtTheMaximumRecursionDepth)
{
	World w = World::Default();

	Ray r = Ray(Tuple::Point(0.0, 0.0, -3.0), Tuple::Vector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0), RAY_DEPTH_LIMIT);

	auto shape = std::make_shared<InfinitePlane>();
	auto ms1 = std::dynamic_pointer_cast<PhongMaterial>(shape->material);
	ms1->reflection.set_value(Color(0.5));
	shape->set_transform(Matrix4::Translation(0.0, -1.0, 0.0));
	w.add_object(shape);

	Intersection i = Intersection(sqrt(2.0), shape);
	IxComps comps = IxComps(i, r);
	Color ref = ms1->reflect(w, comps);

	ASSERT_EQ(ref, Color(0.0));
}

TEST(Chapter11Tests, TransparencyAndRefractiveIndexForTheDefaultMaterial)
{
	PhongMaterial m = PhongMaterial();
	ASSERT_EQ(m.refraction.value(), Color(0.0));
	ASSERT_EQ(m.ior.value(), 1.0);
	ASSERT_EQ(m.refraction_roughness.value(), 0.0);
}

TEST(Chapter11Tests, AHelperForProducingASphereWithAGlassyMaterial)
{
	auto s = Sphere::GlassSphere();
	auto ms = std::dynamic_pointer_cast<PhongMaterial>(s->material);

	ASSERT_EQ(s->get_transform(), Matrix4::Identity());
	ASSERT_EQ(ms->refraction.value(), Color(1.0));
	ASSERT_EQ(ms->ior.value(), 1.5);
}

TEST(Chapter11Tests, Finding_n1_And_n2_AtVariousIntersections)
{
	auto expected_n1 = std::vector<double>({ 1.0, 1.5, 2.0, 2.5, 2.5, 1.5 });
	auto expected_n2 = std::vector<double>({ 1.5, 2.0, 2.5, 2.5, 1.5, 1.0 });

	auto A = Sphere::GlassSphere();
	A->set_name("A");
	A->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));
	auto A_mat = std::dynamic_pointer_cast<PhongMaterial>(A->material);
	A_mat->ior.set_value(1.5);

	auto B = Sphere::GlassSphere();
	B->set_name("B");
	B->set_transform(Matrix4::Translation(0.0, 0.0, -0.25));
	auto B_mat = std::dynamic_pointer_cast<PhongMaterial>(B->material);
	B_mat->ior.set_value(2.0);

	auto C = Sphere::GlassSphere();
	C->set_name("C");
	C->set_transform(Matrix4::Translation(0.0, 0.0, 0.25));
	auto C_mat = std::dynamic_pointer_cast<PhongMaterial>(C->material);
	C_mat->ior.set_value(2.5);

	Ray r = Ray(Tuple::Point(0.0, 0.0, -4.0), Tuple::Vector(0.0, 0.0, 1.0));

	Intersections xs = Intersections({
		Intersection(2.0, A),
		Intersection(2.75, B),
		Intersection(3.25, C),
		Intersection(4.75, B),
		Intersection(5.25, C),
		Intersection(6.0, A)
		});

	for (size_t i = 0; i < expected_n1.size(); i++)
	{
		IxComps comps = IxComps(xs[i], r, xs);

		EXPECT_EQ(comps.n1, expected_n1[i]) << "Index: " << i << ", IX: <" << xs[i].t_value << ", " << xs[i].object->get_name() << ">\n\n";
		EXPECT_EQ(comps.n2, expected_n2[i]) << "Index: " << i << ", IX: <" << xs[i].t_value << ", " << xs[i].object->get_name() << ">\n\n";
	}
}

TEST(Chapter11Tests, TheUnderPointIsOffsetBelowTheSurface)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));
	auto shape = Sphere::GlassSphere();

	shape->set_transform(Matrix4::Translation(0.0, 0.0, 1.0));
	Intersection i = Intersection(5.0, shape);

	Intersections xs = Intersections({ i });

	IxComps comps = IxComps(i, r, xs);

	ASSERT_GT(comps.under_point.z, EPSILON / 2);
	ASSERT_LT(comps.point.z, comps.under_point.z);
}

TEST(Chapter11Tests, TheRefractedColorWithAnOpaqueSurface)
{
	World w = World::Default();

	auto shape = w.get_primitives()[0];

	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));
	Intersections xs = Intersections({ Intersection(4.0, shape), Intersection(4.0, shape) });

	IxComps comps = IxComps(xs[0], r, xs);

	Color c = shape->material->refract(w, comps);

	ASSERT_EQ(c, Color(0.0));
}

// pg. 156
TEST(Chapter11Tests, TheRefractedColorAtTheMaximumRecursiveDepth)
{
	World w = World::Default();

	auto shape = w.get_primitives()[0];

	auto ms1 = std::dynamic_pointer_cast<PhongMaterial>(shape->material);
	ms1->refraction.set_value(Color(1.0));
	ms1->ior.set_value(1.5);
	ms1->transparent_shadows = false;
	ms1->use_schlick = false;

	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0), RAY_DEPTH_LIMIT);
	Intersections xs = Intersections({ Intersection(4.0, shape), Intersection(6.0, shape) });

	IxComps comps = IxComps(xs[0], r, xs);

	Color c = shape->material->refract(w, comps);

	ASSERT_EQ(c, Color(0.0));
}

TEST(Chapter11Tests, TheRefractedColorUnderTotalInternalReflection)
{
	World w = World::Default();

	auto shape = w.get_primitives()[0];

	auto ms1 = std::dynamic_pointer_cast<PhongMaterial>(shape->material);
	ms1->refraction.set_value(Color(1.0));
	ms1->ior.set_value(1.5);

	Ray r = Ray(Tuple::Point(0.0, 0.0, sqrt(2.0)/2.0), Tuple::Vector(0.0, 1.0, 0.0));
	Intersections xs = Intersections({ Intersection(-sqrt(2.0) / 2.0, shape), Intersection(sqrt(2.0) / 2.0, shape) });

	IxComps comps = IxComps(xs[1], r, xs);

	Color c = shape->material->refract(w, comps);

	ASSERT_EQ(c, Color(0.0));
}

TEST(Chapter11Tests, TheRefractColorWithARefractedRay)
{
	World w = World::Default();

	auto A = w.get_primitives()[0];

	auto A_mat = std::dynamic_pointer_cast<PhongMaterial>(A->material);
	A_mat->ambient.set_value(Color(1.0));
	A_mat->color.connect(std::make_shared<TestMap>());

	auto B = w.get_primitives()[1];

	auto B_mat = std::dynamic_pointer_cast<PhongMaterial>(B->material);
	B_mat->refraction.set_value(Color(1.0));
	B_mat->ior.set_value(1.5);

	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.1), Tuple::Vector(0.0, 1.0, 0.0));

	Intersections xs = Intersections({ 
		Intersection(-0.9899, A), 
		Intersection(-0.4899, B),
		Intersection(0.4899, B),
		Intersection(0.9899, A)
		});

	IxComps comps = IxComps(xs[2], r, xs);

	Color c = B->material->refract(w, comps);

	ASSERT_EQ(c, Color(0.0, 0.99888, 0.04725));
}

TEST(Chapter11Tests, shade_hit_FunctionWithATransparentMaterial)
{
	World w = World::Default();

	auto floor = std::make_shared<InfinitePlane>();
	floor->set_transform(Matrix4::Translation(0.0, -1.0, 0.0));
	w.add_object(floor);

	auto floor_mat = std::dynamic_pointer_cast<PhongMaterial>(floor->material);
	floor_mat->refraction.set_value(0.5);
	floor_mat->ior.set_value(1.5);
	floor_mat->transparent_shadows = false;
	floor_mat->use_schlick = false;

	auto ball = std::make_shared<Sphere>();
	ball->set_transform(Matrix4::Translation(0.0, -3.5, -0.5));
	w.add_object(ball);

	auto ball_mat = std::dynamic_pointer_cast<PhongMaterial>(ball->material);
	ball_mat->color.set_value(Color(1.0, 0.0, 0.0));
	ball_mat->ambient.set_value(Color(0.5));

	Ray r = Ray(Tuple::Point(0.0, 0.0, -3.0), Tuple::Vector(0.0, -sqrt(2.0)/2.0, sqrt(2.0) / 2.0));

	Intersections xs = Intersections({
		Intersection(sqrt(2.0), floor)
		});

	IxComps comps = IxComps(xs[0], r, xs);

	Color c = w.shade(comps).get_calculated_rgb();

	ASSERT_EQ(c, Color(0.93642, 0.68642, 0.68642));
}

TEST(Chapter11Tests, TheSchlickApproximationUnderTotalInternalReflection)
{
	auto shape = Sphere::GlassSphere();

	Ray r = Ray(Tuple::Point(0.0, 0.0, sqrt(2.0) / 2.0), Tuple::Vector(0.0, 1.0, 0.0));

	Intersections xs = Intersections({
		Intersection(-sqrt(2.0) / 2.0, shape),
		Intersection(sqrt(2.0) / 2.0, shape)
		});

	IxComps comps = IxComps(xs[1], r, xs);

	double reflectance = schlick(comps);

	ASSERT_TRUE(flt_cmp(reflectance, 1.0)) << reflectance;
}

TEST(Chapter11Tests, TheSchlickApproximationWithAPerpendicularViewingAngle)
{
	auto shape = Sphere::GlassSphere();

	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 1.0, 0.0));

	Intersections xs = Intersections({
		Intersection(-1.0, shape),
		Intersection(1.0, shape)
		});

	IxComps comps = IxComps(xs[1], r, xs);

	double reflectance = schlick(comps);

	ASSERT_TRUE(flt_cmp(reflectance, 0.04)) << reflectance;
}

TEST(Chapter11Tests, TheSchlickApproximationWithASmallAngleAndGreater_n2_Value)
{
	auto shape = Sphere::GlassSphere();

	Ray r = Ray(Tuple::Point(0.0, 0.99, -2.0), Tuple::Vector(0.0, 0.0, 1.0));

	Intersections xs = Intersections({
		Intersection(1.8589, shape)
		});

	IxComps comps = IxComps(xs[0], r, xs);

	double reflectance = schlick(comps);

	ASSERT_TRUE(flt_cmp(reflectance, 0.48873)) << reflectance;
}

TEST(Chapter11Tests, shade_hit_WithAReflectiveAndTransparentMaterial)
{
	World w = World::Default();

	auto floor = std::make_shared<InfinitePlane>();
	floor->set_transform(Matrix4::Translation(0.0, -1.0, 0.0));
	w.add_object(floor);

	auto floor_mat = std::dynamic_pointer_cast<PhongMaterial>(floor->material);
	floor_mat->reflection.set_value(Color(0.5));
	floor_mat->refraction.set_value(Color(0.5));
	floor_mat->transparent_shadows = false;
	floor_mat->ior.set_value(1.5);

	auto ball = std::make_shared<Sphere>();
	ball->set_transform(Matrix4::Translation(0.0, -3.5, -0.5));
	w.add_object(ball);

	auto ball_mat = std::dynamic_pointer_cast<PhongMaterial>(ball->material);
	ball_mat->color.set_value(Color(1.0, 0.0, 0.0));
	ball_mat->ambient.set_value(Color(0.5));

	Ray r = Ray(Tuple::Point(0.0, 0.0, -3.0), Tuple::Vector(0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));

	Intersections xs = Intersections({
		Intersection(sqrt(2.0), floor)
		});

	IxComps comps = IxComps(xs[0], r, xs);

	Color c = w.shade(comps).get_calculated_rgb();

	ASSERT_EQ(c, Color(0.93391, 0.69643, 0.69243));
}

// ------------------------------------------------------------------------
// Chapter 12 Cubes
// ------------------------------------------------------------------------

TEST(Chapter12Tests, ARayIntersectsACube)
{
	auto cube = std::make_shared<Cube>();

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(5.0, 0.5, 0.0),
		Tuple::Point(-5.0, 0.5, 0.0),
		Tuple::Point(0.5, 5.0, 0.0),
		Tuple::Point(0.5, -5.0, 0.0),
		Tuple::Point(0.5, 0.0, 5.0),
		Tuple::Point(0.5, 0.0, -5.0),
		Tuple::Point(0.0, 0.5, 0.0)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(-1.0, 0.0, 0.0),
		Tuple::Vector(1.0, 0.0, 0.0),
		Tuple::Vector(0.0, -1.0, 0.0),
		Tuple::Vector(0.0, 1.0, 0.0),
		Tuple::Vector(0.0, 0.0, -1.0),
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.0, 0.0, 1.0)
		});

	auto results = std::vector<std::vector<double>>({
		{4.0, 6.0},
		{4.0, 6.0},
		{4.0, 6.0},
		{4.0, 6.0},
		{4.0, 6.0},
		{4.0, 6.0},
		{-1.0, 1.0}
		});

	auto labels = std::vector<std::string>({
		"+x",
		"-x",
		"+y",
		"-y",
		"+z",
		"-z",
		"inside"
		});

	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i]);

		Intersections xs = intersect(r, cube);

		ASSERT_EQ(xs.size(), 2) << "Test: " << labels[i] << " Intersections: " << xs;
		EXPECT_TRUE(flt_cmp(xs[0].t_value, results[i][0])) << "Test: " << labels[i] << " " << xs[0].t_value << " == " << results[i][0];
		EXPECT_TRUE(flt_cmp(xs[1].t_value, results[i][1])) << "Test: " << labels[i] << " " << xs[1].t_value << " == " << results[i][1];
	}
}

TEST(Chapter12Tests, ARayMissesACube)
{
	auto cube = std::make_shared<Cube>();

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(-2.0, 0.0, 0.0),
		Tuple::Point(0.0, -2.0, 0.0),
		Tuple::Point(0.0, 0.0, -2.0),
		Tuple::Point(2.0, 0.0, 2.0),
		Tuple::Point(0.0, 2.0, 2.0),
		Tuple::Point(2.0, 2.0, 0.0)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(0.2673, 0.5345, 0.8018),
		Tuple::Vector(0.8018, 0.2673, 0.5345),
		Tuple::Vector(0.5345, 0.8018, 0.2673),
		Tuple::Vector(0.0, 0.0, -1.0),
		Tuple::Vector(0.0, -1.0, 0.0),
		Tuple::Vector(-1.0, 0.0, 0.0)
		});

	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i]);

		Intersections xs = intersect(r, cube);

		EXPECT_EQ(xs.size(), 0) << " Intersections: " << xs;
	}
}

TEST(Chapter12Tests, TheNormalOnTheSurfaceOfACube)
{
	auto cube = std::make_shared<Cube>();

	auto points = std::vector<Tuple>({
		Tuple::Point(1.0, 0.5, -0.8),
		Tuple::Point(-1.0, -0.2, 0.9),
		Tuple::Point(-0.4, 1.0, -0.1),
		Tuple::Point(0.3, -1.0, -0.7),
		Tuple::Point(-0.6, 0.3, 1.0),
		Tuple::Point(0.4, 0.4, -1.0),
		Tuple::Point(1.0, 1.0, 1.0),
		Tuple::Point(-1.0, -1.0, -1.0)
		});

	auto results = std::vector<Tuple>({
		Tuple::Vector(1.0, 0.0, 0.0),
		Tuple::Vector(-1.0, 0.0, 0.0),
		Tuple::Vector(0.0, 1.0, 0.0),
		Tuple::Vector(0.0, -1.0, 0.0),
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.0, 0.0, -1.0),
		Tuple::Vector(1.0, 0.0, 0.0),
		Tuple::Vector(-1.0, 0.0, 0.0)
		});

	for (size_t i = 0; i < points.size(); i++)
	{
		Tuple normal = cube->get_definition()->local_normal_at(points[i]);

		EXPECT_EQ(normal, results[i]) << "Normal At [" << i << "] - Point: " << points[i] << " Expected: " << results[i] << " Actual: " << normal;
	}
}

// ------------------------------------------------------------------------
// Chapter 13 Cylinders
// ------------------------------------------------------------------------


TEST(Chapter13Tests, ARayMissesACylinder)
{
	auto cyl = std::make_shared<Cylinder>();

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(1.0, 0.0, 0.0),
		Tuple::Point(0.0, 0.0, 0.0),
		Tuple::Point(0.0, 0.0, -5.0)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(0.0, 1.0, 0.0),
		Tuple::Vector(0.0, 1.0, 0.0),
		Tuple::Vector(1.0, 1.0, 1.0)
		});

	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i].normalize());

		Intersections xs = intersect(r, cyl);

		EXPECT_EQ(xs.size(), 0) << " Intersections: " << xs;
	}
}

TEST(Chapter13Tests, ARayStrikesACylinder)
{
	auto cyl = std::make_shared<Cylinder>();

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(1.0, 0.0, -5.0),
		Tuple::Point(0.0, 0.0, -5.0),
		Tuple::Point(0.5, 0.0, -5.0)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.1, 1.0, 1.0)
		});

	auto results = std::vector<std::vector<double>>({
		{5.0, 5.0},
		{4.0, 6.0},
		{6.80798, 7.08872}
		});

	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i].normalize());

		Intersections xs = intersect(r, cyl);

		ASSERT_EQ(xs.size(), 2) << " Intersections: " << xs;
		EXPECT_TRUE(flt_cmp(xs[0].t_value, results[i][0])) << "Test: " << xs[0].t_value << " == " << results[i][0];
		EXPECT_TRUE(flt_cmp(xs[1].t_value, results[i][1])) << "Test: " << xs[1].t_value << " == " << results[i][1];
	}
}

TEST(Chapter13Tests, NormalVectorOnACylinder)
{
	auto cyl = std::make_shared<Cylinder>();

	auto points = std::vector<Tuple>({
		Tuple::Point(1.0, 0.0, 0.0),
		Tuple::Point(0.0, 5.0, -1.0),
		Tuple::Point(0.0, -2.0, 1.0),
		Tuple::Point(-1.0, 1.0, 0.0)
		});

	auto results = std::vector<Tuple>({
		Tuple::Vector(1.0, 0.0, 0.0),
		Tuple::Vector(0.0, 0.0, -1.0),
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(-1.0, 0.0, 0.0)
		});

	for (size_t i = 0; i < points.size(); i++)
	{
		Tuple normal = cyl->get_definition()->local_normal_at(points[i]);

		EXPECT_EQ(normal, results[i]) << "Normal At [" << i << "] - Point: " << points[i] << " Expected: " << results[i] << " Actual: " << normal;
	}
}

TEST(Chapter13Tests, TheDefaultMinimumAndMaximumForACylinder)
{
	auto cyl = std::make_shared<Cylinder>();

	ASSERT_EQ(cyl->get_minimum(), -std::numeric_limits<double>::infinity());
	ASSERT_EQ(cyl->get_maximum(), std::numeric_limits<double>::infinity());
}

TEST(Chapter13Tests, IntersectingAConstrainedCylinder)
{
	auto cyl = std::make_shared<Cylinder>(1.0, 2.0);

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(0.0, 1.5, 0.0),
		Tuple::Point(0.0, 3.0, -5.0),
		Tuple::Point(0.0, 0.0, -5.0),
		Tuple::Point(0.0, 2.0, -5.0),
		Tuple::Point(0.0, 1.0, -5.0),
		Tuple::Point(0.0, 1.5, -2.0)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(0.1, 1.0, 0.0),
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.0, 0.0, 1.0)
		});

	auto results = std::vector<size_t>({ 0, 0, 0, 0, 0, 2 });

	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i].normalize());

		Intersections xs = intersect(r, cyl);

		EXPECT_EQ(xs.size(), results[i]) << "Number: " << i + 1 << " Intersections: " << xs;
	}
}

TEST(Chapter13Tests, TheDefaultClosedValueForACylinder)
{
	auto cyl = std::make_shared<Cylinder>();

	ASSERT_FALSE(cyl->get_closed());
}

TEST(Chapter13Tests, IntersectingTheCapsOfAClosedCylinder)
{
	auto cyl = std::make_shared<Cylinder>(1.0, 2.0);
	cyl->set_closed(true);

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(0.0, 3.0, 0.0),
		Tuple::Point(0.0, 3.0, -2.0),
		Tuple::Point(0.0, 4.0, -2.0),
		Tuple::Point(0.0, 0.0, -2.0),
		Tuple::Point(0.0, -1.0, -2.0)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(0.0, -1.0, 0.0),
		Tuple::Vector(0.0, -1.0, 2.0),
		Tuple::Vector(0.0, -1.0, 1.0),
		Tuple::Vector(0.0, 1.0, 2.0),
		Tuple::Vector(0.0, 1.0, 1.0)
		});

	auto results = std::vector<size_t>({ 2, 2, 2, 2, 2 });

	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i].normalize());

		Intersections xs = intersect(r, cyl);

		EXPECT_EQ(xs.size(), results[i]) << "Number: " << i + 1 << " Intersections: " << xs;
	}
}

TEST(Chapter13Tests, TheNormalVectorOnACylindersEndCaps)
{
	auto cyl = std::make_shared<Cylinder>(1.0, 2.0);
	cyl->set_closed(true);

	auto points = std::vector<Tuple>({
		Tuple::Point(0.0, 1.0, 0.0),
		Tuple::Point(0.5, 1.0, 0.0),
		Tuple::Point(0.0, 1.0, 0.5),
		Tuple::Point(0.0, 2.0, 0.0),
		Tuple::Point(0.5, 2.0, 0.0),
		Tuple::Point(0.0, 2.0, 0.5)
		});

	auto results = std::vector<Tuple>({
		Tuple::Vector(0.0, -1.0, 0.0),
		Tuple::Vector(0.0, -1.0, 0.0),
		Tuple::Vector(0.0, -1.0, 0.0),
		Tuple::Vector(0.0, 1.0, 0.0),
		Tuple::Vector(0.0, 1.0, 0.0),
		Tuple::Vector(0.0, 1.0, 0.0)
		});

	for (size_t i = 0; i < points.size(); i++)
	{
		Tuple normal = cyl->get_definition()->local_normal_at(points[i]);

		EXPECT_EQ(normal, results[i]) << "Normal At [" << i << "] - Point: " << points[i] << " Expected: " << results[i] << " Actual: " << normal;
	}
}

TEST(Chapter13Tests, IntersectingAConeWithARay)
{
	auto cone = std::make_shared<DoubleNappedCone>();

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(0.0, 0.0, -5.0),
		Tuple::Point(0.0, 0.0, -5.0),
		Tuple::Point(1.0, 1.0, -5.0)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(1.0, 1.0, 1.0),
		Tuple::Vector(-0.5, -1.0, 1.0)
		});

	auto results = std::vector<std::vector<double>>({
		{5.0, 5.0},
		{8.66025, 8.66025},
		{4.55006, 49.44994}
		});

	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i].normalize());

		Intersections xs = intersect(r, cone);

		ASSERT_EQ(xs.size(), 2) << " Intersections: " << xs;
		EXPECT_TRUE(flt_cmp(xs[0].t_value, results[i][0])) << "Test " << i + 1 << ": " << xs[0].t_value << " == " << results[i][0];
		EXPECT_TRUE(flt_cmp(xs[1].t_value, results[i][1])) << "Test " << i + 1 << ": " << xs[1].t_value << " == " << results[i][1];
	}
}

TEST(Chapter13Tests, IntersectingAConeWithARayParralelToOneOfItsHalves)
{
	auto cone = std::make_shared<DoubleNappedCone>();


	Ray r = Ray(Tuple::Point(0.0, 0.0, -1.0), Tuple::Vector(0.0, 1.0, 1.0).normalize());

	Intersections xs = intersect(r, cone);

	ASSERT_EQ(xs.size(), 1) << " Intersections: " << xs;
	ASSERT_TRUE(flt_cmp(xs[0].t_value, 0.35355)) << xs[0].t_value << " == " << 0.35355;
}

TEST(Chapter13Tests, IntersectingAConesEndCaps)
{
	auto cone = std::make_shared<DoubleNappedCone>(-0.5, 0.5);
	cone->set_closed(true);

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(0.0, 0.0, -5.0),
		Tuple::Point(0.0, 0.0, -0.25),
		Tuple::Point(0.0, 0.0, -0.25)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(0.0, 1.0, 0.0),
		Tuple::Vector(0.0, 1.0, 1.0),
		Tuple::Vector(0.0, 1.0, 0.0)
		});

	auto results = std::vector<size_t>({ 0, 2, 4 });

	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i].normalize());

		Intersections xs = intersect(r, cone);

		EXPECT_EQ(xs.size(), results[i]) << "Number: " << i + 1 << " Intersections: " << xs;
	}
}

TEST(Chapter13Tests, ComputingTheNormalVectorOnACone)
{
	auto cyl = std::make_shared<DoubleNappedCone>();

	auto points = std::vector<Tuple>({
		Tuple::Point(0.0, 0.0, 0.0),
		Tuple::Point(1.0, 1.0, 1.0),
		Tuple::Point(-1.0, -1.0, 0.0)
		});

	auto results = std::vector<Tuple>({
		Tuple::Vector(0.0, 0.0, 0.0),
		Tuple::Vector(1.0, -sqrt(2.0), 1.0),
		Tuple::Vector(-1.0, 1.0, 0.0)
		});

	for (size_t i = 0; i < points.size(); i++)
	{
		Tuple normal = cyl->get_definition()->local_normal_at(points[i]);

		EXPECT_EQ(normal, results[i]) << "Normal At [" << i << "] - Point: " << points[i] << " Expected: " << results[i] << " Actual: " << normal;
	}
}

// ------------------------------------------------------------------------
// Chapter 14 Groups
// ------------------------------------------------------------------------

TEST(Chapter14Tests, CreatingANewGroup)
{
	auto grp = std::make_shared<Group>();

	ASSERT_EQ(grp->get_transform(), Matrix4::Identity());
	ASSERT_EQ(grp->num_children(), 0);
}

TEST(Chapter14Tests, AShapeHasAParentAttribute)
{
	auto s = std::make_shared<TestShape>();

	ASSERT_EQ(s->get_parent(), nullptr);
}

TEST(Chapter14Tests, AddingAChildToTheGroup)
{
	auto grp = std::make_shared<Group>();
	auto s = std::make_shared<TestShape>();

	grp->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ s }));
	ASSERT_EQ(grp->num_children(), 1);
	ASSERT_EQ(grp->get_children()[0], s);
	ASSERT_EQ(s->get_parent(), grp);
}

TEST(Chapter14Tests, IntersectingARayWithAnEmptyGroup)
{
	auto grp = std::make_shared<Group>();

	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 0.0, 1.0));

	Intersections xs = intersect(r, grp);

	ASSERT_EQ(xs.size(), 0);
}

TEST(Chapter14Tests, IntersectingARayWithANonEmptyGroup)
{
	auto grp = std::make_shared<Group>();

	auto s1 = std::make_shared<Sphere>();
	auto s2 = std::make_shared<Sphere>();
	s2->set_transform(Matrix4::Translation(0.0, 0.0, -3.0));
	auto s3 = std::make_shared<Sphere>();
	s3->set_transform(Matrix4::Translation(5.0, 0.0, 0.0));

	grp->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ s1, s2, s3 }));

	Ray r = Ray(Tuple::Point(0.0, 0.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Intersections xs = grp->intersect_i(r);

	ASSERT_EQ(xs.size(), 4);
	EXPECT_EQ(xs[0].object, s2);
	EXPECT_EQ(xs[1].object, s2);
	EXPECT_EQ(xs[2].object, s1);
	EXPECT_EQ(xs[3].object, s1);
}

TEST(Chapter14Tests, IntersectingATransformedGroup)
{
	auto grp = std::make_shared<Group>();
	grp->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));

	auto s = std::make_shared<Sphere>();
	s->set_transform(Matrix4::Translation(5.0, 0.0, 0.0));

	grp->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ s }));

	Ray r = Ray(Tuple::Point(10.0, 0.0, -1.0), Tuple::Vector(0.0, 0.0, 1.0));

	Intersections xs = grp->intersect_i(r);

	ASSERT_EQ(xs.size(), 2);
}

TEST(Chapter14Tests, ConvertingAPointFromWorldToObjectSpace)
{
	auto g1 = std::make_shared<Group>();
	g1->set_transform(Matrix4::Rotation_Y(M_PI / 2.0));

	auto g2 = std::make_shared<Group>();
	g2->set_transform(Matrix4::Scaling(2.0, 2.0, 2.0));

	auto s = std::make_shared<Sphere>();
	s->set_transform(Matrix4::Translation(5.0, 0.0, 0.0));

	g1->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ g2 }));
	g2->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ s }));

	Tuple p = s->point_to_object_space(Tuple::Point(-2.0, 0.0, -10.0));

	ASSERT_EQ(p, Tuple::Point(0.0, 0.0, -1.0));
}

TEST(Chapter14Tests, ConvertingANormalFromObjectToWorldSpace)
{
	auto g1 = std::make_shared<Group>();
	g1->set_transform(Matrix4::Rotation_Y(M_PI / 2.0));
	g1->set_name("g1");

	auto g2 = std::make_shared<Group>();
	g2->set_transform(Matrix4::Scaling(1.0, 2.0, 3.0));
	g2->set_name("g2");

	auto s = std::make_shared<Sphere>();
	s->set_transform(Matrix4::Translation(5.0, 0.0, 0.0));
	s->set_name("s");

	g1->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ g2 }));
	g2->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ s }));

	double x = sqrt(3.0) / 3.0;

	Tuple v = s->normal_vector_to_world_space(Tuple::Vector(x, x, x));

	ASSERT_EQ(v.normalize(), Tuple::Vector(0.2857, 0.4286, -0.8571));
}

TEST(Chapter14Tests, FindingTheNormalOnAChildObject)
{
	auto g1 = std::make_shared<Group>();
	g1->set_transform(Matrix4::Rotation_Y(M_PI / 2.0));

	auto g2 = std::make_shared<Group>();
	g2->set_transform(Matrix4::Scaling(1.0, 2.0, 3.0));

	auto s = std::make_shared<Sphere>();
	s->set_transform(Matrix4::Translation(5.0, 0.0, 0.0));

	g1->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ g2 }));
	g2->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ s }));

	Tuple n = s->normal_at(Tuple::Point(1.7321, 1.1547, -5.5774));

	ASSERT_EQ(n, Tuple::Vector(0.2857, 0.4286, -0.8571));
}

TEST(Chapter14Tests, GettingAGroupedObjectsWorldTransformationMatrix)
{
	auto g1 = std::make_shared<Group>();
	g1->set_transform(Matrix4::Rotation_Y(M_PI / 2.0));

	auto g2 = std::make_shared<Group>();
	g2->set_transform(Matrix4::Scaling(1.0, 2.0, 3.0));

	auto s = std::make_shared<Sphere>();
	s->set_transform(Matrix4::Translation(5.0, 0.0, 0.0));

	g1->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ g2 }));
	g2->parent_children(std::vector<std::shared_ptr<ObjectBase>>({ s }));

	Matrix4 x = s->get_world_transform();

	ASSERT_EQ(x, Matrix4::Translation(5.0, 0.0, 0.0) * Matrix4::Scaling(1.0, 2.0, 3.0) * Matrix4::Rotation_Y(M_PI / 2.0));
}

TEST(Chapter14Tests, ABoundingBoxHasDefaultDimensions)
{
	BoundingBox box = BoundingBox();

	ASSERT_EQ(box.minimum, Tuple::Point(-1.0, -1.0, -1.0));
	ASSERT_EQ(box.maximum, Tuple::Point(1.0, 1.0, 1.0));
}

TEST(Chapter14Tests, RaysIntersectABoundingBox)
{
	BoundingBox box = BoundingBox(Tuple::Point(-1.0, -1.0, -1.0), Tuple::Point(1.0, 1.0, 1.0));

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(0.0, 0.0, -5.0),
		Tuple::Point(0.0, 0.0, 5.0),
		Tuple::Point(0.0, -5.0, 0.0),
		Tuple::Point(0.0, 5.0, 0.0),
		Tuple::Point(-5.0, 0.0, 0.0),
		Tuple::Point(5.0, 0.0, 0.0),
		Tuple::Point(-5.0, -5.0, 0.0),
		Tuple::Point(0.0, -5.0, -5.0),
		Tuple::Point(-5.0, 0.0, -5.0)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.0, 0.0, -1.0),
		Tuple::Vector(0.0, 1.0, 0.0),
		Tuple::Vector(0.0, -1.0, 0.0),
		Tuple::Vector(1.0, 0.0, 0.0),
		Tuple::Vector(-1.0, 0.0, 0.0),
		Tuple::Vector(1.0, 1.0, 0.0),
		Tuple::Vector(0.0, 1.0, 1.0),
		Tuple::Vector(1.0, 0.0, 1.0)
		});


	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i].normalize());

		EXPECT_TRUE(box.intersect(r)) << "Test: " << i + 1 << " - " << r.origin << " --> " << r.direction << std::endl;
	}
}

TEST(Chapter14Tests, RaysMissABoundingBox)
{
    BoundingBox box = BoundingBox(Tuple::Point(-1.0, -1.0, -1.0), Tuple::Point(1.0, 1.0, 1.0));

	auto ray_origins = std::vector<Tuple>({
		Tuple::Point(1.1, 1.0, -5.0),
		Tuple::Point(0.0, 0.0, -5.0),
		Tuple::Point(0.0, 0.0, -5.0),
		Tuple::Point(0.0, 5.0, 0.0)
		});

	auto ray_directions = std::vector<Tuple>({
		Tuple::Vector(0.0, 0.0, 1.0),
		Tuple::Vector(0.0, 0.3, 1.0),
		Tuple::Vector(0.3, 0.0, -1.0),
		Tuple::Vector(0.0, 0.0, 1.0)
		});


	for (size_t i = 0; i < ray_origins.size(); i++)
	{
		Ray r = Ray(ray_origins[i], ray_directions[i].normalize());

		EXPECT_FALSE(box.intersect(r)) << "Test: " << i + 1 << " - " << r.origin << " --> " << r.direction << " inverse: " << r.dir_mult_inv << std::endl;
	}
}

// ------------------------------------------------------------------------
// Quadtrees
// ------------------------------------------------------------------------

TEST(Quadtrees, CreatingAQuadtree)
{
	QuadBranch<Sample> qt = QuadBranch<Sample>(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0), 7);
}

TEST(Quadtrees, InsertingAPoint)
{
	Sample s = Sample();
	s.set_rgb(Color(0.5, 0.5, 1.0));
	auto qt = std::make_shared<QuadBranch<Sample>>(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0), 7);

	auto qn = std::make_shared<QuadNode<Sample>>(Tuple::Point2D(0.5, 0.5), s);

	qt->insert(qn);

	std::vector<std::shared_ptr<QuadNode<Sample>>> vec = qt->get_child_nodes();

	std::cout << vec.size() << std::endl;
	std::cout << typeid(vec.front()).name() << std::endl;

	Sample result = vec[0]->get_data();

	std::cout << typeid(result).name() << std::endl;
	std::cout << result.get_current_rgb() << std::endl;

	ASSERT_EQ(result.get_calculated_rgb(), s.get_calculated_rgb());
}

TEST(Quadtrees, InsertingManyPoints)
{

	auto qt = std::make_shared<QuadBranch<Sample>>(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0), 7);

	for (size_t i = 0; i < 200; i++)
	{
		std::cout << std::endl << "Inserting Point #" << int(i) << std::endl;
		Sample s = Sample();
		s.name = "Sample #" + std::to_string(i);
		s.set_rgb(Color(random_double(0.0, 1.0), random_double(0.0, 1.0), random_double(0.0, 1.0)));

		auto qn = std::make_shared<QuadNode<Sample>>(Tuple::Point2D(random_double(0.0, 1.0), random_double(0.0, 1.0)), s);

		qt->insert(qn);
	}

	std::cout << "Insert Done" << std::endl << std::endl;

	std::vector<std::shared_ptr<QuadNode<Sample>>> vec = qt->get_all_nodes();

	ASSERT_EQ(vec.size(), 200);
}

TEST(Quadtrees, QueryingARange)
{
	AABB2D bounds = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
	AABB2D check_range = AABB2D(Tuple::Point2D(0.2, 0.2), Tuple::Point2D(0.8, 0.8));
	auto qt = std::make_shared<QuadBranch<Sample>>(bounds, 7);

	Sample s1 = Sample();
	s1.name = "Sample Blue";
	s1.set_rgb(Color(0.0, 0.0, 1.0));

	Sample s2 = Sample(s1);
	Sample s3 = Sample(s1);
	Sample s4 = Sample(s1);

	Sample r = Sample();
	r.name = "Sample Red";
	r.set_rgb(Color(1.0, 0.0, 0.0));

	auto qn1 = std::make_shared<QuadNode<Sample>>(Tuple::Point2D(0.1, 0.1), s1);
	auto qn2 = std::make_shared<QuadNode<Sample>>(Tuple::Point2D(0.1, 0.9), s2);
	auto qn3 = std::make_shared<QuadNode<Sample>>(Tuple::Point2D(0.9, 0.1), s3);
	auto qn4 = std::make_shared<QuadNode<Sample>>(Tuple::Point2D(0.9, 0.9), s4);
	auto qn_r = std::make_shared<QuadNode<Sample>>(Tuple::Point2D(0.5, 0.5), r);

	qt->insert(qn1);
	qt->insert(qn2);
	qt->insert(qn3);
	qt->insert(qn4);
	qt->insert(qn_r);

	std::vector<std::shared_ptr<QuadNode<Sample>>> vec = qt->query_range(check_range);

	std::shared_ptr<QuadNode<Sample>> result = vec[0];

	ASSERT_EQ(vec.size(), 1);
	ASSERT_EQ(result->data.get_calculated_rgb(), Color(1.0, 0.0, 0.0));
}

// ------------------------------------------------------------------------
// Sampling
// ------------------------------------------------------------------------

TEST(Sampling, ADefaultSample)
{
	Sample s = Sample();

	ASSERT_EQ(s.get_calculated_rgb(), Color(0.0));
	ASSERT_EQ(s.Alpha, 0.0);
	ASSERT_EQ(s.Depth, 0.0);
	ASSERT_EQ(s.Background, Color(0.0));
	ASSERT_EQ(s.Normal, Color(0.0, 0.0, 0.0));
	ASSERT_EQ(s.Position, Color(0.0));
	ASSERT_EQ(s.Diffuse, Color(0.0));
	ASSERT_EQ(s.Specular, Color(0.0));
	ASSERT_EQ(s.Lighting, Color(0.0));
	ASSERT_EQ(s.GlobalIllumination, Color(0.0));
	ASSERT_EQ(s.Reflection, Color(0.0));
	ASSERT_EQ(s.ReflectionFilter, 0.0);
	ASSERT_EQ(s.Refraction, Color(0.0));
	ASSERT_EQ(s.RefractionFilter, 0.0);
}

TEST(Sampling, SettingSampleValues)
{
	Sample s = Sample();

	s.Alpha = 1.0;
	s.Depth = 33.567;
	s.Background = Color(0.1, 0.1, 0.7);
	s.Normal = Color(0.6, 0.4, 1.0);
	s.Position = Color(22.0, 11.0, 8.5);
	s.Diffuse = Color(0.2, 0.3, 0.4);
	s.Specular = Color(0.1, 0.12, 0.15);
	s.Lighting = Color(0.1, 0.05, 0.1);
	s.GlobalIllumination = Color(0.05, 0.06, 0.001);
	s.Reflection = Color(1.0, 1.0, 0.3);
	s.ReflectionFilter = 0.5;
	s.Refraction =Color(0.01, 0.01, 0.01);
	s.RefractionFilter =0.1;

	ASSERT_EQ(s.get_current_rgb(), Color(0.0));
    ASSERT_EQ(s.get_calculated_rgb(), Color(0.731, 0.754, 1.0414));
	ASSERT_EQ(s.Alpha, 1.0);
	ASSERT_EQ(s.Depth, 33.567);
	ASSERT_EQ(s.Background, Color(0.1, 0.1, 0.7));
	ASSERT_EQ(s.Normal, Color(0.6, 0.4, 1.0));
	ASSERT_EQ(s.Position, Color(22.0, 11.0, 8.5));
	ASSERT_EQ(s.Diffuse, Color(0.2, 0.3, 0.4));
	ASSERT_EQ(s.Specular, Color(0.1, 0.12, 0.15));
	ASSERT_EQ(s.Lighting, Color(0.1, 0.05, 0.1));
	ASSERT_EQ(s.GlobalIllumination, Color(0.05, 0.06, 0.001));
	ASSERT_EQ(s.Reflection, Color(1.0, 1.0, 0.3));
	ASSERT_EQ(s.ReflectionFilter, 0.5);
	ASSERT_EQ(s.Refraction, Color(0.01, 0.01, 0.01));
	ASSERT_EQ(s.RefractionFilter, 0.1);
}

TEST(Sampling, CalculatingSampleValues)
{
	Sample s = Sample();

	s.Alpha = 1.0;
	s.Depth = 33.567;
	s.Background = Color(0.1, 0.1, 0.7);
	s.Normal = Color(0.6, 0.4, 1.0);
	s.Position = Color(22.0, 11.0, 8.5);
	s.Diffuse = Color(0.1, 0.2, 0.3);
	s.Specular = Color(0.1, 0.1, 0.1);
	s.Lighting = Color(0.1, 0.05, 0.1);
	s.GlobalIllumination = Color(0.05, 0.06, 0.001);
	s.Reflection = Color(1.0, 1.0, 0.3);
	s.ReflectionFilter = 0.5;
	s.Refraction =Color(0.01, 0.01, 0.01);
	s.RefractionFilter =0.1;

	s.calculate_sample();

	ASSERT_EQ(s.get_calculated_rgb(), Color(0.716, 0.723, 0.9813));
}

TEST(Sampling, AddingSamples)
{
	Sample s1 = Sample();
	Sample result = Sample();

	s1.Alpha = 0.5;
	s1.Depth = 10.0;
	s1.Background = Color(0.5, 0.5, 0.5);
	s1.Normal = Color(0.6, 0.4, 1.0);
	s1.Position = Color(10.0, 10.0, 10.0);
	s1.Diffuse = Color(0.4, 0.0, 0.8);
	s1.Specular = Color(0.1, 0.1, 0.1);
	s1.Lighting = Color(0.5, 0.5, 0.5);
	s1.GlobalIllumination = Color(0.2, 0.2, 0.2);
	s1.Reflection = Color(1.0, 1.0, 1.0);
	s1.ReflectionFilter = 0.5;
	s1.Refraction =Color(0.1, 0.1, 0.1);
	s1.RefractionFilter =0.1;

	s1.calculate_sample();

	Sample s2 = Sample(s1);

	result = s1 + s2;

	ASSERT_EQ(result.get_calculated_rgb(), Color(1.39 + 1.39, 1.11 + 1.11, 1.67 + 1.67));
	ASSERT_EQ(result.Alpha, 1.0);
	ASSERT_EQ(result.Depth, 20.0);
	ASSERT_EQ(result.Background, Color(1.0, 1.0, 1.0));
	ASSERT_EQ(result.Normal, Color(1.2, 0.8, 2.0));
	ASSERT_EQ(result.Position, Color(20.0, 20.0, 20.0));
	ASSERT_EQ(result.Diffuse, Color(0.8, 0.0, 1.6));
	ASSERT_EQ(result.Specular, Color(0.2, 0.2, 0.2));
	ASSERT_EQ(result.Lighting, Color(1.0, 1.0, 1.0));
	ASSERT_EQ(result.GlobalIllumination, Color(0.4, 0.4, 0.4));
	ASSERT_EQ(result.Reflection, Color(2.0, 2.0, 2.0));
	ASSERT_EQ(result.ReflectionFilter, 1.0);
	ASSERT_EQ(result.Refraction, Color(0.2, 0.2, 0.2));
	ASSERT_EQ(result.RefractionFilter, 0.2);
}

TEST(Sampling, dividingSamplesByAScalar)
{
	Sample s1 = Sample();
	Sample result = Sample();

	s1.Alpha = 0.5;
	s1.Depth = 10.0;
	s1.Background = Color(0.5, 0.5, 0.5);
	s1.Normal = Color(0.6, 0.4, 1.0);
	s1.Position = Color(10.0, 10.0, 10.0);
	s1.Diffuse = Color(0.4, 0.0, 0.8);
	s1.Specular = Color(0.1, 0.1, 0.1);
	s1.Lighting = Color(0.5, 0.5, 0.5);
	s1.GlobalIllumination = Color(0.2, 0.2, 0.2);
	s1.Reflection = Color(1.0, 1.0, 1.0);
	s1.ReflectionFilter = 0.5;
	s1.Refraction =Color(0.1, 0.1, 0.1);
	s1.RefractionFilter =0.1;

	s1.calculate_sample();

	//std::cout << s1.get_calculated_rgb() << std::endl;

	result = s1 / 2.0;

	ASSERT_EQ(result.get_calculated_rgb(), Color(1.39 / 2.0, 1.11 / 2.0, 1.67 / 2.0));
	ASSERT_EQ(result.Alpha, 0.25);
	ASSERT_EQ(result.Depth, 5.0);
	ASSERT_EQ(result.Background, Color(0.25, 0.25, 0.25));
	ASSERT_EQ(result.Normal, Color(0.3, 0.2, 0.5));
	ASSERT_EQ(result.Position, Color(5.0, 5.0, 5.0));
	ASSERT_EQ(result.Diffuse, Color(0.2, 0.0, 0.4));
	ASSERT_EQ(result.Specular, Color(0.05, 0.05, 0.05));
	ASSERT_EQ(result.Lighting, Color(0.25, 0.25, 0.25));
	ASSERT_EQ(result.GlobalIllumination, Color(0.1, 0.1, 0.1));
	ASSERT_EQ(result.Reflection, Color(0.5, 0.5, 0.5));
	ASSERT_EQ(result.ReflectionFilter, 0.25);
	ASSERT_EQ(result.Refraction, Color(0.05, 0.05, 0.05));
	ASSERT_EQ(result.RefractionFilter, 0.05);
}

TEST(Sampling, multiplyingSamplesByAScalar)
{
	Sample s1 = Sample();
	Sample result = Sample();

	s1.Alpha = 0.5;
	s1.Depth = 10.0;
	s1.Background = Color(0.5, 0.5, 0.5);
	s1.Normal = Color(0.6, 0.4, 1.0);
	s1.Position = Color(10.0, 10.0, 10.0);
	s1.Diffuse = Color(0.4, 0.0, 0.8);
	s1.Specular = Color(0.1, 0.1, 0.1);
	s1.Lighting = Color(0.5, 0.5, 0.5);
	s1.GlobalIllumination = Color(0.2, 0.2, 0.2);
	s1.Reflection = Color(1.0, 1.0, 1.0);
	s1.ReflectionFilter = 0.5;
	s1.Refraction =Color(0.1, 0.1, 0.1);
	s1.RefractionFilter =0.1;

	s1.calculate_sample();

	//std::cout << s1.get_calculated_rgb() << std::endl;

	result = s1 * 4.0;

	ASSERT_EQ(result.get_calculated_rgb(), Color(1.39 * 4.0, 1.11 * 4.0, 1.67 * 4.0));
	ASSERT_EQ(result.Alpha, 2.0);
	ASSERT_EQ(result.Depth, 40.0);
	ASSERT_EQ(result.Background, Color(2.0, 2.0, 2.0));
	ASSERT_EQ(result.Normal, Color(2.4, 1.6, 4.0));
	ASSERT_EQ(result.Position, Color(40.0, 40.0, 40.0));
	ASSERT_EQ(result.Diffuse, Color(1.6, 0.0, 3.2));
	ASSERT_EQ(result.Specular, Color(0.4, 0.4, 0.4));
	ASSERT_EQ(result.Lighting, Color(2.0, 2.0, 2.0));
	ASSERT_EQ(result.GlobalIllumination, Color(0.8, 0.8, 0.8));
	ASSERT_EQ(result.Reflection, Color(4.0, 4.0, 4.0));
	ASSERT_EQ(result.ReflectionFilter, 2.0);
	ASSERT_EQ(result.Refraction, Color(0.4, 0.4, 0.4));
	ASSERT_EQ(result.RefractionFilter, 0.4);
}

// ------------------------------------------------------------------------
// Sample Pixel
// ------------------------------------------------------------------------

TEST(SamplePixel, CreatingASamplePixel)
{
    auto px_1 = std::make_shared<SampledPixel>();

    Sample smp = Sample(Tuple::Point(0.5, 0.5, 0.0));

    px_1->add_sample(smp);

    std::vector<Sample> samples_vec = px_1->get_samples();

    ASSERT_EQ(samples_vec.size(), 1);
}

TEST(SamplePixel, CreatingASamplePixelWithData)
{
    Color gray = Color(0.5, 0.5, 0.5);

    auto px_1 = std::make_shared<SampledPixel>();

    Sample smp = Sample(Tuple::Point(0.5, 0.5, 0.0));
    smp.Background = gray;

    px_1->add_sample(smp);
    px_1->full_average();

    ASSERT_EQ(px_1->get_channel(background), gray);
}

TEST(SamplePixel, AveragingASamplePixel)
{
    auto px_1 = std::make_shared<SampledPixel>();

    Sample smp_1 = Sample(Tuple::Point(0.5, 0.5, 0.0));
    smp_1.Background = Color(1.0, 1.0, 0.5);

    Sample smp_2 = Sample(Tuple::Point(0.25, 0.25, 0.0));
    smp_2.Background = Color(1.0, 0.5, 0.25);

    Sample smp_3 = Sample(Tuple::Point(0.25, 0.75, 0.0));
    smp_3.Background = Color(0.5, 0.25, 1.0);

    Sample smp_4 = Sample(Tuple::Point(0.75, 0.25, 0.0));
    smp_4.Background = Color(0.25, 1.0, 1.0);

    Sample smp_5 = Sample(Tuple::Point(0.75, 0.75, 0.0));
    smp_5.Background = Color(1.0, 1.0, 1.0);

    px_1->add_sample(smp_1);
    px_1->add_sample(smp_2);
    px_1->add_sample(smp_3);
    px_1->add_sample(smp_4);
    px_1->add_sample(smp_5);
    px_1->full_average();

    ASSERT_EQ(px_1->get_channel(background), Color(0.75, 0.75, 0.75));
}

// ------------------------------------------------------------------------
// Sample Buffer
// ------------------------------------------------------------------------

TEST(SampleBuffer, CreatingABuffer)
{

    int width = 10;
    int height = 20;
    AABB2D extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
    SampleBuffer sb = SampleBuffer(width, height, extents);


    ASSERT_EQ(sb.width(), width);
    ASSERT_EQ(sb.height(), height);


    Color black = Color();

    for (const std::shared_ptr<SampledPixel> & i : sb)
    {
        ASSERT_EQ(i->get_channel(rgb), black);
    }
}

TEST(SampleBuffer, WritingPixelsToABuffer)
{
    int x = 2;
    int y = 3;


    AABB2D extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
    SampleBuffer sb = SampleBuffer(10, 20, extents);

    Color red = Color(1.0, 0.0, 0.0);
    Sample smp = Sample(sb.coordinates_from_pixel(x, y));
    smp.Diffuse = red;

    sb.write_sample(x, y, smp);

    sb.pixel_at(x, y)->full_average();

    ASSERT_EQ(sb.pixel_at(x, y)->get_samples().size(), 1);

//    for (const Sample & i:sb.pixel_at(x, y)->get_samples()) {
//        std::cout << "Sample: " << i << std::endl;
//    }

    ASSERT_EQ(sb.pixel_at(x, y)->get_channel(diffuse), red);
    ASSERT_EQ(sb.pixel_at(x, y)->get_channel(alpha), Color());
    ASSERT_EQ(sb.pixel_at(x + 1, y + 1)->get_channel(diffuse), Color());
}

TEST(SampleBuffer, PastingIntoABuffer)
{

    AABB2D sb_extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
    SampleBuffer sb = SampleBuffer(10, 20, sb_extents);

    AABB2D sb_small_extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(0.5, 0.5));
    SampleBuffer sb_small = SampleBuffer(5, 10, sb_small_extents);

    Color red = Color(1.0, 0.0, 0.0);

    int i = 0;
    for (const std::shared_ptr<SampledPixel> & p : sb_small)
    {
        Sample smp = Sample(sb_small.coordinates_from_index(i));
        smp.Diffuse = red;

        p->add_sample(smp);
        p->full_average();

        i++;
    }

    sb.write_portion(2, 2, sb_small);

    ASSERT_EQ(sb.pixel_at(2, 2)->get_channel(diffuse), red);
    ASSERT_EQ(sb.pixel_at(6, 11)->get_channel(diffuse), red);
    ASSERT_EQ(sb.pixel_at(4, 4)->get_channel(alpha), Color());
    ASSERT_EQ(sb.pixel_at(1, 1)->get_channel(diffuse), Color());
    ASSERT_EQ(sb.pixel_at(7, 12)->get_channel(diffuse), Color());
}

TEST(SampleBuffer, FillSolid)
{

    AABB2D sb_extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
    SampleBuffer sb = SampleBuffer(20, 20, sb_extents);

    Color red = Color(1.0, 0.0, 0.0);
    Color green = Color(0.0, 1.0, 0.0);
    Color blue = Color(0.0, 0.0, 1.0);

    Sample smp = Sample();

    smp.set_rgb(red);
    sb.fill_solid(smp);

    EXPECT_EQ(sb.pixel_at(10, 10)->get_channel(rgb), red);
    EXPECT_EQ(sb.pixel_at(5, 7)->get_channel(rgb), red);
    EXPECT_EQ(sb.pixel_at(15, 12)->get_channel(rgb), red);

    smp.set_rgb(green);
    sb.fill_solid(smp);

    EXPECT_EQ(sb.pixel_at(10, 10)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(5, 7)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(15, 12)->get_channel(rgb), green);

    smp.set_rgb(blue);
    sb.fill_solid(smp);

    EXPECT_EQ(sb.pixel_at(10, 10)->get_channel(rgb), blue);
    EXPECT_EQ(sb.pixel_at(5, 7)->get_channel(rgb), blue);
    EXPECT_EQ(sb.pixel_at(15, 12)->get_channel(rgb), blue);
}

TEST(SampleBuffer, StitchingBuckets)
{

    AABB2D sb_extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
    SampleBuffer sb = SampleBuffer(20, 20, sb_extents);

    AABB2D bk_01_extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(0.5, 0.5));
    SampleBuffer bk_01 = SampleBuffer(0,0, 10, 10, bk_01_extents);

    AABB2D bk_02_extents = AABB2D(Tuple::Point2D(0.5, 0.0), Tuple::Point2D(0.1, 0.5));
    SampleBuffer bk_02 = SampleBuffer(10, 0, 10, 10, bk_02_extents);

    AABB2D bk_03_extents = AABB2D(Tuple::Point2D(0.0, 0.5), Tuple::Point2D(0.5, 1.0));
    SampleBuffer bk_03 = SampleBuffer(0, 10, 10, 10, bk_03_extents);

    AABB2D bk_04_extents = AABB2D(Tuple::Point2D(0.5, 0.5), Tuple::Point2D(1.0, 1.0));
    SampleBuffer bk_04 = SampleBuffer(10, 10, 10, 10, bk_04_extents);

    Color red = Color(1.0, 0.0, 0.0);
    Color green = Color(0.0, 1.0, 0.0);
    Color blue = Color(0.0, 0.0, 1.0);
    Color yellow = Color(1.0, 1.0, 0.0);

    Sample smp = Sample();

    smp.set_rgb(red);
    bk_01.fill_solid(smp);

    smp.set_rgb(green);
    bk_02.fill_solid(smp);

    smp.set_rgb(blue);
    bk_03.fill_solid(smp);

    smp.set_rgb(yellow);
    bk_04.fill_solid(smp);

    sb.write_portion(bk_01);
    sb.write_portion(bk_02);
    sb.write_portion(bk_03);
    sb.write_portion(bk_04);

    EXPECT_EQ(sb.pixel_at(5, 5)->get_channel(rgb), red);
    EXPECT_EQ(sb.pixel_at(15, 5)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(5, 15)->get_channel(rgb), blue);
    EXPECT_EQ(sb.pixel_at(15, 15)->get_channel(rgb), yellow);
}

TEST(SampleBuffer, StitchingBucketsWithOverflow)
{
    AABB2D sb_extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
    SampleBuffer sb = SampleBuffer(20, 20, sb_extents);

    AABB2D bk_01_extents = AABB2D(Tuple::Point2D(0.5, 0.5), Tuple::Point2D(1.5, 1.5));
    SampleBuffer bk_01 = SampleBuffer(15, 0, 10, 10, bk_01_extents);

    Color red = Color(1.0, 0.0, 0.0);
    Color green = Color(0.0, 1.0, 0.0);

    Sample smp = Sample();

    smp.set_rgb(red);
    bk_01.fill_solid(smp);

    smp.set_rgb(green);
    sb.fill_solid(smp);

    sb.write_portion(bk_01);

    EXPECT_EQ(sb.pixel_at(14, 0)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(15, 0)->get_channel(rgb), red);

    EXPECT_EQ(sb.pixel_at(14, 9)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(14, 10)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(15, 9)->get_channel(rgb), red);
    EXPECT_EQ(sb.pixel_at(15, 10)->get_channel(rgb), green);

    EXPECT_EQ(sb.pixel_at(19, 9)->get_channel(rgb), red);
    EXPECT_EQ(sb.pixel_at(19, 10)->get_channel(rgb), green);
}

TEST(SampleBuffer, StitchingBucketsWithBottomOverflow)
{
    AABB2D sb_extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
    SampleBuffer sb = SampleBuffer(20, 20, sb_extents);
    size_t sb_size = sb.get_pixels().size();

    AABB2D bk_01_extents = AABB2D(Tuple::Point2D(0.5, 0.5), Tuple::Point2D(1.5, 1.5));
    SampleBuffer bk_01 = SampleBuffer(15, 15, 10, 10, bk_01_extents);

    Color red = Color(1.0, 0.0, 0.0);
    Color green = Color(0.0, 1.0, 0.0);

    Sample smp = Sample();

    smp.set_rgb(red);
    bk_01.fill_solid(smp);

    smp.set_rgb(green);
    sb.fill_solid(smp);

    sb.write_portion(bk_01);

//    std::string folder = R"(I:\projects\Raymond\frames\dump\)";
//    canvas_to_ppm(sb.to_canvas(rgb), folder + "StitchingBucketsWithBottomOverflow.ppm", true);

    EXPECT_EQ(sb.pixel_at(0, 0)->get_channel(rgb), green);

    EXPECT_EQ(sb.pixel_at(14, 14)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(14, 15)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(15, 14)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(15, 15)->get_channel(rgb), red);

    EXPECT_EQ(sb.pixel_at(14, 19)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(15, 19)->get_channel(rgb), red);

    EXPECT_EQ(sb.pixel_at(19, 14)->get_channel(rgb), green);
    EXPECT_EQ(sb.pixel_at(19, 15)->get_channel(rgb), red);

    EXPECT_EQ(sb.pixel_at(19, 19)->get_channel(rgb), red);

    ASSERT_EQ(sb_size, sb.get_pixels().size());
}

TEST(SampleBuffer, ToCanvas)
{
    int width = 10;
    int height = 20;
    AABB2D extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
    SampleBuffer sb = SampleBuffer(width, height, extents);

    Color black = Color();

    Canvas c = sb.to_canvas(rgb);

    for (const Color& i : c)
        EXPECT_EQ(i, black);
}

TEST(SampleBuffer, ToCanvasExtraChannel)
{
    int width = 10;
    int height = 20;
    AABB2D extents = AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0));
    SampleBuffer sb = SampleBuffer(width, height, extents);

    Color lavender = Color(0.5, 0.5, 1.0);

    int ind = 0;
    for (const std::shared_ptr<SampledPixel> & p : sb)
    {
        Sample smp = Sample(sb.coordinates_from_index(ind));
        smp.Diffuse = lavender;
        smp.Alpha = 0.5;

        p->add_sample(smp);
        p->full_average();

        ind++;
    }

    Canvas c_diff = sb.to_canvas(diffuse);
    Canvas c_alpha = sb.to_canvas(alpha);

    for (const Color& i : c_diff)
        EXPECT_EQ(i, lavender);

    for (const Color& i : c_alpha)
        EXPECT_EQ(i, Color(0.5));
}

// ------------------------------------------------------------------------
// Multisample Rendering
// ------------------------------------------------------------------------
