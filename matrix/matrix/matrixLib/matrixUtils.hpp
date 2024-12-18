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

/**
 * @brief Retrieves the number of rows in a matrix.
 *
 * @tparam Type The type of the matrix.
 * @param mtrx The matrix.
 * @return The number of rows in the matrix.
 */
template<typename Type>
inline size_t Matrix<Type>::rows_(
    const MatrixType<Type>& mtrx
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
template<typename Type>
inline size_t Matrix<Type>::cols_(
    const MatrixType<Type>& mtrx
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
template<typename Type>
inline void Matrix<Type>::swapRow_(
    MatrixType<Type>& matrix,
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
template<typename Type>
inline void Matrix<Type>::swapCol_(
    MatrixType<Type>& matrix,
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
template<typename Type>
inline typename Matrix<Type>::template MatrixType<Type> Matrix<Type>::transpose_(
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
template<typename Type>
inline bool Matrix<Type>::areSameSize_(
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

/**
 * @brief Transposes the current matrix.
 *
 * @tparam Type The type of the matrix.
 * @return The transposed matrix.
 */
template<typename Type>
inline Matrix<Type> Matrix<Type>::transpose()
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
template<typename Type>
inline Matrix<Type>& Matrix<Type>::swapRow(
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
template<typename Type>
inline Matrix<Type>& Matrix<Type>::swapCol(
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
template<typename Type>
inline Matrix<Type>& Matrix<Type>::resize(
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
template<typename Type>
const size_t Matrix<Type>::rows()
{
    return this->rows_(this->matrix_);
}

/**
 * @brief Retrieves the number of columns in the current matrix.
 *
 * @tparam Type The type of the matrix.
 * @return The number of columns.
 */
template<typename Type>
const size_t Matrix<Type>::cols()
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

/**
 * @brief Retrieves references to all elements in the specified column.
 *
 * @tparam Type The type of the matrix.
 * @param colNum The index of the column.
 * @return A vector of references to the elements in the column.
 */
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

/**
 * @brief Applies a function to all elements of the matrix (no arguments).
 *
 * @tparam Type The type of the matrix.
 * @param func The function to apply.
 * @return A reference to the current matrix.
 */
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

/**
 * @brief Applies a function to all elements of the matrix (with arguments).
 *
 * @tparam Type The type of the matrix.
 * @param func The function to apply. It takes the row, column, and current element as arguments.
 * @return A reference to the current matrix.
 */
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
