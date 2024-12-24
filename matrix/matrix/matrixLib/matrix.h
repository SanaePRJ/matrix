/**
 * @file matrix.h
 * @brief Defines the Matrix class, including member function prototypes and other member definitions.
 */

#ifndef MATRIXCPP_MATRIX_H
#define MATRIXCPP_MATRIX_H

#include <algorithm>
#include <functional>
#include <iomanip>
#include <vector>

#if __has_include("execution")
    #include <execution>
    #include <variant>
#endif

#if __has_include("sycl.hpp")
    #include <CL/sycl.hpp>
#endif

 /**
  * @brief Template definition for the Matrix class
  *
  * @tparam Type Type of the matrix elements
  * @tparam DcmpType Type used during decomposition
  */
template<typename Type, typename DcmpType = double>
class Matrix {
public:
    /// Type alias declaration
    template<typename rowType    > using RowType     = std::vector<rowType>;               ///< Row type
    template<typename rowInitType> using RowInitType = std::initializer_list<rowInitType>; ///< Row type (initialization)

    template<typename matrixType = Type>     using MatrixType     = std::vector<RowType<matrixType>>;                   ///< Matrix type
    template<typename matrixInitType = Type> using MatrixInitType = std::initializer_list<RowInitType<matrixInitType>>; ///< Matrix type (initialization)

	template<typename flatMatrixType = Type> using FlatMatrixType = std::vector<flatMatrixType>; ///< Flat matrix type

	// C++17 features
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703) || __cplusplus >= 201703) && __has_include("execution")
    std::variant< 
        std::execution::sequenced_policy, 
        std::execution::parallel_policy, 
        std::execution::parallel_unsequenced_policy 
    > execPolicy = std::execution::par_unseq;
#endif

	// GPU
#if __has_include("sycl.hpp")
	sycl::queue queue  = sycl::default_selector{};
	bool        useGPU = true;
#else
	const bool useGPU = false; ///< Cna't use GPU
#endif

private:
    MatrixType<> matrix_; ///< Data storage variable

    size_t dcmpHash_ = 0; ///< Hash value for LU decomposition
    std::vector<MatrixType<DcmpType>> dcmp_; ///< Data storage variable after decomposition

public:
    /**
     * @brief Default constructor
     */
    Matrix() = default;

    /**
     * @brief Constructor with parameters for initialization
     *
     * @param init Initializer list
     */
    Matrix(const MatrixInitType<>&);

    /**
     * @brief Copy constructor
     *
     * @param matrix Matrix to copy from
     */
    Matrix(const MatrixType<>&);

    /**
     * @brief Constructor with size specification
     *
     * @param size Size of the matrix
     */
    Matrix(const std::pair<size_t, size_t>&);

    /**
     * @brief Copy constructor
     *
     * @param other Matrix to copy from
     */
    Matrix(const Matrix<Type, DcmpType>&);

    /**
     * @brief Move constructor
     *
     * @param other Matrix to move from
     */
    Matrix(Matrix<Type>&&) noexcept;

    /// Operator overloads
    Matrix<Type>& operator=(const MatrixInitType<Type>&); ///< Assignment operator
    Matrix<Type>& operator=(const Matrix<Type>&);         ///< Assignment operator
    Matrix<Type>& operator<<(const MatrixInitType<Type>&);///< Stream insertion operator
    Matrix<Type>& operator<<(const Matrix<Type>&);        ///< Stream insertion operator

    Matrix<Type>& operator=(Matrix<Type>&&); ///< Move assignment
    Matrix<Type>& operator<<(Matrix<Type>&&); ///< Move stream insertion

    RowType<Type>& operator[](const size_t&); ///< Row access

    Matrix<Type>& operator+=(const Matrix<Type>&); ///< Addition
    Matrix<Type>& operator-=(const Matrix<Type>&); ///< Subtraction
    Matrix<Type>& operator*=(const Matrix<Type>&); ///< Multiplication
    Matrix<Type>& operator^=(const Matrix<Type>&); ///< Hadamard product
    Matrix<Type>& operator/=(const Matrix<Type>&); ///< Hadamard division
    Matrix<Type>& operator*=(const Type&);         ///< Scalar multiplication

    Matrix<Type> operator+(const Matrix<Type>&); ///< Addition
    Matrix<Type> operator-(const Matrix<Type>&); ///< Subtraction
    Matrix<Type> operator*(const Matrix<Type>&); ///< Multiplication
    Matrix<Type> operator^(const Matrix<Type>&); ///< Hadamard product
    Matrix<Type> operator/(const Matrix<Type>&); ///< Hadamard division
    Matrix<Type> operator*(const Type&);         ///< Scalar multiplication

    template<typename Type_>
    explicit operator Matrix<Type_>(); ///< Type conversion

