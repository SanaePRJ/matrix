#ifndef MATRIXCPP_MATRIXCTOR_HPP
#define MATRIXCPP_MATRIXCTOR_HPP

#include "matrix.h"

//サイズを指定して0行列を生成する。
template<typename Type>
Matrix<Type>::Matrix
(
	const Size& size
)
	:matrix_(size.first, std::vector<Type>(size.second, 0))
{}

//以下のように定義することができる。
//{
//	{0, 0},
//	{0, 0},
//	{0, 0}
//}
template<typename Type>
Matrix<Type>::Matrix
(
	const MatrixInitType<Type>& init
)
	:matrix_(init.begin(), init.end())
{
	//バリデート
}

// コピーコンストラクタ
template<typename Type>
Matrix<Type>::Matrix
(
	const Matrix& other
)
{
	// リサイズ
	matrix_.resize(other.matrix_.size());

	// コピー
	copyMatrix_(matrix_, other.matrix_);
}


// moveコンストラクタ
template<typename Type>
Matrix<Type>::Matrix
(
	Matrix<Type>&& other
) 
	noexcept
{
	// リソースの移動
	matrix_ = std::move(other.matrix_);
}

#endif
