#ifndef MATRIXCPP_MATRIXCALC_HPP
#define MATRIXCPP_MATRIXCALC_HPP

#include "matrix.h"

/**
 * @brief Adds the elements of two matrices and stores the result in the destination matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param dest The destination matrix where the result will be stored.
 * @param source The source matrix to be added.
 */
template<typename Type,typename DcmpType>
inline void Matrix<Type,DcmpType>::add_(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    this->calcMatrix_<std::plus<Type>>(dest, source);
}

/**
 * @brief Subtracts the elements of one matrix from another and stores the result in the destination matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param dest The destination matrix where the result will be stored.
 * @param source The source matrix to be subtracted.
 */
template<typename Type,typename DcmpType>
inline void Matrix<Type,DcmpType>::sub_(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    this->calcMatrix_<std::minus<Type>>(dest, source);
}

/**
 * @brief Multiplies two matrices using the GPU and returns the resulting matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param matrix1 The first matrix to multiply.
 * @param matrix2 The second matrix to multiply.
 * @return The resulting matrix after multiplication.
 */
template <typename Type, typename DcmpType>
typename Matrix<Type, DcmpType>::MatrixType<Type> Matrix<Type, DcmpType>::mulGPU_(
    const MatrixType<Type>& matrix1,
    const MatrixType<Type>& matrix2
) {
    if (this->cols_(matrix1) != this->rows_(matrix2))
        throw std::invalid_argument("The number of columns in matrix1 must be the same as the number of rows in matrix2.");

#if __has_include("sycl.hpp")
    const size_t crSize   = this->cols_(matrix1);
    const size_t rsltRows = this->rows_(matrix1);
    const size_t rsltCols = this->cols_(matrix2);

    FlatMatrixType<Type> result(rsltRows * rsltCols, 0);
    FlatMatrixType<Type> source1Flat = this->flatten_(matrix1);
    FlatMatrixType<Type> source2Flat = this->flatten_(matrix2);

    try {
        {
            sycl::buffer<Type> buf_result(result.data(), sycl::range<1>(result.size()));
            sycl::buffer<Type> buf_source1(source1Flat.data(), sycl::range<1>(source1Flat.size()));
            sycl::buffer<Type> buf_source2(source2Flat.data(), sycl::range<1>(source2Flat.size()));

            size_t global_work_size[2] = {
                (rsltRows + 15) / 16 * 16, // Round up to a multiple of 16
                (rsltCols + 15) / 16 * 16
            };
            size_t local_work_size[2] = { 16, 16 };

			queue.submit([&](sycl::handler& cgh) {
                auto acc_result  = buf_result.template  get_access<sycl::access::mode::write>(cgh);
                auto acc_source1 = buf_source1.template get_access<sycl::access::mode::read>(cgh);
                auto acc_source2 = buf_source2.template get_access<sycl::access::mode::read>(cgh);

                cgh.parallel_for(
                    sycl::nd_range<2>(
                        sycl::range<2>(global_work_size[0], global_work_size[1]),
                        sycl::range<2>(local_work_size[0], local_work_size[1])
                    ),
                    [=](sycl::nd_item<2> item) {
                        size_t row = item.get_global_id(0);
                        size_t col = item.get_global_id(1);

                        if (row < rsltRows && col < rsltCols) { // Prevent out of range access
                            Type sum = 0;
                            for (size_t k = 0; k < crSize; ++k)
                                sum += acc_source1[row * crSize + k] * acc_source2[k * rsltCols + col];
                            
                            acc_result[row * rsltCols + col] = sum;
                        }
                    });
                });
        }

        return this->unflatten_(result, rsltRows, rsltCols);
    }
    catch (const sycl::exception& e) {
        throw std::runtime_error(e.what());
    }
#else
    throw std::runtime_error("GPU is not supported on this platform.");
#endif
}

/**
 * @brief Multiplies two matrices and returns the resulting matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param matrix1 The first matrix to multiply.
 * @param matrix2 The second matrix to multiply.
 * @return The resulting matrix after multiplication.
 * @throws std::invalid_argument If the number of columns in the first matrix is not equal to the number of rows in the second matrix.
 */
