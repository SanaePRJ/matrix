#ifndef MATRIXCPP_MATRIXOP_HPP
#define MATRIXCPP_MATRIXOP_HPP


#include "matrix.h"


// 代入演算子 = 初期化リスト用
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(const MatrixInitType<Type>& init)
{
    this->matrix_.clear();
    this->matrix_.assign(init.begin(), init.end());
    this->validateMatrix_(this->matrix_);
    return *this;
}

// コピー代入演算子 =
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(const Matrix<Type>& other)
{
    if (this != &other) {
        copyMatrix_(this->matrix_, other.matrix_);
    }
    return *this;
}

// ムーブ代入演算子 =
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(Matrix<Type>&& other)
{
    if (this != &other) {
        this->matrix_ = std::move(other.matrix_);
    }
    return *this;
}

// 代入演算子 << 初期化リスト用
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(const MatrixInitType<Type>& init)
{
    this->matrix_.clear();
    this->matrix_.assign(init.begin(), init.end());
    this->validateMatrix_(this->matrix_);
    return *this;
}

// 代入演算子 << コピー
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(const Matrix<Type>& other)
{
    if (this != &other) {
        this->matrix_ = other.matrix_;
    }
    return *this;
}

// ムーブ代入演算子 <<
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(Matrix<Type>&& other)
{
    if (this != &other) {
        this->matrix_ = std::move(other.matrix_);
    }
    return *this;
}

// 行アクセスオペレーター
template<typename Type>
typename Matrix<Type>::RowType<Type>& Matrix<Type>::operator[](size_t index)
{
    return this->matrix_[index];
}

// 足し算代入演算子 +=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator+=(const Matrix<Type>& mtrx)
{
    this->add(mtrx);
    return *this;
}

// 引き算代入演算子 -=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator-=(const Matrix<Type>& mtrx)
{
    this->sub(mtrx);
    return *this;
}

// 掛け算代入演算子 *=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator*=(const Matrix<Type>& mtrx)
{
    this->mul(mtrx);
    return *this;
}

// アダマール積代入演算子 ^=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator^=(const Matrix<Type>& mtrx)
{
    this->hadamardMul(mtrx);
    return *this;
}

// アダマール除算代入演算子 /=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator/=(const Matrix<Type>& mtrx)
{
    this->hadamardDiv(mtrx);
    return *this;
}

// スカラー掛け算代入演算子 *=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator*=(const Type& scalar)
{
    this->scalarMul(scalar);
    return *this;
}

// 足し算演算子 +
template<typename Type>
Matrix<Type> Matrix<Type>::operator+(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->add_(result, mtrx.matrix_);
    return result;
}

// 引き算演算子 -
template<typename Type>
Matrix<Type> Matrix<Type>::operator-(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->sub_(result, mtrx.matrix_);
    return result;
}

// 掛け算演算子 *
template<typename Type>
Matrix<Type> Matrix<Type>::operator*(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->mul_(result, mtrx.matrix_);
    return result;
}

// アダマール積演算子 ^
template<typename Type>
Matrix<Type> Matrix<Type>::operator^(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->hadamardMul_(result, mtrx.matrix_);
    return result;
}

// アダマール除算演算子 /
template<typename Type>
Matrix<Type> Matrix<Type>::operator/(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->hadamardDiv_(result, mtrx.matrix_);
    return result;
}

// スカラー掛け算演算子 *
template<typename Type>
Matrix<Type> Matrix<Type>::operator*(const Type& scalar)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->scalarCalc_<std::multiplies>(result, scalar);
    return result;
}


#endif
