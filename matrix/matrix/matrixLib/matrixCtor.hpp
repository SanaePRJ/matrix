#ifndef MATRIXCPP_MATRIXCTOR_HPP
#define MATRIXCPP_MATRIXCTOR_HPP


#include "matrix.h"


//サイズを指定して0行列を生成する。
template<typename Type>
Matrix<Type>::Matrix(
	const std::pair<size_t,size_t>& size
)
	: matrix_(size.first, std::vector<Type>(size.second, 0))
{

}

//以下のように定義することができる。
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

// コピーコンストラクタ
template<typename Type>
Matrix<Type>::Matrix(
	const Matrix& other
)
{
	matrix_.resize(other.matrix_.size());

	// コピー
	copyMatrix_(matrix_, other.matrix_);
}


// moveコンストラクタ
template<typename Type>
Matrix<Type>::Matrix(
	Matrix<Type>&& other
)
	noexcept
{
	// リソースの移動
	matrix_ = std::move(other.matrix_);
}


#endif
