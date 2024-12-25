#ifndef MATRIXCPP_MATRIXUTILS_HPP
#define MATRIXCPP_MATRIXUTILS_HPP

#include "matrix.h"

/**
 * @brief Copies the contents of a source matrix to a destination matrix.
 *
 * @tparam Type The type of the matrix.
 * @tparam CopyType The type of the elements in the source matrix.
 * @param dest The destination matrix.
 * @param src The source matrix.
 */
template<typename Type,typename DcmpType>
template<typename CopyType1,typename CopyType2>
void Matrix<Type,DcmpType>::copyMatrix_(
    MatrixType<CopyType1>& dest,
    const MatrixType<CopyType2>& src
)
{
    dest.resize(src.size()); 

    for (size_t i = 0; i < src.size(); ++i) {
        dest[i].resize(src[i].size()); 
        std::copy(src[i].begin(), src[i].end(), dest[i].begin()); 
    }
}

/**
 * @brief Retrieves the number of rows in a matrix.
 *
 * @tparam Type The type of the matrix.
 * @param mtrx The matrix.
 * @return The number of rows in the matrix.
 */
template<typename Type,typename DcmpType>
template<typename Type_>
inline size_t Matrix<Type,DcmpType>::rows_(
    const MatrixType<Type_>& mtrx
)
const noexcept
{
    return mtrx.size();
}

/**
 * @brief Retrieves the number of columns in a matrix.
 *
 * @tparam Type The type of the matrix.
 * @param mtrx The matrix.
 * @return The number of columns in the matrix.
 */
template<typename Type,typename DcmpType>
template<typename Type_>
inline size_t Matrix<Type,DcmpType>::cols_(
    const MatrixType<Type_>& mtrx
)
const noexcept
{
    return mtrx.empty() ? 0 : mtrx.at(0).size();
}

/**
 * @brief Swaps two rows in a matrix.
 *
 * @tparam Type The type of the matrix.
 * @param matrix The matrix.
 * @param swapRow1 The index of the first row to swap.
 * @param swapRow2 The index of the second row to swap.
 * @throws std::out_of_range If any row index is out of range.
 */
template<typename Type,typename DcmpType>
template<typename Type_>
inline void Matrix<Type,DcmpType>::swapRow_(
    MatrixType<Type_>& matrix,
    const size_t& swapRow1,
    const size_t& swapRow2
)
{
    if (swapRow1 >= this->rows_(matrix) || swapRow2 >= this->rows_(matrix))
        throw std::out_of_range("Row index out of range.");

    std::swap(matrix[swapRow1], matrix[swapRow2]);
}

/**
 * @brief Swaps two columns in a matrix.
 *
 * @tparam Type The type of the matrix.
 * @param matrix The matrix.
 * @param swapCol1 The index of the first column to swap.
 * @param swapCol2 The index of the second column to swap.
 * @throws std::out_of_range If any column index is out of range.
 */
