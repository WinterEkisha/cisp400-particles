#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

namespace Matrices {

class Matrix {
public:
  /// Construct a matrix of the specified size.
  /// Initialize each element to 0.
  Matrix(int rows, int cols);

  /// Read element at row i, column j
  /// usage:  double x = a(i,j);
  const double &operator()(int i, int j) const { return m_values.at(i).at(j); }

  /// Assign element at row i, column j
  /// usage:  a(i,j) = x;
  double &operator()(int i, int j) { return m_values.at(i).at(j); }

  int getRows() const { return m_rows; }
  int getCols() const { return m_cols; }

protected:
  vector<vector<double>> m_values;

private:
  int m_rows;
  int m_cols;
};

/// Add each corresponding element.
/// usage:  c = a + b;
Matrix operator+(const Matrix &a, const Matrix &b);

/// Matrix multiply.  See description.
/// usage:  c = a * b;
Matrix operator*(const Matrix &a, const Matrix &b);

/// Matrix comparison.  See description.
/// usage:  a == b
bool operator==(const Matrix &a, const Matrix &b);

/// Matrix comparison.  See description.
/// usage:  a != b
bool operator!=(const Matrix &a, const Matrix &b);

/// Output matrix.
/// Separate columns by ' ' and rows by '\n'
ostream &operator<<(ostream &os, const Matrix &a);

/*******************************************************************************/

/// 2D rotation matrix
/// usage:  A = R * A rotates A theta radians counter-clockwise
class RotationMatrix : public Matrix {
public:
  /// theta represents the angle of rotation in radians, counter-clockwise
  RotationMatrix(double theta);
};

/// 2D scaling matrix
/// usage:  A = S * A expands or contracts A by the specified scaling factor
class ScalingMatrix : public Matrix {
public:
  /// scale represents the size multiplier
  ScalingMatrix(double scale);
};

/// 2D Translation matrix
/// usage:  A = T + A will shift all coordinates of A by (xShift, yShift)
class TranslationMatrix : public Matrix {
public:
  /// parameters are xShift, yShift, and nCols
  /// nCols represents the number of columns in the matrix
  /// where each column contains one (x,y) coordinate pair
  TranslationMatrix(double xShift, double yShift, int nCols);
};


} // namespace Matrices

#endif
