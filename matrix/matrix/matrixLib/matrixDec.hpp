#ifndef MATRIXCPP_MATRIXDEC_HPP
#define MATRIXCPP_MATRIXDEC_HPP


#include "matrix.h"


/**
 * @brief LU分解を行う内部関数
 *
 * @tparam Type 入力行列の要素型
 * @tparam DcmpType 計算時の要素型
 * @param mtrx 分解対象の行列
 * @param epsilon 許容される最小値（ピボット選択時に使用）
 * @return LU分解されたL行列とU行列のベクトル
 * @exception std::invalid_argument 非正方行列の場合にスローされる
 */
template<typename Type, typename DcmpType>
inline std::vector<typename Matrix<Type, DcmpType>::MatrixType<DcmpType>> Matrix<Type, DcmpType>::luDec_(
    const MatrixType<Type>& mtrx,
    DcmpType epsilon
)
{
    // キャッシュの有効性を確認
    if (mtrx == this->matrix_ && this->matrixHash(this->matrix_) == this->dcmpHash_)
        return this->dcmp_;

    // 正方行列であることを確認
    if (this->rows_(mtrx) != this->cols_(mtrx))
        throw std::invalid_argument("Must be a square matrix.");

    // 行操作関数
    auto applyRowOperation = [&](MatrixType<DcmpType>& lmbdMtrx, size_t lmbdRowFrom, size_t lmbdRowTo, DcmpType lmbdNum)
        {
            for (size_t col = 0; col < this->cols_(lmbdMtrx); col++)
                lmbdMtrx[lmbdRowTo][col] += lmbdNum * lmbdMtrx[lmbdRowFrom][col];
        };

    const size_t size = this->rows_(mtrx);

    // 初期化: 単位行列、ゼロ行列、U行列
    MatrixType<DcmpType> mtrxP = this->identity_<DcmpType>(size); // 単位行列
    MatrixType<DcmpType> mtrxL(size, RowType<DcmpType>(size, 0)); // ゼロ行列
    MatrixType<DcmpType> mtrxU;
    this->copyMatrix_(mtrxU, mtrx);

    for (size_t col = 0; col < size; col++) {
        const size_t baseRow = col;

        // ピボット選択
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

    std::vector<MatrixType<DcmpType>> result{ mtrxL, mtrxU };

    // キャッシュの保存
    if (mtrx == this->matrix_) {
        this->dcmpHash_ = this->matrixHash(this->matrix_);
        this->dcmp_ = result;
    }

    return result;
}

/**
 * @brief 行列の逆行列を求める内部関数
 *
 * @tparam Type 入力行列の要素型
 * @tparam DcmpType 計算時の要素型
 * @param mtrx 対象の行列
 * @param epsilon 許容される最小値（LU分解時に使用）
 * @return 計算された逆行列
 * @exception std::invalid_argument 非正方行列の場合にスローされる
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

	// 各列ごとに逆行列を求める
	for (size_t col = 0; col < size; col++) {
		// 単位行列の列ベクトルを取り出す
		RowType<DcmpType> e(size, 0);
		e[col] = 1;

		// 前進代入を行う
        RowType<DcmpType> mtrxY(size, 0);
		for (size_t i = 0; i < size; i++) {
			mtrxY[i] = e[i];

			for (size_t j = 0; j < i; j++)
				mtrxY[i] -= mtrxL[i][j] * mtrxY[j];

			mtrxY[i] /= mtrxL[i][i];
		}

		// 後退代入を行う
        RowType<DcmpType> mtrxX(size, 0);
		for (long long i = size - 1; i >= 0; i--) {
			mtrxX[i] = mtrxY[i];

			for (size_t j = i + 1; j < size; j++)
				mtrxX[i] -= mtrxU[i][j] * mtrxX[j];

			mtrxX[i] /= mtrxU[i][i];
		}

		// 結果をStore行列の列にコピーする
		for (size_t i = 0; i < size; i++)
			result[i][col] = mtrxX[i];
	}

    return result;
}

/**
 * @brief 行列の行列式を計算する内部関数
 *
 * @tparam Type 入力行列の要素型
 * @tparam DcmpType 計算時の要素型
 * @param mtrx 対象の行列
 * @param epsilon 許容される最小値（LU分解時に使用）
 * @return 行列式の値
 * @exception std::invalid_argument 非正方行列の場合にスローされる
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
 * @brief LU分解を行う公開メンバ関数
 *
 * @tparam Type 行列要素の型
 * @tparam DcmpType 計算時の型
 * @param epsilon 許容される最小値（ピボット選択時に使用）
 * @return 分解されたL行列とU行列を格納するベクトル
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
 * @brief 行列の逆行列を求める公開メンバ関数
 *
 * @tparam Type 行列要素の型
 * @tparam DcmpType 計算時の型
 * @param epsilon 許容される最小値（LU分解時に使用）
 * @return 計算された逆行列
 */
template<typename Type,typename DcmpType>
inline Matrix<DcmpType> Matrix<Type,DcmpType>::inverse(
    DcmpType epsilon
)
{
    return Matrix<DcmpType>(this->inverse_(this->matrix_,epsilon));
}

/**
 * @brief 行列の行列式を計算する公開メンバ関数
 *
 * @tparam Type 行列要素の型
 * @tparam DcmpType 計算時の型
 * @param epsilon 許容される最小値（LU分解時に使用）
 * @return 行列式の値
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
