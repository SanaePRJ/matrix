/**
 * @file matrix.h
 * @brief Matrixクラスの定義, メンバ関数のプロトタイプ宣言などメンバの定義を行います。
 */


#ifndef MATRIXCPP_MATRIX_H
#define MATRIXCPP_MATRIX_H


#include <iomanip>
#include <vector>
#include <utility>
#include <functional>


 /**
  * @brief Matrixクラスのテンプレート定義
  *
  * @tparam Type 行列の要素の型
  * @tparam DcmpType 分解時に使用する型
  */
template<typename Type, typename DcmpType = double>
class Matrix {
public:
    /// 型エイリアス宣言
    template<typename rowType    > using RowType     = std::vector<rowType>;               ///< 行型
    template<typename rowInitType> using RowInitType = std::initializer_list<rowInitType>; ///< 行型(初期化)

    template<typename matrixType     = Type> using MatrixType     = std::vector<RowType<matrixType>>;                   ///< 行列型
    template<typename matrixInitType = Type> using MatrixInitType = std::initializer_list<RowInitType<matrixInitType>>; ///< 行列型(初期化)

private:
    MatrixType<> matrix_; ///< データ格納変数

    size_t dcmpHash_ = 0;
    std::vector<MatrixType<DcmpType>> dcmp_; ///< 分解後データ格納変数

public:
    /**
     * @brief デフォルトコンストラクタ
     */
    Matrix() {};

    /**
     * @brief 初期化用のパラメタ付コンストラクタ
     *
     * @param init 初期化リスト
     */
    Matrix(const MatrixInitType<>& init);

    /**
     * @brief コピーコンストラクタ
     *
     * @param matrix コピー元の行列
     */
    Matrix(const MatrixType<>& matrix);

    /**
     * @brief サイズ指定のパラメタ付コンストラクタ
     *
     * @param size 行列のサイズ
     */
    Matrix(const std::pair<size_t, size_t>& size);

    /**
     * @brief コピーコンストラクタ
     *
     * @param other コピー元の行列
     */
    Matrix(const Matrix<Type,DcmpType>& other);

    /**
     * @brief ムーブコンストラクタ
     *
     * @param other ムーブ元の行列
     */
    Matrix(Matrix<Type>&& other) noexcept;

    /// 演算子オーバーロード
    Matrix<Type>& operator=(const MatrixInitType<Type>&);
    Matrix<Type>& operator=(const Matrix<Type>&);
    Matrix<Type>& operator<<(const MatrixInitType<Type>&);
    Matrix<Type>& operator<<(const Matrix<Type>&);

    Matrix<Type>& operator=(Matrix<Type>&&);
    Matrix<Type>& operator<<(Matrix<Type>&&);

    RowType<Type>& operator[](const size_t&); ///< 行アクセス

    Matrix<Type>& operator+=(const Matrix<Type>&); ///< 加算
    Matrix<Type>& operator-=(const Matrix<Type>&); ///< 減算
    Matrix<Type>& operator*=(const Matrix<Type>&); ///< 乗算
    Matrix<Type>& operator^=(const Matrix<Type>&); ///< アダマール積
    Matrix<Type>& operator/=(const Matrix<Type>&); ///< アダマール除算
    Matrix<Type>& operator*=(const Type&);         ///< スカラ倍

    Matrix<Type> operator+(const Matrix<Type>&); ///< 加算
    Matrix<Type> operator-(const Matrix<Type>&); ///< 減算
    Matrix<Type> operator*(const Matrix<Type>&); ///< 乗算
    Matrix<Type> operator^(const Matrix<Type>&); ///< アダマール積
    Matrix<Type> operator/(const Matrix<Type>&); ///< アダマール除算
    Matrix<Type> operator*(const Type&);         ///< スカラ倍

    template<typename Type_>
    explicit operator Matrix<Type_>(); ///< 型変換

private:
    void add_(MatrixType<Type>&, const MatrixType<Type>&); ///< 加算
    void sub_(MatrixType<Type>&, const MatrixType<Type>&); ///< 減算
    MatrixType<Type> mul_(const MatrixType<Type>&, const MatrixType<Type>&); ///< 乗算

    void hadamardMul_(MatrixType<Type>&, const MatrixType<Type>&); ///< アダマール積
    void hadamardDiv_(MatrixType<Type>&, const MatrixType<Type>&); ///< アダマール除算

