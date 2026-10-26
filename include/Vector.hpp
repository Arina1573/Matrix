#pragma once

#include <cstddef>
#include <initializer_list>
#include <iosfwd>
#include <vector>

class Vector {
private:
    std::vector<double> data_;
public:
    Vector() = default;
    explicit Vector(std::size_t n, double value = 0.0);
    Vector(std::initializer_list<double> values);
    explicit Vector(std::vector<double> values);

    std::size_t size() const noexcept;
    double* data() noexcept;
    const double* data() const noexcept;
    double norm() const noexcept;
    double dot(const Vector& rhs) const;
    double& operator[](std::size_t i);
    const double& operator[](std::size_t i) const;

    Vector& operator+=(const Vector& rhs);
    Vector& operator-=(const Vector& rhs);
    Vector& operator*=(double scalar) noexcept;
    Vector& operator/=(double scalar);
};

Vector operator+(Vector lhs, const Vector& rhs);
Vector operator-(Vector lhs, const Vector& rhs);
Vector operator-(Vector v);
Vector operator*(Vector v, double scalar);
Vector operator*(double scalar, Vector v);
Vector operator/(Vector v, double scalar);
std::ostream& operator<<(std::ostream& out, const Vector& v);