template<typename Type, typename DcmpType>
typename Matrix<Type, DcmpType>::template MatrixType<Type> Matrix<Type, DcmpType>::mul_(
    const MatrixType<Type>& matrix1,
    const MatrixType<Type>& matrix2
)
{
    if (this->cols_(matrix1) != this->rows_(matrix2))
        throw std::invalid_argument("The number of columns in data1 must be the same as the number of rows in data2.");

	if (useGPU)
		return this->mulGPU_(matrix1, matrix2);

    const size_t crSize = this->cols_(matrix1);
    const size_t rsltRows = this->rows_(matrix1);
    const size_t rsltCols = this->cols_(matrix2);

    MatrixType<Type> result(rsltRows, RowType<Type>(rsltCols));

    auto calcLambda = [&matrix1, &matrix2, &crSize](const size_t& posRow, const size_t& posCol) {
        Type resultMatrix = 0;
        for (size_t i = 0; i < crSize; i++)
            resultMatrix += matrix1[posRow][i] * matrix2[i][posCol];

        return resultMatrix;
        };

    for (size_t row = 0; row < rsltRows; row++) {
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703) || __cplusplus >= 201703) && __has_include("execution") // C++17
		std::visit([&](auto&& policy) {
			std::transform(policy, result[row].begin(), result[row].end(), result[row].begin(), [&, row](Type& col) {
				const size_t colIdx = &col - &result[row][0];
				return calcLambda(row, colIdx);
				});
			}, execPolicy);
#else
        for (size_t col = 0; col < rsltCols; col++)
            result[row][col] = calcLambda(row, col);
#endif
    }

    return result;
}

/**
 * @brief Performs element-wise multiplication of two matrices (Hadamard product) and stores the result in the destination matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param dest The destination matrix where the result will be stored.
 * @param source The source matrix to multiply.
 */
template<typename Type,typename DcmpType>
inline void Matrix<Type,DcmpType>::hadamardMul_(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    this->calcMatrix_<std::multiplies<Type>>(dest, source);
}

/**
 * @brief Performs element-wise division of two matrices and stores the result in the destination matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param dest The destination matrix where the result will be stored.
 * @param source The source matrix to divide.
 */
template<typename Type,typename DcmpType>
inline void Matrix<Type,DcmpType>::hadamardDiv_(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    this->calcMatrix_<std::divides<Type>>(dest, source);
}

/**
* @brief Performs an element-wise calculation on two matrices using a specified operation and stores the result in the destination matrix.
* 
* @tparam Type The data type of the matrix elements.
* @tparam CalcType The type of the operation to perform (e.g., addition, subtraction).
* @param dest The destination matrix where the result will be stored.
*/
template<typename Type, typename DcmpType>
template<typename CalcType>
inline void Matrix<Type, DcmpType>::calcMatrixGPU_(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
) {
#if __has_include("sycl.hpp")
    try {
        auto result = this->flatten_(dest);
        auto sourceFlat = this->flatten_(source);

        const size_t rsltSize = result.size();

        size_t local_work_size  = 16;
        size_t global_work_size = ((rsltSize + local_work_size - 1) / local_work_size) * local_work_size;

        CalcType calcType;

        // create buffers for the flattened matrix and the operation type
        {
            sycl::buffer<Type> buf_result(result.data(), sycl::range<1>(result.size()));
            sycl::buffer<Type> buf_source(sourceFlat.data(), sycl::range<1>(sourceFlat.size()));

            queue.submit([&](sycl::handler& cgh) {
                auto acc_result = buf_result.template get_access<sycl::access::mode::read_write>(cgh);
                auto acc_source = buf_source.template get_access<sycl::access::mode::read>(cgh);

                cgh.parallel_for(
                    sycl::nd_range<1>(sycl::range<1>(global_work_size), sycl::range<1>(local_work_size)),
                    [=](sycl::nd_item<1> item) {
                        size_t idx = item.get_global_id(0);

                        if (idx < rsltSize)
                            acc_result[idx] = calcType(acc_result[idx], acc_source[idx]);
                    });
                }).wait_and_throw();
        }

        dest = this->unflatten_(result, this->rows_(dest), this->cols_(dest));
    }
    catch (const sycl::exception& e) {
        throw std::runtime_error(std::string("SYCL Exception: ") + e.what());
    }
#else
    throw std::runtime_error("SYCL is not supported on this platform.");
#endif
}



