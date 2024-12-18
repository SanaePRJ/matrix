#ifndef MATRIXCPP_MATRIXUTILS_HPP
#define MATRIXCPP_MATRIXUTILS_HPP


#include "matrix.h"


// �}�g���b�N�X�̓��e���R�s�[����e���v���[�g�֐�
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

// �s�����擾����e���v���[�g�֐�
template<typename Type>

inline size_t Matrix<Type>::rows_(
    const MatrixType<Type>& mtrx
)
    const noexcept
{
    return mtrx.size();
}

// �񐔂��擾����e���v���[�g�֐�
template<typename Type>

inline size_t Matrix<Type>::cols_(
    const MatrixType<Type>& mtrx
)
    const noexcept
{
    return mtrx.empty() ? 0 : mtrx.at(0).size();
}

// �}�g���b�N�X�̍s�����ւ���e���v���[�g�֐�
template<typename Type>

inline void Matrix<Type>::swapRow_(
    MatrixType<Type>& matrix, 
    const size_t& swapRow1, 
    const size_t& swapRow2
)
{
    // �s�C���f�b�N�X���͈͊O�̏ꍇ�ɗ�O�𓊂���
    if (swapRow1 >= this->rows_(matrix) || swapRow2 >= this->rows_(matrix))
        throw std::out_of_range("Row index out of range.");

    // �s�����ւ���
    std::swap(matrix[swapRow1], matrix[swapRow2]);
}

// �}�g���b�N�X�̗�����ւ���e���v���[�g�֐�
template<typename Type>

inline void Matrix<Type>::swapCol_(
    MatrixType<Type>& matrix, 
    const size_t& swapCol1, 
    const size_t& swapCol2
)
{
    // ��C���f�b�N�X���͈͊O�̏ꍇ�ɗ�O�𓊂���
    if (matrix.empty() || swapCol1 >= this->cols_(matrix) || swapCol2 >= this->cols_(matrix))
        throw std::out_of_range("Column index out of range.");

    // �e�s�̎w�肳�ꂽ������ւ���
    for (auto& row : matrix)
        std::swap(row[swapCol1], row[swapCol2]);
}

// �}�g���b�N�X�̓]�u���s���e���v���[�g�֐�
template<typename Type>
inline typename Matrix<Type>::template MatrixType<Type> Matrix<Type>::transpose_(
    const MatrixType<Type>& mtrx
)
{
    // �]�u��̐V�����}�g���b�N�X���쐬
    MatrixType<Type> newMtrx(this->cols_(mtrx), RowType<Type>(this->rows_(mtrx)));

    // �]�u�����s
    for (size_t row = 0; row < this->rows_(mtrx); row++) {
        for (size_t col = 0; col < this->cols_(mtrx); col++) {
            newMtrx[col][row] = mtrx[row][col];
        }
    }

    return newMtrx;
}

// 2�̃}�g���b�N�X�������T�C�Y���ǂ������`�F�b�N����e���v���[�g�֐�
template<typename Type>
inline bool Matrix<Type>::areSameSize_(
    const MatrixType<Type>& mtrx1, 
    const MatrixType<Type>& mtrx2
)
    const noexcept
{
    bool result;

    // �s���Ɨ񐔂���v���邩�ǂ������m�F
    result = this->rows_(mtrx1) == this->rows_(mtrx2);
    result &= this->cols_(mtrx1) == this->cols_(mtrx2);

    return result;
}

// �}�g���b�N�X�̊e�s�������T�C�Y���ǂ������`�F�b�N����e���v���[�g�֐�
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

// �}�g���b�N�X��]�u����֐�
template<typename Type>
inline Matrix<Type> Matrix<Type>::transpose()
{
    return this->transpose_(this->matrix_);
}

// �w�肳�ꂽ2�̍s�����ւ���֐�
template<typename Type>
inline Matrix<Type>& Matrix<Type>::swapRow(
    const size_t& swapRow1, 
    const size_t& swapRow2
)
{
    this->swapRow_(this->matrix_, swapRow1, swapRow2);
    return *this;
}

// �w�肳�ꂽ2�̗�����ւ���֐�
template<typename Type>
inline Matrix<Type>& Matrix<Type>::swapCol(
    const size_t& swapCol1,
    const size_t& swapCol2
)
{
    this->swapCol_(this->matrix_, swapCol1, swapCol2);
    return *this;
}

// �}�g���b�N�X�̃T�C�Y��ύX����֐�
template<typename Type>
inline Matrix<Type>& Matrix<Type>::resize(
    const size_t& row, 
    const size_t& col
)
{
    this->matrix_.resize(row, RowType<Type>(col, 0));
    return *this;
}

// �s�����擾����֐�
template<typename Type>
const size_t Matrix<Type>::rows()
{
    return this->rows_(this->matrix_);
}

// �񐔂��擾����֐�
template<typename Type>
const size_t Matrix<Type>::cols()
{
    return this->cols_(this->matrix_);
}

// �w�肳�ꂽ�s�̎Q�Ƃ��擾����֐�
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

// �w�肳�ꂽ��̎Q�Ƃ��擾����֐�
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

// �}�g���b�N�X�̑S�v�f�ɑ΂��Ďw�肳�ꂽ�֐���K�p����֐��i�����Ȃ��Łj
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

// �}�g���b�N�X�̑S�v�f�ɑ΂��Ďw�肳�ꂽ�֐���K�p����֐��i��������Łj
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
