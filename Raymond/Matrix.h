#ifndef H_RAYMOND_MATRIX
#define H_RAYMOND_MATRIX

#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include "Tuple.h"

class Matrix
{
public:
	// Constructors
	Matrix(int side, float fill);
	Matrix(int side, std::vector<float> values);
	Matrix(int);
	~Matrix();

	// Factories
	static Matrix Identity(int side);

	//Methods
	// Accessors
	float get(int row, int col) const;
	float& get(int index);
	float get(int index) const;
	float& at(int index);
	float at(int index) const;

	virtual std::vector<float> get_row(int row) const;
	virtual std::vector<float> get_column(int col) const;

	int get_num_columns() const;
	int get_num_rows() const;

	void set(int row, int col, float value);
	void set(int index, float value);
	void set_multiple(const std::vector<float> values);

	void generate_identity();

	std::vector<float> transpose_vector() const;
	Matrix transpose() const;
	std::vector<float> sub_matrix_vector(int remove_row, int remove_col) const;
	Matrix sub_matrix(int remove_row, int remove_col) const;
	float minor(int row, int col) const;
	float cofactor(int row, int col) const;
	float determinant() const;
	bool is_invertable() const;
	std::vector<float> inverse_vector() const;
	Matrix inverse() const;

	std::string to_string() const;

	// iterators
	float * begin();
	const float * begin() const;
	float * end();
	const float * end() const;

	// Overloaded Operators
	float& operator[](int index);

	// Friendly Overloaded Operators
	friend std::ostream & operator<<(std::ostream & ox, const Matrix & m);
	friend bool operator==(const Matrix & left_matrix, const Matrix & right_matrix);
	friend bool operator!=(const Matrix & left_matrix, const Matrix & right_matrix);

private:
	// Properties
	std::vector<float> m_data_;
	int m_num_columns_, m_num_rows_;

	//Methods
	int m_index_from_coordinates_(int row, int col) const;
};

class Matrix2 :
	public Matrix
{
public:
	// Constructors
	Matrix2();
	Matrix2(float fill);
	Matrix2(const std::vector<float> v);

	// Factories
	static Matrix2 Identity();

	// Methods
	std::vector<float> get_row(int row) const;
	std::vector<float> get_column(int col) const;
	float determinant() const;
	Matrix2 inverse() const;
	Matrix2 transpose() const;
};

class Matrix3 :
	public Matrix
{
public:
	// Constructors
	Matrix3();
	Matrix3(float fill);
	Matrix3(const std::vector<float> v);

	// Factories
	static Matrix3 Identity();

	// Methods
	std::vector<float> get_row(int row) const;
	std::vector<float> get_column(int col) const;

	Matrix2 sub_matrix3(int remove_row, int remove_col) const;
	float minor(int row, int col) const;
	float cofactor(int row, int col) const;
	float determinant() const;
	Matrix3 inverse() const;
	Matrix3 transpose() const;
};

class Matrix4 :
	public Matrix
{
public:
	// Constructors
	Matrix4();
	Matrix4(float fill);
	Matrix4(const std::vector<float> v);

	// Factories
	static Matrix4 Identity();

	// Transformation Matrix Factories
	static Matrix4 Translation(float x, float y, float z);
	static Matrix4 Translation(Tuple p);
	static Matrix4 Scaling(float x, float y, float z);
	static Matrix4 Scaling(Tuple s);
	static Matrix4 Rotation_X(float radians);
	static Matrix4 Rotation_Y(float radians);
	static Matrix4 Rotation_Z(float radians);
	static Matrix4 Shear(float xy, float xz, float yx, float yz, float zx, float zy);
	static Matrix4 ViewTransform(Tuple from, Tuple to, Tuple up);

	// Methods
	std::vector<float> get_row(int row) const;
	std::vector<float> get_column(int col) const;
	Tuple get_row_tuple(int row) const;
	Tuple get_column_tuple(int col) const;

	Matrix3 sub_matrix4(int remove_row, int remove_col) const;

	float minor(int row, int col) const;
	float cofactor(int row, int col) const;
	float determinant() const;
	Matrix4 inverse() const;
	Matrix4 transpose() const;

	// Transformation Matrix Getters
	Tuple position();
	Tuple scale();

	// Overloaded Operators
	Matrix4 operator*(const Matrix4 & right_matrix) const;
	Tuple operator*(const Tuple & tuple) const;
};

class NoninvertableMatrix : public std::logic_error
{
public:
	NoninvertableMatrix(const Matrix);
};

#endif
