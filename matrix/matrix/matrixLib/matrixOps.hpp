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
    if (this != &other) {
        copyMatrix_(this->matrix_, other.matrix_);
    }
    return *this;
}

// ���[�u������Z�q =
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(Matrix<Type>&& other)
{
    if (this != &other) {
        this->matrix_ = std::move(other.matrix_);
    }
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
    if (this != &other) {
        this->matrix_ = other.matrix_;
    }
    return *this;
}

// ���[�u������Z�q <<
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(Matrix<Type>&& other)
{
    if (this != &other) {
        this->matrix_ = std::move(other.matrix_);
    }
    return *this;
}

// �s�A�N�Z�X�I�y���[�^�[
template<typename Type>
typename Matrix<Type>::RowType<Type>& Matrix<Type>::operator[](size_t index)
{
    return this->matrix_[index];
}

// �����Z������Z�q +=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator+=(const Matrix<Type>& mtrx)
{
    this->add(mtrx);
    return *this;
}

// �����Z������Z�q -=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator-=(const Matrix<Type>& mtrx)
{
    this->sub(mtrx);
    return *this;
}

// �|���Z������Z�q *=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator*=(const Matrix<Type>& mtrx)
{
    this->mul(mtrx);
    return *this;
}

// �A�_�}�[���ϑ�����Z�q ^=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator^=(const Matrix<Type>& mtrx)
{
    this->hadamardMul(mtrx);
    return *this;
}

// �A�_�}�[�����Z������Z�q /=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator/=(const Matrix<Type>& mtrx)
{
    this->hadamardDiv(mtrx);
    return *this;
}

// �X�J���[�|���Z������Z�q *=
template<typename Type>
Matrix<Type>& Matrix<Type>::operator*=(const Type& scalar)
{
    this->scalarMul(scalar);
    return *this;
}

// �����Z���Z�q +
template<typename Type>
Matrix<Type> Matrix<Type>::operator+(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->add_(result, mtrx.matrix_);
    return result;
}

// �����Z���Z�q -
template<typename Type>
Matrix<Type> Matrix<Type>::operator-(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->sub_(result, mtrx.matrix_);
    return result;
}

// �|���Z���Z�q *
template<typename Type>
Matrix<Type> Matrix<Type>::operator*(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->mul_(result, mtrx.matrix_);
    return result;
}

// �A�_�}�[���ω��Z�q ^
template<typename Type>
Matrix<Type> Matrix<Type>::operator^(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->hadamardMul_(result, mtrx.matrix_);
    return result;
}

// �A�_�}�[�����Z���Z�q /
template<typename Type>
Matrix<Type> Matrix<Type>::operator/(const Matrix<Type>& mtrx)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->hadamardDiv_(result, mtrx.matrix_);
    return result;
}

// �X�J���[�|���Z���Z�q *
template<typename Type>
Matrix<Type> Matrix<Type>::operator*(const Type& scalar)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->scalarCalc_<std::multiplies>(result, scalar);
    return result;
}


#endif
