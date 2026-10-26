#include "Matrix.hpp"

#include <algorithm>
#include <ostream>
#include <stdexcept>
#include <utility>

Matrix::Matrix(std::size_t n) : Matrix(n, n) {}

Matrix::Matrix(std::size_t rows, std::size_t cols, double value)
    : rows_(rows), cols_(cols), data_(rows * cols, value) {}

Matrix::Matrix(std::size_t rows, std::size_t cols, std::initializer_list<double> flat)
    : rows_(rows), cols_(cols), data_(flat) {
    if (flat.size() != rows * cols)
        throw std::invalid_argument("Matrix: flat element count mismatch");
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> rows) {
    rows_ = rows.size();
    cols_ = rows_ ? rows.begin()->size() : 0;
    data_.reserve(rows_ * cols_);
    for (const auto& row : rows) {
        if (row.size() != cols_) throw std::invalid_argument("Matrix: ragged rows");
        data_.insert(data_.end(), row.begin(), row.end());
    }
}

Matrix::Matrix(const std::vector<std::vector<double>>& rows) {
    rows_ = rows.size();
    cols_ = rows_ ? rows.front().size() : 0;
    data_.reserve(rows_ * cols_);
    for (const auto& row : rows) {
        if (row.size() != cols_) throw std::invalid_argument("Matrix: ragged rows");
        data_.insert(data_.end(), row.begin(), row.end());
    }
}
Matrix::Matrix(Matrix&& other) noexcept {
    swap(*this, other);
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        Matrix temp(std::move(other));
        swap(*this, temp);
    }
    return *this;
}

void swap(Matrix& a, Matrix& b) noexcept {
    std::swap(a.rows_, b.rows_);
    std::swap(a.cols_, b.cols_);
    a.data_.swap(b.data_);
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        Matrix temp(other);
        swap(*this, temp);
    }
    return *this;
}

std::size_t Matrix::rows() const noexcept { return rows_; }
std::size_t Matrix::cols() const noexcept { return cols_; }
double* Matrix::data() noexcept { return data_.data(); }
const double* Matrix::data() const noexcept { return data_.data(); }

double& Matrix::operator()(std::size_t row, std::size_t col) {
    if (row >= rows_ || col >= cols_) throw std::out_of_range("Matrix index");
    return data_[row * cols_ + col];
}
const double& Matrix::operator()(std::size_t row, std::size_t col) const {
    if (row >= rows_ || col >= cols_) throw std::out_of_range("Matrix index");
    return data_[row * cols_ + col];
}

Matrix& Matrix::operator+=(const Matrix& rhs) {
    if (rows_ != rhs.rows_ || cols_ != rhs.cols_)
        throw std::invalid_argument("Matrix +: dimension mismatch");
    for (std::size_t i = 0; i < data_.size(); ++i) data_[i] += rhs.data_[i];
    return *this;
}
Matrix& Matrix::operator-=(const Matrix& rhs) {
    if (rows_ != rhs.rows_ || cols_ != rhs.cols_)
        throw std::invalid_argument("Matrix -: dimension mismatch");
    for (std::size_t i = 0; i < data_.size(); ++i) data_[i] -= rhs.data_[i];
    return *this;
}
Matrix& Matrix::operator*=(double scalar) noexcept {
    for (double& value : data_) value *= scalar;
    return *this;
}

Matrix Matrix::transposed() const {
    Matrix result(cols_, rows_);
    for (std::size_t i = 0; i < rows_; ++i)
        for (std::size_t j = 0; j < cols_; ++j)
            result.data_[j * rows_ + i] = data_[i * cols_ + j];
    return result;
}

Matrix operator+(Matrix lhs, const Matrix& rhs) { return lhs += rhs; }
Matrix operator-(Matrix lhs, const Matrix& rhs) { return lhs -= rhs; }
Matrix operator-(Matrix matrix) { return matrix *= -1.0; }
Matrix operator*(Matrix matrix, double scalar) { return matrix *= scalar; }
Matrix operator*(double scalar, Matrix matrix) { return matrix *= scalar; }

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
    if (lhs.cols() != rhs.rows())
        throw std::invalid_argument("Matrix * Matrix: dimension mismatch");
    Matrix out(lhs.rows(), rhs.cols());
    const auto* a = lhs.data();
    const auto* b = rhs.data();
    auto* c = out.data();
    const auto m = lhs.rows();
    const auto kSize = lhs.cols();
    const auto n = rhs.cols();
    if (m == 0 || n == 0 || kSize == 0) return out;
    for (std::size_t i = 0; i < m; ++i) {
        double* outRow = c + i * n;
        for (std::size_t k = 0; k < kSize; ++k) {
            const double aik = a[i * kSize + k];
            const double* rhsRow = b + k * n;
            for (std::size_t j = 0; j < n; ++j)
                outRow[j] += aik * rhsRow[j];
        }
    }
    return out;
}

Vector operator*(const Matrix& lhs, const Vector& rhs) {
    if (lhs.cols() != rhs.size())
        throw std::invalid_argument("Matrix * Vector: dimension mismatch");
    Vector out(lhs.rows());
    if (lhs.cols() == 0) return out;
    const double* a = lhs.data();
    const double* x = rhs.data();
    double* y = out.data();
    for (std::size_t i = 0; i < lhs.rows(); ++i) {
        double sum = 0.0;
        const double* row = a + i * lhs.cols();
        for (std::size_t j = 0; j < lhs.cols(); ++j) sum += row[j] * x[j];
        y[i] = sum;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
    for (std::size_t i = 0; i < matrix.rows(); ++i) {
        out << '[';
        for (std::size_t j = 0; j < matrix.cols(); ++j) {
            if (j) out << ", ";
            out << matrix(i, j);
        }
        out << ']';
        if (i + 1 < matrix.rows()) out << '\n';
    }
    return out;
}

