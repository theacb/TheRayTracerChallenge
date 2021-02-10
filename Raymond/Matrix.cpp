#include "pch.h"
#include "Matrix.h"


// ------------------------------------------------------------------------
//
// Base Matrix
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

// Optional parameter for a fill value
Matrix::Matrix(int rows, int columns, float fill)
{
	this->m_num_columns_ = rows;
	this->m_num_rows_ = columns;

	this->m_data_.resize(rows * columns, fill);
}

Matrix::Matrix(int rows, int columns): Matrix(rows, columns, 0.0f)
{
}

// Destructor
Matrix::~Matrix()
{
}

// ------------------------------------------------------------------------
// Factories
// ------------------------------------------------------------------------

Matrix Matrix::Identity(int rows, int columns)
{
	Matrix result = Matrix(rows, columns);
	result.generate_identity();
	return result;
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

float Matrix::get(int row, int col)
{
	// Check if the index is within the bounds of the matrix
	if (col >= 0 && col < this->m_num_columns_ && row >= 0 && row < this->m_num_rows_)
		return this->m_data_.at(m_index_from_coordinates_(row, col));
	else
		// Out of bounds throws an error
		throw std::out_of_range(
			"The requested index, (" +
			std::to_string(col) +
			", " +
			std::to_string(row) +
			"), is not within the bounds of the matrix."
		);
}

float& Matrix::at(int index)
{
	return  this->m_data_.at(index);
}

float Matrix::at(int index) const
{
	return  this->m_data_.at(index);
}

int Matrix::get_num_columns() const
{
	return  this->m_num_columns_;
}

int Matrix::get_num_rows() const
{
	return  this->m_num_rows_;
}

void Matrix::set(int row, int col, float value)
{
	// Check if the index is within the bounds of the matrix
	if (col >= 0 && col < this->m_num_columns_ && row >= 0 && row < this->m_num_rows_)
		this->m_data_.at(m_index_from_coordinates_(row, col)) = value;
}

void Matrix::set_multiple(const std::vector<float> values)
{
	// Check vector size before replacing it.
	if (values.size() == this->m_data_.size())
		this->m_data_ = values;
	else
	{
		// Vector is the wrong size
		throw std::out_of_range(
			"The input vector (" +
			std::to_string(values.size()) +
			") is not the same size as the matrix(" +
			std::to_string(this->m_data_.size()) +
			")."
		);
	}

}

// Overwite the Matrix with the Identity matrix
void Matrix::generate_identity()
{
	int columns = this->get_num_columns();
	int rows = this->get_num_rows();

	int next_diag = 0;

	for (int i = 0; i < (columns * rows); i++)
	{
		if (i == next_diag)
		{
			this->m_data_.at(i) = 1.0f;
			next_diag += (rows + 1);
		}
		else
		{
			this->m_data_.at(i) = 0.0f;
		}
	}
}

// iterators
// Allows iteration access to the underlying vector
float * Matrix::begin()
{
	return this->m_data_.data();
}

const float * Matrix::begin() const
{
	return this->m_data_.data();
}

float * Matrix::end()
{
	return this->m_data_.data() + this->m_data_.size();
}

const float * Matrix::end() const
{
	return this->m_data_.data() + this->m_data_.size();
}

// Private
// -------

// Converts x,y cartesian coords to an index in the internal vector
int Matrix::m_index_from_coordinates_(int row, int col)
{
	return (row *  this->m_num_columns_) + col;
}

// ------------------------------------------------------------------------
// Overloaded Operators
// ------------------------------------------------------------------------

// Output
std::ostream & operator<<(std::ostream & os, const Matrix & m)
{
	int current = 0;
	int col = 1;

	int num_cols = m.get_num_columns();
	int last = (m.get_num_rows() * num_cols) - 1;


	os << "([";
	for (float v : m)
	{
		if (col == num_cols && current != last)
		{
			col = 0;
			os << v << "], [";
		}
		else if (current == last)
		{
			os << v;
		}
		else
		{
			os << v << ", ";
		}
		++col;
		++current;
	}
	os << "])";
	return os;
}

// Equality
bool operator==(const Matrix & left_matrix, const Matrix & right_matrix)
{
	// Do not bother comparing matrices of different sizes
	if (left_matrix.m_num_columns_ == right_matrix.m_num_columns_ &&
		left_matrix.m_num_rows_ == right_matrix.m_num_rows_)
	{
		// Compares each element to the corresponding using the flt_cmp() function as comparator
		return std::equal(
			left_matrix.m_data_.begin(),
			left_matrix.m_data_.end(),
			right_matrix.m_data_.begin(),
			flt_cmp
		);
	}
	else
	{
		return false;
	}
}

// Inequality
bool operator!=(const Matrix & left_matrix, const Matrix & right_matrix)
{
	return !(left_matrix == right_matrix);
}

// ------------------------------------------------------------------------
//
// Matrix4
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Matrix4::Matrix4(): Matrix(4, 4)
{
}

Matrix4::Matrix4(float fill) : Matrix(4, 4, fill)
{
}

Matrix4::Matrix4(const std::vector<float> v) : Matrix(4, 4)
{
	this->set_multiple(v);
}

// ------------------------------------------------------------------------
// Factories
// ------------------------------------------------------------------------

Matrix4 Matrix4::Identity()
{
	Matrix4 result = Matrix4();
	result.generate_identity();
	return result;
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

Tuple Matrix4::get_row(int row) const
{
	int num_col = this->get_num_columns();
	float x, y, z, w;
	x = this->at(row * num_col);
	y = this->at((row * num_col) + 1);
	z = this->at((row * num_col) + 2);
	w = this->at((row * num_col) + 3);
	return Tuple(x, y, z, w);
}

Tuple Matrix4::get_col(int col) const
{
	int num_row = this->get_num_rows();
	float x, y, z, w;
	x = this->at(col);
	y = this->at(col + num_row);
	z = this->at(col + (num_row * 2));
	w = this->at(col + (num_row * 3));
	return Tuple(x, y, z, w);
}

// ------------------------------------------------------------------------
// Overloaded Operators
// ------------------------------------------------------------------------

Matrix4 Matrix4::operator*(const Matrix4 & right_matrix) const
{
	Matrix4 result = Matrix4();

	// Goes through each number in the result matrix, row by column.
	for (int r = 0; r < this->get_num_rows(); r++)
	{
		for (int c = 0; c < right_matrix.get_num_columns(); c++)
		{
			// Generate Tuples for each row and column
			// The tuples are sets of the appropriate length which have a dot product function
			Tuple row = this->get_row(r);
			Tuple col = right_matrix.get_col(c);

			float dot_product = Tuple::dot(row, col);

			// Set the result of the dot product to the position in the new Matrix
			result.at((r * this->get_num_rows()) + c) = dot_product;
		}
	};

	return result;
}

// External Overloaded Operators
Tuple Matrix4::operator*(const Tuple & tuple) const
{
	float x, y, z, w;

	x = Tuple::dot(tuple, this->get_row(0));
	y = Tuple::dot(tuple, this->get_row(1));
	z = Tuple::dot(tuple, this->get_row(2));
	w = Tuple::dot(tuple, this->get_row(3));

	return Tuple(x, y, z, w);
}
