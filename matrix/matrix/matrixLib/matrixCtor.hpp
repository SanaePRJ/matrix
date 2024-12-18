#ifndef MATRIXCPP_MATRIXCTOR_HPP
#define MATRIXCPP_MATRIXCTOR_HPP


#include "matrix.h"


//�T�C�Y���w�肵��0�s��𐶐�����B
template<typename Type>
Matrix<Type>::Matrix(
	const std::pair<size_t,size_t>& size
)
	: matrix_(size.first, std::vector<Type>(size.second, 0))
{

}

//�ȉ��̂悤�ɒ�`���邱�Ƃ��ł���B
//{
//	{0, 0},
//	{0, 0},
//	{0, 0}
//}
template<typename Type>
Matrix<Type>::Matrix(
	const MatrixInitType<Type>& init
)
	:matrix_(init.begin(), init.end())
{
	this->validateMatrix_(this->matrix_);
}

template<typename Type>
inline Matrix<Type>::Matrix(
	const MatrixType<>& init
)
	:matrix_(init.begin(), init.end())
{
	this->validateMatrix_(this->matrix_);
}

// �R�s�[�R���X�g���N�^
template<typename Type>
Matrix<Type>::Matrix(
	const Matrix& other
)
{
	matrix_.resize(other.matrix_.size());

	// �R�s�[
	copyMatrix_(matrix_, other.matrix_);
}


// move�R���X�g���N�^
template<typename Type>
Matrix<Type>::Matrix(
	Matrix<Type>&& other
)
	noexcept
{
	// ���\�[�X�̈ړ�
	matrix_ = std::move(other.matrix_);
}


#endif
