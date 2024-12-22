#ifndef MATRIXCPP_MATRIXCTOR_HPP
#define MATRIXCPP_MATRIXCTOR_HPP


#include "matrix.h"


/**
 * @brief Constructs a zero matrix of the specified size.
 *
 * @tparam Type The data type of the matrix elements.
 * @param size A pair specifying the number of rows and columns.
 */
template<typename Type,typename DcmpType>
Matrix<Type,DcmpType>::Matrix(
    const std::pair<size_t, size_t>& size
)
    : matrix_(size.first, std::vector<Type>(size.second, 0))
{

}

/**
 * @brief Constructs a matrix using an initializer list.
 *
 * This constructor allows the creation of matrices with a list like:
 * @code
 * {
 *     {0, 0},
 *     {0, 0},
 *     {0, 0}
 * }
 * @endcode
 *
 * @tparam Type The data type of the matrix elements.
 * @param init The initializer list to define the matrix.
 */
template<typename Type,typename DcmpType>
Matrix<Type,DcmpType>::Matrix(
    const MatrixInitType<Type>& init
)
    : matrix_(init.begin(), init.end())
{
    this->validateMatrix_(this->matrix_);
}

/**
 * @brief Constructs a matrix using another matrix-like structure.
 *
 * @tparam Type The data type of the matrix elements.
 * @param init The input matrix structure.
 */
template<typename Type,typename DcmpType>
inline Matrix<Type,DcmpType>::Matrix(
    const MatrixType<>& init
)
    : matrix_(init.begin(), init.end())
{
    this->validateMatrix_(this->matrix_);
}

/**
 * @brief Copy constructor for the Matrix class.
 *
 * Creates a new matrix as a copy of another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param other The matrix to copy.
 */
template<typename Type,typename DcmpType>
Matrix<Type,DcmpType>::Matrix(
    const Matrix& other
)
{
    matrix_.resize(other.matrix_.size());

    // Copy the data
    copyMatrix_(matrix_, other.matrix_);
}

/**
 * @brief Move constructor for the Matrix class.
 *
 * Transfers the resources of another matrix to the current matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param other The matrix to move.
 */
template<typename Type,typename DcmpType>
Matrix<Type,DcmpType>::Matrix(
    Matrix<Type>&& other
)
    noexcept
{
    // Move the resources
    matrix_ = std::move(other.matrix_);
}


#endif
