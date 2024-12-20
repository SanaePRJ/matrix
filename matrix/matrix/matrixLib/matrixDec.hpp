#ifndef MATRIXCPP_MATRIXDEC_HPP
#define MATRIXCPP_MATRIXDEC_HPP


#include "matrix.h"
#include <iostream>


template<typename Type,typename DcmpType>
inline std::vector<typename Matrix<Type,DcmpType>::MatrixType<DcmpType>> Matrix<Type,DcmpType>::luDec_(
    const MatrixType<Type>& mtrx,
    DcmpType epsilon
)
{
    // �L���b�V���̗L�������m�F
    if (mtrx == this->matrix_ && this->matrixHash(this->matrix_) == this->dcmpHash_)
        return this->dcmp_;

    if (this->rows_(mtrx) != this->cols_(mtrx))
        throw std::invalid_argument("Must be a square matrix.");

    // ���ׂĂ̗�ɑ΂��� lmbdRowFrom �s�� lmbdNum �{�������̂� lmbdRowTo �s�։��Z����B
    auto applyRowOperation = [&](MatrixType<DcmpType>& lmbdMtrx, size_t lmbdRowFrom, size_t lmbdRowTo, DcmpType lmbdNum)
        {
            for (size_t col = 0; col < this->cols_(lmbdMtrx); col++)
                lmbdMtrx[lmbdRowTo][col] += lmbdNum * lmbdMtrx[lmbdRowFrom][col];
        };

    const size_t size = this->rows_(mtrx);

    // �P�ʍs��擾
    MatrixType<DcmpType> mtrxL = this->identity_<DcmpType>(size);
    MatrixType<DcmpType> mtrxU;
    this->copyMatrix_(mtrxU, mtrx);

    for (size_t col = 0; col < size; col++) {
        // baseRow�s�𑼂̍s�։��Z��0�ɂ���B
        const size_t baseRow = col;

        // �s�{�b�g�I��
        size_t pivotRow = std::distance(mtrxU.begin(), std::max_element(mtrxU.begin() + col, mtrxU.end(), [&](const auto& row1, const auto& row2)
            {
                return std::abs(row1[col]) < std::abs(row2[col]);
            }
        ));
        if (baseRow != pivotRow) {
            this->swapRow_(mtrxU, baseRow, pivotRow);
            this->swapRow_(mtrxL, baseRow, pivotRow);
        }

        // �����s��łȂ��ꍇ�ߎ������o���B
        if (std::abs(mtrxU[baseRow][baseRow]) < epsilon)
            mtrxU[baseRow][baseRow] = (mtrxU[baseRow][baseRow] >= 0) ? epsilon : -epsilon;

        for (size_t row = baseRow + 1; row < size; row++) {
            // 0 = A[row][col] + A[baseRow][col]*mulNum
            // mulNum = -A[row][col] / A[baseRow][col]
            DcmpType mulNum = (- 1 * mtrxU[row][col]) / mtrxU[baseRow][col];

            applyRowOperation(mtrxU, baseRow, row, mulNum);
            mtrxL[row][col] = (- 1 * mulNum);
        }
    }

    std::vector<MatrixType<DcmpType>>result{ mtrxL, mtrxU };

    // matrix_�̂Ƃ��n�b�V����o�^
    if (mtrx == this->matrix_) {
        this->dcmpHash_ = this->matrixHash(this->matrix_);
        this->dcmp_     = result;
    }

    return result;
}

template<typename Type,typename DcmpType>
inline typename Matrix<Type,DcmpType>::MatrixType<DcmpType> Matrix<Type,DcmpType>::inverse_(
	const MatrixType<Type>& mtrx,
    DcmpType epsilon
)
{
	if (this->rows_(mtrx) != this->cols_(mtrx))
        throw std::invalid_argument("Must be a square matrix.");

	const size_t size = this->rows_(mtrx);
    std::vector<MatrixType<DcmpType>> mtrxLU = this->luDec_(mtrx,epsilon);

    MatrixType<DcmpType>& mtrxL = mtrxLU[0];
    MatrixType<DcmpType>& mtrxU = mtrxLU[1];

    MatrixType<DcmpType> result(size, RowType<DcmpType>(size, 0));
	MatrixType<DcmpType> identMtrx = this->identity_<DcmpType>(size);

	// �e�񂲂Ƃɋt�s������߂�
	for (size_t col = 0; col < size; col++) {
		// �P�ʍs��̗�x�N�g�������o��
		RowType<DcmpType> e(size, 0);
		e[col] = 1;

		// �O�i������s��
        RowType<DcmpType> mtrxY(size, 0);
		for (size_t i = 0; i < size; i++) {
			mtrxY[i] = e[i];

			for (size_t j = 0; j < i; j++)
				mtrxY[i] -= mtrxL[i][j] * mtrxY[j];

			mtrxY[i] /= mtrxL[i][i];
		}

		// ��ޑ�����s��
        RowType<DcmpType> mtrxX(size, 0);
		for (long long i = size - 1; i >= 0; i--) {
			mtrxX[i] = mtrxY[i];

			for (size_t j = i + 1; j < size; j++)
				mtrxX[i] -= mtrxU[i][j] * mtrxX[j];

			mtrxX[i] /= mtrxU[i][i];
		}

		// ���ʂ�Store�s��̗�ɃR�s�[����
		for (size_t i = 0; i < size; i++)
			result[i][col] = mtrxX[i];
	}

    return result;
}

template<typename Type,typename DcmpType>
inline std::vector<Matrix<DcmpType>> Matrix<Type,DcmpType>::luDec() {
    this->luDec_(this->matrix_);

    std::vector<Matrix<DcmpType>> matrices;
    matrices.push_back(Matrix<DcmpType>(dcmp_[0]));
    matrices.push_back(Matrix<DcmpType>(dcmp_[1]));

    return matrices;
}

template<typename Type,typename DcmpType>
inline Matrix<DcmpType> Matrix<Type,DcmpType>::inverse()
{
    return Matrix<DcmpType>(this->inverse_(this->matrix_,1e-6));
}


#endif
