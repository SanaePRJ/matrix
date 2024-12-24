#include "matrixLib/matrix"
#include <iostream>
#include <vector>

int main() {
	Matrix<float> matrix1({ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f} });
	Matrix<float> matrix2({ {7.0f, 8.0f, 9.0f}, {10.0f, 11.0f, 12.0f} });

	std::cout << matrix1.scalarMul(3.0f) << std::endl;
}