private:
    void add_(MatrixType<Type>&, const MatrixType<Type>&); ///< Addition operation
    void sub_(MatrixType<Type>&, const MatrixType<Type>&); ///< Subtraction operation
    MatrixType<Type> mul_(const MatrixType<Type>&, const MatrixType<Type>&); ///< Multiplication operation

    void hadamardMul_(MatrixType<Type>&, const MatrixType<Type>&); ///< Hadamard product
    void hadamardDiv_(MatrixType<Type>&, const MatrixType<Type>&); ///< Hadamard division

    template<typename CalcType>
	void calcMatrixGPU_(MatrixType<Type>&, const MatrixType<Type>&); ///< General calculation operation (GPU)
    template<typename CalcType>
    void calcMatrix_(MatrixType<Type>&, const MatrixType<Type>&); ///< General calculation operation

    template<typename CalcType>
	void scalarCalcGPU_(MatrixType<Type>&, const Type&); ///< Scalar operation (GPU)
    template<typename CalcType>
    void scalarCalc_(MatrixType<Type>&, const Type&); ///< Scalar operation

    std::vector<MatrixType<DcmpType>> luDec_(const MatrixType<Type>&, DcmpType epsilon = 1e-9); ///< LU decomposition
    MatrixType<DcmpType> inverse_(const MatrixType<Type>&, DcmpType epsilon = 1e-9); ///< Inverse matrix
    DcmpType det_(const MatrixType<Type>&, DcmpType epsilon = 1e-9); ///< Determinant

public:
    Matrix<Type>& add(const Matrix<Type>&); ///< Addition
    Matrix<Type>& sub(const Matrix<Type>&); ///< Subtraction
    Matrix<Type>& mul(const Matrix<Type>&); ///< Multiplication
    Matrix<Type>& scalarMul(const Type&);   ///< Scalar multiplication
    Matrix<Type>& hadamardMul(const Matrix<Type>&); ///< Hadamard product
    Matrix<Type>& hadamardDiv(const Matrix<Type>&); ///< Hadamard division
    template<typename CalcType>
    Matrix<Type>& scalarCalc(const Type&); ///< Scalar calculation

    std::vector<Matrix<DcmpType>> luDec(DcmpType epsilon = 1e-9); ///< LU decomposition
    Matrix<DcmpType> inverse(DcmpType epsilon = 1e-9); ///< Inverse matrix
    DcmpType det(DcmpType epsilon = 1e-9); ///< Determinant

private:
    template<typename CopyType1, typename CopyType2>
    void copyMatrix_(MatrixType<CopyType1>&, const MatrixType<CopyType2>&); ///< Copy each element

    template<typename Type_ = Type>
    size_t rows_(const MatrixType<Type_>&) const noexcept; ///< Get the number of rows
    template<typename Type_ = Type>
    size_t cols_(const MatrixType<Type_>&) const noexcept; ///< Get the number of columns

    template<typename Type_ = Type>
    void swapRow_(MatrixType<Type_>&, const size_t&, const size_t&); ///< Swap rows
    template<typename Type_ = Type>
    void swapCol_(MatrixType<Type_>&, const size_t&, const size_t&); ///< Swap columns

    MatrixType<Type> transpose_(const MatrixType<Type>&); ///< Transpose

    bool areSameSize_(const MatrixType<Type>&, const MatrixType<Type>&) const noexcept; ///< Compare sizes
    void validateMatrix_(const MatrixType<Type>&); ///< Validate the matrix

    template<typename Type_ = Type>
    static MatrixType<Type_> identity_(const size_t&); ///< Generate identity matrix

    template<typename Type_ = Type>
    size_t matrixHash_(const MatrixType<Type_>&); ///< Generate matrix hash value

    FlatMatrixType<Type> flatten_(const MatrixType<Type>&); ///< Flatten matrix
    MatrixType<Type>     unflatten_(const FlatMatrixType<Type>&, const size_t&, const size_t&); ///< Unflatten matrix

public:
    Matrix<Type> transpose(); ///< Transpose
    Matrix<Type>& swapRow(const size_t&, const size_t&); ///< Swap rows
    Matrix<Type>& swapCol(const size_t&, const size_t&); ///< Swap columns

    Matrix<Type>& resize(const size_t&, const size_t&); ///< Resize matrix

    const size_t rows() const; ///< Get the number of rows
    const size_t cols() const; ///< Get the number of columns

    std::vector<std::reference_wrapper<Type>> rowRef(const size_t&); ///< Row reference
    std::vector<std::reference_wrapper<Type>> colRef(const size_t&); ///< Column reference

    Matrix<Type>& forEach(std::function<Type()>); ///< Apply operation to each element
    Matrix<Type>& forEach(std::function<Type(size_t, size_t, Type&)>); ///< Apply operation to each element (row, column, value)

    template<typename Type_ = Type>
    static Matrix<Type_> identity(const size_t&); ///< Generate identity matrix

};

/**
 * @brief Overload for outputting a matrix
 *
 * @tparam CharT Character type of the output stream
 * @tparam Traits Traits of the output stream
 * @tparam MatrixType Element type of the matrix
 * @param ostrm Output stream
 * @param mtrx Matrix object
 * @return Output stream
 */
template<typename CharT, typename Traits, typename MatrixType = double>
std::basic_ostream<CharT, Traits>& operator <<(
    std::basic_ostream<CharT, Traits>& ostrm,
    Matrix<MatrixType> mtrx
)
{
    for (size_t row = 0; row < mtrx.rows(); row++) {
        for (MatrixType col : mtrx[row])
            ostrm << std::setw(10) << col;

        ostrm << std::endl;
    }

    return ostrm;
}

template<typename CharT, typename Traits, typename MatrixType = double>
std::basic_ostream<CharT, Traits>& operator <<(
    std::basic_ostream<CharT, Traits>& ostrm,
    std::vector<std::vector<MatrixType>> mtrx
)
{
    for (std::vector<MatrixType>& row : mtrx) {
        for (MatrixType& col : row)
            ostrm << std::setw(10) << col;

        ostrm << std::endl;
    }

    return ostrm;
}

#endif