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
	Matrix(int, int, float);
	Matrix(int, int, std::vector<float>);
	Matrix(int, int);
	~Matrix();

	// Factories
	static Matrix Identity(int, int);

	//Methods
	// Accessors
	float get(int, int);
	float& at(int);
	float at(int) const;

	virtual std::vector<float> get_row(int) const;
	virtual std::vector<float> get_column(int) const;

	int get_num_columns() const;
	int get_num_rows() const;

	void set(int, int, float);
	void set_multiple(const std::vector<float>);

	Matrix transpose();
	void generate_identity();
	std::vector<float> sub_matrix_vector(int, int) const;
	Matrix sub_matrix(int, int) const;

	// iterators
	float * begin();
	const float * begin() const;
	float * end();
	const float * end() const;

	// Friendly Overloaded Operators
	friend std::ostream & operator<<(std::ostream &, const Matrix &);
	friend bool operator==(const Matrix &, const Matrix &);
	friend bool operator!=(const Matrix &, const Matrix &);

private:
	// Properties
	std::vector<float> m_data_;
	int m_num_columns_, m_num_rows_;

	//Methods
	int m_index_from_coordinates_(int, int);
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

	// Methods
	std::vector<float> get_row(int) const;
	std::vector<float> get_column(int) const;
	Tuple get_row_tuple(int) const;
	Tuple get_column_tuple(int) const;

	Matrix3 sub_matrix4(int, int) const;

	// Overloaded Operators
	Matrix4 operator*(const Matrix4 &) const;
	Tuple operator*(const Tuple &) const;
};
