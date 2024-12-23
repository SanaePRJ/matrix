# matrixCpp
## �T�v
���̃v���W�F�N�g��**�V���v��**�Ȏ�������**�y��**�ȍs�񑀍���\�Ƃ��郉�C�u�����̊J����ړI�Ƃ��Ă��܂��B

> [!NOTE]
> �R���p�C����MSVC2022 Cpp14 or 17�Ƃ��܂��B

## �v�����@
�J���ɋ��͂��Ă���������͈ȉ��̕��@�ł��肢���܂��B

### �v��
1. ���|�W�g�����t�H�[�N���܂��B
2. �V�����u�����`���쐬���܂� (`git checkout -b feature/userName`).
3. �ύX���R�~�b�g���܂� (`git commit -m 'comment'`).
4. �u�����`�Ƀv�b�V�����܂� (`git push origin feature/userName`).
5. �v�����N�G�X�g���쐬���܂��B

## �N���X
### **matrix**�^
`template`��p����`int`��`double`�^�̍s��𐶐����܂��B  
```cpp
template<typename Type>
class Matrix{
	// member
}

Matrix<int> mint;
Matrix<double> mdouble;
```

## ����
 - �f�[�^�̊i�[
	�f�[�^��`std::vector<std::vector<Type>>`�Ŋi�[���������܂��B
	�����I��`CUDA`�Ȃ�GPU��ł̏������s�킹��\��ł��̂ňꎟ���z��֓W�J���郁�\�b�h���`���܂��B
 - �^�G�C���A�X
	`std::vector<std::vector<Type>>`�͎�������ۂɏ����������߈ȉ��̃R�[�h�ɂ��^�G�C���A�X`RowType`,`RowInitType`,`MatrixType`,`MatrixInitType`���`���܂��B
	�܂��N���X�O���ł��g�p�ł���悤`public`�ɂ��Ă����܂��B
	```cpp
	template<typename Type>
	class matrix{
	public:
		template<typename rowType    > using RowType     = std::vector<rowType>;
		template<typename rowInitType> using RowInitType = std::initializer_list<rowInitType>;

		template<typename matrixType = Type> using MatrixType     = std::vector          <RowType<matrixType>>;
		template<typename matrixType = Type> using MatrixInitType = std::initializer_list<RowInitType<matrixType>>;
	} 
	```
### �w�b�_�t�@�C��
���̃v���W�F�N�g�̃��C�u�����͕ێ琫����̂���`��`�t�@�C��`��`�����t�@�C��`�ɕ����鎖�Ƃ��܂��B
- ��`�t�@�C��
	- �N���X�̐錾
	- �֐��̃v���g�^�C�v�錾
	- �}�N���̒�`�Ȃ�
- �����t�@�C��
	- �֐��̎���
	- �����o�֐��̎���

#### Include Guard
�C���N���[�h�K�[�h�̖�����`MATRIXCPP_�t�@�C����_�g���q`�Ƃ��܂��B
```cpp
// test.hpp�̏ꍇ
#ifndef MATRIXCPP_TEST_HPP
#define MATRIXCPP_TEST_HPP

// proc...

#endif
```

#### ����
`.h`�͒�`�t�@�C���A`.hpp`�͎����t�@�C�����Ӗ����܂��B
 - matrix
	matrix.h matrixCalc.hpp�Ȃǂ��ׂẴt�@�C�����C���N���[�h���܂��B
 - matrix.h
	matrix�N���X�̒�`,�����o�֐��̃v���g�^�C�v�錾�Ȃǃ����o�̒�`���s���܂��B
 - matrixCtor.hpp
	�R���X�g���N�^���`���܂��B
 - matrixCalc.hpp
	���Z,���Z,�A�_�}�[���ςȂǌv�Z���s���܂��B
 - matrixOp.hpp
	���Z�q�֐��̎������s���܂��B
 - matrixDec.hpp
	LU�����ȂǍs�񕪉����s���܂��B�t�s����܂ށB
 - matrixUtils.hpp
	�s����ւ��ȂǍs��̑�����s���܂��B

```
matrix ---- matrix.h
        |-- matrixCtor.hpp
        |-- matrixCalc.hpp
        |-- matrixOp.hpp
        |-- matrixDec.hpp
        |__ matrixUtils.hpp
```
#### 
## ����
- �����o����[`camelCase`](https://en.wikipedia.org/wiki/Camel_case)�ŋL�q���܂��B
- �N���X����`PascalCase`�ŋL�q���܂��B
- �v���C�x�[�g�����o
	�v���C�x�[�g�����o��(`camelCase`)�̌��`_`�����܂��B
- �v���g�^�C�v�錾
	`�Ԃ�l�^ �����o��(�����^1,�����^2,...);`

## �L�@
### ��`�t�@�C��
```cpp
template<typename Type>
class Matrix{
private:
	// �v���C�x�[�g�����o

	// �ϐ������o
	Type testValuePrivate_ = 0; // �R�����g

	// ���������o(�v���g�^�C�v�錾)
	Type testFuncPrivate_(const Matrix&,const Matrix&); // �R�����g

public:
	// �p�u���b�N�����o
	
	// �ϐ������o
	Type testValuePublic = 0; // �R�����g

	// ���������o(�v���g�^�C�v�錾)
	Type testFuncPublic(const Matrix&,const Matrix&); // �R�����g
}
```
### �����t�@�C��
```cpp

// �R�����g
template<typename Type>
Type Matrix<Type>::testFuncPrivate_(const Matrix& a,const Matrix& b){
	return Type();
}

// �R�����g
template<typename Type>
Type Matrix<Type>::testFuncPublic(const Matrix& a,const Matrix& b){
	return Type();
}
```

## �@�\�ꗗ
### �R���X�g���N�^
- `Matrix() = default;`  
  �f�t�H���g�R���X�g���N�^

- `Matrix(const MatrixInitType<>&);`  
  �������p�̃p�����[�^�t���R���X�g���N�^

- `Matrix(const MatrixType<>&);`  
  �R�s�[�R���X�g���N�^

- `Matrix(const std::pair<size_t, size_t>&);`  
  �T�C�Y�w��̃p�����[�^�t���R���X�g���N�^

- `Matrix(const Matrix<Type, DcmpType>&);`  
  �R�s�[�R���X�g���N�^

- `Matrix(Matrix<Type>&&) noexcept;`  
  ���[�u�R���X�g���N�^

### ���Z�q�I�[�o�[���[�h
- `Matrix<Type>& operator=(const MatrixInitType<Type>&);`  
  ������Z�q

- `Matrix<Type>& operator=(const Matrix<Type>&);`  
  ������Z�q

- `Matrix<Type>& operator<<(const MatrixInitType<Type>&);`  
  �X�g���[���}�����Z�q

- `Matrix<Type>& operator<<(const Matrix<Type>&);`  
  �X�g���[���}�����Z�q

- `Matrix<Type>& operator=(Matrix<Type>&&);`  
  ���[�u������Z�q

- `Matrix<Type>& operator<<(Matrix<Type>&&);`  
  ���[�u�X�g���[���}�����Z�q

- `RowType<Type>& operator[](const size_t&);`  
  �s�A�N�Z�X

- `Matrix<Type>& operator+=(const Matrix<Type>&);`  
  ���Z

- `Matrix<Type>& operator-=(const Matrix<Type>&);`  
  ���Z

- `Matrix<Type>& operator*=(const Matrix<Type>&);`  
  ��Z

- `Matrix<Type>& operator^=(const Matrix<Type>&);`  
  �A�_�}�[����

- `Matrix<Type>& operator/=(const Matrix<Type>&);`  
  �A�_�}�[�����Z

- `Matrix<Type>& operator*=(const Type&);`  
  �X�J����Z

- `Matrix<Type> operator+(const Matrix<Type>&);`  
  ���Z

- `Matrix<Type> operator-(const Matrix<Type>&);`  
  ���Z

- `Matrix<Type> operator*(const Matrix<Type>&);`  
  ��Z

- `Matrix<Type> operator^(const Matrix<Type>&);`  
  �A�_�}�[����

- `Matrix<Type> operator/(const Matrix<Type>&);`  
  �A�_�}�[�����Z

- `Matrix<Type> operator*(const Type&);`  
  �X�J����Z

- `template<typename Type_>`  
  `explicit operator Matrix<Type_>();`  
  �^�ϊ�

### �����o�֐�
- `Matrix<Type>& add(const Matrix<Type>&);`  
  ���Z

- `Matrix<Type>& sub(const Matrix<Type>&);`  
  ���Z

- `Matrix<Type>& mul(const Matrix<Type>&);`  
  ��Z

- `Matrix<Type>& scalarMul(const Type&);`  
  �X�J����Z

- `Matrix<Type>& hadamardMul(const Matrix<Type>&);`  
  �A�_�}�[����

- `Matrix<Type>& hadamardDiv(const Matrix<Type>&);`  
  �A�_�}�[�����Z

- `template<typename calcType>`  
  `Matrix<Type>& scalarCalc(const Matrix<Type>&);`  
  �X�J���v�Z

- `std::vector<Matrix<DcmpType>> luDec(DcmpType epsilon = 1e-9);`  
  LU����

- `Matrix<DcmpType> inverse(DcmpType epsilon = 1e-9);`  
  �t�s��

- `DcmpType det(DcmpType epsilon = 1e-9);`  
  �s��

- `Matrix<Type> transpose();`  
  �]�u

- `Matrix<Type>& swapRow(const size_t&, const size_t&);`  
  �s�̓���ւ�

- `Matrix<Type>& swapCol(const size_t&, const size_t&);`  
  ��̓���ւ�

- `Matrix<Type>& resize(const size_t&, const size_t&);`  
  �T�C�Y�ύX

- `const size_t rows() const;`  
  �s���̎擾

- `const size_t cols() const;`  
  �񐔂̎擾

- `std::vector<std::reference_wrapper<Type>> rowRef(const size_t&);`  
  �s�Q��

- `std::vector<std::reference_wrapper<Type>> colRef(const size_t&);`  
  ��Q��

- `Matrix<Type>& forEach(std::function<Type()>);`  
  �e�v�f�ւ̑���

- `Matrix<Type>& forEach(std::function<Type(size_t, size_t, Type&)>);`  
  �e�v�f�ւ̑���i�s�A��A���̃|�C���g�̒l�j

- `template<typename Type_ = Type>`  
  `static Matrix<Type_> identity(const size_t&);`  
  �P�ʍs�񐶐�

### �X�g���[���o�̓I�[�o�[���[�h
- `template<typename CharT, typename Traits, typename MatrixType = double>`  
  `std::basic_ostream<CharT, Traits>& operator <<(std::basic_ostream<CharT, Traits>&, Matrix<MatrixType>);`  
  �s��̏o�̓I�[�o�[���[�h

- `template<typename CharT, typename Traits, typename MatrixType = double>`  
  `std::basic_ostream<CharT, Traits>& operator <<(std::basic_ostream<CharT, Traits>&, std::vector<std::vector<MatrixType>>);`  
  2�����x�N�g���̏o�̓I�[�o�[���[�h

> [!NOTE]
> ���̕���@�͒ǉ��\��[`QR����`,`�R���X�L�[����`,`�ŗL�l����`,`SVD����`,`�W�����_���W���``](https://en.wikipedia.org/wiki/Matrix_decomposition)