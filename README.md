# matrixCpp
## 概要
このプロジェクトは**シンプル**な実装かつ**軽量**な行列操作を可能とするライブラリの開発を目的としています。

> [!NOTE]
> コンパイラはMSVC2022 C++17とします。

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

また[templateの特殊化](https://en.wikipedia.org/wiki/Partial_template_specialization)を行い`float`や`double`のような実数型が指定された場合のみ使用できるメソッドが存在します。  

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
|操作          |演算子 |関数          |引数                  |返り値型                                          |特殊化|
|----          |----   |----          |----                  |----                                              |----  |
|代入          |=,<<   |None          |`MatrixInitType<Type>`|`Matrix<Type>&`                                   |No    |
|コピー        |=,<<   |None          |`const Matrix<Type>&` |`Matrix<Type>&`                                   |No    |
|ムーブ代入    |=      |None          |`Matrix<Type>&&`      |`Matrix<Type>&`                                   |No    |
|行参照        |[]     |rowRef        |`size_t`              |`std::vector<Type>`/`std::reference_wrapper<Type>`|No    |
|列参照        |None   |colRef        |`size_t`              |`std::vector<Type>`/`std::reference_wrapper<Type>`|No    |
|加算          |+ , += |add           |`const Matrix<Type>&` |`Matrix<Type>`/`Matrix<Type>&`                    |No    |
|減算          |- , -= |sub           |`const Matrix<Type>&` |`Matrix<Type>`/`Matrix<Type>&`                    |No    |
|乗算          |* , *= |mul           |`const Matrix<Type>&` |`Matrix<Type>`/`Matrix<Type>&`                    |No    |
|スカラ倍      |* , *= |scalarMul     |`const Matrix<Type>&` |`Matrix<Type>`/`Matrix<Type>&`                    |No    |
|アダマール積  |^ , ^= |hadamardMul   |`const Matrix<Type>&` |`Matrix<Type>`/`Matrix<Type>&`                    |No    |
|アダマール除算|/ , /= |hadamardDiv   |`const Matrix<Type>&` |`Matrix<Type>`/`Matrix<Type>&`                    |No    |
|LU分解        |None   |luDec         |`void`                |`std::vector<Matrix<Type>>`                       |Yes   |
|行列式        |None   |det           |`void`                |`ty`                                              |No    |
|逆行列        |None   |inverse       |`void`                |`Matrix<Type>`                                    |Yes   |
|転置          |None   |transpose     |`void`                |`Matrix<Type>`                                    |No    |
|行入れ替え    |None   |swapRow       |`size_t`,`size_t`     |`Matrix<Type>&`                                   |No    |
|列入れ替え    |None   |swapCol       |`size_t`,`size_t`     |`Matrix<Type>&`                                   |No    |
|サイズ変更    |None   |resize        |`size_t`,`size_t`     |`Matrix<Type>&`                                   |No    |
|行サイズ取得  |None   |rows          |`void`                |`size_t`                                          |No    |
|列サイズ取得  |None   |cols          |`void`                |`size_t`                                          |No    |
|各要素への操作|None   |forEach       |`std::function<ty()>` |`Matrix<Type>&`                                   |No    |
|各要素への操作|None   |forEach       |`std::function<ty(size_t,size_t,ty&)>`|`Matrix<Type>&`                   |No    |
> [!NOTE]
> 他の分解法は追加予定[`QR分解`,`コレスキー分解`,`固有値分解`,`SVD分解`,`ジョルダン標準形`](https://en.wikipedia.org/wiki/Matrix_decomposition)