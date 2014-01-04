#ifndef MULIB_ANGLE_HPP
#define MULIB_ANGLE_HPP

#include "common.hpp"
/*!===================================
 ** Angle: 角度クラス **
 *
 * 内部では全てdouble型のラジアンで保持。
 * 取り出し時にラジアンかディグリーかで選択
 * 0 <= Angle < 2πの間に収まることが保証される
====================================*/
#define ANGLE_MIN 0 //最小値: 0
#define ANGLE_MAX 2 * 3.1415926535897932384 //最大値(<2π)

namespace mulib{
class Angle{
public:
	//代入時にディグリーで代入するかラジアンで代入するかのフラグ
	enum TypeOfAngle{
		Radian,
		Degree
	};

private:
	double _rad; //実質的なメンバ

	//角度が範囲内に収まるよう補正(収まるまでループする)
		#ifdef MULIB_USE_CONSTEXPR
		constexpr
		#endif
		double _check(double rad){
		while(rad < ANGLE_MIN || rad >= ANGLE_MAX){
			if(rad < ANGLE_MIN){
				rad += ANGLE_MAX;
			}else{
				rad -= ANGLE_MAX;
			}
		}
		return rad;
	}


	//ディグリーからラジアンへ
	#ifdef MULIB_USE_CONSTEXPR
	constexpr
	#endif
	double _deg2rad(double deg){
		return(_check(deg * 0.017453292519943));
	}

	//ラジアンからディグリーへ
	#ifdef MULIB_USE_CONSTEXPR
	constexpr
	#endif
	double _rad2deg(double rad){
		return(_check(rad) * 57.295779513082321);
	}

public:
	/*---
	コンストラクタ
	---*/
	Angle(): _rad(0){}
	Angle(double angle,TypeOfAngle type = Radian){
		switch(type){
		case Radian:
			_rad=_check(angle);
			break;
		case Degree:
			_rad = _deg2rad(angle);
			break;
		}
	}
	Angle(int angle, TypeOfAngle type = Degree){
		*this = Angle(static_cast<double>(angle), type);
	}
	void Init(){
		_rad = 0;
	}

 /*------
	*セッタ・ゲッタ
	*------*/
	void Set(double angle, TypeOfAngle type = Radian){
		switch(type){
		case Radian:
			_rad = _check(angle);
			break;
		case Degree:
			_rad = _deg2rad(angle);
			break;
		}
	}


	/*---
	関数
	---*/
	double rad(){
		return _rad;
	}
	double deg(){
		return _rad2deg(_rad);
	}



	/*==============================
	オペレータオーバーロード
	================================*/

	//単項演算子
	Angle operator-(){
		return Angle(0 - _rad);
	}
	//インスタンス同士の結合(+,-のみ)
	Angle operator+(const Angle& rhs){
		return Angle(_rad+rhs._rad);
	}
	Angle operator-(const Angle& rhs){
		return Angle(_rad-rhs._rad);
	}
	Angle& operator+=(const Angle& rhs){
		_rad=_check(_rad+rhs._rad);
		return *this;
	}
	Angle& operator-=(const Angle& rhs){
		_rad=_check(_rad-rhs._rad);
		return *this;
	}

	/*-----------------------------
		*スカラとの結合
		*---------------------------*/
	//doubleとの結合(*, /のみ)
	Angle operator*(double rhs){
		return Angle(_rad*rhs);
	}
	Angle operator/(double rhs){
		return Angle(_rad/rhs);
	}
	Angle& operator*=(double rhs){
		_rad=_check(_rad*rhs);
		return *this;
	}
	Angle& operator/=(double rhs){
		_rad=_check(_rad/rhs);
		return *this;
	}

	//floatとの結合(*, /のみ)
	Angle operator*(float rhs){
		return Angle(_rad*rhs);
	}
	Angle operator/(float rhs){
		return Angle(_rad/rhs);
	}
	Angle& operator*=(float rhs){
		_rad=_check(_rad*rhs);
		return *this;
	}
	Angle& operator/=(float rhs){
		_rad=_check(_rad/rhs);
		return *this;
	}

	//intとの結合(*, /のみ)
	Angle operator*(int rhs){
		return Angle(_rad*rhs);
	}
	Angle operator/(int rhs){
		return Angle(_rad/rhs);
	}
	Angle& operator*=(int rhs){
		_rad=_check(_rad*rhs);
		return *this;
	}
	Angle& operator/=(int rhs){
		_rad=_check(_rad/rhs);
		return *this;
	}

	//比較演算子
	bool operator<(const Angle& rhs){
		if(_rad<rhs._rad){
			return true;
		}else{
			return false;
		}
	}
	bool operator>(const Angle& rhs){
		if(_rad<rhs._rad){
			return true;
		}else{
			return false;
		}
	}

	bool operator<=(const Angle& rhs){
		return !(*this > rhs);
	}
	bool operator>=(const Angle& rhs){
		return !(*this < rhs);
	}
	bool operator==(const Angle& rhs){
		if(_rad==rhs._rad){
			return true;
		}else{
			return false;
		}
	}
	bool operator!=(const Angle& rhs){
		return !(*this == rhs);
	}
};

}; //end of namespace mulib
#undef ANGLE_MIN
#undef ANGLE_MAX

#endif //__ANGLE_HPP
