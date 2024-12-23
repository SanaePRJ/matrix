# matrixCpp
## 概要
このプロジェクトは**シンプル**な実装かつ**軽量**な行列操作を可能とするライブラリの開発を目的としています。

> [!NOTE]
> コンパイラはMSVC2022 Cpp14 or 17とします。

## 貢献方法
開発に協力してくださる方は以下の方法でお願いします。

### 貢献
1. リポジトリをフォークします。
2. 新しいブランチを作成します (`git checkout -b feature/userName`).
3. 変更をコミットします (`git commit -m 'comment'`).
4. ブランチにプッシュします (`git push origin feature/userName`).
5. プルリクエストを作成します。

## クラス
### **matrix**型
`template`を用いて`int`や`double`型の行列を生成します。  
```cpp
template<typename Type>
class Matrix{
	// member
}

Matrix<int> mint;
Matrix<double> mdouble;
```

## 実装
 - データの格納
	データは`std::vector<std::vector<Type>>`で格納し処理します。
	将来的に`CUDA`などGPU上での処理を行わせる予定ですので一次元配列へ展開するメソッドを定義します。
 - 型エイリアス
	`std::vector<std::vector<Type>>`は実装する際に少し長いため以下のコードにより型エイリアス`RowType`,`RowInitType`,`MatrixType`,`MatrixInitType`を定義します。
	またクラス外部でも使用できるよう`public`にしておきます。
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
### ヘッダファイル
このプロジェクトのライブラリは保守性向上のため`定義ファイル`と`実装ファイル`に分ける事とします。
- 定義ファイル
	- クラスの宣言
	- 関数のプロトタイプ宣言
	- マクロの定義など
- 実装ファイル
	- 関数の実装
	- メンバ関数の実装

#### Include Guard
インクルードガードの命名は`MATRIXCPP_ファイル名_拡張子`とします。
```cpp
// test.hppの場合
#ifndef MATRIXCPP_TEST_HPP
#define MATRIXCPP_TEST_HPP

// proc...

#endif
```

#### 分割
`.h`は定義ファイル、`.hpp`は実装ファイルを意味します。
 - matrix
	matrix.h matrixCalc.hppなどすべてのファイルをインクルードします。
 - matrix.h
	matrixクラスの定義,メンバ関数のプロトタイプ宣言などメンバの定義を行います。
 - matrixCtor.hpp
	コンストラクタを定義します。
 - matrixCalc.hpp
	加算,減算,アダマール積など計算を行います。
 - matrixOp.hpp
	演算子関数の実装を行います。
 - matrixDec.hpp
	LU分解など行列分解を行います。逆行列も含む。
 - matrixUtils.hpp
	行入れ替えなど行列の操作を行います。

