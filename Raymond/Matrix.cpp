#include "pch.h"
#include "Matrix.h"


// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

// Optional parameter for a fill value
Matrix::Matrix(int width, int height, float fill)
{
	m_width_ = width;
	m_height_ = height;

	m_data_.resize(width * height, fill);
}

Matrix::Matrix(int width, int height)
{
	m_width_ = width;
	m_height_ = height;

	m_data_.resize(width * height, 0.0f);
}

// Destructor
Matrix::~Matrix()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

float Matrix::get(int y, int x)
{
	// Check if the index is within the bounds of the matrix
	if (x >= 0 && x < m_width_ && y >= 0 && y < m_height_)
		return m_data_.at(m_index_from_coordinates_(x, y));
	else
		// Out of bounds throws an error
		throw std::out_of_range(
			"The requested index, (" +
			std::to_string(x) +
			", " +
			std::to_string(y) +
			"), is not within the bounds of the matrix."
		);
}

void Matrix::set(int x, int y, float value)
{
	// Check if the index is within the bounds of the matrix
	if (x >= 0 && x < m_width_ && y >= 0 && y < m_height_)
		m_data_.at(m_index_from_coordinates_(x, y)) = value;
}

void Matrix::set_multiple(const std::vector<float> values)
{
	// Check vector size before replacing it.
	if (values.size() == m_data_.size())
		m_data_ = values;
	else
	{
		// Vector is the wrong size
		throw std::out_of_range(
			"The input vector (" +
			std::to_string(values.size()) +
			") is not the same size as the matrix(" +
			std::to_string(m_data_.size()) +
			")."
		);
	}

}

// Private
// -------

int Matrix::m_index_from_coordinates_(int x, int y)
{
	return (y * m_width_) + x;
}

// ------------------------------------------------------------------------
// Overloaded Operator
// ------------------------------------------------------------------------

std::ostream & operator<<(std::ostream & os, const Matrix & m)
{
	os << "(" << ", " << ")";
	return os;
}
