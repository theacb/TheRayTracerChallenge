#include "pch.h"

#include "math.h"
#include <sstream> 

#include "../Raymond/Tuple.h"
#include "../Raymond/Canvas.h"
#include "../Raymond/Color.h"
#include "../Raymond/Matrix.h"
#include "../Raymond/Ray.h"
#include "../Raymond/Object.h"
#include "../Raymond/Primitive.h"
#include "../Raymond/Light.h"
#include "../Raymond/Material.h"

// ------------------------------------------------------------------------
// Chapter 01 Tuples, Points, and Vectors
// ------------------------------------------------------------------------

TEST(Chapter01Tests, PointTuple) 
{
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

TEST(Chapter01Tests, VectporTuple) 
{
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

TEST(Chapter01Tests, PointFactory) 
{
	float x = 4.0f;
	float y = -4.0f;
	float z = 3.0f;
	float w = 1.0f;

	Tuple a = Tuple(x, y, z, w);
	Tuple p = Tuple::Point(x, y, z);

	ASSERT_EQ(a, p);
}

TEST(Chapter01Tests, VectorFactory) 
{
	float x = 4.0f;
	float y = -4.0f;
	float z = 3.0f;
	float w = 0.0f;

	Tuple a = Tuple(x, y, z, w);
	Tuple p = Tuple::Vector(x, y, z);

	ASSERT_EQ(a, p);
}

TEST(Chapter01Tests, AddingTuples) 
{
	Tuple a1 = Tuple(3.0f, -2.0f, 5.0f, 1.0f);
	Tuple a2 = Tuple(-2.0f, 3.0f, 1.0f, 0.0f);

	ASSERT_EQ(a1 + a2, Tuple(1.0f, 1.0f, 6.0f, 1.0f));
}

TEST(Chapter01Tests, SubtractingTwoPoints) 
{
	Tuple p1 = Tuple::Point(3.0f, 2.0f, 1.0f);
	Tuple p2 = Tuple::Point(5.0f, 6.0f, 7.0f);

	ASSERT_EQ(p1 - p2, Tuple::Vector(-2.0f, -4.0f, -6.0f));
}

TEST(Chapter01Tests, SubtractingAVectorFromAPoint) 
{
	Tuple p = Tuple::Point(3.0f, 2.0f, 1.0f);
	Tuple v = Tuple::Vector(5.0f, 6.0f, 7.0f);

	ASSERT_EQ(p - v, Tuple::Point(-2.0f, -4.0f, -6.0f));
}

TEST(Chapter01Tests, SubtractingTwoVectors) 
{
	Tuple v1 = Tuple::Vector(3.0f, 2.0f, 1.0f);
	Tuple v2 = Tuple::Vector(5.0f, 6.0f, 7.0f);

	ASSERT_EQ(v1 - v2, Tuple::Vector(-2.0f, -4.0f, -6.0f));
}

TEST(Chapter01Tests, SubtractingAZeroVector) 
{
	Tuple zero = Tuple::Vector(0.0f, 0.0f, 0.0f);
	Tuple v = Tuple::Vector(1.0f, -2.0f, 3.0f);

	ASSERT_EQ(zero - v, Tuple::Vector(-1.0f, 2.0f, -3.0f));
}

TEST(Chapter01Tests, NegatingATuple) 
{
	Tuple a = Tuple(1.0f, -2.0f, 3.0f, -4.0f);

	ASSERT_EQ(-a, Tuple(-1.0f, 2.0f, -3.0f, 4.0f));
}

TEST(Chapter01Tests, MultiplyingATupleByAScalar) 
{
	Tuple a = Tuple(1.0f, -2.0f, 3.0f, -4.0f);

	ASSERT_EQ(a * 3.5, Tuple(3.5f, -7.0f, 10.5f, -14.0f));
}

TEST(Chapter01Tests, MultiplyingATupleByAFraction) 
{
	Tuple a = Tuple(1.0f, -2.0f, 3.0f, -4.0f);

	ASSERT_EQ(a * 0.5, Tuple(0.5f, -1.0f, 1.5f, -2.0f));
}

TEST(Chapter01Tests, DividingATupleByAScalar) 
{
	Tuple a = Tuple(1.0f, -2.0f, 3.0f, -4.0f);

	ASSERT_EQ(a / 2, Tuple(0.5f, -1.0f, 1.5f, -2.0f));
}

TEST(Chapter01Tests, ComputingMagnitudeOfAVector) 
{
	Tuple v1 = Tuple::Vector(1.0f, 0.0f, 0.0f);
	ASSERT_TRUE(flt_cmp(v1.magnitude(), 1.0));

	Tuple v2 = Tuple::Vector(0.0f, 1.0f, 0.0f);
	ASSERT_TRUE(flt_cmp(v2.magnitude(), 1.0));

	Tuple v3 = Tuple::Vector(0.0f, 0.0f, 1.0f);
	ASSERT_TRUE(flt_cmp(v3.magnitude(), 1.0));

	Tuple v4 = Tuple::Vector(1.0f, 2.0f, 3.0f);
	ASSERT_TRUE(flt_cmp(v4.magnitude(), sqrt(14.0f)));

	Tuple v5 = Tuple::Vector(-1.0f, -2.0f, -3.0f);
	ASSERT_TRUE(flt_cmp(v5.magnitude(), sqrt(14.0f)));
}

TEST(Chapter01Tests, NormalizingAVector) 
{
	Tuple v1 = Tuple::Vector(4.0f, 0.0f, 0.0f);
	ASSERT_EQ(v1.normalize(), Tuple::Vector(1.0f, 0.0f, 0.0f));

	Tuple v2 = Tuple::Vector(1.0f, 2.0f, 3.0f);
	ASSERT_EQ(v2.normalize(), Tuple::Vector(0.26726f, 0.53452f, 0.80178f));
}

TEST(Chapter01Tests, ComputingMagnitudeOfANormalizedVector) 
{
	Tuple v = Tuple::Vector(1.0f, 2.0f, 3.0f);
	Tuple norm = v.normalize();
	ASSERT_TRUE(flt_cmp(norm.magnitude(), 1.0));
}

TEST(Chapter01Tests, DotProductOfTwoVectors) 
{
	Tuple v1 = Tuple::Vector(1.0f, 2.0f, 3.0f);
	Tuple v2 = Tuple::Vector(2.0f, 3.0f, 4.0f);

	ASSERT_TRUE(flt_cmp(Tuple::dot(v1, v2), 20.0));
}

TEST(Chapter01Tests, CrossProductOfTwoVectors) 
{
	Tuple a = Tuple::Vector(1.0f, 2.0f, 3.0f);
	Tuple b = Tuple::Vector(2.0f, 3.0f, 4.0f);

	ASSERT_EQ(Tuple::cross(a, b), Tuple::Vector(-1.0f, 2.0f, -1.0f));
	ASSERT_EQ(Tuple::cross(b, a), Tuple::Vector(1.0f, -2.0f, 1.0f));
}

// ------------------------------------------------------------------------
// Chapter 02 Drawing on a Canvas
// ------------------------------------------------------------------------

TEST(Chapter02Tests, CallingColorChannels) 
{
	float red = -0.5f;
	float green = 0.4f;
	float blue = 1.7f;

	Color c = Color(red, green, blue);

	ASSERT_EQ(c.r(), red);
	ASSERT_EQ(c.g(), green);
	ASSERT_EQ(c.b(), blue);
}

TEST(Chapter02Tests, AssigningColorChannels) 
{
	float red = -0.5f;
	float green = 0.4f;
	float blue = 1.7f;

	Color c = Color(0.0f, 0.0f, 0.0f);

	c.r() = red;
	c.g() = green;
	c.b() = blue;

	ASSERT_EQ(c, Color(red, green, blue));
}

TEST(Chapter02Tests, AddingColors) 
{
	Color c1 = Color(0.9f, 0.6f, 0.75f);
	Color c2 = Color(0.7f, 0.1f, 0.25f);

	ASSERT_EQ(c1 + c2, Color(1.6f, 0.7f, 1.0f));
}

TEST(Chapter02Tests, SubtractingColors) 
{
	Color c1 = Color(0.9f, 0.6f, 0.75f);
	Color c2 = Color(0.7f, 0.1f, 0.25f);

	ASSERT_EQ(c1 - c2, Color(0.2f, 0.5f, 0.5f));
}

TEST(Chapter02Tests, MultiplyingColorByAScalar) 
{
	Color c = Color(0.2f, 0.3f, 0.4f);

	ASSERT_EQ(c * 2.0f, Color(0.4f, 0.6f, 0.8f));
}

TEST(Chapter02Tests, MultiplyingColors) 
{
	Color c1 = Color(1.0f, 0.2f, 0.4f);
	Color c2 = Color(0.9f, 1.0f, 0.1f);

	ASSERT_EQ(c1.multiply(c2), Color(0.9f, 0.2f, 0.04f));
}

TEST(Chapter02Tests, ConvertingColorToInteger) 
{
	Color c = Color(1.0f, 0.2f, 0.4f);

	ASSERT_EQ(Color8Bit(c), Color8Bit(255, 51, 102));
}

TEST(Chapter02Tests, ConvertingOutOfRangeColorToInteger) 
{
	Color c = Color(-1.0f, 2.2f, 10.4f);

	ASSERT_EQ(Color8Bit(c), Color8Bit(0, 255, 255));
}

TEST(Chapter02Tests, ConvertingLinearColorToSRGB) 
{
	Color c = Color(0.5f, 0.5f, 1.0f);
	Color con = c.convert_linear_to_srgb();

	ASSERT_GT(con.magnitude(), Color(0.7f, 0.7f, 0.99f).magnitude());
}

TEST(Chapter02Tests, ConvertingSRGBToLinear) 
{
	Color c = Color(0.5f, 0.5f, 1.0f);
	Color con = c.convert_srgb_to_linear();

	ASSERT_LT(con.magnitude(), Color(0.3f, 0.3f, 1.0f).magnitude());
}

TEST(Chapter02Tests, CreatingACanvas) 
{
	int width = 10;
	int height = 20;
	Canvas c = Canvas(width, height);

	ASSERT_EQ(c.width(), width);
	ASSERT_EQ(c.height(), height);

	Color black = Color();

	for (Color i : c)
		ASSERT_EQ(i, black);
}

TEST(Chapter02Tests, WritingPixelsToACanvas) 
{
	int x = 2;
	int y = 3;

	Canvas c = Canvas(10, 20);

	Color red = Color(1.0f, 0.0f, 0.0f);

	c.write_pixel(x, y, red);

	ASSERT_EQ(c.pixel_at(x, y), red);
}

TEST(Chapter02Tests, ConstructingThePPMPixelData) 
{
	Canvas c = Canvas(5, 3);

	Color c1 = Color(1.5f, 0.0f, 0.0f);
	Color c2 = Color(0.0f, 0.5f, 0.0f);
	Color c3 = Color(-0.5f, 0.0f, 1.0f);

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

	Color c1 = Color(1.0f, 0.8f, 0.6f);

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

	std::string file_path = "E:\\dump\\projects\\Raymond\\frames\\PPMFilesTerminateWithNewline.ppm";

	canvas_to_ppm(c, file_path);

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
		1.0f, 2.0f, 3.0f, 4.0f,
		5.5f, 6.5f, 7.5f, 8.5f,
		9.0f, 10.0f, 11.0f, 12.0f,
		13.5f, 14.5f, 15.5f, 16.5f
		});

	ASSERT_TRUE(flt_cmp(m.get(0, 0), 1.0f));
	ASSERT_TRUE(flt_cmp(m.get(0, 3), 4.0f));
	ASSERT_TRUE(flt_cmp(m.get(1, 0), 5.5f));
	ASSERT_TRUE(flt_cmp(m.get(1, 2), 7.5f));
	ASSERT_TRUE(flt_cmp(m.get(2, 2), 11.0f));
	ASSERT_TRUE(flt_cmp(m.get(3, 0), 13.5f));
	ASSERT_TRUE(flt_cmp(m.get(3, 2), 15.5f));
}

