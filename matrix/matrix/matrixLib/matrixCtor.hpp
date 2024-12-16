#ifndef MATRIXCPP_MATRIXCTOR_HPP
#define MATRIXCPP_MATRIXCTOR_HPP

#include "matrix.h"

// �R�s�[�R���X�g���N�^
template<typename Type>
Matrix<Type>::Matrix
(
	const Matrix<Type>& other
)
{
	// �f�B�[�v�R�s�[
	copyMatrix_(matrix_, other.matrix_);
}

// move�R���X�g���N�^
template<typename Type>
Matrix<Type>::Matrix
(
	Matrix<Type>&& other
) 
	noexcept
{
	// ���\�[�X�̈ړ�
	matrix_ = std::move(other.matrix_);
}

// �f�t�H���g�R���X�g���N�^
template<typename Type>
Matrix<Type>::Matrix
(
	const MatrixInitType<>& init
)
{
	// ���������X�g����}�g���b�N�X���쐬
	*this = init; // �����̑�����Z�q�̍ė��p
}

// �T�C�Y�w��R���X�g���N�^
template<typename Type>
Matrix<Type>::Matrix
(
	const Size& size
)
{
	// �w�肳�ꂽ�T�C�Y�Ń}�g���b�N�X�����T�C�Y
	matrix_.resize(size.first);
	for (auto& row : matrix_)
	{
		row.resize(size.second);
	}
}

#endif
