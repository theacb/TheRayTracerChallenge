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
	Matrix(int, int);
	~Matrix();

	// Factories
	static Matrix Identity(int, int);

	//Methods
	// Accessors
	float get(int, int);
	float& at(int);
	float at(int) const;
	int get_num_columns() const;
	int get_num_rows() const;

	void set(int, int, float);
	void set_multiple(const std::vector<float>);

	void generate_identity();

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

class Matrix4 :
	public Matrix
{
public:
	Matrix4();
	Matrix4(float);
	Matrix4(const std::vector<float>);

	// Factories
	static Matrix4 Identity();

	// Methods
	Tuple get_row(int) const;
	Tuple get_col(int) const;

	// Overloaded Operators
	Matrix4 operator*(const Matrix4 &) const;
	Tuple operator*(const Tuple &) const;
};