TEST(Chapter03Tests, ConstructingAndInspectingATwoByTwoMatrix) 
{
	Matrix m = Matrix(2);
	m.set_multiple({
		-3.0f, 5.0f,
		1.0f, -2.0f
		});

	ASSERT_TRUE(flt_cmp(m.get(0, 0), -3.0f));
	ASSERT_TRUE(flt_cmp(m.get(0, 1), 5.0f));
	ASSERT_TRUE(flt_cmp(m.get(1, 0), 1.0f));
	ASSERT_TRUE(flt_cmp(m.get(1, 1), -2.0f));
}

TEST(Chapter03Tests, ConstructingAndInspectingAThreeByThreeMatrix) 
{
	Matrix m = Matrix(3);
	m.set_multiple({
		-3.0f, 5.0f, 0.0f,
		1.0f, -2.0f, -7.0f,
		0.0f, 1.0f, 1.0f
		});

	ASSERT_TRUE(flt_cmp(m.get(0, 0), -3.0f));
	ASSERT_TRUE(flt_cmp(m.get(1, 1), -2.0f));
	ASSERT_TRUE(flt_cmp(m.get(2, 2), 1.0f));
}

TEST(Chapter03Tests, MatrixSizeLimitations) 
{
	bool too_large_error_thrown = false;
	bool too_small_error_thrown = false;

	Matrix m = Matrix(2);
	try
	{
		m.set_multiple({
		-3.0f, 5.0f, 0.0f,
		1.0f, -2.0f, -7.0f,
		0.0f, 1.0f, 1.0f
			});
	}
	catch (std::out_of_range e)
	{
		too_large_error_thrown = true;
	}

	ASSERT_TRUE(too_large_error_thrown);

	try
	{
		m.set_multiple({
		-3.0f, 5.0f
			});
	}
	catch (std::out_of_range e)
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
	catch (std::out_of_range e)
	{
		x_error_thrown = true;
	}

	ASSERT_TRUE(x_error_thrown);

	try
	{
		m.get(10, 0);
	}
	catch (std::out_of_range e)
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
		1.0f, 2.0f, 3.0f, 4.0f,
		5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 8.0f, 7.0f, 6.0f,
		5.0f, 4.0f, 3.0f, 2.0f
		});

	m2.set_multiple({
		1.0f, 2.0f, 3.0f, 4.0f,
		5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 8.0f, 7.0f, 6.0f,
		5.0f, 4.0f, 3.0f, 2.0f
		});

	
	ASSERT_EQ(m1, m2);
}

