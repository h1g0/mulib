#ifndef MULIB_VECTOR_HPP
#define MULIB_VECTOR_HPP

#include<cmath>
#include<array>
#include "common.hpp"
#include "angle.hpp"
namespace mulib{
/*!=================================
 ** Vec2D: 2次元座標空間ベクタクラス **
 *==================================*/
template <typename T = double> class Vec2D{
public:
	//比較演算子で比較した際にどの座標軸を基準にするか。
	//デフォルトではxOnly(Xでのみ比較する)
	enum CompareBy{
		Xaxis,
		Yaxis,
		LengthFromOrigin,
		Area
	};

private:
	 //距離の二乗を求める関数(publicのラッパ関数を用意するのでprivate)
	#ifdef MULIB_USE_CONSTEXPR
	constexpr
	#endif
		T _Len2(const Vec2D& rhs){
		T diffx = _x - rhs._x;
		T diffy = _y - rhs._y;
		return(diffx*diffx + diffy*diffy);
	}
	inline T _Area(){
		return std::abs(_x * _y);
	}
	CompareBy _compare;
	/*--------
	メンバ
	--------*/
	T _x;
	T _y;

public:

	/*--------------
	コンストラクタ
	---------------*/
	Vec2D():_x(0),_y(0),_compare(Xaxis){} //何も代入しない(0で初期化される)
	Vec2D(CompareBy what): _x(0), _y(0), _compare(what){}
	//Vec2D(Vec2D vec){*this = vec;} //コピー
	Vec2D(T scalar, CompareBy what = Xaxis): //スカラー値がx,y両方に代入される
		_x(scalar), _y(scalar), _compare(what){}
	Vec2D(T x, T y, CompareBy what = Xaxis): //x,yそれぞれを指定する
		_x(x), _y(y), _compare(what){}

	/*------------
	ゲッタ・セッタ
	------------*/

	//比較対象フラグをセット
	void SetCompareBy(CompareBy what){
		_compare = what;
	}

	//比較対象をゲット
	CompareBy GetCompareBy(){
		return _compare;
	}

	virtual void Init(){
		_x = 0;
		_y = 0;
	}

	virtual void Set(T x, T y){
		_x = x;
		_y = y;
	}

	virtual void Set(T scalar){
		Set(scalar,scalar);
	}

	virtual void Set(std::array<T, 2> ar){
		Set(ar[0],ar[1]);
	}

	T x(){
		return _x;
	}

	T y(){
		return _y;
	}

	std::array<T, 2> xy(){
		std::array<T,2> temp = {_x,_y};
		return temp;
	}

	/*----------
	関数
	---------*/

	//原点からの距離の二乗を求める
	T Length2(){
		Vec2D<T> ZeroVector();
		return(_Len2(ZeroVector));
	}

	//距離の二乗を求める
	T Length2(const Vec2D& rhs){
		return(_Len2(rhs));
	}

	//原点からの距離を求める
	double Length(){
		Vec2D<T> ZeroVector();
		return(
			std::sqrt(
			static_cast<double>(_Len2(ZeroVector))
			));
	}
	//距離を求める
	double Length(const Vec2D& rhs){
		return(
			std::sqrt(
			static_cast<double>(_Len2(rhs))
			));
	}

	//成分の最大値を求める
	T Max(){
		if(x() < y()){return y();}
		else{return x();}
	}

	//成分の最小値を求める
	T Min(){
		if(x() < y()){return x();}
		else{return y();}
	}

	//成分の平均値を求める
	double Average(){
		return ((x() + y()) / 2);
	}

	//fromからtoの線形補完。tは補完率
	template<typename T>
	static Vec2D<T> Lerp(const Vec2D<T>& from, const Vec2D<T>& to, double t){
		if(t < 0){return from;}
		else if(t > 1.0){return to;}
		return (from + (to - from) * t);
	}

	//正規化されたベクトルを返す。Vec2D<double>を返す。
	Vec2D<double> Normarized(){
		double perlen = 1.0 / Length();
		return Vec2D<double>(_x * perlen, _y * perlen);
	}

	//内積(ドット積)を求める
	T Dot(const Vec2D& rhs){
		return _x * rhs._x + _x * rhs._y;
	}

	//外積(クロス積)を求める
	T Cross(const Vec2D& rhs){
		return _x * rhs._y - rhs._x * _y;
	}

	/*--------------------
	オペレータオーバーロード
	---------------------*/
	//単項演算
	Vec2D<T> operator-(){
		return Vec2D(0 - _x, 0 - _y);
	}

	//インスタンス同士の結合
	Vec2D<T> operator+(const Vec2D& rhs){
		return Vec2D(_x + rhs._x,_y + rhs._y);
	}
	Vec2D<T> operator-(const Vec2D& rhs){
		return Vec2D(_x-rhs._x,_y-rhs._y);
	}
	Vec2D operator*(const Vec2D& rhs){
		return Vec2D(_x*rhs._x,_y*rhs._y);
	}
	Vec2D operator/(const Vec2D& rhs){
		return Vec2D(_x/rhs._x,_y/rhs._y);
	}

	Vec2D& operator+=(const Vec2D& rhs){
		Set(_x + rhs._x, _y + rhs._y);
		return *this;
	}
	Vec2D& operator-=(const Vec2D& rhs){
		Set(_x - rhs._x, _y - rhs._y);
		return *this;
	}
	Vec2D& operator*=(const Vec2D& rhs){
		Set(_x * rhs._x, _y * rhs._y);
		return *this;
	}
	Vec2D& operator /=(const Vec2D& rhs){
		Set(_x / rhs._x, _y / rhs._y);
		return *this;
	}

	//スカラとの結合(一度インスタンスに変換してから結合するよりちょっとだけ高速な気がする)
	Vec2D<T> operator +(T rhs){
		return Vec2D<T>(_x+rhs,_y+rhs);
	}

	Vec2D<T> operator -(T rhs){
		return Vec2D<T>(_x-rhs,_y-rhs);
	}
	Vec2D<T> operator *(T rhs){
		return Vec2D<T>(_x*rhs,_y*rhs);
	}
	Vec2D<T> operator /(T rhs){
		return Vec2D<T>(_x/rhs,_y/rhs);
	}
	Vec2D<T>& operator +=(T rhs){
		_x += rhs;
		_y += rhs;
		return *this;
	}
	Vec2D<T>& operator -=(T rhs){
		_x -= rhs;
		_y -= rhs;
		return *this;
	}
	Vec2D<T>& operator *=(T rhs){
		_x*=rhs;
		_y*=rhs;
		return *this;
	}
	Vec2D<T>& operator /=(T rhs){
		_x/=rhs;
		_y/=rhs;
		return *this;
	}

	//比較演算子
	//<, <=, >, >=の4つの演算子は_compareの中身によって処理が変わってくる
	bool operator <(const Vec2D& rhs){
		switch(_compare){
		case Xaxis:
			if(_x<rhs._x){
				return true;
			}else{
				return false;
			}
			break;
		case Yaxis:
			if(_y<rhs._y){
				return true;
			}else{
				return false;
			}
			break;
		case LengthFromOrigin:
			if(_Len2()<rhs._Len2()){
				return true;
			}else{
				return false;
			}
			break;
		case Area:
			if(_Area()<rhs._Area()){
				return true;
			}else{
				return false;
			}
			break;
		}
	}

		bool operator >(const Vec2D& rhs){
		switch(_compare){
		case Xaxis:
			if(_x>rhs._x){
				return true;
			}else{
				return false;
			}
			break;
		case Yaxis:
			if(_y>rhs._y){
				return true;
			}else{
				return false;
			}
			break;
		case LengthFromOrigin:
			if(_Len2()>rhs._Len2()){
				return true;
			}else{
				return false;
			}
			break;
		case Area:
			if(_Area()>rhs._Area()){
				return true;
			}else{
				return false;
			}
			break;
		}
	}

	bool operator <=(const Vec2D& rhs){
		return !(*this>rhs);
	}

	bool operator >=(const Vec2D& rhs){
		return !(*this < rhs);
	}
	//==, !=の演算子はxとyが完全に一致するかしないか

	bool operator ==(const Vec2D& rhs){
		if(_x==rhs._x && _y==rhs._y){
			return true;
		}else{
			return false;
		}
	}

	bool operator !=(const Vec2D& rhs){
		return !(*this == rhs);
	}
};
/* 
//TODO: 実装する
class Vec2DAngle: public Vec2D<double>{
		Angle _angle;
		double _scalar;
public:
		void Init(){
			Vec2D::Init();
			_angle = 0;
			_scalar = 0;
		}
		void Set(T x, T y){
			Vec2D::Set(x,y);
			_scalar=Length();
			_angle=atan2(y,x);
		}

		void Set(const Angle& angle, double scalar){
			_angle = angle;
			_scalar = scalar;
			Vec2D::Set(cos(_angle) * _scalar, sin(_angle) * _scalar);
		}

		void SetAngle(const Angle& angle){
			Set(angle, _scalar);
		}

		void SetScalar(double scalar){
			Set(_angle, scalar);
		}

		const Angle& angle(){
			return _angle;
		}

		double scalar(){
			return _scalar;
		}
};

*/
/*====================================
 ** Vec3D: 3次元座標空間ベクタクラス **
====================================*/
//TODO: (気が向いたら|必要になったら)書く。
/*
template <typename T> class Vec3D: public Vec2D<T>{
};
*/
} //end of namespace mulib

#endif //MULIB_VECTOR_HPP
