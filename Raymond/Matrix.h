#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include "tuple.h"

class Matrix
{
public:
	// Constructors
	Matrix(int, float);
	Matrix(int, std::vector<float>);
	Matrix(int);
	~Matrix();

	// Factories
	static Matrix Identity(int);

	//Methods
	// Accessors
	float get(int, int) const;
	float& get(int);
	float get(int) const;
	float& at(int);
	float at(int) const;

	virtual std::vector<float> get_row(int) const;
	virtual std::vector<float> get_column(int) const;

	int get_num_columns() const;
	int get_num_rows() const;

	void set(int, int, float);
	void set(int, float);
	void set_multiple(const std::vector<float>);

	void generate_identity();

	std::vector<float> transpose_vector() const;
	Matrix transpose() const;
	std::vector<float> sub_matrix_vector(int, int) const;
	Matrix sub_matrix(int, int) const;
	float minor(int, int) const;
	float cofactor(int, int) const;
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
	friend std::ostream & operator<<(std::ostream &, const Matrix &);
	friend bool operator==(const Matrix &, const Matrix &);
	friend bool operator!=(const Matrix &, const Matrix &);

private:
	// Properties
	std::vector<float> m_data_;
	int m_num_columns_, m_num_rows_;

	//Methods
	int m_index_from_coordinates_(int, int) const;
};

class Matrix2 :
	public Matrix
{
public:
	// Constructors
	Matrix2();
	Matrix2(float);
	Matrix2(const std::vector<float>);

	// Factories
	static Matrix2 Identity();

	// Methods
	std::vector<float> get_row(int) const;
	std::vector<float> get_column(int) const;
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
	Matrix3(float);
	Matrix3(const std::vector<float>);

	// Factories
	static Matrix3 Identity();

	// Methods
	std::vector<float> get_row(int) const;
	std::vector<float> get_column(int) const;

	Matrix2 sub_matrix3(int, int) const;
	float minor(int, int) const;
	float cofactor(int, int) const;
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
	Matrix4(float);
	Matrix4(const std::vector<float>);

	// Factories
	static Matrix4 Identity();

	// Transformation Matrix Factories
	static Matrix4 Translation(float, float, float);
	static Matrix4 Scale(float, float, float);
	static Matrix4 Rotation_X(float);
	static Matrix4 Rotation_Y(float);
	static Matrix4 Rotation_Z(float);
	static Matrix4 Shear(float, float, float, float, float, float);

	// Methods
	std::vector<float> get_row(int) const;
	std::vector<float> get_column(int) const;
	Tuple get_row_tuple(int) const;
	Tuple get_column_tuple(int) const;

	Matrix3 sub_matrix4(int, int) const;

	float minor(int, int) const;
	float cofactor(int, int) const;
	float determinant() const;
	Matrix4 inverse() const;
	Matrix4 transpose() const;

	// Overloaded Operators
	Matrix4 operator*(const Matrix4 &) const;
	Tuple operator*(const Tuple &) const;
};

class NoninvertableMatrix : public std::logic_error
{
public:
	NoninvertableMatrix(const Matrix);
};
