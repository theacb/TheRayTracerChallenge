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
Matrix::Matrix(int side, float fill)
{
	this->m_num_columns_ = side;
	this->m_num_rows_ = side;

	this->m_data_.resize(side * side, fill);
}

Matrix::Matrix(int side, const std::vector<float> values)
{
	this->m_num_columns_ = side;
	this->m_num_rows_ = side;

	// Check vector size before replacing it.
	if (values.size() == side * side)
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

Matrix::Matrix(int side): Matrix(side, 0.0f)
{
}

// Destructor
Matrix::~Matrix()
{
}

// ------------------------------------------------------------------------
// Factories
// ------------------------------------------------------------------------

Matrix Matrix::Identity(int side)
{
	Matrix result = Matrix(side);
	result.generate_identity();
	return result;
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

float Matrix::get(int row, int col) const
{
	// Check if the index is within the bounds of the matrix
	if (col >= 0 && col < this->m_num_columns_ && row >= 0 && row < this->m_num_rows_)
		return this->m_data_[this->m_index_from_coordinates_(row, col)];
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

float & Matrix::get(int index)
{
	return this->m_data_[index];
}

float Matrix::get(int index) const
{
	return this->m_data_[index];
}

// At() wrappers

float& Matrix::at(int index)
{
	return  this->m_data_.at(index);
}

float Matrix::at(int index) const
{
	return  this->m_data_.at(index);
}

// Get Row and Columns

std::vector<float> Matrix::get_row(int row) const
{
	// Returns a portion of the internal vector that is equal to the requested row
	// (RowNumber * NumberOfColumns) - to - ((RowNumber * NumberOfColumns) + LengthOfARow)
	return std::vector<float>(
		this->m_data_.begin() + (row * this->m_num_columns_),
		this->m_data_.begin() + ((row * this->m_num_columns_) + this->m_num_columns_)
		);
}

std::vector<float> Matrix::get_column(int col) const
{
	std::vector<float> result = std::vector<float>(this->m_num_rows_, 0.0f);

	// Obtains the members of the row from the internal vector 
	// using the number of rows as an offset
	for (int i = 0; i < this->m_num_rows_; i++)
	{
		result[i] = this->m_data_[col + (this->m_num_rows_ * i)];
	}

	return result;
}

// Number of Columns

int Matrix::get_num_columns() const
{
	return  this->m_num_columns_;
}

int Matrix::get_num_rows() const
{
	return  this->m_num_rows_;
}

// Set a value by it's row, col coordinate
void Matrix::set(int row, int col, float value)
{
	// Check if the index is within the bounds of the matrix
	if (col >= 0 && col < this->m_num_columns_ && row >= 0 && row < this->m_num_rows_)
		this->m_data_[m_index_from_coordinates_(row, col)] = value;
}

void Matrix::set(int index, float value)
{
	this->m_data_[index] = value;
}

// Set the data of the matrix by passing in a vector with each row in series.
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
			this->m_data_[i] = 1.0f;
			next_diag += (rows + 1);
		}
		else
		{
			this->m_data_[i] = 0.0f;
		}
	}
}

std::vector<float> Matrix::transpose_vector() const
{
	std::vector<float> result = std::vector<float>(this->m_num_rows_ * this->m_num_columns_ ,0.0f);

	for (int r = 0; r < this->m_num_rows_; r++)
	{
		for (int c = 0; c < this->m_num_columns_; c++)
		{
			result[(c * this->get_num_columns()) + r] = this->m_data_[(r * this->m_num_rows_) + c];
		}
	}

	return result;
}

// Turns columns into rows and rows into columns
Matrix Matrix::transpose() const
{
	return Matrix(
		this->m_num_rows_,
		this->transpose_vector()
	);
}

std::vector<float> Matrix::sub_matrix_vector(int remove_row, int remove_col) const
{
	// Seperates functionality from specified class return to facilitate overloading

	// Check that the requested row and column are in bounds
	if (
		remove_row >= 0 && 
		remove_row < this->m_num_rows_
		)
	{
		// Create a new vector which is smaller by 1 row and 1 column
		std::vector<float> result = std::vector<float>(
			(this->m_num_rows_ - 1) * (this->m_num_columns_ - 1), 
			0.0f
			);

		int index = 0;

		for (int r = 0; r < this->m_num_rows_; r++)
		{
			if (r != remove_row)
			{
				for (int c = 0; c < this->m_num_columns_; c++)
				{
					if (c != remove_col)
					{
						// In order to skip certain rows and columns, an index variable 
						// is stored seperately of the loop variables.
						// The data is retrieved from the internal vector using the actual index (r + c),
						// while the new index is assigned using the seperate variable.
						result[index] = this->m_data_[(r *  this->m_num_columns_) + c];
						++index;
					}
				}
			}

		}

		return result;
	}
	else
	{
		// Requested out of bounds row or column
		throw std::out_of_range(
			"The matrix does not have the requested row or column (" +
			std::to_string(remove_row) +
			", " +
			std::to_string(remove_col) +
			")."
		);
	}
}

