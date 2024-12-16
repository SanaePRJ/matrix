#ifndef MATRIXCPP_MATRIXOP_HPP
#define MATRIXCPP_MATRIXOP_HPP

#include "matrix.h"

// ������Z�q =
//	matrix =
//	{
//		{1, 0, 0, 0},
//		{0, 1, 0, 0},
//		{0, 0, 1, 0},
//		{0, 0, 0, 1}
//	}
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=
(
	const MatrixInitType<Type>& init
)
{
	// �}�g���b�N�X�̎���������
	size_t rows = init.size();
	size_t cols = init.begin()->size();


	// �}�g���b�N�X�����T�C�Y
	matrix_.resize(rows);
	for (auto& row : matrix_)
	{
		row.resize(cols);
	}


	// ���������X�g�ɃR�s�[
	size_t r = 0;
	for (const auto& row : init)
	{
		size_t c = 0;
		for (const auto& val : row)
		{
			matrix_[r][c] = val;
			c++;
		}
		r++;
	}

	return *this;
}

// �R�s�[������Z�q =
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=
(
	const Matrix<Type>& other
)
{
	if (this != &other) {
		copyMatrix_(matrix_, other.matrix_);
	}

	return *this;
}

// ���[�u������Z�q =
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=
(
	Matrix<Type>&& other
)
{
	if (this != &other) {
		matrix_ = std::move(other.matrix_);
	}

	return *this;
}

// ������Z�q << ���������X�g�p
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<
(
	const MatrixInitType<Type>& init
)
{
	// ������Z�q�̍ė��p
	return *this = init;
}

// ������Z�q << Matrix�p
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<
(
	const Matrix<Type>& other
)
{
	// ������Z�q�̍ė��p
	return *this = other;
}

// ���[�u������Z�q <<
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<
(
	Matrix<Type>&& other
)
{
	// �R�s�[������Z�q�̍ė��p
	return *this = std::move(other);
}

// �s�A�N�Z�X
template<typename Type>
typename Matrix<Type>::RowType<Type>& Matrix<Type>::operator[]
(
	size_t index
)
{
	return matrix_[index];
}


#endif