TEST(Chapter03Tests, MatrixInequalityWithDifferentMatrices) 
{
	Matrix m1 = Matrix(4);
	Matrix m2 = Matrix(4);
	m1.set_multiple({
		1.0f, 2.0f, 3.0f, 4.0f,
		5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 8.0f, 7.0f, 6.0f,
		5.0f, 4.0f, 3.0f, 2.0f
		});

	m2.set_multiple({
		5.0f, 4.0f, 3.0f, 2.0f,
		9.0f, 8.0f, 7.0f, 6.0f,
		5.0f, 6.0f, 7.0f, 8.0f,
		1.0f, 2.0f, 3.0f, 4.0f
		});


	ASSERT_NE(m1, m2);
}

TEST(Chapter03Tests, MatrixInequalityWithDifferentSizedMatrices) 
{
	Matrix m1 = Matrix(4);
	Matrix m2 = Matrix(2);
	m1.set_multiple({
		1.0f, 2.0f, 3.0f, 4.0f,
		5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 8.0f, 7.0f, 6.0f,
		5.0f, 4.0f, 3.0f, 2.0f
		});

	m2.set_multiple({
		1.0f, 2.0f,
		3.0f, 4.0f
		});


	ASSERT_NE(m1, m2);
}

TEST(Chapter03Tests, Matrix4Multiplication) 
{
	Matrix4 m1 = Matrix4();
	Matrix4 m2 = Matrix4();
	Matrix4 result = Matrix4();

	m1.set_multiple({
		1.0f, 2.0f, 3.0f, 4.0f,
		5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 8.0f, 7.0f, 6.0f,
		5.0f, 4.0f, 3.0f, 2.0f
		});

	m2.set_multiple({
		-2.0f, 1.0f, 2.0f, 3.0f,
		3.0f, 2.0f, 1.0f, -1.0f,
		4.0f, 3.0f, 6.0f, 5.0f,
		1.0f, 2.0f, 7.0f, 8.0f
		});

	result.set_multiple({
		20.0f, 22.0f, 50.0f, 48.0f,
		44.0f, 54.0f, 114.0f, 108.0f,
		40.0f, 58.0f, 110.0f, 102.0f,
		16.0f, 26.0f, 46.0f, 42.0f
		});


	ASSERT_EQ((m1 * m2), result) << (m1 * m2);
}

TEST(Chapter03Tests, Matrix4MultupliedWithATuple) 
{
	Matrix4 m = Matrix4();

	m.set_multiple({
		1.0f, 2.0f, 3.0f, 4.0f,
		2.0f, 4.0f, 4.0f, 2.0f,
		8.0f, 6.0f, 4.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		});

	Tuple t = Tuple(1.0f, 2.0f, 3.0f, 1.0f);

	ASSERT_EQ((m * t), Tuple(18.0f, 24.0f, 33.0f, 1.0f));
}

TEST(Chapter03Tests, MultiplyingAMatrix4ByTheIdentityMatrix) 
{
	Matrix4 m = Matrix4();

	m.set_multiple({
		0.0f, 1.0f, 2.0f, 4.0f,
		1.0f, 2.0f, 4.0f, 8.0f,
		2.0f, 4.0f, 8.0f, 16.0f,
		4.0f, 8.0f, 16.0f, 32.0f
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
		0.0f, 9.0f, 3.0f, 0.0f,
		9.0f, 8.0f, 0.0f, 8.0f,
		1.0f, 8.0f, 5.0f, 3.0f,
		0.0f, 0.0f, 5.0f, 8.0f
		});

	Matrix4 result = Matrix4();

	result.set_multiple({
		0.0f, 9.0f, 1.0f, 0.0f,
		9.0f, 8.0f, 8.0f, 0.0f,
		3.0f, 0.0f, 5.0f, 5.0f,
		0.0f, 8.0f, 3.0f, 8.0f
		});

	ASSERT_EQ(m.transpose(), result);
}

TEST(Chapter03Tests, DeterminateOfAMatrix2) 
{
	Matrix2 m = Matrix2({
		1.0f, 5.0f,
		-3.0f, 2.0f
		});

	ASSERT_TRUE(flt_cmp(m.determinant(), 17.0f));
}

TEST(Chapter03Tests, ASubMatrixOfAMatrix4IsAMatrix3) 
{
	Matrix4 m = Matrix4({
		-6.0f, 1.0f, 1.0f, 6.0f,
		-8.0f, 5.0f, 8.0f, 6.0f,
		-1.0f, 0.0f, 8.0f, 2.0f,
		-7.0f, 1.0f, -1.0f, 1.0f
		});

	Matrix3 expected_result = Matrix3({
		-6.0f, 1.0f, 6.0f,
		-8.0f, 8.0f, 6.0f,
		-7.0f, -1.0f, 1.0f
		});

	Matrix3 result = m.sub_matrix4(2, 1);

	ASSERT_EQ(result, expected_result) << result << " != " << expected_result;
}

TEST(Chapter03Tests, ASubMatrixOfAMatrix3IsAMatrix2) 
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

	ASSERT_EQ(result, expected_result) << result << " != " << expected_result;
}

