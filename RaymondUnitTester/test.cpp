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
#include "../Raymond/World.h"
#include "../Raymond/IxComps.h"
#include "../Raymond/Background.h"
#include "../Raymond/Camera.h"
#include "../Raymond/Constants.h"
#include "../Raymond/Utilities.h"

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

	ASSERT_EQ(c1.multiply(c2), Color(0.9, 0.2, 0.04));
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

	for (Color i : c)
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
	catch (std::out_of_range e)
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

	std::vector<double> xs = S.local_intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], 4.0));
	ASSERT_TRUE(flt_cmp(xs[1], 6.0));
}

TEST(Chapter05Tests, ARayIntersectsASphereAtATangent)
{
	Ray r = Ray(Tuple::Point(0.0, 1.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Sphere S = Sphere();

	std::vector<double> xs = S.local_intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], 5.0)) << xs[0];
	ASSERT_TRUE(flt_cmp(xs[1], 5.0)) << xs[1];
}

TEST(Chapter05Tests, ARayMissesASphere)
{
	Ray r = Ray(Tuple::Point(0.0, 2.0, -5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Sphere S = Sphere();

	std::vector<double> xs = S.local_intersect_t(r);

	ASSERT_EQ(xs.size(), 0);
}

TEST(Chapter05Tests, ARayOriginatesInsideASphere)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 0.0, 1.0));

	Sphere S = Sphere();

	std::vector<double> xs = S.local_intersect_t(r);

	ASSERT_EQ(xs.size(), 2);
	ASSERT_TRUE(flt_cmp(xs[0], -1.0));
	ASSERT_TRUE(flt_cmp(xs[1], 1.0));
}

TEST(Chapter05Tests, ASphereIsBehindARay)
{
	Ray r = Ray(Tuple::Point(0.0, 0.0, 5.0), Tuple::Vector(0.0, 0.0, 1.0));

	Sphere s = Sphere();

	std::vector<double> xs = s.local_intersect_t(r);

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
	ASSERT_TRUE(flt_cmp(m.ambient, 0.1));
	ASSERT_TRUE(flt_cmp(m.diffuse, 0.9));
	ASSERT_TRUE(flt_cmp(m.specular, 0.9));
	ASSERT_TRUE(flt_cmp(m.shininess, 200.0));
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
	m->ambient = 1.0;

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

	std::shared_ptr<Primitive> s = w.get_primitives()[0];
	Intersection i = Intersection(4.0, s);

	IxComps comps = IxComps(i, r);
	Color sample = w.shade(comps);

	ASSERT_EQ(sample, Color(0.38066, 0.47583, 0.2855));
}

TEST(Chapter07Tests, ShadingAnIntersectionFromTheInside)
{
	World w = World::Default();

	w.add_object(std::make_shared<PointLight>(Tuple::Point(0.0, 0.25, 0.0), Color(1.0, 1.0, 1.0)));

	w.remove_light(0);

	Ray r = Ray(Tuple::Point(0.0, 0.0, 0.0), Tuple::Vector(0.0, 0.0, 1.0));

	std::shared_ptr<Primitive> s = w.get_primitives()[1];
	Intersection i = Intersection(0.5, s);

	IxComps comps = IxComps(i, r);
	Color sample = w.shade(comps);

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

	std::shared_ptr<Primitive> outer = w.get_primitives()[0];
	auto outer_mat = std::dynamic_pointer_cast<PhongMaterial>(outer->material);
	outer_mat->ambient = 1.0;

	std::shared_ptr<Primitive> inner = w.get_primitives()[1];
	auto inner_mat = std::dynamic_pointer_cast<PhongMaterial>(inner->material);
	inner_mat->ambient = 1.0;

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

	Color c = w.shade(comps);

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
	m->ambient = 1.0;

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

	Tuple n1 = p.local_normal_at(Tuple::Point(0.0, 0.0, 0.0));
	Tuple n2 = p.local_normal_at(Tuple::Point(10.0, 0.0, -10.0));
	Tuple n3 = p.local_normal_at(Tuple::Point(-50.0, 0.0, 150.0));

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
