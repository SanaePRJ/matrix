#include <iostream>
#include "matrixLib/matrix"

int main()
{
    std::cout << "Hello World!\n";
    Matrix<float> m1;

    m1 = {
        {1.0f, 2.0f},
        {3.0f, 4.0f}
    };

    Matrix<float> m2 = m1;

    Matrix<float> m3 = std::move(m1);

    Matrix<float> m4;
    m4 << Matrix<float>{
        {5.0f, 6.0f},
        {7.0f, 8.0f}
    };

    auto firstRow = m4[0][0];

    std::cout << firstRow << std::endl;
} 