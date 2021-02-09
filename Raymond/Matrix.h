#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>

class Matrix
{
public:
	// Constructors
	Matrix(int, int, float);
	Matrix(int, int);
	~Matrix();

	//Methods
	float get(int, int);
	void set(int, int, float);
	void set_multiple(const std::vector<float>);

	// Overloaded Operators
	friend std::ostream & operator<<(std::ostream &, const Matrix &);

private:
	// Properties
	std::vector<float> m_data_;
	int m_width_, m_height_;

	//Methods
	int m_index_from_coordinates_(int, int);
};
