//-----------------------------------------------------------------------------
// This is the header file for the LMatrix class.  Specific class details can
// be found in the "LMatrix.cpp" file.  This is the same as the Matrix class
// except its elements are long doubles.
//-----------------------------------------------------------------------------

#ifndef LMATRIX_H
#define LMATRIX_H

#include <vector>

using namespace std;

class LMatrix {

  // friend arithmetic operators involving a scalar k and matrix A
  friend LMatrix operator*(const long double k, const LMatrix & A);  // k * A
  friend LMatrix operator*(const LMatrix & A, const long double k);  // A * k
  friend LMatrix operator^(const LMatrix & A, const int k);     // A^k

  public:
    // constructors
    LMatrix();              // constructs a 1x1 matrix of zeroes
    LMatrix(int n, int m);  // constructs an nxm matrix of zeroes
    
    // member access functions
    int numRows() const;                    // returns number of rows
    int numCols() const;                    // returns number of columns
    const long double & operator()(int i, int j) const;  // returns the element aij

    // population functions
    void setElement(int i, int j, long double aij);  // sets value of element aij
    long double & operator()(int i, int j);  // sets value of element A(i,j)
    void addRow(vector<long double> row);  // adds a row at the end of the Matrix

    // other member functions
    void print() const;  // prints the matrix

    // assignment operators for matrix objects
    const LMatrix & operator=(const LMatrix & rhs);
    const LMatrix & operator+=(const LMatrix & rhs);
    const LMatrix & operator-=(const LMatrix & rhs);
    const LMatrix & operator*=(const LMatrix & rhs);

  private:
    vector< vector<long double> > M;  // 2D vector containing matrix elements
    int rows;                    // number of rows
    int cols;                    // number of columns

};

// arithmetic operators for matrix objects A and B
LMatrix operator+(const LMatrix & lhs, const LMatrix & rhs);  // A + B
LMatrix operator-(const LMatrix & lhs, const LMatrix & rhs);  // A - B
LMatrix operator*(const LMatrix & lhs, const LMatrix & rhs);  // A * B

// non-member functions
LMatrix identity(int n);  // creates an nxn identity matrix

#endif