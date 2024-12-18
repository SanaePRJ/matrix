#ifndef MATRIXCPP_MATRIXOP_HPP
#define MATRIXCPP_MATRIXOP_HPP


#include "matrix.h"


/**
 * @brief Assignment operator for initializer lists.
 *
 * Assigns the contents of an initializer list to the matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param init The initializer list to assign.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(
    const MatrixInitType<Type>& init
)
{
    this->matrix_.clear();
    this->matrix_.assign(init.begin(), init.end());
    this->validateMatrix_(this->matrix_);
    return *this;
}

/**
 * @brief Copy assignment operator.
 *
 * Copies the contents of another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param other The matrix to copy.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(
    const Matrix<Type>& other
)
{
    if (this != &other)
        copyMatrix_(this->matrix_, other.matrix_);

    return *this;
}

/**
 * @brief Move assignment operator.
 *
 * Moves the contents of another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param other The matrix to move.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator=(
    Matrix<Type>&& other
)
{
    if (this != &other)
        this->matrix_ = std::move(other.matrix_);

    return *this;
}

/**
 * @brief Assignment operator for initializer lists using `<<`.
 *
 * Assigns the contents of an initializer list to the matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param init The initializer list to assign.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(
    const MatrixInitType<Type>& init
)
{
    this->matrix_.clear();
    this->matrix_.assign(init.begin(), init.end());
    this->validateMatrix_(this->matrix_);
    return *this;
}

/**
 * @brief Copy assignment operator using `<<`.
 *
 * Copies the contents of another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param other The matrix to copy.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(
    const Matrix<Type>& other
)
{
    if (this != &other)
        this->matrix_ = other.matrix_;

    return *this;
}

/**
 * @brief Move assignment operator using `<<`.
 *
 * Moves the contents of another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param other The matrix to move.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator<<(
    Matrix<Type>&& other
)
{
    if (this != &other)
        this->matrix_ = std::move(other.matrix_);

    return *this;
}

/**
 * @brief Row access operator.
 *
 * Provides access to a specific row of the matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param index The index of the row to access.
 * @return Reference to the row.
 */
template<typename Type>
typename Matrix<Type>::RowType<Type>& Matrix<Type>::operator[](
    const size_t& index
)
{
    return this->matrix_[index];
}

/**
 * @brief Addition assignment operator.
 *
 * Adds another matrix to this matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix to add.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator+=(
    const Matrix<Type>& mtrx
)
{
    this->add(mtrx);
    return *this;
}

/**
 * @brief Subtraction assignment operator.
 *
 * Subtracts another matrix from this matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix to subtract.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator-=(
    const Matrix<Type>& mtrx
)
{
    this->sub(mtrx);
    return *this;
}

/**
 * @brief Multiplication assignment operator.
 *
 * Multiplies this matrix by another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix to multiply.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator*=(
    const Matrix<Type>& mtrx
)
{
    this->mul(mtrx);
    return *this;
}

/**
 * @brief Hadamard product assignment operator.
 *
 * Performs the Hadamard product (element-wise multiplication) with another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix for the Hadamard product.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator^=(
    const Matrix<Type>& mtrx
)
{
    this->hadamardMul(mtrx);
    return *this;
}

/**
 * @brief Hadamard division assignment operator.
 *
 * Performs element-wise division by another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix for element-wise division.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator/=(
    const Matrix<Type>& mtrx
)
{
    this->hadamardDiv(mtrx);
    return *this;
}

/**
 * @brief Scalar multiplication assignment operator.
 *
 * Multiplies all elements of this matrix by a scalar value.
 *
 * @tparam Type The data type of the matrix elements.
 * @param scalar The scalar value to multiply by.
 * @return Reference to the modified matrix.
 */
template<typename Type>
Matrix<Type>& Matrix<Type>::operator*=(
    const Type& scalar
)
{
    this->scalarMul(scalar);
    return *this;
}

/**
 * @brief Addition operator.
 *
 * Adds this matrix to another matrix and returns the result.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix to add.
 * @return A new matrix representing the result of the addition.
 */
template<typename Type>
Matrix<Type> Matrix<Type>::operator+(
    const Matrix<Type>& mtrx
)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->add_(result, mtrx.matrix_);
    return result;
}

/**
 * @brief Subtraction operator.
 *
 * Subtracts another matrix from this matrix and returns the result.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix to subtract.
 * @return A new matrix representing the result of the subtraction.
 */
template<typename Type>
Matrix<Type> Matrix<Type>::operator-(
    const Matrix<Type>& mtrx
)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->sub_(result, mtrx.matrix_);
    return result;
}

/**
 * @brief Multiplication operator.
 *
 * Multiplies this matrix by another matrix and returns the result.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix to multiply.
 * @return A new matrix representing the result of the multiplication.
 */
template<typename Type>
Matrix<Type> Matrix<Type>::operator*(
    const Matrix<Type>& mtrx
)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->mul_(result, mtrx.matrix_);
    return result;
}

/**
 * @brief Hadamard product operator.
 *
 * Performs the Hadamard product (element-wise multiplication) with another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix for the Hadamard product.
 * @return A new matrix representing the result of the Hadamard product.
 */
template<typename Type>
Matrix<Type> Matrix<Type>::operator^(
    const Matrix<Type>& mtrx
)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->hadamardMul_(result, mtrx.matrix_);
    return result;
}

/**
 * @brief Hadamard division operator.
 *
 * Performs element-wise division by another matrix and returns the result.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix for element-wise division.
 * @return A new matrix representing the result of the Hadamard division.
 */
template<typename Type>
Matrix<Type> Matrix<Type>::operator/(
    const Matrix<Type>& mtrx
)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->hadamardDiv_(result, mtrx.matrix_);
    return result;
}

/**
 * @brief Scalar multiplication operator.
 *
 * Multiplies all elements of this matrix by a scalar value and returns the result.
 *
 * @tparam Type The data type of the matrix elements.
 * @param scalar The scalar value to multiply by.
 * @return A new matrix representing the result of the scalar multiplication.
 */
template<typename Type>
Matrix<Type> Matrix<Type>::operator*(
    const Type& scalar
)
{
    MatrixType<Type> result;
    this->copyMatrix_(result, this->matrix_);
    this->scalarCalc_<std::multiplies>(result, scalar);
    return result;
}


#endif
