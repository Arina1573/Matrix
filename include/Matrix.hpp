#pragma once

#include "Vector.hpp"
#include <cstddef>
#include <initializer_list>
#include <iosfwd>
#include <vector>

class Matrix {
    private:
        std::size_t rows_ = 0;
        std::size_t cols_ = 0;
        std::vector<double> data_;
        
    public:
        Matrix() = default;
        explicit Matrix(std::size_t n);
        Matrix(std::size_t rows, std::size_t cols, double value = 0.0);
        Matrix(std::size_t rows, std::size_t cols, std::initializer_list<double> flat);
        Matrix(std::initializer_list<std::initializer_list<double>> rows);
        explicit Matrix(const std::vector<std::vector<double>>& rows);

        Matrix(const Matrix&) = default;
        Matrix& operator=(const Matrix& other);
        Matrix(Matrix&& other) noexcept;
        Matrix& operator=(Matrix&& other) noexcept;
        ~Matrix() = default;

        std::size_t rows() const noexcept;
        std::size_t cols() const noexcept;
        double* data() noexcept;
        const double* data() const noexcept;

        double& operator()(std::size_t row, std::size_t col);
        const double& operator()(std::size_t row, std::size_t col) const;
        Matrix& operator+=(const Matrix& rhs);
        Matrix& operator-=(const Matrix& rhs);
        Matrix& operator*=(double scalar) noexcept;
        Matrix transposed() const;

        friend void swap(Matrix& a, Matrix& b) noexcept;
};

void swap(Matrix& a, Matrix& b) noexcept;
Matrix operator+(Matrix lhs, const Matrix& rhs);
Matrix operator-(Matrix lhs, const Matrix& rhs);
Matrix operator-(Matrix m);
Matrix operator*(Matrix m, double scalar);
Matrix operator*(double scalar, Matrix m);
Matrix operator*(const Matrix& lhs, const Matrix& rhs);
Vector operator*(const Matrix& lhs, const Vector& rhs);
std::ostream& operator<<(std::ostream& out, const Matrix& m);

