#ifndef MATRIXCPP_MATRIXOP_HPP
#define MATRIXCPP_MATRIXOP_HPP

#include "matrix.h"

// 代入演算子 =
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
	// マトリックスの次元を検証
	size_t rows = init.size();
	size_t cols = init.begin()->size();


	// マトリックスをリサイズ
	matrix_.resize(rows);
	for (auto& row : matrix_)
	{
		row.resize(cols);
	}


	// 初期化リストにコピー
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

// コピー代入演算子 =
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

// ムーブ代入演算子 =
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

// 代入演算子 << 初期化リスト用
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<
(
	const MatrixInitType<Type>& init
)
{
	// 代入演算子の再利用
	return *this = init;
}

// 代入演算子 << Matrix用
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<
(
	const Matrix<Type>& other
)
{
	// 代入演算子の再利用
	return *this = other;
}

// ムーブ代入演算子 <<
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<
(
	Matrix<Type>&& other
)
{
	// コピー代入演算子の再利用
	return *this = std::move(other);
}

// 行アクセス
template<typename Type>
typename Matrix<Type>::RowType<Type>& Matrix<Type>::operator[]
(
	size_t index
)
{
	return matrix_[index];
}


#endif
