#ifndef MATRIXCPP_MATRIXUTILS_HPP
#define MATRIXCPP_MATRIXUTILS_HPP


#include "matrix.h"


// マトリックスの内容をコピーするテンプレート関数
template<typename Type>
template<typename CopyType>
void Matrix<Type>::copyMatrix_(
    MatrixType<CopyType>& dest,
    const MatrixType<CopyType>& src
)
{
    dest.resize(src.size());

    std::copy(src.begin(), src.end(), dest.begin());
}

// 行数を取得するテンプレート関数
template<typename Type>

inline size_t Matrix<Type>::rows_(
    const MatrixType<Type>& mtrx
)
    const noexcept
{
    return mtrx.size();
}

// 列数を取得するテンプレート関数
template<typename Type>

inline size_t Matrix<Type>::cols_(
    const MatrixType<Type>& mtrx
)
    const noexcept
{
    return mtrx.empty() ? 0 : mtrx.at(0).size();
}

// マトリックスの行を入れ替えるテンプレート関数
template<typename Type>

inline void Matrix<Type>::swapRow_(
    MatrixType<Type>& matrix, 
    const size_t& swapRow1, 
    const size_t& swapRow2
)
{
    // 行インデックスが範囲外の場合に例外を投げる
    if (swapRow1 >= this->rows_(matrix) || swapRow2 >= this->rows_(matrix))
        throw std::out_of_range("Row index out of range.");

    // 行を入れ替える
    std::swap(matrix[swapRow1], matrix[swapRow2]);
}

// マトリックスの列を入れ替えるテンプレート関数
template<typename Type>

inline void Matrix<Type>::swapCol_(
    MatrixType<Type>& matrix, 
    const size_t& swapCol1, 
    const size_t& swapCol2
)
{
    // 列インデックスが範囲外の場合に例外を投げる
    if (matrix.empty() || swapCol1 >= this->cols_(matrix) || swapCol2 >= this->cols_(matrix))
        throw std::out_of_range("Column index out of range.");

    // 各行の指定された列を入れ替える
    for (auto& row : matrix)
        std::swap(row[swapCol1], row[swapCol2]);
}

// マトリックスの転置を行うテンプレート関数
template<typename Type>
inline typename Matrix<Type>::template MatrixType<Type> Matrix<Type>::transpose_(
    const MatrixType<Type>& mtrx
)
{
    // 転置後の新しいマトリックスを作成
    MatrixType<Type> newMtrx(this->cols_(mtrx), RowType<Type>(this->rows_(mtrx)));

    // 転置を実行
    for (size_t row = 0; row < this->rows_(mtrx); row++) {
        for (size_t col = 0; col < this->cols_(mtrx); col++) {
            newMtrx[col][row] = mtrx[row][col];
        }
    }

    return newMtrx;
}

// 2つのマトリックスが同じサイズかどうかをチェックするテンプレート関数
template<typename Type>
inline bool Matrix<Type>::areSameSize_(
    const MatrixType<Type>& mtrx1, 
    const MatrixType<Type>& mtrx2
)
    const noexcept
{
    bool result;

    // 行数と列数が一致するかどうかを確認
    result = this->rows_(mtrx1) == this->rows_(mtrx2);
    result &= this->cols_(mtrx1) == this->cols_(mtrx2);

    return result;
}

// マトリックスの各行が同じサイズかどうかをチェックするテンプレート関数
template<typename Type>

inline void Matrix<Type>::validateMatrix_(
    const MatrixType<Type>& mtrx
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

// マトリックスを転置する関数
template<typename Type>
inline Matrix<Type> Matrix<Type>::transpose()
{
    return this->transpose_(this->matrix_);
}

// 指定された2つの行を入れ替える関数
template<typename Type>
inline Matrix<Type>& Matrix<Type>::swapRow(
    const size_t& swapRow1, 
    const size_t& swapRow2
)
{
    this->swapRow_(this->matrix_, swapRow1, swapRow2);
    return *this;
}

// 指定された2つの列を入れ替える関数
template<typename Type>
inline Matrix<Type>& Matrix<Type>::swapCol(
    const size_t& swapCol1,
    const size_t& swapCol2
)
{
    this->swapCol_(this->matrix_, swapCol1, swapCol2);
    return *this;
}

// マトリックスのサイズを変更する関数
template<typename Type>
inline Matrix<Type>& Matrix<Type>::resize(
    const size_t& row, 
    const size_t& col
)
{
    this->matrix_.resize(row, RowType<Type>(col, 0));
    return *this;
}

// 行数を取得する関数
template<typename Type>
const size_t Matrix<Type>::rows()
{
    return this->rows_(this->matrix_);
}

// 列数を取得する関数
template<typename Type>
const size_t Matrix<Type>::cols()
{
    return this->cols_(this->matrix_);
}

// 指定された行の参照を取得する関数
template<typename Type>
inline std::vector<std::reference_wrapper<Type>> Matrix<Type>::rowRef(
    const size_t& rowNum
)
{
    std::vector<std::reference_wrapper<Type>> resultRef;
    resultRef.reserve(this->cols());

    for (auto& elem : this->matrix_[rowNum])
        resultRef.push_back(std::ref(elem));

    return resultRef;
}

// 指定された列の参照を取得する関数
template<typename Type>
inline std::vector<std::reference_wrapper<Type>> Matrix<Type>::colRef(
    const size_t& colNum
)
{
    std::vector<std::reference_wrapper<Type>> resultRef;
    resultRef.reserve(this->rows());

    for (auto& row : this->matrix_)
        resultRef.push_back(std::ref(row[colNum]));

    return resultRef;
}

// マトリックスの全要素に対して指定された関数を適用する関数（引数なし版）
template<typename Type>
inline Matrix<Type>& Matrix<Type>::forEach(
    std::function<Type()> func
)
{
    for (auto& row : this->matrix_) {
        for (auto& elem : row)
            elem = func();
    }

    return *this;
}

// マトリックスの全要素に対して指定された関数を適用する関数（引数あり版）
template<typename Type>
Matrix<Type>& Matrix<Type>::forEach(
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
