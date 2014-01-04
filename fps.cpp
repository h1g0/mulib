//FPS����
#include "fps.hpp"
#include "inter_lib.hpp"
namespace mulib{
using namespace interlib;
/*==============================================================================
 *FPS�������I�ɌŒ肷��
==============================================================================*/
void FpsManager::Fix(){
	//�t���O�������Ă��Ȃ���Δ�����
	if(_fixFPS == false){return;}

	int nowTime = GetNowCount(); //����̎���
	static int oldTime = 0; //�O��̎���
	int difTime = nowTime - oldTime;	//����

	for(;;){ //�K�莞�Ԍo�߂܂Ń��[�v
		if(difTime >= _frameTime){ //�K�莞�Ԃ��o�߁H
			oldTime = nowTime;	//����̎��Ԃ�ۑ����ďI��
			return;
		}else{ //�K�莞�Ԃ��o�߂��Ă��Ȃ���Αҋ@
			_WaitTimer(1); //1ms�҂�
			nowTime = _GetNowCount(); //���݂̎��Ԃƍ������X�V
			difTime = nowTime - oldTime;
		}
	}
}

/*==============================================================================
 * FPS���擾����
==============================================================================*/
int FpsManager::Get(){
	//FPS��\�����Ȃ��H
	if(_showFPS == false){return -1;}

	static int oldTime = _GetNowCount(); //�O��̎���
	static int count = 0;  //�J�E���^
	static int fps = 0;
	int nowTime = _GetNowCount();
	count++;
	//1000ms�߂������H
	if((nowTime - oldTime) >= 1000){
		int differ = ((nowTime - oldTime) - 1000); //�덷����
		//�O��̎��Ԃ��X�V
		oldTime = nowTime - differ;	//�덷�C��
		//FPS��ۑ�
		fps = count;
		//�J�E���^��������
		count = 0;
	}
	return fps;
}
}
