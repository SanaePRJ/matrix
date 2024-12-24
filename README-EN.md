# matrixCpp
## Overview
This project aims to develop a library for **simple** and **lightweight** matrix operations.

> [!NOTE]
> The compiler used is MSVC2022 Cpp14 or Cpp17.

## Contribution Guidelines
If you would like to contribute, please follow these steps:

### Steps to Contribute
1. Fork the repository.
2. Create a new branch (`git checkout -b feature/userName`).
3. Commit your changes (`git commit -m 'comment'`).
4. Push to the branch (`git push origin feature/userName`).
5. Create a pull request.

## Classes
### **matrix** Type
Generate matrices of `int` or `double` types using `template`.
```cpp
template<typename Type>
class Matrix{
	// member
};

Matrix<int> mint;
Matrix<double> mdouble;
```

## Implementation Details
- **Data Storage**
  Data is stored and processed in `std::vector<std::vector<Type>>`.
  To enable GPU processing (e.g., with `CUDA`) in the future, a method to expand data into a one-dimensional array will be defined.

- **Type Aliases**
  Since `std::vector<std::vector<Type>>` can be lengthy during implementation, the following type aliases are defined for convenience: `RowType`, `RowInitType`, `MatrixType`, and `MatrixInitType`. These aliases will also be made `public` for external use.
  ```cpp
  template<typename Type>
  class matrix{
  public:
      template<typename rowType    > using RowType     = std::vector<rowType>;
      template<typename rowInitType> using RowInitType = std::initializer_list<rowInitType>;

      template<typename matrixType = Type> using MatrixType     = std::vector          <RowType<matrixType>>;
      template<typename matrixType = Type> using MatrixInitType = std::initializer_list<RowInitType<matrixType>>;
  };
  ```

### Header Files
The library will separate `definition files` and `implementation files` to enhance maintainability.
- **Definition Files**
  - Class declarations
  - Function prototype declarations
  - Macro definitions

- **Implementation Files**
  - Function implementations
  - Member function implementations

#### Include Guards
The naming convention for include guards is `MATRIXCPP_FILENAME_EXTENSION`.
```cpp
// For test.hpp
#ifndef MATRIXCPP_TEST_HPP
#define MATRIXCPP_TEST_HPP

// proc...

#endif
```

#### File Structure
Files are divided into `.h` for definition and `.hpp` for implementation.
- **matrix**
  Includes all related files such as matrix.h and matrixCalc.hpp.
- **matrix.h**
  Contains matrix class definitions, member declarations, and prototypes.
- **matrixCtor.hpp**
  Defines constructors.
- **matrixCalc.hpp**
  Handles operations such as addition, subtraction, and Hadamard product.
- **matrixOp.hpp**
  Implements operator functions.
- **matrixDec.hpp**
  Performs matrix decompositions such as LU decomposition, including inverse matrix computation.
- **matrixUtils.hpp**
  Manages matrix manipulations such as row swapping.

```
matrix ---- matrix.h
        |-- matrixCtor.hpp
        |-- matrixCalc.hpp
        |-- matrixOp.hpp
        |-- matrixDec.hpp
        |__ matrixUtils.hpp
```

