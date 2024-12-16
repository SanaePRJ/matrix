#ifndef MATRIXCPP_MATRIXCTOR_HPP
#define MATRIXCPP_MATRIXCTOR_HPP

#include "matrix.h"

// コピーコンストラクタ
template<typename Type>
Matrix<Type>::Matrix
(
	const Matrix<Type>& other
)
{
	// ディープコピー
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

// デフォルトコンストラクタ
template<typename Type>
Matrix<Type>::Matrix
(
	const MatrixInitType<>& init
)
{
	// 初期化リストからマトリックスを作成
	*this = init; // 既存の代入演算子の再利用
}

// サイズ指定コンストラクタ
template<typename Type>
Matrix<Type>::Matrix
(
	const Size& size
)
{
	// 指定されたサイズでマトリックスをリサイズ
	matrix_.resize(size.first);
	for (auto& row : matrix_)
	{
		row.resize(size.second);
	}
}

#endif