template<typename Type,typename DcmpType>
template<typename Type_>
inline void Matrix<Type,DcmpType>::swapCol_(
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

/**
 * @brief Transposes a matrix.
 *
 * @tparam Type The type of the matrix.
 * @param mtrx The matrix to transpose.
 * @return The transposed matrix.
 */
template<typename Type,typename DcmpType>
inline typename Matrix<Type,DcmpType>::template MatrixType<Type> Matrix<Type,DcmpType>::transpose_(
    const MatrixType<Type>& mtrx
)
{
    MatrixType<Type> newMtrx(this->cols_(mtrx), RowType<Type>(this->rows_(mtrx)));

    for (size_t row = 0; row < this->rows_(mtrx); row++) {
        for (size_t col = 0; col < this->cols_(mtrx); col++) {
            newMtrx[col][row] = mtrx[row][col];
        }
    }

    return newMtrx;
}

/**
 * @brief Checks if two matrices have the same dimensions.
 *
 * @tparam Type The type of the matrices.
 * @param mtrx1 The first matrix.
 * @param mtrx2 The second matrix.
 * @return True if the matrices have the same dimensions, false otherwise.
 */
template<typename Type,typename DcmpType>
inline bool Matrix<Type,DcmpType>::areSameSize_(
    const MatrixType<Type>& mtrx1,
    const MatrixType<Type>& mtrx2
)
const noexcept
{
    bool result = (this->rows_(mtrx1) == this->rows_(mtrx2));
    result &= (this->cols_(mtrx1) == this->cols_(mtrx2));

    return result;
}

/**
 * @brief Validates that all rows in a matrix have the same number of columns.
 *
 * @tparam Type The type of the matrix.
 * @param mtrx The matrix to validate.
 * @throws std::invalid_argument If rows have differing numbers of columns.
 */
template<typename Type,typename DcmpType>
inline void Matrix<Type,DcmpType>::validateMatrix_(
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

/**
 * @brief Generates an identity matrix of a given size.
 *
 * @tparam Type_ The type of elements in the matrix.
 * @tparam Type The type of the parent Matrix object.
 * @tparam DcmpType The type used for computations within the Matrix object.
 * @param size The size of the identity matrix (number of rows and columns).
 * @return A square identity matrix of the specified size.
 */
template<typename Type,typename DcmpType>
template<typename Type_>
inline typename Matrix<Type,DcmpType>::MatrixType<Type_> Matrix<Type,DcmpType>::identity_(const size_t& size)
{
    MatrixType<Type_> result(size,RowType<Type_>(size,0));

    for (size_t i = 0; i < size; i++)
        result[i][i] = 1;

    return result;
}

/**
 * @brief Computes a hash value for a given matrix.
 *
 * This function generates a hash value based on the matrix elements, making it useful for caching or quick comparisons.
 * The hashing process uses a combination of element-wise hashes and a constant derived from the golden ratio to reduce collisions.
 *
 * @tparam Type_ The type of elements in the matrix.
 * @tparam Type The type of the parent Matrix object.
 * @tparam DCMPType The type used for computations within the Matrix object.
 * @param mtrx The matrix to be hashed.
 * @return A hash value representing the matrix.
 */
template<typename Type, typename DCMPType>
template<typename Type_>
inline size_t Matrix<Type, DCMPType>::matrixHash_(const MatrixType<Type_>& mtrx)
{
    size_t hash = 0;
    for (const auto& row : mtrx) {
        for (const auto& elem : row)
            hash ^= std::hash<Type_>{}(elem)+0x9e3779b9 + (hash << 6) + (hash >> 2); // 各要素をハッシュ化して結合 0x9e3779b9:黄金比
    }
    return hash;
}

/**
 * @brief Flattens a matrix into a single vector.
 *
 * @tparam Type The type of the elements in the matrix.
 * @tparam DcmpType The decomposition type of the matrix.
 * @param mtrx The matrix to be flattened.
 * @return FlatMatrixType<Type> A single vector containing all elements of the matrix in row-major order.
 */
template<typename Type, typename DcmpType>
inline typename Matrix<Type, DcmpType>::FlatMatrixType<Type> Matrix<Type, DcmpType>::flatten_(
    const MatrixType<Type>& mtrx
)
{
    FlatMatrixType<Type> result;
    result.reserve(this->rows_(mtrx) * this->cols_(mtrx));

    for (const auto& row : mtrx)
        result.insert(result.end(), row.begin(), row.end());

    return result;
}

/**
 * @brief Unflattens a vector into a matrix with specified rows and columns.
 *
 * @tparam Type The type of the elements in the matrix.
 * @tparam DcmpType The decomposition type of the matrix.
 * @param mtrx The flattened vector to be converted back into a matrix.
 * @param rows The number of rows in the resulting matrix.
 * @param cols The number of columns in the resulting matrix.
 * @return MatrixType<Type> A matrix constructed from the elements of the vector.
 */
template<typename Type, typename DcmpType>
inline typename Matrix<Type, DcmpType>::MatrixType<Type> Matrix<Type, DcmpType>::unflatten_(
    const FlatMatrixType<Type>& mtrx,
    const size_t& rows,
    const size_t& cols
)
{
    MatrixType<Type> result(rows, RowType<Type>(cols, 0));
    for (size_t i = 0; i < rows; i++)
        std::copy(mtrx.begin() + i * cols, mtrx.begin() + (i + 1) * cols, result[i].begin());

    return result;
}

/**
 * @brief Transposes the current matrix.
 *
 * @tparam Type The type of the matrix.
 * @return The transposed matrix.
 */
template<typename Type,typename DcmpType>
inline Matrix<Type> Matrix<Type,DcmpType>::transpose()
{
    return this->transpose_(this->matrix_);
}

/**
 * @brief Swaps two rows in the current matrix.
 *
 * @tparam Type The type of the matrix.
 * @param swapRow1 The index of the first row to swap.
 * @param swapRow2 The index of the second row to swap.
 * @return A reference to the current matrix.
 */
template<typename Type,typename DcmpType>
inline Matrix<Type>& Matrix<Type,DcmpType>::swapRow(
    const size_t& swapRow1,
    const size_t& swapRow2
)
{
    this->swapRow_(this->matrix_, swapRow1, swapRow2);
    return *this;
}

/**
 * @brief Swaps two columns in the current matrix.
 *
 * @tparam Type The type of the matrix.
 * @param swapCol1 The index of the first column to swap.
 * @param swapCol2 The index of the second column to swap.
 * @return A reference to the current matrix.
 */
template<typename Type,typename DcmpType>
inline Matrix<Type>& Matrix<Type,DcmpType>::swapCol(
    const size_t& swapCol1,
    const size_t& swapCol2
)
{
    this->swapCol_(this->matrix_, swapCol1, swapCol2);
    return *this;
}

/**
 * @brief Resizes the matrix to the specified dimensions.
 *
 * @tparam Type The type of the matrix.
 * @param row The number of rows.
 * @param col The number of columns.
 * @return A reference to the current matrix.
 */
template<typename Type,typename DcmpType>
inline Matrix<Type>& Matrix<Type,DcmpType>::resize(
    const size_t& row,
    const size_t& col
)
{
    this->matrix_.resize(row, RowType<Type>(col, 0));
    return *this;
}

/**
 * @brief Retrieves the number of rows in the current matrix.
 *
 * @tparam Type The type of the matrix.
 * @return The number of rows.
 */
template<typename Type,typename DcmpType>
const size_t Matrix<Type,DcmpType>::rows() const
{
    return this->rows_(this->matrix_);
}

/**
 * @brief Retrieves the number of columns in the current matrix.
 *
 * @tparam Type The type of the matrix.
 * @return The number of columns.
 */
template<typename Type,typename DcmpType>
const size_t Matrix<Type,DcmpType>::cols() const
{
    return this->cols_(this->matrix_);
}

/**
 * @brief Retrieves references to all elements in the specified row.
 *
 * @tparam Type The type of the matrix.
 * @param rowNum The index of the row.
 * @return A vector of references to the elements in the row.
 */
template<typename Type,typename DcmpType>
inline std::vector<std::reference_wrapper<Type>> Matrix<Type,DcmpType>::rowRef(
    const size_t& rowNum
)
{
    std::vector<std::reference_wrapper<Type>> resultRef;
    resultRef.reserve(this->cols());

    for (auto& elem : this->matrix_[rowNum])
        resultRef.push_back(std::ref(elem));

    return resultRef;
}

/**
 * @brief Retrieves references to all elements in the specified column.
 *
 * @tparam Type The type of the matrix.
 * @param colNum The index of the column.
 * @return A vector of references to the elements in the column.
 */
template<typename Type,typename DcmpType>
inline std::vector<std::reference_wrapper<Type>> Matrix<Type,DcmpType>::colRef(
    const size_t& colNum
)
{
    std::vector<std::reference_wrapper<Type>> resultRef;
    resultRef.reserve(this->rows());

    for (auto& row : this->matrix_)
        resultRef.push_back(std::ref(row[colNum]));

    return resultRef;
}

/**
 * @brief Applies a function to all elements of the matrix (no arguments).
 *
 * @tparam Type The type of the matrix.
 * @param func The function to apply.
 * @return A reference to the current matrix.
 */
template<typename Type,typename DcmpType>
inline Matrix<Type>& Matrix<Type,DcmpType>::forEach(
    std::function<Type()> func
)
{
    for (auto& row : this->matrix_) {
        for (auto& elem : row)
            elem = func();
    }

    return *this;
}

/**
 * @brief Applies a function to all elements of the matrix (with arguments).
 *
 * @tparam Type The type of the matrix.
 * @param func The function to apply. It takes the row, column, and current element as arguments.
 * @return A reference to the current matrix.
 */
template<typename Type,typename DcmpType>
Matrix<Type>& Matrix<Type,DcmpType>::forEach(
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

/**
 * @brief Creates an identity matrix of a given size.
 *
 * This function generates a square identity matrix, where all diagonal elements are set to 1
 * and all other elements are set to 0.
 *
 * @tparam Type_ The type of elements in the identity matrix.
 * @tparam Type The type of the parent Matrix object.
 * @tparam DcmpType The type used for computations within the Matrix object.
 * @param size The size of the identity matrix (number of rows and columns).
 * @return A square identity matrix of the specified size as a `Matrix<Type_>`.
 */
template<typename Type,typename DcmpType>
template<typename Type_>
inline typename Matrix<Type_> Matrix<Type,DcmpType>::identity(const size_t& size)
{
    Matrix<Type_> result(std::make_pair(size,size));
    for (size_t i = 0; i < size; i++)
        result[i][i] = 1;

    return result;
}

#endif
