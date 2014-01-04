#ifndef MULIB_MATH_HPP
#define MULIB_MATH_HPP

#include "angle.hpp"
#include "vector.hpp"

#define MULIB_PI 3.1415926535897932384626433832795 //π=3.14159265...
#define MULIB_RIGHT_ANGLE MULIB_PI/2 //π/2=90°
#define MULIB_PI_3_2 MULIB_PI*3/2 //3π/2=270°
#define MULIB_PI_F 3.1415926535897932384626433832795f
#define MULIB_RIGHT_ANGLE_F MULIB_PI_F/2.0f
#define MULIB_RAD MULIB_PI/180

//======================
//高速三角関数関係
//======================

namespace mulib{
namespace priv{
	//三角関係定数
	extern const double sin_table[];
	extern const double atan_table[];
	static const int sin_table_num = 1024;
	static const int atan_table_num = 1024;
}
	double sin(Angle t);
	double cos(Angle t);

	Angle atan2(double y, double x);
	Angle atan2(Vec2D<double> vec);

	/*!===========================================================================
		** 2次ベジェ曲線を求める(座標のみ)
		*
		*説明: 2次ベジェ曲線の任意の段階を求める。
		*
		*引数: startPos: 始点座標, endPos: 終点座標, ctrlPos: 制御点座標
		*			 divNum: 分割数(>0), nowPhase: 分割数中の求める段階(<=divNum)
		*
		*返り値: 座標クラス
	============================================================================*/
	template<typename T>
	Vec2D Get2DBezier(const Vec2D<T>& startPos,
										const Vec2D<T>& endPos,
										const Vec2D<T>& ctrlPos,
										const int divNum, const int nowPhase);

	/*!===========================================================================
		** 2次ベジェ曲線を求める(中心座標・角度・長さスカラ)
		*
		*説明: 2次ベジェ曲線の任意の段階を求める。
		*			 中心座標(指定した段階と次の段階の座標の中間点)と
		*			 角度と長さを求められるので、画像を中心で回転させて描画するのに便利。
		*
		*引数: startPos: 始点座標, endPos: 終点座標, ctrlPos: 制御点座標
		*			 divNum: 分割数(>0), nowPhase: 分割数中の求める段階(<=divNum)
		*			 rtnPosPtr: 求める座標を入れるポインタ(nullptrで求めない),
		*			 rtnVelPtr: 求める長さ(中心から端)を入れるポインタ(nullptrで求めない)
		*			 rtnRadPtr: 求める角度を入れるポインタ(nullptrで求めない)
		*
		*返り値: なし
		*
		*備考: 中心座標を求める都合上、同じ関数の多態(座標のみを求める方)とは
		*			 微妙に返される曲線が異なる(分割数が1多い)ので注意
	============================================================================*/
	template<typename T>
	void Get2DBezier(const Vec2D<T>& startPos,
									 const Vec2D<T>& endPos,
									 const Vec2D<T>& ctrlPos,
									 const int divNum, const int nowPhase,
									 Vec2D<T>* rtnPosPtr,
									 double* rtnVelPtr,
									 Angle* rtnRadPtr);
}

#endif
