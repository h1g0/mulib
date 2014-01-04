#ifndef MULIB_ANGLE_HPP
#define MULIB_ANGLE_HPP

#include "common.hpp"
/*!===================================
 ** Angle: �p�x�N���X **
 *
 * �����ł͑S��double�^�̃��W�A���ŕێ��B
 * ���o�����Ƀ��W�A�����f�B�O���[���őI��
 * 0 <= Angle < 2�΂̊ԂɎ��܂邱�Ƃ��ۏ؂����
====================================*/
#define ANGLE_MIN 0 //�ŏ��l: 0
#define ANGLE_MAX 2 * 3.1415926535897932384 //�ő�l(<2��)

namespace mulib{
class Angle{
public:
	//������Ƀf�B�O���[�ő�����邩���W�A���ő�����邩�̃t���O
	enum TypeOfAngle{
		Radian,
		Degree
	};

private:
	double _rad; //�����I�ȃ����o

	//�p�x���͈͓��Ɏ��܂�悤�␳(���܂�܂Ń��[�v����)
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


	//�f�B�O���[���烉�W�A����
	#ifdef MULIB_USE_CONSTEXPR
	constexpr
	#endif
	double _deg2rad(double deg){
		return(_check(deg * 0.017453292519943));
	}

	//���W�A������f�B�O���[��
	#ifdef MULIB_USE_CONSTEXPR
	constexpr
	#endif
	double _rad2deg(double rad){
		return(_check(rad) * 57.295779513082321);
	}

public:
	/*---
	�R���X�g���N�^
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
	*�Z�b�^�E�Q�b�^
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
	�֐�
	---*/
	double rad(){
		return _rad;
	}
	double deg(){
		return _rad2deg(_rad);
	}



	/*==============================
	�I�y���[�^�I�[�o�[���[�h
	================================*/

	//�P�����Z�q
	Angle operator-(){
		return Angle(0 - _rad);
	}
	//�C���X�^���X���m�̌���(+,-�̂�)
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
		*�X�J���Ƃ̌���
		*---------------------------*/
	//double�Ƃ̌���(*, /�̂�)
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

	//float�Ƃ̌���(*, /�̂�)
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

	//int�Ƃ̌���(*, /�̂�)
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

	//��r���Z�q
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
