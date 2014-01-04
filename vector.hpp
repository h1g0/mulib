#ifndef MULIB_VECTOR_HPP
#define MULIB_VECTOR_HPP

#include<cmath>
#include<array>
#include "common.hpp"
#include "angle.hpp"
namespace mulib{
/*!=================================
 ** Vec2D: 2�������W��ԃx�N�^�N���X **
 *==================================*/
template <typename T = double> class Vec2D{
public:
	//��r���Z�q�Ŕ�r�����ۂɂǂ̍��W������ɂ��邩�B
	//�f�t�H���g�ł�xOnly(X�ł̂ݔ�r����)
	enum CompareBy{
		Xaxis,
		Yaxis,
		LengthFromOrigin,
		Area
	};

private:
	 //�����̓������߂�֐�(public�̃��b�p�֐���p�ӂ���̂�private)
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
	�����o
	--------*/
	T _x;
	T _y;

public:

	/*--------------
	�R���X�g���N�^
	---------------*/
	Vec2D():_x(0),_y(0),_compare(Xaxis){} //����������Ȃ�(0�ŏ����������)
	Vec2D(CompareBy what): _x(0), _y(0), _compare(what){}
	//Vec2D(Vec2D vec){*this = vec;} //�R�s�[
	Vec2D(T scalar, CompareBy what = Xaxis): //�X�J���[�l��x,y�����ɑ�������
		_x(scalar), _y(scalar), _compare(what){}
	Vec2D(T x, T y, CompareBy what = Xaxis): //x,y���ꂼ����w�肷��
		_x(x), _y(y), _compare(what){}

	/*------------
	�Q�b�^�E�Z�b�^
	------------*/

	//��r�Ώۃt���O���Z�b�g
	void SetCompareBy(CompareBy what){
		_compare = what;
	}

	//��r�Ώۂ��Q�b�g
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
	�֐�
	---------*/

	//���_����̋����̓������߂�
	T Length2(){
		Vec2D<T> ZeroVector();
		return(_Len2(ZeroVector));
	}

	//�����̓������߂�
	T Length2(const Vec2D& rhs){
		return(_Len2(rhs));
	}

	//���_����̋��������߂�
	double Length(){
		Vec2D<T> ZeroVector();
		return(
			std::sqrt(
			static_cast<double>(_Len2(ZeroVector))
			));
	}
	//���������߂�
	double Length(const Vec2D& rhs){
		return(
			std::sqrt(
			static_cast<double>(_Len2(rhs))
			));
	}

	//�����̍ő�l�����߂�
	T Max(){
		if(x() < y()){return y();}
		else{return x();}
	}

	//�����̍ŏ��l�����߂�
	T Min(){
		if(x() < y()){return x();}
		else{return y();}
	}

	//�����̕��ϒl�����߂�
	double Average(){
		return ((x() + y()) / 2);
	}

	//from����to�̐��`�⊮�Bt�͕⊮��
	template<typename T>
	static Vec2D<T> Lerp(const Vec2D<T>& from, const Vec2D<T>& to, double t){
		if(t < 0){return from;}
		else if(t > 1.0){return to;}
		return (from + (to - from) * t);
	}

	//���K�����ꂽ�x�N�g����Ԃ��BVec2D<double>��Ԃ��B
	Vec2D<double> Normarized(){
		double perlen = 1.0 / Length();
		return Vec2D<double>(_x * perlen, _y * perlen);
	}

	//����(�h�b�g��)�����߂�
	T Dot(const Vec2D& rhs){
		return _x * rhs._x + _x * rhs._y;
	}

	//�O��(�N���X��)�����߂�
	T Cross(const Vec2D& rhs){
		return _x * rhs._y - rhs._x * _y;
	}

	/*--------------------
	�I�y���[�^�I�[�o�[���[�h
	---------------------*/
	//�P�����Z
	Vec2D<T> operator-(){
		return Vec2D(0 - _x, 0 - _y);
	}

	//�C���X�^���X���m�̌���
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

	//�X�J���Ƃ̌���(��x�C���X�^���X�ɕϊ����Ă��猋�������肿����Ƃ��������ȋC������)
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

	//��r���Z�q
	//<, <=, >, >=��4�̉��Z�q��_compare�̒��g�ɂ���ď������ς���Ă���
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
	//==, !=�̉��Z�q��x��y�����S�Ɉ�v���邩���Ȃ���

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
//TODO: ��������
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
 ** Vec3D: 3�������W��ԃx�N�^�N���X **
====================================*/
//TODO: (�C����������|�K�v�ɂȂ�����)�����B
/*
template <typename T> class Vec3D: public Vec2D<T>{
};
*/
} //end of namespace mulib

#endif //MULIB_VECTOR_HPP
