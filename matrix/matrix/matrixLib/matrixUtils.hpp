#ifndef MATRIXCPP_MATRIXUTILS_HPP
#define MATRIXCPP_MATRIXUTILS_HPP

#include "matrix.h"

template<typename Type>
template<typename CopyType_>
void Matrix<Type>::copyMatrix_
(
	MatrixType<CopyType_>& dest, const MatrixType<CopyType_>& src
)
{
	// マトリックスの次元をリサイズ
	dest.resize(src.size());

	// それぞれの行をコピー
	for (size_t i = 0; i < src.size(); ++i)
	{
		dest[i] = src[i];
	}
}

// 行数取得
template<typename Type>
const size_t Matrix<Type>::rows()
{
	return matrix_.size();
}

// 列数取得
template<typename Type>
const size_t Matrix<Type>::cols()
{
	return matrix_.empty() ? 0 : matrix_[0].size();
}

#endif
