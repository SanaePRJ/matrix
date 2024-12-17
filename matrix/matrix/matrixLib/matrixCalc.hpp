#ifndef MATRIXCPP_MATRIXCALC_HPP
#define MATRIXCPP_MATRIXCALC_HPP


#include "matrix.h"
#include <algorithm>


template<typename Type>
template<typename Type_>
inline void Matrix<Type>::add_
(
    MatrixType<Type_>&       dest, 
    const MatrixType<Type_>& source
)
{
    this->calcMatrix_<Type_,std::plus<Type_>>(dest,source);
}

template<typename Type>
template<typename Type_>
inline void Matrix<Type>::sub_
(
    MatrixType<Type_>&       dest, 
    const MatrixType<Type_>& source
)
{
    this->calcMatrix_<Type_, std::minus<Type_>>(dest, source);
}

template<typename Type>
template<typename Type_>
inline Matrix<Type>::MatrixType<Type_> Matrix<Type>::mul_
(
    const MatrixType<Type_>& matrix1, 
    const MatrixType<Type_>& matrix2
)
{
    // ���ꂼ��̗�ƍs���ꏏ�łȂ���΂Ȃ�Ȃ��B
    if (this->cols_(matrix1) != this->rows_(matrix2))
        throw std::invalid_argument("The number of columns in data1 must be the same as the number of rows in data2.");

    const size_t crSize   = this->cols_(matrix1); // ��Z����s��͊e��ƍs��������
    const size_t rsltRows = this->rows_(matrix1);
    const size_t rsltCols = this->cols_(matrix2);

    MatrixType<Type_> result;
    result.resize(rsltRows, RowType<Type_>(rsltCols)); // �o�̓T�C�Y�w��

    // A * B = C
    // Cij = ��(k=1,n){Aik * Bkj}
    auto calcLambda = [&crSize, &matrix1, &matrix2](const size_t& posRow,const size_t& posCol) {
        Type_ resultMatrix = 0; // ����

        for (size_t i = 0; i < crSize; i++)
            resultMatrix += matrix1[posRow][i] * matrix2[i][posCol]; // �e��v�f�ƍs�v�f����Z�������̂����Z����B

        return resultMatrix;
        };

    // �S�v�f�����߂�
    for (size_t row = 0; row < rsltRows; row++) {
        for (size_t col = 0; col < rsltCols; col++) {
            result[row][col] = calcLambda(std::make_pair(row,col));
        }
    }

    return result;
}

template<typename Type>
template<typename Type_>
inline void Matrix<Type>::hadamardMul_
(
    MatrixType<Type_>&       dest, 
    const MatrixType<Type_>& source
)
{
    this->calcMatrix_<Type_, std::multiplies<Type_>>(dest, source);
}

template<typename Type>
template<typename Type_>
inline void Matrix<Type>::hadamardDiv_
(
    MatrixType<Type_>& dest, 
    const MatrixType<Type_>& source
)
{
    this->calcMatrix_<Type_, std::divides<Type_>>(dest, source);
}

template<typename Type>
template<typename Type_, typename calcType_>
inline void Matrix<Type>::calcMatrix_
(
    MatrixType<Type_>&       dest, 
    const MatrixType<Type_>& source
)
{
    // �T�C�Y�������łȂ���΂Ȃ�Ȃ��B
    if (!this->areSameSize_(dest, source))
        throw std::invalid_argument("The number of rows and columns of data1 and data2 must be equal.");

    // �S�v�f�ɑ΂��Ċ֐��I�u�W�F�N�g�̌v�Z���s���B
    for (size_t row = 0; row < this->rows_(dest); ++row) {
        for (size_t col = 0; col < this->cols_(source); ++col) {
            dest[row][col] = calcType_()(dest[row][col], source[row][col]);
        }
    }
}

template<typename Type>
template<typename Type_, typename calcType_>
inline void Matrix<Type>::scalarCalc_
(
    MatrixType<Type_>& dest, 
    const Type_&       source
)
{
    // �S�v�f�ɑ΂��Ċ֐��I�u�W�F�N�g�̌v�Z���s���B
    for (size_t row = 0; row < this->rows_(dest); ++row) {
        for (size_t col = 0; col < this->cols_(dest); ++col) {
            dest[row][col] = calcType_()(dest[row][col], source);
        }
    }
}


#endif