TEST(Chapter03Tests, ASubMatrixOfAMatrixIsASmallerMatrix) 
{
	Matrix m = Matrix(5, {
		1.0f, 5.0f, 0.0f, 1.0f, 2.0f,
		-3.0f, 2.0f, 7.0f, 3.0f, 5.0f,
		0.0f, 6.0f, -3.0f, 4.0f, 6.0f,
		-1.0f, -2.0f, -3.0f, -4.0f, -5.0f,
		6.0f, 7.0f, 8.0f, 9.0f, 10.0f
		});

	Matrix4 expected_result = Matrix4({
		1.0f, 0.0f, 1.0f, 2.0f,
		-3.0f, 7.0f, 3.0f, 5.0f,
		-1.0f, -3.0f, -4.0f, -5.0f,
		6.0f, 8.0f, 9.0f, 10.0f
		});

	Matrix result = m.sub_matrix(2, 1);

	ASSERT_EQ(result, expected_result) << result << " != " << expected_result;
}

TEST(Chapter03Tests, MinorOfAMatrix3) 
{
	Matrix3 m = Matrix3({
		3.0f, 5.0f, 0.0f,
		2.0f, -1.0f, -7.0f,
		6.0f, -1.0f, 5.0f
		});

	float det = (m.sub_matrix3(1, 0)).determinant();
	float min = m.minor(1, 0);

	ASSERT_TRUE(flt_cmp(det, min));
}

TEST(Chapter03Tests, CofactorOfAMatrix3) 
{
	Matrix3 m = Matrix3({
		3.0f, 5.0f, 0.0f,
		2.0f, -1.0f, -7.0f,
		6.0f, -1.0f, 5.0f
		});

	ASSERT_TRUE(flt_cmp(m.minor(0, 0), -12.0f));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 0), -12.0f));
	ASSERT_TRUE(flt_cmp(m.minor(1, 0), 25.0f));
	ASSERT_TRUE(flt_cmp(m.cofactor(1, 0), -25.0f));

	ASSERT_TRUE(flt_cmp(m.minor(0, 0), m.cofactor(0, 0)));
	ASSERT_FALSE(flt_cmp(m.minor(1, 0), m.cofactor(1, 0)));
}

TEST(Chapter03Tests, DeterminantOfAMatrix3) 
{
	Matrix3 m = Matrix3({
		1.0f, 2.0f, 6.0f,
		-5.0f, 8.0f, -4.0f,
		2.0f, 6.0f, 4.0f
		});

	ASSERT_TRUE(flt_cmp(m.cofactor(0, 0), 56.0f));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 1), 12.0f));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 2), -46.0f));
	ASSERT_TRUE(flt_cmp(m.determinant(), -196.0f));
}

TEST(Chapter03Tests, DeterminantOfAMatrix4) 
{
	Matrix4 m = Matrix4({
		-2.0f, -8.0f, 3.0f, 5.0f,
		-3.0f, 1.0f, 7.0f, 3.0f,
		1.0f, 2.0f, -9.0f, 6.0f,
		-6.0f, 7.0f, 7.0f, -9.0f
		});

	ASSERT_TRUE(flt_cmp(m.cofactor(0, 0), 690.0f));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 1), 447.0f));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 2), 210.0f));
	ASSERT_TRUE(flt_cmp(m.cofactor(0, 3), 51.0f));
	ASSERT_TRUE(flt_cmp(m.determinant(), -4071.0f));
}

TEST(Chapter03Tests, DeterminantOfAnArbitrarySizedMatrix) 
{
	Matrix m = Matrix(5, {
		1.0f, 5.0f, 0.0f, 1.0f, 2.0f,
		-3.0f, 2.0f, 7.0f, 3.0f, 5.0f,
		0.0f, 6.0f, -3.0f, 4.0f, 6.0f,
		-1.0f, -2.0f, -3.0f, -4.0f, -5.0f,
		6.0f, 7.0f, 8.0f, 9.0f, 10.0f
		});

	ASSERT_EQ(m.determinant(), -405.0f) << m.determinant();
}

TEST(Chapter03Tests, TestingInvertibleMatrixForInvertability) 
{
	Matrix4 m = Matrix4({
		6.0f, 4.0f, 4.0f, 4.0f,
		5.0f, 5.0f, 7.0f, 6.0f,
		4.0f, -9.0f, 3.0f, -7.0f,
		9.0f, 1.0f, 7.0f, -6.0f
		});

	ASSERT_TRUE(m.is_invertable());
}

TEST(Chapter03Tests, TestingNoninvertibleMatrixForInvertability) 
{
	Matrix4 m = Matrix4({
		-4.0f, 2.0f, -2.0f, -3.0f,
		9.0f, 6.0f, 2.0f, 6.0f,
		0.0f, -5.0f, 1.0f, -5.0f,
		0.0f, 0.0f, 0.0f, 0.0f
		});

	ASSERT_FALSE(m.is_invertable());
}

TEST(Chapter03Tests, CalculatingTheInverseOfAMatrix) 
{
	Matrix m = Matrix(4, {
		-5.0f, 2.0f, 6.0f, -8.0f,
		1.0f, -5.0f, 1.0f, 8.0f,
		7.0f, 7.0f, -6.0f, -7.0f,
		1.0f, -3.0f, 7.0f, 4.0f
		});

	Matrix result = m.inverse();

	Matrix expected_result = Matrix(4, {
		0.21805f, 0.45113f, 0.24060f, -0.04511f,
		-0.80827f, -1.45677f, -0.44361f, 0.52068f,
		-0.07895f, -0.22368f, -0.05263f, 0.19737f,
		-0.52256f, -0.81391f, -0.30075f, 0.30639f
	});

	ASSERT_TRUE(flt_cmp(m.determinant(), 532));

	ASSERT_TRUE(flt_cmp(m.cofactor(2, 3), -160.0f));
	ASSERT_TRUE(flt_cmp(result.get(3, 2), -160.0f / 532));

	ASSERT_TRUE(flt_cmp(m.cofactor(3, 2), 105.0f));
	ASSERT_TRUE(flt_cmp(result.get(2, 3), 105.0f / 532));

	ASSERT_EQ(result, expected_result);
}

TEST(Chapter03Tests, CalculatingTheInverseOfAnotherMatrix) 
{
	Matrix m = Matrix(4, {
		8.0f, -5.0f, 9.0f, 2.0f,
		7.0f, 5.0f, 6.0f, 1.0f,
		-6.0f, 0.0f, 9.0f, 6.0f,
		-3.0f, 0.0f, -9.0f, -4.0f
		});

	Matrix result = m.inverse();

	Matrix expected_result = Matrix(4, {
		-0.15385f, -0.15385f, -0.28205f, -0.53846f,
		-0.07692f, 0.12308f, 0.02564f, 0.03077f,
		0.35897f, 0.35897f, 0.43590f, 0.92308f,
		-0.69231f, -0.69231f, -0.76923f, -1.92308f
		});

	ASSERT_EQ(result, expected_result);
}

