#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

#include "matrixLib/matrix"

void testDefaultConstructor() {
    Matrix<int> matrix;
    assert(matrix.rows() == 0);
    assert(matrix.cols() == 0);
    std::cout << "DefaultConstructor test passed." << std::endl;
}

void testInitListConstructor() {
    Matrix<int> matrix({ {1, 2}, {3, 4} });
    assert(matrix.rows() == 2);
    assert(matrix.cols() == 2);
    assert(matrix[0][0] == 1);
    assert(matrix[1][1] == 4);
    std::cout << "InitListConstructor test passed." << std::endl;
}

void testSizeConstructor() {
    Matrix<int> matrix({ 2, 3 });
    assert(matrix.rows() == 2);
    assert(matrix.cols() == 3);
    std::cout << "SizeConstructor test passed." << std::endl;
}

void testCopyConstructor() {
    Matrix<int> original({ {1, 2}, {3, 4} });
    Matrix<int> copy(original);
    assert(copy.rows() == 2);
    assert(copy.cols() == 2);
    assert(copy[0][1] == 2);
    std::cout << "CopyConstructor test passed." << std::endl;
}

void testMoveConstructor() {
    Matrix<int> original({ {1, 2}, {3, 4} });
    Matrix<int> moved(std::move(original));
    assert(moved.rows() == 2);
    assert(moved.cols() == 2);
    assert(moved[1][0] == 3);
    std::cout << "MoveConstructor test passed." << std::endl;
}

void testAssignmentOperator() {
    Matrix<int> matrix1({ {1, 2}, {3, 4} });
    Matrix<int> matrix2;
    matrix2 = matrix1;
    assert(matrix2.rows() == 2);
    assert(matrix2[0][0] == 1);
    std::cout << "AssignmentOperator test passed." << std::endl;
}

void testAddOperator() {
    Matrix<int> matrix1({ {1, 2}, {3, 4} });
    Matrix<int> matrix2({ {5, 6}, {7, 8} });
    Matrix<int> result = matrix1 + matrix2;
    assert(result[0][0] == 6);
    assert(result[1][1] == 12);
    std::cout << "AddOperator test passed." << std::endl;
}

void testTranspose() {
    Matrix<int> matrix({ {1, 2, 3}, {4, 5, 6} });
    Matrix<int> transposed = matrix.transpose();
    assert(transposed.rows() == 3);
    assert(transposed.cols() == 2);
    assert(transposed[0][1] == 4);
    std::cout << "Transpose test passed." << std::endl;
}

void testDeterminant() {
    Matrix<int> matrix({ {1, 2}, {3, 4} });
    assert(matrix.det() == -2);
    std::cout << "Determinant test passed." << std::endl;
}

void testInverse() {
    Matrix<double> matrix({ {4, 7}, {2, 6} });
    Matrix<double> inverse = matrix.inverse();
    assert(std::abs(inverse[0][0] - 0.6) < 1e-9);
    assert(std::abs(inverse[1][1] - 0.4) < 1e-9);
    std::cout << "Inverse test passed." << std::endl;
}

void testLUDecomposition() {
    Matrix<double> matrix({ {4, 3}, {6, 3} });
    auto lu = matrix.luDec();
    assert(lu.size() == 2); // L行列とU行列
    std::cout << "LUDecomposition test passed." << std::endl;
}

void testForEach() {
    Matrix<int> matrix({ {1, 2}, {3, 4} });
    matrix.forEach([](size_t, size_t, int& val) { return val *= 2; });
    assert(matrix[0][0] == 2);
    assert(matrix[1][1] == 8);
    std::cout << "ForEach test passed." << std::endl;
}

void testIdentityMatrix() {
    auto identity = Matrix<int>::identity(3);
    assert(identity.rows() == 3);
    assert(identity[0][0] == 1);
    assert(identity[1][2] == 0);
    std::cout << "IdentityMatrix test passed." << std::endl;
}


static void calcSpeedTest(size_t n, std::function<Matrix<double>(Matrix<double>&, Matrix<double>&)> Func)
{
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703) || __cplusplus >= 201703) && __has_include("execution") // C++17
    using namespace std::chrono;

    std::default_random_engine       engine;      //エンジン
    std::normal_distribution<double> dist(0, 1);  //平均0,標準偏差1

    //n*n行列を作成
    Matrix<double> buf0 = std::pair<size_t, size_t>{ n,n };
    Matrix<double> buf1 = std::pair<size_t, size_t>{ n,n };

    buf0.forEach([&engine, &dist](){return dist(engine); });
    buf1.forEach([&engine, &dist](){return dist(engine); });

    auto calc = [&buf0, &buf1, &engine, &dist, &Func]() {
        auto buf_time0 = system_clock::now();

        Func(buf0, buf1);

        auto buf_time1 = system_clock::now();

        return duration_cast<milliseconds>(buf_time1 - buf_time0).count();
        };

    std::cout << "CPU calculation speedup test:" << std::endl;

    std::cout << "execution::seq" << std::endl;
	buf0.execPolicy = std::execution::seq;
    const double time1 = static_cast<double>(calc());
    std::cout << time1 << "ms" << std::endl;

    std::cout << "execution::par" << std::endl;
    buf0.execPolicy = std::execution::par;
    const double time2 = static_cast<double>(calc());
    std::cout << time2 << "ms\t" << (time1 / time2) << "faster!!" << std::endl;

    std::cout << "execution::par_unseq" << std::endl;
    buf0.execPolicy = std::execution::par_unseq;
    const double time3 = static_cast<double>(calc());
    std::cout << time3 << "ms\t" << (time1 / time3) << "faster!!" << std::endl;
#endif
}

int main() {
    testDefaultConstructor();
    testInitListConstructor();
    testSizeConstructor();
    testCopyConstructor();
    testMoveConstructor();
    testAssignmentOperator();
    testAddOperator();
    testTranspose();
    testDeterminant();
    testInverse();
    testLUDecomposition();
    testForEach();
    testIdentityMatrix();
    calcSpeedTest(5000, [](Matrix<double>& arg1, Matrix<double>& arg2) {return arg1 * arg2; });
   
    return 0;
}
