#ifndef MATRIXCPP_MATRIXDEC_HPP
#define MATRIXCPP_MATRIXDEC_HPP

#include "matrix.h"

/**
 * @brief Internal function to perform LU decomposition
 *
 * @tparam Type Element type of the input matrix
 * @tparam DcmpType Element type used during computation
 * @param mtrx The matrix to be decomposed
 * @param epsilon The smallest permissible value (used in pivot selection)
 * @return A vector containing the L and U matrices resulting from LU decomposition
 * @exception std::invalid_argument Thrown if the matrix is not square
 */
template<typename Type, typename DcmpType>
inline std::vector<typename Matrix<Type, DcmpType>::MatrixType<DcmpType>> Matrix<Type, DcmpType>::luDec_(
    const MatrixType<Type>& mtrx,
    DcmpType epsilon
)
{
    // �L���b�V���̗L�������m�F
    if (mtrx == this->matrix_ && this->matrixHash_(this->matrix_) == this->dcmpHash_)
        return this->dcmp_;

    // �����s��ł��邱�Ƃ��m�F
    if (this->rows_(mtrx) != this->cols_(mtrx))
        throw std::invalid_argument("Must be a square matrix.");

    // �s����֐�
    auto applyRowOperation = [&](MatrixType<DcmpType>& lmbdMtrx, size_t lmbdRowFrom, size_t lmbdRowTo, DcmpType lmbdNum)
        {
            for (size_t col = 0; col < this->cols_(lmbdMtrx); col++)
                lmbdMtrx[lmbdRowTo][col] += lmbdNum * lmbdMtrx[lmbdRowFrom][col];
        };

    const size_t size = this->rows_(mtrx);

    // ������: �P�ʍs��A�[���s��AU�s��
    MatrixType<DcmpType> mtrxP = this->identity_<DcmpType>(size); // �P�ʍs��
    MatrixType<DcmpType> mtrxL(size, RowType<DcmpType>(size, 0)); // �[���s��
    MatrixType<DcmpType> mtrxU;
    this->copyMatrix_(mtrxU, mtrx);

    for (size_t col = 0; col < size; col++) {
        const size_t baseRow = col;

        // �s�{�b�g�I��
        if (std::abs(mtrxU[baseRow][baseRow]) < epsilon) {
            auto it = std::max_element(mtrxU.begin() + baseRow + 1, mtrxU.end(),
                [&](auto& row1, auto& row2) { return std::abs(row1[col]) < std::abs(row2[col]); });

            if (it != mtrxU.end()) {
                size_t pivotRow = std::distance(mtrxU.begin(), it);
                this->swapRow_(mtrxU, baseRow, pivotRow);
                this->swapRow_(mtrxP, baseRow, pivotRow);
                this->swapRow_(mtrxL, baseRow, pivotRow);
            }
        }

        if (std::abs(mtrxU[baseRow][baseRow]) < epsilon)
            mtrxU[baseRow][baseRow] = (mtrxU[baseRow][baseRow] >= 0) ? epsilon : -epsilon;

        for (size_t row = baseRow + 1; row < size; row++) {
            DcmpType mulNum = -mtrxU[row][col] / mtrxU[baseRow][col];
            applyRowOperation(mtrxU, baseRow, row, mulNum);
            mtrxL[row][col] = -mulNum;
        }
    }

    for (size_t i = 0; i < size; i++)
        mtrxL[i][i] = 1;

    std::vector<MatrixType<DcmpType>> result{ mtrxL, mtrxU, mtrxP };

    // �L���b�V���̕ۑ�
    if (mtrx == this->matrix_) {
        this->dcmpHash_ = this->matrixHash_(this->matrix_);
        this->dcmp_ = result;
    }

    return result;
}

/**
 * @brief Internal function to calculate the inverse of a matrix
 *
 * @tparam Type Element type of the input matrix
 * @tparam DcmpType Element type used during computation
 * @param mtrx The matrix to be inverted
 * @param epsilon The smallest permissible value (used in LU decomposition)
 * @return The calculated inverse matrix
 * @exception std::invalid_argument Thrown if the matrix is not square
 */
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

/**
 * @brief Internal function to calculate the determinant of a matrix
 *
 * @tparam Type Element type of the input matrix
 * @tparam DcmpType Element type used during computation
 * @param mtrx The matrix for which the determinant is to be calculated
 * @param epsilon The smallest permissible value (used in LU decomposition)
 * @return The value of the determinant
 * @exception std::invalid_argument Thrown if the matrix is not square
 */
template<typename Type, typename DcmpType>
inline DcmpType Matrix<Type, DcmpType>::det_(
    const MatrixType<Type>& mtrx,
    DcmpType epsilon
) 
{
    if (this->rows_(mtrx) != this->cols_(mtrx))
        throw std::invalid_argument("Must be a square matrix.");
    
    MatrixType<DcmpType> mtrxU = this->luDec_(mtrx,epsilon)[1];
    DcmpType rslt = 1;

    for (size_t i = 0; i < mtrxU.size(); i++)
        rslt *= mtrxU[i][i];

    return rslt;
}

/**
 * @brief Public member function to perform LU decomposition
 *
 * @tparam Type Type of the matrix elements
 * @tparam DcmpType Type used during computation
 * @param epsilon The smallest permissible value (used in pivot selection)
 * @return A vector containing the decomposed L and U matrices
 */
template<typename Type,typename DcmpType>
inline std::vector<Matrix<DcmpType>> Matrix<Type,DcmpType>::luDec(
    DcmpType epsilon
) 
{
    this->luDec_(this->matrix_,epsilon);

    std::vector<Matrix<DcmpType>> matrices;
    matrices.push_back(Matrix<DcmpType>(dcmp_[0]));
    matrices.push_back(Matrix<DcmpType>(dcmp_[1]));

    return matrices;
}

/**
 * @brief Public member function to calculate the inverse of a matrix
 *
 * @tparam Type Type of the matrix elements
 * @tparam DcmpType Type used during computation
 * @param epsilon The smallest permissible value (used in LU decomposition)
 * @return The calculated inverse matrix
 */
template<typename Type,typename DcmpType>
inline Matrix<DcmpType> Matrix<Type,DcmpType>::inverse(
    DcmpType epsilon
)
{
    return Matrix<DcmpType>(this->inverse_(this->matrix_,epsilon));
}

/**
 * @brief Public member function to calculate the determinant of a matrix
 *
 * @tparam Type Type of the matrix elements
 * @tparam DcmpType Type used during computation
 * @param epsilon The smallest permissible value (used in LU decomposition)
 * @return The value of the determinant
 */
template<typename Type, typename DcmpType>
inline DcmpType Matrix<Type, DcmpType>::det(
    DcmpType epsilon
)
{
    if (this->rows() != this->cols())
        throw std::invalid_argument("Must be a square matrix.");

    MatrixType<DcmpType> mtrxU = this->luDec_(this->matrix_,epsilon)[1];
    DcmpType rslt = 1;

    for (size_t i = 0; i < mtrxU.size(); i++)
        rslt *= mtrxU[i][i];

    return rslt;
}

#endif