TEST(Chapter03Tests, CalculatingTheInverseOfAThirdMatrix) 
{
	Matrix m = Matrix(4, {
		9.0f, 3.0f, 0.0f, 9.0f,
		-5.0f, -2.0f, -6.0f, -3.0f,
		-4.0f, 9.0f, 6.0f, 4.0f,
		-7.0f, 6.0f, 6.0f, 2.0f
		});

	Matrix result = m.inverse();

	Matrix expected_result = Matrix(4, {
		-0.04074f, -0.07778f, 0.14444f, -0.22222f,
		-0.07778f, 0.03333f, 0.36667f, -0.33333f,
		-0.02901f, -0.14630f, -0.10926f, 0.12963f,
		0.17778f, 0.06667f, -0.26667f, 0.33333f
		});

	ASSERT_EQ(result, expected_result);
}

TEST(Chapter03Tests, MultiplyingAProductbyItsInverse) 
{
	Matrix4 m1 = Matrix4({
		3.0f, -9.0f, 7.0f, 3.0f,
		3.0f, -8.0f, 2.0f, -9.0f,
		-4.0f, 4.0f, 4.0f, 1.0f,
		-6.0f, 5.0f, -1.0f, 1.0f
		});

	Matrix4 m2 = Matrix4({
		8.0f, 2.0f, 2.0f, 2.0f,
		3.0f, -1.0f, 7.0f, 0.0f,
		7.0f, 0.0f, 5.0f, 4.0f,
		6.0f, -2.0f, 0.0f, 5.0f
		});

	Matrix4 m3 = m1 * m2;

	ASSERT_EQ(m3 * m2.inverse(), m1);
}

// ------------------------------------------------------------------------
// Chapter 04 Matrix Transformations
// ------------------------------------------------------------------------

TEST(Chapter04Tests, MultiplyingByATranslationMatrix) 
{
	Matrix4 tm = Matrix4::Translation(5.0f, -3.0f, 2.0f);
	Tuple p = Tuple::Point(-3.0f, 4.0f, 5.0f);

	ASSERT_EQ(tm * p, Tuple::Point(2.0f, 1.0f, 7.0f));
}

TEST(Chapter04Tests, MultiplyingByATheInverseOfATranslationMatrix) 
{
	Matrix4 tm = Matrix4::Translation(5.0f, -3.0f, 2.0f);
	Tuple p = Tuple::Point(-3.0f, 4.0f, 5.0f);

	ASSERT_EQ(tm.inverse() * p, Tuple::Point(-8.0f, 7.0f, 3.0f));
}

TEST(Chapter04Tests, TranslationDoesNotAffectVectors) 
{
	Matrix4 tm = Matrix4::Translation(5.0f, -3.0f, 2.0f);
	Tuple v = Tuple::Vector(-3.0f, 4.0f, 5.0f);

	ASSERT_EQ(tm * v, v);
}

TEST(Chapter04Tests, ScalingMatrixAppliedToAPoint) 
{
	Matrix4 tm = Matrix4::Scaling(2.0f, 3.0f, 4.0f);
	Tuple p = Tuple::Point(-4.0f, 6.0f, 8.0f);

	ASSERT_EQ(tm * p, Tuple::Point(-8.0f, 18.0f, 32.0f));
}

TEST(Chapter04Tests, ScalingMatrixAppliedToAVector) 
{
	Matrix4 tm = Matrix4::Scaling(2.0f, 3.0f, 4.0f);
	Tuple v = Tuple::Vector(-4.0f, 6.0f, 8.0f);

	ASSERT_EQ(tm * v, Tuple::Vector(-8.0f, 18.0f, 32.0f));
}

TEST(Chapter04Tests, MultiplyingByTheInverseOfTheScalingMatrix) 
{
	Matrix4 tm = Matrix4::Scaling(2.0f, 3.0f, 4.0f);
	Tuple p = Tuple::Vector(-4.0f, 6.0f, 8.0f);

	ASSERT_EQ(tm.inverse() * p, Tuple::Vector(-2.0f, 2.0f, 2.0f));
}

TEST(Chapter04Tests, ReflectionIsScalingByANegativeValue) 
{
	Matrix4 tm = Matrix4::Scaling(-1.0f, 1.0f, 1.0f);
	Tuple p = Tuple::Point(2.0f, 3.0f, 4.0f);

	ASSERT_EQ(tm * p, Tuple::Point(-2.0f, 3.0f, 4.0f));
}

TEST(Chapter04Tests, RotatingAPointAroundTheXAxis) 
{
	Tuple p = Tuple::Point(0.0f, 1.0f, 0.0f);

	Matrix4 eighth = Matrix4::Rotation_X(static_cast<float>(M_PI) / 4.0f);
	Matrix4 quarter = Matrix4::Rotation_X(static_cast<float>(M_PI) / 2.0f);

	ASSERT_EQ(eighth * p, Tuple::Point(0.0f, sqrt(2.0f) / 2.0f, sqrt(2.0f) / 2.0f));
	ASSERT_EQ(quarter * p, Tuple::Point(0.0f, 0.0f, 1.0f));
}

TEST(Chapter04Tests, TheInverseOfAnXRotationRotatesInTheOpositeDirection) 
{
	Tuple p = Tuple::Point(0.0f, 1.0f, 0.0f);

	Matrix4 eighth = Matrix4::Rotation_X(static_cast<float>(M_PI) / 4.0f);

	ASSERT_EQ(eighth.inverse() * p, Tuple::Point(0.0f, sqrt(2.0f) / 2.0f, -(sqrt(2.0f) / 2.0f)));
}

TEST(Chapter04Tests, RotatingAPointAroundTheYAxis) 
{
	Tuple p = Tuple::Point(0.0f, 0.0f, 1.0f);

	Matrix4 eighth = Matrix4::Rotation_Y(static_cast<float>(M_PI) / 4.0f);
	Matrix4 quarter = Matrix4::Rotation_Y(static_cast<float>(M_PI) / 2.0f);

	ASSERT_EQ(eighth * p, Tuple::Point(sqrt(2.0f) / 2.0f, 0.0f, sqrt(2.0f) / 2.0f));
	ASSERT_EQ(quarter * p, Tuple::Point(1.0f, 0.0f, 0.0f));
}

TEST(Chapter04Tests, RotatingAPointAroundTheZAxis) 
{
	Tuple p = Tuple::Point(0.0f, 1.0f, 0.0f);

	Matrix4 eighth = Matrix4::Rotation_Z(static_cast<float>(M_PI) / 4.0f);
	Matrix4 quarter = Matrix4::Rotation_Z(static_cast<float>(M_PI) / 2.0f);

	ASSERT_EQ(eighth * p, Tuple::Point(-(sqrt(2.0f) / 2.0f), sqrt(2.0f) / 2.0f, 0.0f));
	ASSERT_EQ(quarter * p, Tuple::Point(-1.0f, 0.0f, 0.0f));
}

