#include <iostream>
#include "MatrixLib/matrix"

void testMatrixCreation() {
    Matrix<int> mat1 = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    std::cout << "Initial matrix:" << std::endl;
    std::cout << mat1 << std::endl;
}

void testMatrixAddition() {
    Matrix<int> mat1 = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    Matrix<int> mat2 = { {9, 8, 7}, {6, 5, 4}, {3, 2, 1} };
    Matrix<int> result = mat1 + mat2;

    std::cout << "Matrix Addition:" << std::endl;
    std::cout << result << std::endl;
}

void testMatrixMultiplication() {
    Matrix<int> mat1 = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    Matrix<int> mat2 = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
    Matrix<int> result = mat1 * mat2;

    std::cout << "Matrix Multiplication:" << std::endl;
    std::cout << result << std::endl;
}

void testMatrixTranspose() {
    Matrix<int> mat1 = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    Matrix<int> result = mat1.transpose();

    std::cout << "Matrix Transpose:" << std::endl;
    std::cout << result << std::endl;
}

void testMatrixRowSwap() {
    Matrix<int> mat1 = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    mat1.swapRow(0, 2);

    std::cout << "Matrix Row Swap:" << std::endl;
    std::cout << mat1 << std::endl;
}

void testMatrixResize() {
    Matrix<int> mat1 = { {1, 2}, {3, 4} };
    mat1.resize(3, 3);

    std::cout << "Matrix Resize:" << std::endl;
    std::cout << mat1 << std::endl;
}

int main() {
    testMatrixCreation();
    testMatrixAddition();
    testMatrixMultiplication();
    testMatrixTranspose();
    testMatrixRowSwap();
    testMatrixResize();

    return 0;
}