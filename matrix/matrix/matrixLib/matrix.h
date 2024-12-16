// matrixクラスの定義,メンバ関数のプロトタイプ宣言などメンバの定義を行います。
#ifndef MATRIXCPP_MATRIX_H
#define MATRIXCPP_MATRIX_H


#include <vector>


template<typename Type>
class Matrix {
public:
	template<typename rowType    > using RowType     = std::vector<rowType>;
	template<typename rowInitType> using RowInitType = std::initializer_list<rowInitType>;

	template<typename matrixType = Type> using MatrixType     = std::vector          <RowType<matrixType>>;
	template<typename matrixType = Type> using MatrixInitType = std::initializer_list<RowInitType<matrixType>>;
};


#endif