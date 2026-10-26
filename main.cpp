#include "Matrix.hpp"
#include "Vector.hpp"

#include <cstddef>
#include <iostream>
#include <stdexcept>


Vector minimalResidual(
    const Matrix& A,
    const Vector& b,
    double tolerance = 1e-10,
    std::size_t maxIterations = 10000
) {
    if (A.rows() != A.cols() || A.rows() != b.size()) {
        throw std::invalid_argument("Invalid system dimensions");
    }

    Vector x(b.size(), 0.0);
    Vector r = b - A * x;

    double residual = r.norm();
    std::size_t iterations = 0;

    while (residual > tolerance && iterations < maxIterations) {
        Vector Ar = A * r;

        double numerator = r.dot(Ar);
        double denominator = Ar.dot(Ar);

        if (denominator == 0.0) {
            throw std::runtime_error("Division by zero");
        }

        double alpha = numerator / denominator;
        x = x + alpha * r;
        r = r - alpha * Ar;
        ++iterations;

        if (iterations % 64 == 0) {
            r = b - A * x;
        }

        residual = r.norm();
    }

    residual = (b - A * x).norm();

    if (residual > tolerance) {
        throw std::runtime_error("Minimal residual method did not converge");
    }

    std::cout << "Iterations: " << iterations << '\n';
    std::cout << "Residual: " << residual << '\n';

    return x;
}


int main() {
    Matrix A1{
        {4, 1},
        {1, 3}
    };

    Vector b1{1, 2};

    Vector x1 = minimalResidual(A1, b1);

    std::cout << "Example 1:\n";
    std::cout << "Solution: " << x1 << "\n\n";

    Matrix A2{
        {5, 1, 0},
        {1, 4, 1},
        {0, 1, 3}
    };

    Vector b2{6, 6, 4};

    Vector x2 = minimalResidual(A2, b2);

    std::cout << "Example 2:\n";
    std::cout << "Solution: " << x2 << '\n';

    return 0;
}