#ifndef MATRIXCPP_MATRIXOP_HPP
#define MATRIXCPP_MATRIXOP_HPP


#include "matrix.h"


// ������Z�q = ���������X�g�p
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(const MatrixInitType<Type>& init)
{
    this->matrix_.clear();
    this->matrix_.assign(init.begin(), init.end());

    this->validateMatrix_(this->matrix_);

    return *this;
}

// �R�s�[������Z�q =
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(const Matrix<Type>& other)
{
    if (this != &other)
        copyMatrix_(this->matrix_, other.matrix_);

    return *this;
}

// ���[�u������Z�q =
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(Matrix<Type>&& other)
{
    if (this != &other)
        this->matrix_ = std::move(other.matrix_);
    
    return *this;
}

// ������Z�q << ���������X�g�p
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(const MatrixInitType<Type>& init)
{
    this->matrix_.clear();
    this->matrix_.assign(init.begin(), init.end());

    this->validateMatrix_(this->matrix_);

    return *this;
}

// ������Z�q << �R�s�[
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(const Matrix<Type>& other)
{
    if (this != &other)
        this->matrix_ = other.matrix_;
    
    return *this;
}

// ���[�u������Z�q <<
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(Matrix<Type>&& other)
{
    if (this != &other)
        this->matrix_ = std::move(other.matrix_);
    
    return *this;
}

// �s�A�N�Z�X�I�y���[�^�[
template<typename Type>
typename Matrix<Type>::RowType<Type>& Matrix<Type>::operator[](size_t index)
{
    return this->matrix_[index];
}


#endif