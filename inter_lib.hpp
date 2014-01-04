#ifndef INTER_LIB_HPP
#define INTER_LIB_HPP
/*!###########################################################
	**���C�u�����Ԃ̍��ق��z������
	*
	*��ɃQ�[���p���C�u����(DxLib��SDL�Ȃ�)���g�p����ۂɁA
	*���C�u�����Ԃ̍��ق��z�����邽�߂̃N�b�V���������ނ��Ƃ�
	*�ڐA���̌����}��B
	*�J������DxLib���g�p�����֌W��ADxLib�̎d�l�ɐe�a���������B
	*##########################################################*/

#include <DxLib.h> //�g�p���郉�C�u����

#include <string>
#include "vector.hpp"

namespace mulib{
	typedef int ImgHolder; //�摜��ێ�����ϐ��^
	typedef int KeyHolder; //�{�^��/�L�[��ێ�����ϐ��^
namespace interlib{
	/*!====================================================
	 *LoadImg: �摜�����[�h����
	 *
	 *string�^�̃t�@�C���p�X�����A�擾����ImgHolder��Ԃ��B
	 *=====================================================*/
	ImgHolder LoadImg(const std::string& file);
	Vec2D<int> GetImgSize(const ImgHolder& imgHdl);
	int UnloadImg(const ImgHolder& imgHdl);

	bool CheckIsKeyPushed(const KeyHolder& handle);

	void _WaitTimer(int mSec){WaitTimer(mSec);}
	int _GetNowCount(){return GetNowCount();}
}}

#endif