    template<typename calcType>
    void calcMatrix_(MatrixType<Type>&, const MatrixType<Type>&); ///< 四則演算

    template<typename calcType>
    void scalarCalc_(MatrixType<Type>&, const Type&); ///< スカラ演算

    std::vector<MatrixType<DcmpType>> luDec_  (const MatrixType<Type>&, DcmpType epsilon = 1e-9); ///< LU分解
    MatrixType<DcmpType>              inverse_(const MatrixType<Type>&, DcmpType epsilon = 1e-9); ///< 逆行列
    DcmpType                          det_    (const MatrixType<Type>&, DcmpType epsilon = 1e-9); ///< 行列式

public:
    Matrix<Type>& add(const Matrix<Type>&); ///< 加算
    Matrix<Type>& sub(const Matrix<Type>&); ///< 減算
    Matrix<Type>& mul(const Matrix<Type>&); ///< 乗算
    Matrix<Type>& scalarMul  (const Type&); ///< スカラ倍
    Matrix<Type>& hadamardMul(const Matrix<Type>&); ///< アダマール積
    Matrix<Type>& hadamardDiv(const Matrix<Type>&); ///< アダマール除算
    template<typename calcType>
    Matrix<Type>& scalarCalc (const Matrix<Type>&); ///< スカラ計算

    std::vector<Matrix<DcmpType>> luDec  (DcmpType epsilon = 1e-9); ///< LU分解
    Matrix<DcmpType>              inverse(DcmpType epsilon = 1e-9); ///< 逆行列
    DcmpType                      det    (DcmpType epsilon = 1e-9); ///< 行列式

private:
    template<typename CopyType1,typename CopyType2>
    void copyMatrix_(MatrixType<CopyType1>&, const MatrixType<CopyType2>&); ///< 各要素をコピー

    template<typename Type_ = Type>
    size_t rows_(const MatrixType<Type_>&) const noexcept; ///< 行数取得
    template<typename Type_ = Type>
    size_t cols_(const MatrixType<Type_>&) const noexcept; ///< 列数取得

    template<typename Type_ = Type>
    void swapRow_(MatrixType<Type_>&, const size_t&, const size_t&); ///< 行入れ替え
    template<typename Type_ = Type>
    void swapCol_(MatrixType<Type_>&, const size_t&, const size_t&); ///< 列入れ替え
    MatrixType<Type> transpose_(const MatrixType<Type>&);            ///< 転置

    bool areSameSize_   (const MatrixType<Type>&, const MatrixType<Type>&) const noexcept; ///< サイズ比較
    void validateMatrix_(const MatrixType<Type>&); ///< 行列のバリデーション

    template<typename Type_ = Type>
    static MatrixType<Type_> identity_(const size_t&); //< 単位行列生成

    template<typename Type_ = Type>
    size_t matrixHash(const MatrixType<Type_>&);

public:
    Matrix<Type>  transpose();                           ///< 転置
    Matrix<Type>& swapRow(const size_t&, const size_t&); ///< 行入れ替え
    Matrix<Type>& swapCol(const size_t&, const size_t&); ///< 列入れ替え

    Matrix<Type>& resize(const size_t&, const size_t&);  ///< サイズ変更

    const size_t rows() const; ///< 行数取得
    const size_t cols() const; ///< 列数取得

    std::vector<std::reference_wrapper<Type>> rowRef(const size_t&); ///< 行参照
    std::vector<std::reference_wrapper<Type>> colRef(const size_t&); ///< 列参照

    Matrix<Type>& forEach(std::function<Type()>); ///< 各要素への操作
    Matrix<Type>& forEach(std::function<Type(size_t, size_t, Type&)>); ///< 各要素への操作(行,列,そのポイントの値)

    template<typename Type_ = Type>
    static Matrix<Type_> identity(const size_t&); ///< 単位行列生成
};


/**
 * @brief 行列を出力するためのオーバーロード
 *
 * @tparam CharT 出力ストリームの文字型
 * @tparam Traits 出力ストリームの特性
 * @tparam MatrixType 行列の要素型
 * @param ArgOstream 出力ストリーム
 * @param Matrix 行列オブジェクト
 * @return 出力ストリーム
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
    for (std::vector<MatrixType>& row:mtrx) {
        for (MatrixType& col:row)
            ostrm << std::setw(10) << col;

        ostrm << std::endl;
    }

    return ostrm;
}


#endif