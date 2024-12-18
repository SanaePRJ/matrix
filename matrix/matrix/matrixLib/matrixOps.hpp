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
    if (this != &other)
        copyMatrix_(this->matrix_, other.matrix_);

    return *this;
}

// ムーブ代入演算子 =
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(Matrix<Type>&& other)
{
    if (this != &other)
        this->matrix_ = std::move(other.matrix_);
    
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
    if (this != &other)
        this->matrix_ = other.matrix_;
    
    return *this;
}

// ムーブ代入演算子 <<
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(Matrix<Type>&& other)
{
    if (this != &other)
        this->matrix_ = std::move(other.matrix_);
    
    return *this;
}

// 行アクセスオペレーター
template<typename Type>
typename Matrix<Type>::RowType<Type>& Matrix<Type>::operator[](size_t index)
{
    return this->matrix_[index];
}


#endif