TEST(Chapter04Tests, AShearingTransformationMovesXInProportionToY) 
{
	Matrix4 shear = Matrix4::Shear(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Tuple p = Tuple::Point(2.0f, 3.0f, 4.0f);

	ASSERT_EQ(shear * p, Tuple::Point(5.0f, 3.0f, 4.0f));
}

TEST(Chapter04Tests, AShearingTransformationMovesXInProportionToZ) 
{
	Matrix4 shear = Matrix4::Shear(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Tuple p = Tuple::Point(2.0f, 3.0f, 4.0f);

	ASSERT_EQ(shear * p, Tuple::Point(6.0f, 3.0f, 4.0f));
}

TEST(Chapter04Tests, AShearingTransformationMovesYInProportionToX)
{
	Matrix4 shear = Matrix4::Shear(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	Tuple p = Tuple::Point(2.0f, 3.0f, 4.0f);

	ASSERT_EQ(shear * p, Tuple::Point(2.0f, 5.0f, 4.0f));
}

TEST(Chapter04Tests, AShearingTransformationMovesYInProportionToZ)
{
	Matrix4 shear = Matrix4::Shear(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	Tuple p = Tuple::Point(2.0f, 3.0f, 4.0f);

	ASSERT_EQ(shear * p, Tuple::Point(2.0f, 7.0f, 4.0f));
}

TEST(Chapter04Tests, AShearingTransformationMovesZInProportionToX)
{
	Matrix4 shear = Matrix4::Shear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Tuple p = Tuple::Point(2.0f, 3.0f, 4.0f);

	ASSERT_EQ(shear * p, Tuple::Point(2.0f, 3.0f, 6.0f));
}

TEST(Chapter04Tests, AShearingTransformationMovesZInProportionToY)
{
	Matrix4 shear = Matrix4::Shear(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	Tuple p = Tuple::Point(2.0f, 3.0f, 4.0f);

	ASSERT_EQ(shear * p, Tuple::Point(2.0f, 3.0f, 7.0f));
}

TEST(Chapter04Tests, IndividualTransfromsAreAppliedInSequence)
{
	Tuple p = Tuple::Point(1.0f, 0.0f, 1.0f);
	Matrix4 A = Matrix4::Rotation_X(static_cast<float>(M_PI) / 2.0f);
	Matrix4 B = Matrix4::Scaling(5.0f, 5.0f, 5.0f);
	Matrix4 C = Matrix4::Translation(10.0f, 5.0f, 7.0f);

	Tuple p2 = A * p;

	ASSERT_EQ(p2, Tuple::Point(1.0f, -1.0f, 0.0f));

	Tuple p3 = B * p2;

	ASSERT_EQ(p3, Tuple::Point(5.0f, -5.0f, 0.0f));

	Tuple p4 = C * p3;

	ASSERT_EQ(p4, Tuple::Point(15.0f, 0.0f, 7.0f));
}

TEST(Chapter04Tests, ChainedTransfromationsMustBeAppliedInReverseOrder)
{
	Tuple p = Tuple::Point(1.0f, 0.0f, 1.0f);
	Matrix4 A = Matrix4::Rotation_X(static_cast<float>(M_PI) / 2.0f);
	Matrix4 B = Matrix4::Scaling(5.0f, 5.0f, 5.0f);
	Matrix4 C = Matrix4::Translation(10.0f, 5.0f, 7.0f);

	Matrix4 T = C * B * A;

	ASSERT_EQ(T * p, Tuple::Point(15.0f, 0.0f, 7.0f));
}

// ------------------------------------------------------------------------
// Chapter 05 Ray-Sphere Intersections
// ------------------------------------------------------------------------

TEST(Chapter05Tests, CreatingAndQueryingARay)
{
	Tuple origin = Tuple::Point(1.0f, 2.0f, 3.0f);
	Tuple direction = Tuple::Vector(4.0f, 5.0f, 6.0f);
	Ray r = Ray(origin, direction);

	ASSERT_EQ(r.origin, origin);
	ASSERT_EQ(r.direction, direction);
}

TEST(Chapter05Tests, ComputingAPointFromADistance)
{
	Ray r = Ray(Tuple::Point(2.0f, 3.0f, 4.0f), Tuple::Vector(1.0f, 0.0f, 0.0f));

	ASSERT_EQ(r.position(0.0f), Tuple::Point(2.0f, 3.0f, 4.0f));
	ASSERT_EQ(r.position(1.0f), Tuple::Point(3.0f, 3.0f, 4.0f));
	ASSERT_EQ(r.position(-1.0f), Tuple::Point(1.0f, 3.0f, 4.0f));
	ASSERT_EQ(r.position(2.5f), Tuple::Point(4.5f, 3.0f, 4.0f));
}

TEST(Chapter05Tests, ARayIntersectsASphereAtTwoPoints)
{
	Ray r = Ray(Tuple::Point(0.0f, 0.0f, -5.0f), Tuple::Vector(0.0f, 0.0f, 1.0f));

	Sphere S = Sphere();

	std::vector<float> xs = S.intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], 4.0));
	ASSERT_TRUE(flt_cmp(xs[1], 6.0));
}

TEST(Chapter05Tests, ARayIntersectsASphereAtATangent)
{
	Ray r = Ray(Tuple::Point(0.0f, 1.0f, -5.0f), Tuple::Vector(0.0f, 0.0f, 1.0f));

	Sphere S = Sphere();

	std::vector<float> xs = S.intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], 5.0)) << xs[0];
	ASSERT_TRUE(flt_cmp(xs[1], 5.0)) << xs[1];
}

TEST(Chapter05Tests, ARayMissesASphere)
{
	Ray r = Ray(Tuple::Point(0.0f, 2.0f, -5.0f), Tuple::Vector(0.0f, 0.0f, 1.0f));

	Sphere S = Sphere();

	std::vector<float> xs = S.intersect_t(r);

	ASSERT_EQ(xs.size(), 0);
}

TEST(Chapter05Tests, ARayOriginatesInsideASphere)
{
	Ray r = Ray(Tuple::Point(0.0f, 0.0f, 0.0f), Tuple::Vector(0.0f, 0.0f, 1.0f));

	Sphere S = Sphere();

	std::vector<float> xs = S.intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], -1.0));
	ASSERT_TRUE(flt_cmp(xs[1], 1.0));
}

