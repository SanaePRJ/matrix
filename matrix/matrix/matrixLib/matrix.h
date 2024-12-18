// matrixクラスの定義,メンバ関数のプロトタイプ宣言などメンバの定義を行います。
#ifndef MATRIXCPP_MATRIX_H
#define MATRIXCPP_MATRIX_H


#include <vector>
#include <utility>
#include <functional>


template<typename Type>
class Matrix {
public:
    // 型エイリアス宣言
    template<typename rowType    > using RowType     = std::vector<rowType>; // 行型
    template<typename rowInitType> using RowInitType = std::initializer_list<rowInitType>; // 行型(初期化)

    template<typename matrixType = Type> using MatrixType         = std::vector<RowType<matrixType>>; // 行列型
    template<typename matrixInitType = Type> using MatrixInitType = std::initializer_list<RowInitType<matrixInitType>>; // 行列型(初期化)

    using Size = std::pair<size_t, size_t>; // サイズ指定

private:
    // データ格納変数
    MatrixType<> matrix_;

public:
    // matrixCtor.hpp
    Matrix() {};                     // デフォルトコンストラクタ

    Matrix(const MatrixInitType<>&); // パラメタ付コンストラクタ 初期化
    Matrix(const Size&);             // パラメタ付コンストラクタ サイズ指定

    Matrix(const Matrix<Type>&);     // コピーコンストラクタ
    Matrix(Matrix<Type>&&) noexcept; // moveコンストラクタ

    // matrixOps.hpp
    Matrix<Type>& operator = (const MatrixInitType<Type>&);
    Matrix<Type>& operator = (const Matrix        <Type>&);
    Matrix<Type>& operator <<(const MatrixInitType<Type>&);
    Matrix<Type>& operator <<(const Matrix        <Type>&);

    Matrix<Type>& operator = (Matrix<Type>&&);
    Matrix<Type>& operator <<(Matrix<Type>&&);

    RowType<Type>& operator [](size_t); // 行アクセス

    // 破壊的処理
    Matrix<Type>& operator +=(const Matrix<Type>&); // 加算
    Matrix<Type>& operator -=(const Matrix<Type>&); // 減算
    Matrix<Type>& operator *=(const Matrix<Type>&); // 乗算
    Matrix<Type>& operator ^=(const Matrix<Type>&); // アダマール積
    Matrix<Type>& operator /=(const Matrix<Type>&); // アダマール除算
    Matrix<Type>& operator *=(const Type&);         // スカラ倍

    // 非破壊的処理
    Matrix<Type> operator +(const Matrix<Type>&); // 加算
    Matrix<Type> operator -(const Matrix<Type>&); // 減算
    Matrix<Type> operator *(const Matrix<Type>&); // 乗算
    Matrix<Type> operator ^(const Matrix<Type>&); // アダマール積
    Matrix<Type> operator /(const Matrix<Type>&); // アダマール除算
    Matrix<Type> operator *(const Type&);         // スカラ倍

    // matrixCalc.hpp
private:
    template<typename Type_> void add_(MatrixType<Type_>&, const MatrixType<Type_>&); // 加算
    template<typename Type_> void sub_(MatrixType<Type_>&, const MatrixType<Type_>&); // 減算
    template<typename Type_> MatrixType<Type_> mul_(const MatrixType<Type_>&, const MatrixType<Type_>&); // 乗算

    template<typename Type_> void hadamardMul_(MatrixType<Type_>&, const MatrixType<Type_>&); // アダマール積
    template<typename Type_> void hadamardDiv_(MatrixType<Type_>&, const MatrixType<Type_>&); // アダマール除算

    template<typename Type_, typename calcType_>
    void calcMatrix_(MatrixType<Type_>&, const MatrixType<Type_>&);

    template<typename Type_, typename calcType_>
    void scalarCalc_(MatrixType<Type_>&, const Type_&);
public:

    Matrix<Type>& add(const Matrix<Type>&); // 加算
    Matrix<Type>& sub(const Matrix<Type>&); // 減算
    Matrix<Type>& mul(const Matrix<Type>&); // 乗算
    Matrix<Type>& scalarMul(const Type&); // スカラ倍
    Matrix<Type>& hadamardMul(const Matrix<Type>&); // アダマール積
    Matrix<Type>& hadamardDiv(const Matrix<Type>&); // アダマール除算

    template<typename calcType>
    Matrix<Type>& scalarCalc(const Matrix<Type>&); // スカラ計算

    // matrixDec.hpp
    std::vector<Matrix<Type>&> luDec();   // LU分解
    Matrix<Type>               inverse(); // 逆行列

    // matrixUtils.hpp
private:
    template<typename CopyType_ = Type>
    void copyMatrix_(MatrixType<CopyType_>&, const MatrixType<CopyType_>&); // 各要素をコピー

    template<typename Type_> size_t rows_(const MatrixType<Type_>&) const noexcept;
    template<typename Type_> size_t cols_(const MatrixType<Type_>&) const noexcept;

    template<typename Type1_, typename Type2_>
    bool areSameSize_(const MatrixType<Type1_>&, const MatrixType<Type2_>&) const noexcept;

    template<typename Type_>
    void validateMatrix_(const MatrixType<Type_>&);

public:
    Matrix<Type>  transpose();                            // 転置
    Matrix<Type>& swapRow(const size_t&, const size_t&); // 行入れ替え
    Matrix<Type>& swapCol(const size_t&, const size_t&); // 列入れ替え

    Matrix<Type>& resize(const size_t&, const size_t&); // サイズ変更

    const size_t rows(); // 行数取得
    const size_t cols(); // 列数取得

    Type det(); // 行列式

    std::vector<std::reference_wrapper<Type>> rowRef(const size_t&); // 行参照
    std::vector<std::reference_wrapper<Type>> colRef(const size_t&); // 列参照

    Matrix<Type>& forEach(std::function<Type()>); // 各要素への操作
    Matrix<Type>& forEach(std::function<Type(size_t, size_t, Type&)>); // 各要素への操作(行,列,そのポイントの値)
};


#include <iomanip>
// std::coutで出力
template<typename CharT, typename Traits, typename MatrixType = double>
std::basic_ostream<CharT, Traits>& operator <<
(
    std::basic_ostream<CharT, Traits>& ArgOstream,
    Matrix<MatrixType> Matrix
    )
{
    for (size_t Row = 0; Row < Matrix.rows(); Row++) {
        for (MatrixType Column : Matrix[Row])
            ArgOstream << std::setw(10) << Column;

        ArgOstream << std::endl;
    }

    return ArgOstream;
}


#endif