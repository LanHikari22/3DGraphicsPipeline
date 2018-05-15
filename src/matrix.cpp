#include "matrix.h"
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

// Parameterized constructor
matrix::matrix(unsigned int rows, unsigned int cols):rows(rows),cols(cols) 
{  
	if (rows < 1 || cols < 1)
	{
		throw matrixException("p-constructor bad arguments");
	}
	
	// Implementation: Single array of size rows*cols
	the_matrix = new double[rows*cols];
	
	// Zero out the matrix!
	this->clear();
}

// Copy constructor
matrix::matrix(const matrix& from):rows(from.rows),cols(from.cols)
{
	// Create a new the_matrix in the heap!
	the_matrix = new double[rows*cols];
	
	// Copy the matrix data!
	for(unsigned int r=0; r<this->rows; r++)
	{
		for(unsigned int c=0; c<this->cols; c++)
		{
			this->the_matrix[this->cols*r + c] = from[r][c];
		}
	}

}

// Destructor
matrix::~matrix()
{
	delete [] the_matrix;
	// stub
}

// Assignment operator
matrix& matrix::operator=(const matrix& rhs)
{
	// Copy rhs's cols and rows
	this->rows = rhs.rows;
	this->cols = rhs.cols;
	
	// replace the_matrix with a new one
	delete [] this->the_matrix;
	this->the_matrix = new double[this->rows*this->cols];
	for(unsigned int r=0; r<this->rows; r++)
	{
		for(unsigned int c=0; c<this->cols; c++)
		{
			this->the_matrix[this->cols*r + c] = rhs[r][c];
		}
	}
	
	return *this;
}

// Named constructor (static)
matrix matrix::identity(unsigned int size)
{
	if (size < 1)
	{
		throw matrixException("matrix size less than 1");
	}
	
	// use p-constructor
	matrix m(size, size);
	for (unsigned int i=0; i<size; i++)
	{
		m[i][i] = 1;
	}
	return m;
}

int matrix::getRows() const
{
	return rows;
}

int matrix::getCols() const
{
	return cols;
}

// Binary operations
matrix matrix::operator+(const matrix& rhs) const
{
	// Adding is simply element by element. Both rows and columns must be identical
	if (this->rows != rhs.rows || this->cols != rhs.cols)
	{
		throw matrixException("Added matrices must be of exact dimensions");
	}
	
	// Add into a new matrix!
	matrix retVal(rhs);
	for (unsigned int r=0; r<this->rows; r++)
	{
		for (unsigned int c=0; c<this->cols; c++)
		{
			retVal[r][c] += (*this)[r][c];
		}
	}
	
	return retVal;
}


matrix matrix::operator*(const matrix& rhs) const
{
	//  A = n x m matrix and B = m × p matrix -> AB = n × p matrix
	// Check compatibility, inner dimensions must match
	if (this->cols != rhs.rows)
	{
		throw matrixException("inner dimensions of multiplied matrices don't match");
	}
	
	// Result is a matrix with size of outer dimensions nxp
	matrix retVal(this->rows, rhs.cols);
		
	// perform multiplication (i don't like this O(n*m*p) algorithm
	for (unsigned int r=0; r<this->rows; r++)
	{
		for (unsigned int rhs_c=0; rhs_c<rhs.cols; rhs_c++)
		{
			// each column of this will be multiplied by each row of rhs. 
			// We know those sides are identical
			// repeat this process for each column of rhs
			
			// each time this loop runs, one element of retVal will be computed: 
			// retVal[r][rhs_c]; Horizontal to vertical multiplication scan!
			for (unsigned int i=0; i<this->cols; i++)
			{
				retVal[r][rhs_c] += (*this)[r][i] * rhs[i][rhs_c];
			}
		}
	}
	
	return retVal;
}

matrix matrix::operator*(const double scale) const
{
	matrix retVal(*this);

	for (unsigned int r=0; r<this->rows; r++)
	{
		for (unsigned int c=0; c<this->cols; c++)
		{
			retVal[r][c] *= scale;
		}
	}	
	return retVal;
}


// Unary operations
matrix matrix::operator~() const
{
	// Create a matrix with swapped rows/cols
	matrix retVal(this->cols, this->rows);
	
	// Copy rows into cols.
	for (unsigned int r=0; r<this->rows; r++)
	{
		for (unsigned int c=0; c<this->cols; c++)
		{
			retVal[c][r] = (*this)[r][c];
		}
	}
	
	return retVal;
}
	

void matrix::clear()
{
	for(unsigned int i=0; i<this->rows*this->cols; i++)
	{
		the_matrix[i] = 0;
	}	
}

matrix::row matrix::operator[](unsigned int row)
{
	if (row > this->rows)
	{
		throw matrixException("Row accessed are out of range");
	}
	
	// all rows are stacked together in 1D array, containing cols elements each
	return matrix::row(this->cols, &this->the_matrix[row*this->cols]); 
}

const matrix::row matrix::operator[](unsigned int row) const
{
	if (row > this->rows)
	{
		throw matrixException("Rows accessed are out of range");
	}
	
	const matrix::row output(this->cols, &this->the_matrix[row*this->cols]);
	return output;
}


std::ostream& matrix::out(std::ostream& os) const
{
	// Matrix form will be consistent with this layout:
	// [[ r0c0 r0c1 r0c2 ]       [[  5000 3.333 60000 ]
	//  [ r1c0 r1c1 r1c2 ]  --->  [     2  5000     1 ]
	//  [ r0c0 r0c1 r0c2 ]]       [     0  2.34  1000 ]]
	// Structure will look good as long as number doesn't exceed 5 chars.
	os << "[";
	for (unsigned int r=0; r<this->rows; r++)
	{
		os << "[";
		for (unsigned int c=0; c<this->cols; c++)
		{
			os << setw(5) << (*this)[r][c] << " "; 
		}
		os << "]";
		if (r < this->rows - 1)
			os << endl << " "; // space added so all ['s match
	}
	os << "]";
	
	return os;	
}



// Global insertion and operator
std::ostream& operator<<(std::ostream& os, const matrix& rhs)
{
	rhs.out(os);
	return os;
}

// Global scalar multiplication
matrix operator*(const double scale, const matrix& rhs)
{
	// Same thing, but just call the other version.
	matrix retval = rhs * scale;
	return retval;
}


matrix::row::row(unsigned int cols, double* the_row): 
		cols(cols), the_row(the_row) 
{
	if (cols < 1)
	{
		throw matrixException("invalid row size -- must be at least 1");
	}
}

double& matrix::row::operator[](unsigned int col)
{
	if (col >= cols)
	{
		throw matrixException("Columns accessed are out of range");
	}
	return the_row[col];
}
	   

double matrix::row::operator[](unsigned int col) const
{
	if (col >= cols)
	{
		throw matrixException("Columns accessed are out of range");
	}
	
	return the_row[col];
}