// Sub Matrix

Matrix Matrix::sub_matrix(int remove_row, int remove_col) const
{
	return Matrix(
		this->m_num_rows_ - 1, 
		this->sub_matrix_vector(remove_row, remove_col)
	);
}

// Minor
float Matrix::minor(int row, int col) const
{
	// Determinant of the submatrix
	return (this->sub_matrix(row, col)).determinant();
}

float Matrix::cofactor(int row, int col) const
{
	float minor = this->minor(row, col);
	if (((row + col) % 2) == 0)
	{
		return minor;
	}
	else
	{
		return -minor;
	}
}

float Matrix::determinant() const
{
	if (this->m_num_columns_ == 2)
	{
		return (this->m_data_[0] * this->m_data_[3]) - (this->m_data_[1] * this->m_data_[2]);
	}
	else
	{
		float result = 0.0f;
		for (int i = 0; i < this->get_num_columns(); i++)
		{
			float element = this->m_data_[i];
			float det = this->cofactor(0, i);

			result += (element * det);
		}
		return result;
	}
	}

bool Matrix::is_invertable() const
{
	return ! (flt_cmp(this->determinant(), 0.0f));
}

std::vector<float> Matrix::inverse_vector() const
{
	// Seperates functionality from specified class return to facilitate overloading
	float det = this->determinant();
	if (flt_cmp(det, 0.0f))
	{
		// Requested out of bounds row or column
		throw NoninvertableMatrix(*this);
	}
	std::vector<float> result = std::vector<float>(this->m_num_rows_ * this->m_num_columns_);

	// Transposed vector of cofactors devided by the original Matrix's determinant

	for (int r = 0; r < this->m_num_rows_; r++)
	{
		for (int c = 0; c < this->m_num_columns_; c++)
		{
			result[(c *  this->m_num_rows_) + r] = cofactor(r, c) / det;
		}
	}

	return result;
}

Matrix Matrix::inverse() const
{
	return Matrix(this->m_num_rows_, this->inverse_vector());
}

std::string Matrix::to_string() const
{
	int current = 0;
	int col = 1;

	int last = (this->m_num_rows_ * this->m_num_columns_) - 1;

	std::string result = "([";

	// Iterates through the internal vector, adding formatting when conditions are met.
	for (float v : *this)
	{
		std::string vs = std::to_string(v);
		if (col == this->m_num_columns_ && current != last)
		{
			col = 0;
			result += (vs + "], [");
		}
		else if (current == last)
		{
			result += (vs + "])");
		}
		else
		{
			result += (vs + ", ");
		}
		++col;
		++current;
	}
	return result;
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

// ------------------------------------------------------------------------
// Overloaded Operators
// ------------------------------------------------------------------------

// Overloaded Operators

float & Matrix::operator[](int index)
{
	return this->m_data_[index];
}

// Output
std::ostream & operator<<(std::ostream & os, const Matrix & m)
{
	int current = 0;
	int col = 1;

	int num_cols = m.get_num_columns();
	int last = (m.get_num_rows() * num_cols) - 1;

	// Iterates through the internal vector, adding formatting when conditions are met.
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
			os << v << "])";
		}
		else
		{
			os << v << ", ";
		}
		++col;
		++current;
	}
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
// Private
// ------------------------------------------------------------------------

// Converts x,y cartesian coords to an index in the internal vector
int Matrix::m_index_from_coordinates_(int row, int col) const
{
	return (row *  this->m_num_columns_) + col;
}

// ------------------------------------------------------------------------
//
// Matrix2
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Matrix2::Matrix2() : Matrix(2)
{
}

Matrix2::Matrix2(float fill) : Matrix(2, fill)
{
}

Matrix2::Matrix2(const std::vector<float> v) : Matrix(2, v)
{
}

// ------------------------------------------------------------------------
// Factories
// ------------------------------------------------------------------------