/**
 * @brief Performs an element-wise calculation on two matrices using a specified operation and stores the result in the destination matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @tparam CalcType The type of the operation to perform (e.g., addition, subtraction).
 * @param dest The destination matrix where the result will be stored.
 * @param source The source matrix for the operation.
 * @throws std::invalid_argument If the matrices are not of the same size.
 */
template<typename Type,typename DcmpType>
template<typename CalcType>
void Matrix<Type, DcmpType>::calcMatrix_(
    MatrixType<Type>& dest,
    const MatrixType<Type>& source
)
{
    if (!this->areSameSize_(dest, source))
        throw std::invalid_argument("The number of rows and columns of data1 and data2 must be equal.");

    if (useGPU) {
		this->calcMatrixGPU_<CalcType>(dest, source);
        return;
    }

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703) || __cplusplus >= 201703) && __has_include("execution") // C++17
    std::visit([&](auto&& policy) {
        auto rowCalcLambda = [&](const size_t row) {
            std::transform(policy, dest[row].begin(), dest[row].end(), source[row].begin(), dest[row].begin(), CalcType());
            };
        std::for_each(policy, dest.begin(), dest.end(), [&rowCalcLambda, row = 0](RowType<Type>&) mutable { rowCalcLambda(row); ++row; });
        }, execPolicy);
#else
    for (size_t row = 0; row < this->rows_(dest); ++row) {
        for (size_t col = 0; col < this->cols_(source); ++col)
            dest[row][col] = CalcType()(dest[row][col], source[row][col]);
    }
#endif
}

/**
 * @brief Performs an element-wise calculation on a matrix with a scalar value using a specified operation.
 *
 * @tparam Type The data type of the matrix elements.
 * @tparam CalcType The type of the operation to perform (e.g., multiplication, division).
 * @param dest The destination matrix where the result will be stored.
 * @param source The scalar value to use in the operation.
 */
template<typename Type, typename DcmpType>
template<typename CalcType>
inline void Matrix<Type, DcmpType>::scalarCalcGPU_(
    MatrixType<Type>& dest,
    const Type& source
)
{
#if __has_include("sycl.hpp")
    try {
        FlatMatrixType<Type> result = this->flatten_(dest);
        CalcType calcType;

		// Create buffers for the flattened matrix and the operation type
        {
            sycl::buffer<Type> buf_result(result.data(), sycl::range<1>(result.size()));
            sycl::buffer<Type> buf_source(&source, sycl::range<1>(1));
            sycl::buffer<CalcType> buf_calcType(&calcType, sycl::range<1>(1));

            queue.submit([&](sycl::handler& cgh) {
                auto acc_result = buf_result.template get_access<sycl::access::mode::read_write>(cgh);
                auto acc_source = buf_source.template get_access<sycl::access::mode::read>(cgh);
                auto acc_calcType = buf_calcType.template get_access<sycl::access::mode::read>(cgh);

                cgh.parallel_for<class matrix_scalar_calc>(
                    sycl::range<1>(result.size()),
                    [=](sycl::id<1> idx) {
                        acc_result[idx] = acc_calcType[0](acc_result[idx], acc_source[0]);
                    });
                }).wait_and_throw();
        }

        dest = this->unflatten_(result, this->rows_(dest), this->cols_(dest));
    }
    catch (const sycl::exception& e) {
        throw std::runtime_error(e.what());
    }
#else
    throw std::runtime_error("GPU is not supported on this platform.");
#endif
}

/**
 * @brief Performs an element-wise calculation on a matrix with a scalar value using a specified operation.
 *
 * @tparam Type The data type of the matrix elements.
 * @tparam CalcType The type of the operation to perform (e.g., multiplication, division).
 * @param dest The destination matrix where the result will be stored.
 * @param source The scalar value to use in the operation.
 */
