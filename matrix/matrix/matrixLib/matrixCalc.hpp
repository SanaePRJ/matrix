#ifndef MATRIXCPP_MATRIXCALC_HPP
#define MATRIXCPP_MATRIXCALC_HPP


#include "matrix.h"
#include <algorithm>


template<typename Type>
inline void Matrix<Type>::add_
(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    this->calcMatrix_<std::plus<Type>>(dest, source);
}

template<typename Type>
inline void Matrix<Type>::sub_
(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    this->calcMatrix_<std::minus<Type>>(dest, source);
}

template<typename Type>
typename Matrix<Type>::template MatrixType<Type> Matrix<Type>::mul_
(
    const MatrixType<Type>& matrix1,
    const MatrixType<Type>& matrix2
)
{
    if (this->cols_(matrix1) != this->rows_(matrix2))
        throw std::invalid_argument("The number of columns in data1 must be the same as the number of rows in data2.");

    const size_t crSize = this->cols_(matrix1);
    const size_t rsltRows = this->rows_(matrix1);
    const size_t rsltCols = this->cols_(matrix2);

    MatrixType<Type> result(rsltRows, RowType<Type>(rsltCols));

    auto calcLambda = [&matrix1, &matrix2](const size_t& posRow, const size_t& posCol) {
        Type resultMatrix = 0;
        for (size_t i = 0; i < matrix1[0].size(); i++) {
            resultMatrix += matrix1[posRow][i] * matrix2[i][posCol];
        }
        return resultMatrix;
        };

    for (size_t row = 0; row < rsltRows; row++) {
        for (size_t col = 0; col < rsltCols; col++) {
            result[row][col] = calcLambda(row, col);
        }
    }

    return result;
}

template<typename Type>
inline void Matrix<Type>::hadamardMul_
(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    this->calcMatrix_<std::multiplies<Type>>(dest, source);
}

template<typename Type>
inline void Matrix<Type>::hadamardDiv_
(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    this->calcMatrix_<std::divides<Type>>(dest, source);
}

template<typename Type>
template<typename calcType>
void Matrix<Type>::calcMatrix_
(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    if (!this->areSameSize_(dest, source))
        throw std::invalid_argument("The number of rows and columns of data1 and data2 must be equal.");

    for (size_t row = 0; row < this->rows_(dest); ++row) {
        for (size_t col = 0; col < this->cols_(source); ++col)
            dest[row][col] = calcType()(dest[row][col], source[row][col]);
    }
}

template<typename Type>
template<typename calcType>
void Matrix<Type>::scalarCalc_
(
    MatrixType<Type>& dest,
    const Type& source
)
{
    for (size_t row = 0; row < this->rows_(dest); ++row) {
        for (size_t col = 0; col < this->cols_(dest); ++col)
            dest[row][col] = calcType()(dest[row][col], source);
    }
}

template<typename Type>
Matrix<Type>& Matrix<Type>::add
(
    const Matrix<Type>& mtrx
)
{
    this->add_(this->matrix_, mtrx.matrix_);

    return *this;
}

template<typename Type>
Matrix<Type>& Matrix<Type>::sub
(
    const Matrix<Type>& mtrx
)
{
    this->sub_(this->matrix_, mtrx.matrix_);

    return *this;
}

template<typename Type>
Matrix<Type>& Matrix<Type>::mul
(
    const Matrix<Type>& mtrx
)
{
    this->matrix_ = this->mul_(this->matrix_, mtrx.matrix_);

    return *this;
}

template<typename Type>
Matrix<Type>& Matrix<Type>::scalarMul
(
    const Type& source
)
{
    this->scalarCalc_<Type, std::multiplies<Type>>(this->matrix_, source);

    return *this;
}

template<typename Type>
Matrix<Type>& Matrix<Type>::hadamardMul
(
    const Matrix<Type>& source
)
{
    this->hadamardMul_<Type>(this->matrix_, source);

    return *this;
}

template<typename Type>
Matrix<Type>& Matrix<Type>::hadamardDiv(const Matrix<Type>& source)
{
    this->hadamardDiv_<Type>(this->matrix_, source);

    return *this;
}

template<typename Type>
template<typename calcType>
Matrix<Type>& Matrix<Type>::scalarCalc(const Matrix<Type>& source)
{
    this->scalarCalc_<Type, calcType>(this->matrix_, source);

    return *this;
}


#endif