```
matrix ---- matrix.h
        |-- matrixCtor.hpp
        |-- matrixCalc.hpp
        |-- matrixOp.hpp
        |-- matrixDec.hpp
        |__ matrixUtils.hpp
```
#### 
## 命名
- メンバ名は[`camelCase`](https://en.wikipedia.org/wiki/Camel_case)で記述します。
- クラス名は`PascalCase`で記述します。
- プライベートメンバ
	プライベートメンバ名(`camelCase`)の後に`_`をつけます。
- プロトタイプ宣言
	`返り値型 メンバ名(引数型1,引数型2,...);`

## 記法
### 定義ファイル
```cpp
template<typename Type>
class Matrix{
private:
	// プライベートメンバ

	// 変数メンバ
	Type testValuePrivate_ = 0; // コメント

	// 函数メンバ(プロトタイプ宣言)
	Type testFuncPrivate_(const Matrix&,const Matrix&); // コメント

public:
	// パブリックメンバ
	
	// 変数メンバ
	Type testValuePublic = 0; // コメント

	// 函数メンバ(プロトタイプ宣言)
	Type testFuncPublic(const Matrix&,const Matrix&); // コメント
}
```
### 実装ファイル
```cpp

// コメント
template<typename Type>
Type Matrix<Type>::testFuncPrivate_(const Matrix& a,const Matrix& b){
	return Type();
}

// コメント
template<typename Type>
Type Matrix<Type>::testFuncPublic(const Matrix& a,const Matrix& b){
	return Type();
}
```

## 機能一覧
### コンストラクタ
- `Matrix() = default;`  
  デフォルトコンストラクタ

- `Matrix(const MatrixInitType<>&);`  
  初期化用のパラメータ付きコンストラクタ

- `Matrix(const MatrixType<>&);`  
  コピーコンストラクタ

- `Matrix(const std::pair<size_t, size_t>&);`  
  サイズ指定のパラメータ付きコンストラクタ

- `Matrix(const Matrix<Type, DcmpType>&);`  
  コピーコンストラクタ

- `Matrix(Matrix<Type>&&) noexcept;`  
  ムーブコンストラクタ

### 演算子オーバーロード
- `Matrix<Type>& operator=(const MatrixInitType<Type>&);`  
  代入演算子

- `Matrix<Type>& operator=(const Matrix<Type>&);`  
  代入演算子

- `Matrix<Type>& operator<<(const MatrixInitType<Type>&);`  
  ストリーム挿入演算子

- `Matrix<Type>& operator<<(const Matrix<Type>&);`  
  ストリーム挿入演算子

- `Matrix<Type>& operator=(Matrix<Type>&&);`  
  ムーブ代入演算子

- `Matrix<Type>& operator<<(Matrix<Type>&&);`  
  ムーブストリーム挿入演算子

- `RowType<Type>& operator[](const size_t&);`  
  行アクセス

- `Matrix<Type>& operator+=(const Matrix<Type>&);`  
  加算

- `Matrix<Type>& operator-=(const Matrix<Type>&);`  
  減算

- `Matrix<Type>& operator*=(const Matrix<Type>&);`  
  乗算

- `Matrix<Type>& operator^=(const Matrix<Type>&);`  
  アダマール積

- `Matrix<Type>& operator/=(const Matrix<Type>&);`  
  アダマール除算

- `Matrix<Type>& operator*=(const Type&);`  
  スカラ乗算

- `Matrix<Type> operator+(const Matrix<Type>&);`  
  加算

- `Matrix<Type> operator-(const Matrix<Type>&);`  
  減算

- `Matrix<Type> operator*(const Matrix<Type>&);`  
  乗算

- `Matrix<Type> operator^(const Matrix<Type>&);`  
  アダマール積

- `Matrix<Type> operator/(const Matrix<Type>&);`  
  アダマール除算

- `Matrix<Type> operator*(const Type&);`  
  スカラ乗算

- `template<typename Type_>`  
  `explicit operator Matrix<Type_>();`  
  型変換

### メンバ関数
- `Matrix<Type>& add(const Matrix<Type>&);`  
  加算

- `Matrix<Type>& sub(const Matrix<Type>&);`  
  減算

- `Matrix<Type>& mul(const Matrix<Type>&);`  
  乗算

- `Matrix<Type>& scalarMul(const Type&);`  
  スカラ乗算

- `Matrix<Type>& hadamardMul(const Matrix<Type>&);`  
  アダマール積

- `Matrix<Type>& hadamardDiv(const Matrix<Type>&);`  
  アダマール除算

- `template<typename calcType>`  
  `Matrix<Type>& scalarCalc(const Matrix<Type>&);`  
  スカラ計算

- `std::vector<Matrix<DcmpType>> luDec(DcmpType epsilon = 1e-9);`  
  LU分解

- `Matrix<DcmpType> inverse(DcmpType epsilon = 1e-9);`  
  逆行列

- `DcmpType det(DcmpType epsilon = 1e-9);`  
  行列式

- `Matrix<Type> transpose();`  
  転置

- `Matrix<Type>& swapRow(const size_t&, const size_t&);`  
  行の入れ替え

- `Matrix<Type>& swapCol(const size_t&, const size_t&);`  
  列の入れ替え

- `Matrix<Type>& resize(const size_t&, const size_t&);`  
  サイズ変更

- `const size_t rows() const;`  
  行数の取得

- `const size_t cols() const;`  
  列数の取得

- `std::vector<std::reference_wrapper<Type>> rowRef(const size_t&);`  
  行参照

- `std::vector<std::reference_wrapper<Type>> colRef(const size_t&);`  
  列参照

- `Matrix<Type>& forEach(std::function<Type()>);`  
  各要素への操作

- `Matrix<Type>& forEach(std::function<Type(size_t, size_t, Type&)>);`  
  各要素への操作（行、列、そのポイントの値）

- `template<typename Type_ = Type>`  
  `static Matrix<Type_> identity(const size_t&);`  
  単位行列生成

### ストリーム出力オーバーロード
- `template<typename CharT, typename Traits, typename MatrixType = double>`  
  `std::basic_ostream<CharT, Traits>& operator <<(std::basic_ostream<CharT, Traits>&, Matrix<MatrixType>);`  
  行列の出力オーバーロード

- `template<typename CharT, typename Traits, typename MatrixType = double>`  
  `std::basic_ostream<CharT, Traits>& operator <<(std::basic_ostream<CharT, Traits>&, std::vector<std::vector<MatrixType>>);`  
  2次元ベクトルの出力オーバーロード

> [!NOTE]
> 他の分解法は追加予定[`QR分解`,`コレスキー分解`,`固有値分解`,`SVD分解`,`ジョルダン標準形`](https://en.wikipedia.org/wiki/Matrix_decomposition)