#include "Vector.hpp"

#include <cmath>
#include <ostream>
#include <stdexcept>
#include <utility>

Vector::Vector(std::size_t n, double value) : data_(n, value) {}
Vector::Vector(std::initializer_list<double> values) : data_(values) {}
Vector::Vector(std::vector<double> values) : data_(std::move(values)) {}

std::size_t Vector::size() const noexcept { return data_.size(); }
double* Vector::data() noexcept { return data_.data(); }
const double* Vector::data() const noexcept { return data_.data(); }

double Vector::norm() const noexcept {
    double sum = 0;
    for (double x : data_) sum = std::hypot(sum, x);
    return sum;
}
double Vector::dot(const Vector& rhs) const {
    if (size() != rhs.size()) throw std::invalid_argument("Vector dot: size mismatch");
    double sum = 0;
    for (std::size_t i = 0; i < size(); ++i) sum += data_[i] * rhs.data_[i];
    return sum;
}
double& Vector::operator[](std::size_t i) { return data_.at(i); }
const double& Vector::operator[](std::size_t i) const { return data_.at(i); }

Vector& Vector::operator+=(const Vector& rhs) {
    if (size() != rhs.size()) throw std::invalid_argument("Vector +: size mismatch");
    for (std::size_t i = 0; i < size(); ++i) data_[i] += rhs.data_[i];
    return *this;
}
Vector& Vector::operator-=(const Vector& rhs) {
    if (size() != rhs.size()) throw std::invalid_argument("Vector -: size mismatch");
    for (std::size_t i = 0; i < size(); ++i) data_[i] -= rhs.data_[i];
    return *this;
}
Vector& Vector::operator*=(double scalar) noexcept {
    for (double& x : data_) x *= scalar;
    return *this;
}
Vector& Vector::operator/=(double scalar) {
    if (scalar == 0.0) throw std::invalid_argument("Vector division by zero");
    for (double& x : data_) x /= scalar;
    return *this;
}

Vector operator+(Vector lhs, const Vector& rhs) { return lhs += rhs; }
Vector operator-(Vector lhs, const Vector& rhs) { return lhs -= rhs; }
Vector operator-(Vector value) { return value *= -1.0; }
Vector operator*(Vector value, double scalar) { return value *= scalar; }
Vector operator*(double scalar, Vector value) { return value *= scalar; }
Vector operator/(Vector value, double scalar) { return value /= scalar; }

std::ostream& operator<<(std::ostream& out, const Vector& value) {
    out << '[';
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (i) out << ", ";
        out << value[i];
    }
    return out << ']';
}

