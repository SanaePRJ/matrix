#ifndef MATRIXCPP_MATRIXUTILS_HPP
#define MATRIXCPP_MATRIXUTILS_HPP


#include "matrix.h"


template<typename Type>
template<typename CopyType_>
void Matrix<Type>::copyMatrix_
(
	MatrixType<CopyType_>& dest, 
	const MatrixType<CopyType_>& src
)
{
	// マトリックスの次元をリサイズ
	dest.resize(src.size());

	std::copy(src.begin(), src.end(), dest.begin());
}

template<typename Type>
template<typename Type_>
inline size_t Matrix<Type>::rows_
(
	const MatrixType<Type_>& mtrx
)
	const noexcept
{
	return mtrx.size();
}

template<typename Type>
template<typename Type_>
inline size_t Matrix<Type>::cols_
(
	const MatrixType<Type_>& mtrx
)
	const noexcept
{
	return mtrx.empty() ? 0 : mtrx.at(0).size();
}

template<typename Type>
template<typename Type_>
inline void Matrix<Type>::swapRow_
(
	MatrixType<Type_>& matrix, 
	const size_t& swapRow1, 
	const size_t& swapRow2
)
{
	if (swapRow1 >= this->rows_(matrix) || swapRow2 >= this->rows_(matrix))
		throw std::out_of_range("Row index out of range.");

	std::swap(matrix[swapRow1], matrix[swapRow2]);
}

template<typename Type>
template<typename Type_>
inline void Matrix<Type>::swapCol_
(
	MatrixType<Type_>& matrix, 
	const size_t& swapCol1,
	const size_t& swapCol2
)
{
	if (matrix.empty() || swapCol1 >= this->cols_(matrix) || swapCol2 >= this->cols_(matrix))
		throw std::out_of_range("Column index out of range.");

	for (auto& row : matrix)
		std::swap(row[swapCol1], row[swapCol2]);
}

template<typename Type>
template<typename Type_>
inline typename Matrix<Type>::template MatrixType<Type_> Matrix<Type>::transpose_
(
	const MatrixType<Type_>& mtrx
)
{
	MatrixType<Type_> newMtrx(this->cols_(mtrx), RowType<Type_>(this->rows_(mtrx)));

	for (size_t row = 0; row < this->rows_(mtrx); row++) {
		for (size_t col = 0; col < this->cols_(mtrx); col++) {
			newMtrx[col][row] = mtrx[row][col];
		}
	}

	return newMtrx;
}

template<typename Type>
template<typename Type1_, typename Type2_>
inline bool Matrix<Type>::areSameSize_
(
	const MatrixType<Type1_>& mtrx1,
	const MatrixType<Type2_>& mtrx2
)
	const noexcept
{
	bool result;

	result  = this->rows_(mtrx1) == this->rows_(mtrx2);
	result &= this->cols_(mtrx1) == this->cols_(mtrx2);

	return result;
}

template<typename Type>
template<typename Type_>
inline void Matrix<Type>::validateMatrix_
(
	const MatrixType<Type_>& mtrx
)
{
	if (mtrx.empty())
		return;

	size_t baseSize = this->cols_(mtrx);
	for (const auto& row : mtrx) {
		if (row.size() != baseSize)
			throw std::invalid_argument("invalid matrix.");
	}
}

template<typename Type>
inline Matrix<Type> Matrix<Type>::transpose()
{
	return this->transpose_(this->matrix_);
}

// 行数取得
template<typename Type>
const size_t Matrix<Type>::rows()
{
	return this->rows_(this->matrix_);
}

// 列数取得
template<typename Type>
const size_t Matrix<Type>::cols()
{
	return this->cols_(this->matrix_);
}

template<typename Type>
inline std::vector<std::reference_wrapper<Type>> Matrix<Type>::rowRef
(
	const size_t& rowNum
)
{
	std::vector<std::reference_wrapper<Type>> resultRef; 
	resultRef.reserve(this->cols()); 

	for (auto& elem : this->matrix_[rowNum]) 
		resultRef.push_back(std::ref(elem)); 

	return resultRef;
}

template<typename Type>
inline std::vector<std::reference_wrapper<Type>> Matrix<Type>::colRef
(
	const size_t& colNum
)
{
	std::vector<std::reference_wrapper<Type>> resultRef; 
	resultRef.reserve(this->rows()); 

	for (auto& row : this->matrix_) 
		resultRef.push_back(std::ref(row[colNum])); 
	
	return resultRef;
}

template<typename Type>
inline Matrix<Type>& Matrix<Type>::forEach
(
	std::function<Type()> func
)
{
	for (auto& row : this->matrix_) { 
		for (auto& elem : row)
			elem = func(); 
	}

	return *this;
}

template<typename Type> 
Matrix<Type>& Matrix<Type>::forEach
(
	std::function<Type(size_t, size_t, Type&)> func
) 
{
	for (size_t row = 0; row < this->rows(); row++) 
	{
		for (size_t col = 0; col < this->cols(); col++) 
			this->matrix_[row][col] = func(row, col, this->matrix_[row][col]); 
	} 

	return *this; 
}


#endif