TEST(Chapter05Tests, ASphereIsBehindARay)
{
	Ray r = Ray(Tuple::Point(0.0f, 0.0f, 5.0f), Tuple::Vector(0.0f, 0.0f, 1.0f));

	Sphere s = Sphere();

	std::vector<float> xs = s.intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], -6.0));
	ASSERT_TRUE(flt_cmp(xs[1], -4.0));
}

TEST(Chapter05Tests, AnIntersectionEncapsulatesTAndAnObject)
{
	Sphere s = Sphere();

	Intersection i = Intersection(3.5f, &s);

	ASSERT_TRUE(flt_cmp(i.t_value, 3.5f));
	ASSERT_EQ(i.object, &s);
}

TEST(Chapter05Tests, AggregatingIntersections)
{
	Sphere s = Sphere();

	Intersection i1 = Intersection(1.0f, &s);
	Intersection i2 = Intersection(2.0f, &s);

	Intersections xs = Intersections({ i1, i2 });

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0].t_value, 1.0f));
	ASSERT_TRUE(flt_cmp(xs[1].t_value, 2.0f));
}

TEST(Chapter05Tests, IntersectSetsTheObjectOnTheIntersection)
{
	Ray r = Ray(Tuple::Point(0.0f, 0.0f, -5.0f), Tuple::Vector(0.0f, 0.0f, 1.0f));

	Sphere s = Sphere();

	Intersections xs = intersect(r, &s);

	ASSERT_EQ(xs.size(), 2) << xs;
	ASSERT_EQ(xs[0].object, &s);
	ASSERT_EQ(xs[1].object, &s);
}

TEST(Chapter05Tests, TheHitWhenAllIntersectionsHavePositiveT)
{
	Sphere s = Sphere();

	Intersection i1 = Intersection(1.0f, &s);
	Intersection i2 = Intersection(2.0f, &s);

	Intersections xs = Intersections({ i1, i2 });

	Intersection hit = xs.hit();
	ASSERT_EQ(hit, i1) << xs;
}

TEST(Chapter05Tests, TheHitWhenSomeIntersectionsHaveNegativeT)
{
	Sphere s = Sphere();

	Intersection i1 = Intersection(-1.0f, &s);
	Intersection i2 = Intersection(1.0f, &s);

	Intersections xs = Intersections({ i1, i2 });

	Intersection hit = xs.hit();
	ASSERT_EQ(hit, i2);
}

TEST(Chapter05Tests, TheHitWhenAllIntersectionsHaveNegativeT)
{
	Sphere s = Sphere();

	Intersection i1 = Intersection(-2.0f, &s);
	Intersection i2 = Intersection(-1.0f, &s);

	Intersections xs = Intersections({ i1, i2 });

	Intersection hit = xs.hit();
	ASSERT_FALSE(hit.is_valid());
}

TEST(Chapter05Tests, TheHitIsAlwaysTheLowestNonNegativeIntersection)
{
	Sphere s = Sphere();

	Intersection i1 = Intersection(5.0f, &s);
	Intersection i2 = Intersection(7.0f, &s);
	Intersection i3 = Intersection(-3.0f, &s);
	Intersection i4 = Intersection(2.0f, &s);

	Intersections xs = Intersections({ i1, i2, i3, i4 });

	Intersection hit = xs.hit();
	ASSERT_EQ(hit, i4);
}

TEST(Chapter05Tests, TranslatingARay)
{
	Ray r = Ray(Tuple::Point(1.0f, 2.0f, 3.0f), Tuple::Vector(0.0f, 1.0f, 0.0f));
	Matrix4 m = Matrix4::Translation(3.0f, 4.0f, 5.0f);

	Ray r2 = r.transform(m);
	ASSERT_EQ(r2.origin, Tuple::Point(4.0f, 6.0f, 8.0f));
	ASSERT_EQ(r2.direction, Tuple::Vector(0.0f, 1.0f, 0.0f));
}

TEST(Chapter05Tests, ScalingARay)
{
	Ray r = Ray(Tuple::Point(1.0f, 2.0f, 3.0f), Tuple::Vector(0.0f, 1.0f, 0.0f));
	Matrix4 m = Matrix4::Scaling(2.0f, 3.0f, 4.0f);

	Ray r2 = r.transform(m);
	ASSERT_EQ(r2.origin, Tuple::Point(2.0f, 6.0f, 12.0f));
	ASSERT_EQ(r2.direction, Tuple::Vector(0.0f, 3.0f, 0.0f));
}

TEST(Chapter05Tests, ASpheresDefaultTransformation)
{
	Sphere s = Sphere();

	ASSERT_EQ(s.get_transform(), Matrix4::Identity());
}

TEST(Chapter05Tests, ChangingASpheresTransformation)
{
	Sphere s = Sphere();
	Matrix4 t = Matrix4::Translation(2.0f, 3.0f, 4.0f);

	s.set_transform(t);

	ASSERT_EQ(s.get_transform(), t);
}

TEST(Chapter05Tests, IntersectingAScaledSphereWithARay)
{
	Ray r = Ray(Tuple::Point(0.0f, 0.0f, -5.0f), Tuple::Vector(0.0f, 0.0f, 1.0f));
	Sphere s = Sphere();
	s.set_transform(Matrix4::Scaling(2.0f, 2.0f, 2.0f));
	Intersections xs = intersect(r, &s);

	ASSERT_EQ(xs.size(), 2);

	ASSERT_TRUE(flt_cmp(xs[0].t_value, 3.0f)) << xs[0].t_value;
	ASSERT_TRUE(flt_cmp(xs[1].t_value, 7.0f));
}

TEST(Chapter05Tests, IntersectingATranslatedSphereWithARay)
{
	Ray r = Ray(Tuple::Point(0.0f, 0.0f, -5.0f), Tuple::Vector(0.0f, 0.0f, 1.0f));
	Sphere s = Sphere();
	s.set_transform(Matrix4::Translation(5.0f, 0.0f, 0.0f));
	Intersections xs = intersect(r, &s);

	ASSERT_EQ(xs.size(), 0);
}

// ------------------------------------------------------------------------
// Chapter 06 Light and Shading
// ------------------------------------------------------------------------

TEST(Chapter06Tests, TheNormalOnASphereAtAPointOnTheXAxis)
{
	Sphere s = Sphere();
	
	Tuple n = s.normal_at(Tuple::Point(1.0f, 0.0f, 0.0f));

	ASSERT_EQ(n, Tuple::Vector(1.0f, 0.0f, 0.0f));
}

TEST(Chapter06Tests, TheNormalOnASphereAtAPointOnTheYAxis)
{
	Sphere s = Sphere();

	Tuple n = s.normal_at(Tuple::Point(0.0f, 1.0f, 0.0f));

	ASSERT_EQ(n, Tuple::Vector(0.0f, 1.0f, 0.0f));
}

