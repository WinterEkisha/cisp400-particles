#include "Matrices.h"

#include <iomanip>
#include <stdexcept>
#include <vector>

namespace Matrices {


Matrix::Matrix(int rows, int cols)
    : m_values(
          std::vector<std::vector<double>>(rows, std::vector<double>(cols, 0))),
      m_rows(rows), m_cols(cols) {};


Matrix operator+(const Matrix &a, const Matrix &b) {
  if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) {
    throw std::domain_error("Error: mismatched dimensions");
  }

  Matrix result(a.getRows(), a.getCols());

  for (int i = 0; i < a.getRows(); i++) {
    for (int j = 0; j < a.getCols(); j++) {
      result(i, j) = a(i, j) + b(i, j);
    }
  }

  return result;
}


Matrix operator*(const Matrix &a, const Matrix &b) {
  if (a.getCols() != b.getRows()) {
    throw std::domain_error("Error: mismatched inner dimensions");
  }

  Matrix result(a.getRows(), b.getCols());

  for (int i = 0; i < a.getRows(); i++) {
    for (int k = 0; k < b.getCols(); k++) {
      float sum = 0;
      for (int j = 0; j < a.getCols(); j++) {
        sum += a(i, j) * b(j, k);
      }
      result(i, k) = sum;
    }
  }

  return result;
}


bool operator==(const Matrix &a, const Matrix &b) {
  if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) {
    return false;
  }

  Matrix result(a.getRows(), a.getCols());

  for (int i = 0; i < a.getRows(); i++) {
    for (int j = 0; j < a.getCols(); j++) {
      if (a(i, j) != b(i, j)) {
        return false;
      }
    }
  }

  return true;
}


bool operator!=(const Matrix &a, const Matrix &b) { return !(a == b); }


std::ostream &operator<<(std::ostream &os, const Matrix &a) {
  for (int row = 0; row < a.getRows(); row++) {
    for (int col = 0; col < a.getRows(); col++) {
      os << std::setw(10) << a(row, col) << ' ';
    }
    os << '\n';
  }
  return os;
}


RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2) {
  const double cosTheta = cos(theta);
  const double sinTheta = sin(theta);

  m_values[0][0] = cosTheta;
  m_values[0][1] = -sinTheta;
  m_values[1][0] = sinTheta;
  m_values[1][1] = cosTheta;
}


ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2) {

  m_values[0][0] = scale;
  m_values[1][1] = scale;
}


TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols)
    : Matrix(2, nCols) {

  for (size_t i = 0; i < nCols; i++) {
    m_values[i][0] = xShift;
    m_values[i][1] = yShift;
  }
}


} // namespace Matrices
