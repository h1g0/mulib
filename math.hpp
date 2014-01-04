#ifndef MULIB_MATH_HPP
#define MULIB_MATH_HPP

#include "angle.hpp"
#include "vector.hpp"

#define MULIB_PI 3.1415926535897932384626433832795 //��=3.14159265...
#define MULIB_RIGHT_ANGLE MULIB_PI/2 //��/2=90��
#define MULIB_PI_3_2 MULIB_PI*3/2 //3��/2=270��
#define MULIB_PI_F 3.1415926535897932384626433832795f
#define MULIB_RIGHT_ANGLE_F MULIB_PI_F/2.0f
#define MULIB_RAD MULIB_PI/180

//======================
//�����O�p�֐��֌W
//======================

namespace mulib{
namespace priv{
	//�O�p�֌W�萔
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
		** 2���x�W�F�Ȑ������߂�(���W�̂�)
		*
		*����: 2���x�W�F�Ȑ��̔C�ӂ̒i�K�����߂�B
		*
		*����: startPos: �n�_���W, endPos: �I�_���W, ctrlPos: ����_���W
		*			 divNum: ������(>0), nowPhase: ���������̋��߂�i�K(<=divNum)
		*
		*�Ԃ�l: ���W�N���X
	============================================================================*/
	template<typename T>
	Vec2D Get2DBezier(const Vec2D<T>& startPos,
										const Vec2D<T>& endPos,
										const Vec2D<T>& ctrlPos,
										const int divNum, const int nowPhase);

	/*!===========================================================================
		** 2���x�W�F�Ȑ������߂�(���S���W�E�p�x�E�����X�J��)
		*
		*����: 2���x�W�F�Ȑ��̔C�ӂ̒i�K�����߂�B
		*			 ���S���W(�w�肵���i�K�Ǝ��̒i�K�̍��W�̒��ԓ_)��
		*			 �p�x�ƒ��������߂���̂ŁA�摜�𒆐S�ŉ�]�����ĕ`�悷��̂ɕ֗��B
		*
		*����: startPos: �n�_���W, endPos: �I�_���W, ctrlPos: ����_���W
		*			 divNum: ������(>0), nowPhase: ���������̋��߂�i�K(<=divNum)
		*			 rtnPosPtr: ���߂���W������|�C���^(nullptr�ŋ��߂Ȃ�),
		*			 rtnVelPtr: ���߂钷��(���S����[)������|�C���^(nullptr�ŋ��߂Ȃ�)
		*			 rtnRadPtr: ���߂�p�x������|�C���^(nullptr�ŋ��߂Ȃ�)
		*
		*�Ԃ�l: �Ȃ�
		*
		*���l: ���S���W�����߂�s����A�����֐��̑���(���W�݂̂����߂��)�Ƃ�
		*			 �����ɕԂ����Ȑ����قȂ�(��������1����)�̂Œ���
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
