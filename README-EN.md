# matrixCpp

## Overview
This project aims to develop a **simple** and **lightweight** library for matrix operations.

> [!NOTE]
> The compiler used is MSVC2022 C++17.

## Contribution Guidelines
For those who wish to contribute to the development, please follow these steps:

### How to Contribute
1. Fork the repository.
2. Create a new branch (`git checkout -b feature/userName`).
3. Commit your changes (`git commit -m 'comment'`).
4. Push your branch (`git push origin feature/userName`).
5. Open a pull request.

## Classes

### **Matrix** Type
Using `template`, you can create matrices of types such as `int` or `double`.
```cpp
template<typename Type>
class Matrix {
    // member
};

Matrix<int> mint;
Matrix<double> mdouble;
```
Additionally, there are methods available only when specialized with real-number types like `float` or `double` using [partial template specialization](https://en.wikipedia.org/wiki/Partial_template_specialization).

## Implementation

### Data Storage
Data is stored and processed as `std::vector<std::vector<Type>>`. A method to expand the data into a one-dimensional array will be defined for potential future processing on GPUs such as CUDA.

### Type Aliases
Since `std::vector<std::vector<Type>>` is somewhat lengthy to write during implementation, the following type aliases are defined as `RowType`, `RowInitType`, `MatrixType`, and `MatrixInitType`. These are made `public` for usage outside the class.
```cpp
template<typename Type>
class matrix {
public:
    template<typename rowType    > using RowType     = std::vector<rowType>;
    template<typename rowInitType> using RowInitType = std::initializer_list<rowInitType>;

    template<typename matrixType = Type> using MatrixType     = std::vector          <RowType<matrixType>>;
    template<typename matrixType = Type> using MatrixInitType = std::initializer_list<RowInitType<matrixType>>;
};
```

### Header Files
To enhance maintainability, this library separates `declaration files` and `implementation files`.

- Declaration Files
  - Class declarations
  - Function prototype declarations
  - Macro definitions, etc.
- Implementation Files
  - Function implementations
  - Member function implementations

#### Include Guard
The naming convention for include guards is `MATRIXCPP_filename_extension`.
```cpp
// For test.hpp
#ifndef MATRIXCPP_TEST_HPP
#define MATRIXCPP_TEST_HPP

// proc...

#endif
```

#### File Structure
Files are named with `.h` for declarations and `.hpp` for implementations.

- matrix: Includes all files such as matrix.h, matrixCalc.hpp, etc.
- matrix.h: Defines the `Matrix` class and declares member function prototypes.
- matrixCtor.hpp: Defines constructors.
- matrixCalc.hpp: Handles operations such as addition, subtraction, and Hadamard products.
- matrixOp.hpp: Implements operator functions.
- matrixDec.hpp: Handles matrix decompositions such as LU decomposition and inverse matrices.
- matrixUtils.hpp: Handles operations like row swapping.

```
matrix ---- matrix.h
        |-- matrixCtor.hpp
        |-- matrixCalc.hpp
        |-- matrixOp.hpp
        |-- matrixDec.hpp
        |-- matrixUtils.hpp
```

### Naming Conventions
- Member names are written in [`camelCase`](https://en.wikipedia.org/wiki/Camel_case).
- Class names are written in `PascalCase`.
- Private members: Append `_` to `camelCase` member names.
- Prototype declaration: `returnType memberName(argumentType1, argumentType2, ...);`

### Declaration Files
```cpp
template<typename Type>
class Matrix {
private:
    // Private members

    // Variable members
    Type testValuePrivate_ = 0; // Comment

    // Function members (prototype declaration)
    Type testFuncPrivate_(const Matrix&, const Matrix&); // Comment

public:
    // Public members

    // Variable members
    Type testValuePublic = 0; // Comment

    // Function members (prototype declaration)
    Type testFuncPublic(const Matrix&, const Matrix&); // Comment
};
```

### Implementation Files
```cpp
// Comment
template<typename Type>
Type Matrix<Type>::testFuncPrivate_(const Matrix& a, const Matrix& b) {
    return Type();
}

// Comment
template<typename Type>
Type Matrix<Type>::testFuncPublic(const Matrix& a, const Matrix& b) {
    return Type();
}
```

## Feature List

| Operation          | Operator | Function       | Arguments                          | Return Type                                         | Specialization |
|--------------------|----------|----------------|-------------------------------------|---------------------------------------------------|----------------|
| Assignment         | =, <<    | None           | `MatrixInitType<Type>`             | `Matrix<Type>&`                                    | No             |
| Copy               | =, <<    | None           | `const Matrix<Type>&`              | `Matrix<Type>&`                                    | No             |
| Move Assignment    | =        | None           | `Matrix<Type>&&`                   | `Matrix<Type>&`                                    | No             |
| Row Reference      | []       | rowRef         | `size_t`                           | `std::vector<Type>` / `std::reference_wrapper<Type>` | No             |
| Column Reference   | None     | colRef         | `size_t`                           | `std::vector<Type>` / `std::reference_wrapper<Type>` | No             |
| Addition           | +, +=    | add            | `const Matrix<Type>&`              | `Matrix<Type>` / `Matrix<Type>&`                  | No             |
| Subtraction        | -, -=    | sub            | `const Matrix<Type>&`              | `Matrix<Type>` / `Matrix<Type>&`                  | No             |
| Multiplication     | *, *=    | mul            | `const Matrix<Type>&`              | `Matrix<Type>` / `Matrix<Type>&`                  | No             |
| Scalar Multiplication | *, *= | scalarMul      | `const Matrix<Type>&`              | `Matrix<Type>` / `Matrix<Type>&`                  | No             |
| Hadamard Product   | ^, ^=    | hadamardMul    | `const Matrix<Type>&`              | `Matrix<Type>` / `Matrix<Type>&`                  | No             |
| Hadamard Division  | /, /=    | hadamardDiv    | `const Matrix<Type>&`              | `Matrix<Type>` / `Matrix<Type>&`                  | No             |
| LU Decomposition   | None     | luDec          | `void`                             | `std::vector<Matrix<Type>>`                       | Yes            |
| Determinant        | None     | det            | `void`                             | `ty`                                             | No             |
| Inverse Matrix     | None     | inverse        | `void`                             | `Matrix<Type>`                                    | Yes            |
| Transpose          | None     | transpose      | `void`                             | `Matrix<Type>`                                    | No             |
| Row Swap           | None     | swapRow        | `size_t`, `size_t`                 | `Matrix<Type>&`                                   | No             |
| Column Swap        | None     | swapCol        | `size_t`, `size_t`                 | `Matrix<Type>&`                                   | No             |
| Resize             | None     | resize         | `size_t`, `size_t`                 | `Matrix<Type>&`                                   | No             |
| Get Row Size       | None     | rows           | `void`                             | `size_t`                                         | No             |
| Get Column Size    | None     | cols           | `void`                             | `size_t`                                         | No             |
| Element Operation  | None     | forEach        | `std::function<ty()>`              | `Matrix<Type>&`                                   | No             |
| Element Operation  | None     | forEach        | `std::function<ty(size_t, size_t, ty&)>` | `Matrix<Type>&`                             | No             |

> [!NOTE]
> Additional decompositions such as [`QR decomposition`, `Cholesky decomposition`, `Eigenvalue decomposition`, `SVD decomposition`, `Jordan normal form`](https://en.wikipedia.org/wiki/Matrix_decomposition) are planned.