Matrix2 Matrix2::Identity()
{
	Matrix2 result = Matrix2();
	result.generate_identity();
	return result;
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<float> Matrix2::get_row(int row) const
{
	int num_col = this->get_num_columns();
	float x, y;
	x = this->get(row * num_col);
	y = this->get((row * num_col) + 1);
	return { x, y };
}

std::vector<float> Matrix2::get_column(int col) const
{
	int num_row = this->get_num_rows();
	float x, y;
	x = this->get(col);
	y = this->get(col + num_row);
	return { x, y };
}

float Matrix2::determinant() const
{
	return (this->get(0) * this->get(3)) - (this->get(1) * this->get(2));
}

Matrix2 Matrix2::inverse() const
{
	return Matrix2(this->inverse_vector());
}

Matrix2 Matrix2::transpose() const
{
	return Matrix2(this->transpose_vector());
}

// ------------------------------------------------------------------------
//
// Matrix3
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Matrix3::Matrix3() : Matrix(3)
{
}

Matrix3::Matrix3(float fill) : Matrix(3, fill)
{
}

Matrix3::Matrix3(const std::vector<float> v) : Matrix(3, v)
{
}

// ------------------------------------------------------------------------
// Factories
// ------------------------------------------------------------------------

Matrix3 Matrix3::Identity()
{
	Matrix3 result = Matrix3();
	result.generate_identity();
	return result;
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<float> Matrix3::get_row(int row) const
{
	int num_col = this->get_num_columns();
	float x, y, z;
	x = this->get(row * num_col);
	y = this->get((row * num_col) + 1);
	z = this->get((row * num_col) + 2);
	return { x, y, z };
}

std::vector<float> Matrix3::get_column(int col) const
{
	int num_row = this->get_num_rows();
	float x, y, z;
	x = this->get(col);
	y = this->get(col + num_row);
	z = this->get(col + (num_row * 2));
	return { x, y, z };
}

// Submatrix
Matrix2 Matrix3::sub_matrix3(int remove_row, int remove_col) const
{
	return Matrix2(this->sub_matrix_vector(remove_row, remove_col));
}

// Minor
float Matrix3::minor(int row, int col) const
{
	// Determinant of the submatrix
	return (this->sub_matrix3(row, col)).determinant();
}

float Matrix3::cofactor(int row, int col) const
{
	float minor = this->minor(row, col);
	if (((row + col) % 2) == 0)
	{
		return minor;
	}
	else
	{
		return -minor;
	}
}

float Matrix3::determinant() const
{
	float result = 0.0f;
	for (int i = 0; i < this->get_num_columns(); i++)
	{
		float element = this->get(i);
		float det = this->cofactor(0, i);

		result += (element * det);
	}
	return result;
}

Matrix3 Matrix3::inverse() const
{
	return Matrix3(this->inverse_vector());
}

Matrix3 Matrix3::transpose() const
{
	return Matrix3(this->transpose_vector());
}

// ------------------------------------------------------------------------
//
// Matrix4
//
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

Matrix4::Matrix4(): Matrix(4)
{
}

Matrix4::Matrix4(float fill) : Matrix(4, fill)
{
}

Matrix4::Matrix4(const std::vector<float> v) : Matrix(4, v)
{
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

Matrix4 Matrix4::Translation(float x, float y, float z)
{
	Matrix4 result = Matrix4::Identity();
	result[3] = x;
	result[7] = y;
	result[11] = z;
	return result;
}

Matrix4 Matrix4::Translation(Tuple p)
{
	return Matrix4::Translation(p.x, p.y, p.z);
}

Matrix4 Matrix4::Scaling(float x, float y, float z)
{
	Matrix4 result = Matrix4::Identity();
	result[0] = x;
	result[5] = y;
	result[10] = z;
	return result;
}

Matrix4 Matrix4::Scaling(Tuple s)
{
	return Matrix4::Scaling(s.x, s.y, s.z);
}

Matrix4 Matrix4::Rotation_X(float radians)
{
	Matrix4 result = Matrix4::Identity();
	result[5] = cos(radians);
	result[6] = -(sin(radians));
	result[9] = sin(radians);
	result[10] = cos(radians);
	return result;
}

Matrix4 Matrix4::Rotation_Y(float radians)
{
	Matrix4 result = Matrix4::Identity();
	result[0] = cos(radians);
	result[2] = sin(radians);
	result[8] = -(sin(radians));
	result[10] = cos(radians);
	return result;
}

Matrix4 Matrix4::Rotation_Z(float radians)
{
	Matrix4 result = Matrix4::Identity();
	result[0] = cos(radians);
	result[1] = -(sin(radians));
	result[4] = sin(radians);
	result[5] = cos(radians);
	return result;
}

Matrix4 Matrix4::Shear(float xy, float xz, float yx, float yz, float zx, float zy)
{
	Matrix4 result = Matrix4::Identity();
	result[1] = xy;
	result[2] = xz;
	result[4] = yx;
	result[6] = yz;
	result[8] = zx;
	result[9] = zy;
	return result;
}

Matrix4 Matrix4::ViewTransform(Tuple from, Tuple to, Tuple up)
{
	Tuple forward = (to - from).normalize();
	Tuple left = Tuple::cross(forward, up.normalize());
	Tuple true_up = Tuple::cross(left, forward);

	Matrix4 orientation = Matrix4({
		left.x, left.y, left.z, 0.0f,
		true_up.x, true_up.y, true_up.z, 0.0f,
		-forward.x, -forward.y, -forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		});

	return orientation * Matrix4::Translation(-from.x, -from.y, -from.z);
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

std::vector<float> Matrix4::get_row(int row) const
{
	int num_col = this->get_num_columns();
	float x, y, z, w;
	x = this->get(row * num_col);
	y = this->get((row * num_col) + 1);
	z = this->get((row * num_col) + 2);
	w = this->get((row * num_col) + 3);
	return { x, y, z, w };
}

std::vector<float> Matrix4::get_column(int col) const
{
	int num_row = this->get_num_rows();
	float x, y, z, w;
	x = this->get(col);
	y = this->get(col + num_row);
	z = this->get(col + (num_row * 2));
	w = this->get(col + (num_row * 3));
	return { x, y, z, w };
}

Tuple Matrix4::get_row_tuple(int row) const
{
	int num_col = this->get_num_columns();
	float x, y, z, w;
	x = this->get(row * num_col);
	y = this->get((row * num_col) + 1);
	z = this->get((row * num_col) + 2);
	w = this->get((row * num_col) + 3);
	return Tuple(x, y, z, w);
}

Tuple Matrix4::get_column_tuple(int col) const
{
	int num_row = this->get_num_rows();
	float x, y, z, w;
	x = this->get(col);
	y = this->get(col + num_row);
	z = this->get(col + (num_row * 2));
	w = this->get(col + (num_row * 3));
	return Tuple(x, y, z, w);
}

Matrix3 Matrix4::sub_matrix4(int remove_row, int remove_col) const
{
	return Matrix3(this->sub_matrix_vector(remove_row, remove_col));
}

// Minor
float Matrix4::minor(int row, int col) const
{
	// Determinant of the submatrix
	return (this->sub_matrix4(row, col)).determinant();
}

float Matrix4::cofactor(int row, int col) const
{
	float minor = this->minor(row, col);
	if (((row + col) % 2) == 0)
	{
		return minor;
	}
	else
	{
		return -minor;
	}
}

float Matrix4::determinant() const
{
	float result = 0.0f;
	for (int i = 0; i < this->get_num_columns(); i++)
	{
		float element = this->get(i);
		float det = this->cofactor(0, i);

		result += (element * det);
	}
	return result;
}

Matrix4 Matrix4::inverse() const
{
	return Matrix4(this->inverse_vector());
}

Matrix4 Matrix4::transpose() const
{
	return Matrix4(this->transpose_vector());
}

Tuple Matrix4::position()
{
	return Tuple::Point(
		this->operator[](3),
		this->operator[](7),
		this->operator[](11)
	);
}

Tuple Matrix4::scale()
{
	Tuple s0 = (this->get_row_tuple(0));
	s0.w = 0;
	Tuple s1 = (this->get_row_tuple(1));
	s1.w = 0;
	Tuple s2 = (this->get_row_tuple(2));
	s2.w = 0;

	return Tuple::Point(s0.magnitude(), s1.magnitude(), s2.magnitude());
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
			Tuple row = this->get_row_tuple(r);
			Tuple col = right_matrix.get_column_tuple(c);

			float dot_product = Tuple::dot(row, col);

			// Set the result of the dot product to the position in the new Matrix
			result.set((r * this->get_num_rows()) + c, dot_product);
		}
	};

	return result;
}

// External Overloaded Operators
Tuple Matrix4::operator*(const Tuple & tuple) const
{
	float x, y, z, w;

	x = Tuple::dot(tuple, this->get_row_tuple(0));
	y = Tuple::dot(tuple, this->get_row_tuple(1));
	z = Tuple::dot(tuple, this->get_row_tuple(2));
	w = Tuple::dot(tuple, this->get_row_tuple(3));

	return Tuple(x, y, z, w);
}

// ------------------------------------------------------------------------
// Exceptions
// ------------------------------------------------------------------------

NoninvertableMatrix::NoninvertableMatrix(Matrix m) : 
	logic_error(
		"Matrix: " + 
		m.to_string() + 
		" has a determinate of 0.0 and is therefore unable to be inverted."
	)
{
}