TEST(Chapter06Tests, TheNormalOnASphereAtAPointOnTheZAxis)
{
	Sphere s = Sphere();

	Tuple n = s.normal_at(Tuple::Point(0.0f, 0.0f, 1.0f));

	ASSERT_EQ(n, Tuple::Vector(0.0f, 0.0f, 1.0f));
}

TEST(Chapter06Tests, TheNormalOnASphereAtANonAxialPoint)
{
	Sphere s = Sphere();

	float sq_3 = sqrt(3.0f) / 3.0f;

	Tuple n = s.normal_at(Tuple::Point(sq_3, sq_3, sq_3));

	ASSERT_EQ(n, Tuple::Vector(sq_3, sq_3, sq_3)) << s.get_transform();
}

TEST(Chapter06Tests, TheNormalIsANormalizedVector)
{
	Sphere s = Sphere();

	float sq_3 = sqrt(3.0f) / 3.0f;

	Tuple n = s.normal_at(Tuple::Point(sq_3, sq_3, sq_3));

	ASSERT_EQ(n, n.normalize());
}

TEST(Chapter06Tests, ComputingTheNormalOnATranslatedSphere)
{
	Sphere s = Sphere();
	s.set_transform(Matrix4::Translation(0.0f, 1.0f, 0.0f));

	Tuple n = s.normal_at(Tuple::Point(0.0f, 1.70711f, -0.70711f));

	ASSERT_EQ(n, Tuple::Vector(0.0f, 0.70711f, -0.70711f));
}

TEST(Chapter06Tests, ComputingTheNormalOnATransformedSphere)
{
	Sphere s = Sphere();
	Matrix4 m = Matrix4::Scaling(1.0f, 0.5f, 1.0f) * Matrix4::Rotation_Z(static_cast<float>(M_PI) / 5.0f);

	s.set_transform(m);

	Tuple n = s.normal_at(Tuple::Point(0.0f, sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f));

	ASSERT_EQ(n, Tuple::Vector(0.0f, 0.97014f, -0.24254f));
}

TEST(Chapter06Tests, ReflectingAVectorApproachingAt45Degrees)
{
	Tuple v = Tuple::Vector(1.0f, -1.0f, 0.0f);
	Tuple n = Tuple::Vector(0.0f, 1.0f, 0.0f);

	Tuple r = Tuple::reflect(v, n);

	ASSERT_EQ(r, Tuple::Vector(1.0f, 1.0f, 0.0f));
}

TEST(Chapter06Tests, ReflectingAVectorOffOfASlantedSurface)
{
	Tuple v = Tuple::Vector(0.0f, -1.0f, 0.0f);
	Tuple n = Tuple::Vector(sqrt(2.0f) / 2.0f, sqrt(2.0f) / 2.0f, 0.0f);

	Tuple r = Tuple::reflect(v, n);

	ASSERT_EQ(r, Tuple::Vector(1.0f, 0.0f, 0.0f));
}

TEST(Chapter06Tests, APointLightHasPositionAndIntensity)
{
	Color intensity = Color(1.0f, 1.0f, 1.0f);
	Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);

	PointLight light = PointLight(position, intensity);

	ASSERT_EQ(light.position(), position);
	ASSERT_EQ(light.color, intensity);
}

TEST(Chapter06Tests, TheDefaultMaterial)
{
	Material m = Material();
	
	ASSERT_EQ(m.color, Color(1.0f, 1.0f, 1.0f));
	ASSERT_TRUE(flt_cmp(m.ambient, 0.1f));
	ASSERT_TRUE(flt_cmp(m.diffuse, 0.9f));
	ASSERT_TRUE(flt_cmp(m.specular, 0.9f));
	ASSERT_TRUE(flt_cmp(m.shininess, 200.0f));
}

TEST(Chapter06Tests, ASphereHasADefaulMaterial)
{
	Sphere s = Sphere();
	Material m = Material();

	ASSERT_EQ(m, s.material);
}

TEST(Chapter06Tests, ASphereMayBeAssignedAMaterial)
{
	Sphere s = Sphere();
	Material m = Material();
	m.ambient = 1.0f;

	s.material = m;

	ASSERT_EQ(m, s.material);
}

TEST(Chapter06Tests, LightingWithTheEyeBewteenTheLightAndTheSurface)
{
	Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);
	Material m = Material();

	Tuple eye_v = Tuple::Vector(0.0f, 0.0f, -1.0f);
	Tuple normal_v = Tuple::Vector(0.0f, 0.0f, -1.0f);

	PointLight light = PointLight(Tuple::Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
	Color result = lighting(m, &light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(1.9f, 1.9f, 1.9f));
}

TEST(Chapter06Tests, LightingWithTheEyeBewteenTheLightAndTheSurfaceEyeOffset45Degrees)
{
	Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);
	Material m = Material();

	Tuple eye_v = Tuple::Vector(0.0f, sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f);
	Tuple normal_v = Tuple::Vector(0.0f, 0.0f, -1.0f);

	PointLight light = PointLight(Tuple::Point(0.0f, 0.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
	Color result = lighting(m, &light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(1.0f, 1.0f, 1.0f));
}

TEST(Chapter06Tests, LightingWithEyeOppositeSurfaceLightOffset45Degrees)
{
	Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);
	Material m = Material();

	Tuple eye_v = Tuple::Vector(0.0f, 0.0f, -1.0);
	Tuple normal_v = Tuple::Vector(0.0f, 0.0f, -1.0f);

	PointLight light = PointLight(Tuple::Point(0.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
	Color result = lighting(m, &light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(0.7364f, 0.7364f, 0.7364f));
}

TEST(Chapter06Tests, LightingWithEyeInThePathOfTheReflectionVector)
{
	Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);
	Material m = Material();

	Tuple eye_v = Tuple::Vector(0.0f, -sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f);
	Tuple normal_v = Tuple::Vector(0.0f, 0.0f, -1.0f);

	PointLight light = PointLight(Tuple::Point(0.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
	Color result = lighting(m, &light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(1.63639f, 1.63639f, 1.63639f));
}

TEST(Chapter06Tests, LightingWithTheLightBehindTheSurface)
{
	Tuple position = Tuple::Point(0.0f, 0.0f, 0.0f);
	Material m = Material();

	Tuple eye_v = Tuple::Vector(0.0f, 0.0f, -1.0);
	Tuple normal_v = Tuple::Vector(0.0f, 0.0f, -1.0f);

	PointLight light = PointLight(Tuple::Point(0.0f, 0.0f, 10.0f), Color(1.0f, 1.0f, 1.0f));
	Color result = lighting(m, &light, position, eye_v, normal_v);

	ASSERT_EQ(result, Color(0.1f, 0.1f, 0.1f));
}