template<typename Type,typename DcmpType>
template<typename CalcType>
void Matrix<Type,DcmpType>::scalarCalc_(
    MatrixType<Type>& dest,
    const Type& source
)
{
	if (useGPU) {
		this->scalarCalcGPU_<CalcType>(dest, source);
		return;
	}
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703) || __cplusplus >= 201703) && __has_include("execution") // C++17
    std::visit([&](auto&& policy) {
        auto rowCalcLambda = [&](const size_t row) {
            std::transform(policy, dest[row].begin(), dest[row].end(), dest[row].begin(), [&](const Type& d) {return CalcType()(d, source); });
            };
        std::for_each(policy, dest.begin(), dest.end(), [&rowCalcLambda, row = 0](RowType<Type>&) mutable { rowCalcLambda(row); ++row; });
        }, execPolicy);
#else
    for (size_t row = 0; row < this->rows_(dest); ++row) {
        for (size_t col = 0; col < this->cols_(dest); ++col)
            dest[row][col] = CalcType()(dest[row][col], source);
    }
#endif
}

/**
 * @brief Adds another matrix to the current matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix to add.
 * @return A reference to the current matrix after addition.
 */
template<typename Type,typename DcmpType>
Matrix<Type>& Matrix<Type,DcmpType>::add(
    const Matrix<Type>& mtrx
)
{ 
    this->add_(this->matrix_, mtrx.matrix_);

    return *this;
}

/**
 * @brief Subtracts another matrix from the current matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix to subtract.
 * @return A reference to the current matrix after subtraction.
 */
template<typename Type,typename DcmpType>
Matrix<Type>& Matrix<Type,DcmpType>::sub(
    const Matrix<Type>& mtrx
)
{
    this->sub_(this->matrix_, mtrx.matrix_);

    return *this;
}

/**
 * @brief Multiplies the current matrix with another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param mtrx The matrix to multiply with.
 * @return A reference to the current matrix after multiplication.
 */
template<typename Type,typename DcmpType>
Matrix<Type>& Matrix<Type,DcmpType>::mul(
    const Matrix<Type>& mtrx
)
{
    this->matrix_ = this->mul_(this->matrix_, mtrx.matrix_);

    return *this;
}

/**
 * @brief Multiplies each element of the current matrix by a scalar value.
 *
 * @tparam Type The data type of the matrix elements.
 * @param source The scalar value to multiply with.
 * @return A reference to the current matrix after scalar multiplication.
 */
template<typename Type,typename DcmpType>
Matrix<Type>& Matrix<Type,DcmpType>::scalarMul(
    const Type& source
)
{
    this->scalarCalc_<std::multiplies<Type>>(this->matrix_, source);

    return *this;
}

/**
 * @brief Performs an element-wise multiplication (Hadamard product) with another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param source The matrix to perform the Hadamard product with.
 * @return A reference to the current matrix after the operation.
 */
template<typename Type,typename DcmpType>
Matrix<Type>& Matrix<Type,DcmpType>::hadamardMul(
    const Matrix<Type>& source
)
{
    this->hadamardMul_(this->matrix_, source);

    return *this;
}

/**
 * @brief Performs an element-wise division with another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @param source The matrix to divide by.
 * @return A reference to the current matrix after the operation.
 */
template<typename Type,typename DcmpType>
Matrix<Type>& Matrix<Type,DcmpType>::hadamardDiv(
    const Matrix<Type>& source
)
{
    this->hadamardDiv_(this->matrix_, source);

    return *this;
}

/**
 * @brief Performs a scalar operation on the current matrix with another matrix.
 *
 * @tparam Type The data type of the matrix elements.
 * @tparam CalcType The type of the operation to perform.
 * @param source The matrix to use in the operation.
 * @return A reference to the current matrix after the operation.
 */
template<typename Type,typename DcmpType>
template<typename CalcType>
Matrix<Type>& Matrix<Type,DcmpType>::scalarCalc(
    const Type& source
)
{
    this->scalarCalc_<CalcType>(this->matrix_, source);

    return *this;
}

#endif