## Naming Conventions
- Use [`camelCase`](https://en.wikipedia.org/wiki/Camel_case) for member names.
- Use `PascalCase` for class names.
- **Private Members:** Append `_` to private member names (e.g., `camelCase_`).
- **Prototype Declarations:** Use the format `ReturnType MemberName(ArgType1, ArgType2, ...);`.

## Code Style
### Definition Files
```cpp
template<typename Type>
class Matrix{
private:
    // Private Members

    // Variable Members
    Type testValuePrivate_ = 0; // Comment

    // Function Members (Prototypes)
    Type testFuncPrivate_(const Matrix&, const Matrix&); // Comment

public:
    // Public Members

    // Variable Members
    Type testValuePublic = 0; // Comment

    // Function Members (Prototypes)
    Type testFuncPublic(const Matrix&, const Matrix&); // Comment
};
```

### Implementation Files
```cpp
// Comment
template<typename Type>
Type Matrix<Type>::testFuncPrivate_(const Matrix& a, const Matrix& b){
    return Type();
}

// Comment
template<typename Type>
Type Matrix<Type>::testFuncPublic(const Matrix& a, const Matrix& b){
    return Type();
}
```

## Feature List
### Constructors
- `Matrix() = default;`  
  Default constructor.

- `Matrix(const MatrixInitType<>&);`  
  Constructor with initialization parameters.

- `Matrix(const MatrixType<>&);`  
  Copy constructor.

- `Matrix(const std::pair<size_t, size_t>&);`  
  Constructor with size specification.

- `Matrix(const Matrix<Type, DcmpType>&);`  
  Copy constructor.

- `Matrix(Matrix<Type>&&) noexcept;`  
  Move constructor.

### Operator Overloading
- `Matrix<Type>& operator=(const MatrixInitType<Type>&);`  
  Assignment operator.

- `Matrix<Type>& operator=(const Matrix<Type>&);`  
  Assignment operator.

- `Matrix<Type>& operator<<(const MatrixInitType<Type>&);`  
  Stream insertion operator.

- `Matrix<Type>& operator<<(const Matrix<Type>&);`  
  Stream insertion operator.

- `Matrix<Type>& operator=(Matrix<Type>&&);`  
  Move assignment operator.

- `Matrix<Type>& operator<<(Matrix<Type>&&);`  
  Move stream insertion operator.

- `RowType<Type>& operator[](const size_t&);`  
  Row access.

- `Matrix<Type>& operator+=(const Matrix<Type>&);`  
  Addition.

- `Matrix<Type>& operator-=(const Matrix<Type>&);`  
  Subtraction.

- `Matrix<Type>& operator*=(const Matrix<Type>&);`  
  Multiplication.

- `Matrix<Type>& operator^=(const Matrix<Type>&);`  
  Hadamard product.

- `Matrix<Type>& operator/=(const Matrix<Type>&);`  
  Hadamard division.

- `Matrix<Type>& operator*=(const Type&);`  
  Scalar multiplication.

- `Matrix<Type> operator+(const Matrix<Type>&);`  
  Addition.

- `Matrix<Type> operator-(const Matrix<Type>&);`  
  Subtraction.

- `Matrix<Type> operator*(const Matrix<Type>&);`  
  Multiplication.

- `Matrix<Type> operator^(const Matrix<Type>&);`  
  Hadamard product.

- `Matrix<Type> operator/(const Matrix<Type>&);`  
  Hadamard division.

- `Matrix<Type> operator*(const Type&);`  
  Scalar multiplication.

- `template<typename Type_>`  
  `explicit operator Matrix<Type_>();`  
  Type conversion.

### Member Functions
- `Matrix<Type>& add(const Matrix<Type>&);`  
  Addition.

- `Matrix<Type>& sub(const Matrix<Type>&);`  
  Subtraction.

- `Matrix<Type>& mul(const Matrix<Type>&);`  
  Multiplication.

- `Matrix<Type>& scalarMul(const Type&);`  
  Scalar multiplication.

- `Matrix<Type>& hadamardMul(const Matrix<Type>&);`  
  Hadamard product.

- `Matrix<Type>& hadamardDiv(const Matrix<Type>&);`  
  Hadamard division.

- `template<typename calcType>`  
  `Matrix<Type>& scalarCalc(const Matrix<Type>&);`  
  Scalar calculation.

- `std::vector<Matrix<DcmpType>> luDec(DcmpType epsilon = 1e-9);`  
  LU decomposition.

- `Matrix<DcmpType> inverse(DcmpType epsilon = 1e-9);`  
  Inverse matrix.

- `DcmpType det(DcmpType epsilon = 1e-9);`  
  Determinant.

- `Matrix<Type> transpose();`  
  Transpose.

- `Matrix<Type>& swapRow(const size_t&, const size_t&);`  
  Row swapping.

- `Matrix<Type>& swapCol(const size_t&, const size_t&);`  
  Column swapping.

- `Matrix<Type>& resize(const size_t&, const size_t&);`  
  Resize.

- `const size_t rows() const;`  
  Get the number of rows.

- `const size_t cols() const;`  
  Get the number of columns.

- `std::vector<std::reference_wrapper<Type>> rowRef(const size_t&);`  
  Row reference.

- `std::vector<std::reference_wrapper<Type>> colRef(const size_t&);`  
  Column reference.

- `Matrix<Type>& forEach(std::function<Type()>);`  
  Apply operations to each element.

- `Matrix<Type>& forEach(std::function<Type(size_t, size_t, Type&)>);`  
  Apply operations to each element (row, column, and value).

- `template<typename Type_ = Type>`  
  `static Matrix<Type_> identity(const size_t&);`  
  Generate an identity matrix.

### Stream Output Overloading
- `template<typename CharT, typename Traits, typename MatrixType = double>`  
  `std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>&, Matrix<MatrixType>);`  
  Output matrix.

- `template<typename CharT, typename Traits, typename MatrixType = double>`  
  `std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>&, std::vector<std::vector<MatrixType>>);`  
  Output 2D vectors.

> [!NOTE]
> Additional decomposition methods such as [`QR decomposition`, `Cholesky decomposition`, `Eigenvalue decomposition`, `SVD decomposition`, and `Jordan normal form`](https://en.wikipedia.org/wiki/Matrix_decomposition) are planned.