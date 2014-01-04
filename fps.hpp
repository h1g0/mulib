#ifndef FPS_HPP
#define FPS_HPP

namespace mulib{
/*#############################################################################
	FPS�Œ�E�Z�o�N���X
===============================================================================
	�g����:
	���C�����[�v�J�n�O�ɐ錾����(�܂�������O����)
	��
	FPS��`�悷��^�C�~���O��Get()���Ă�(���C�����[�v�̂Ȃ�ׂ��Ō�̕����D�܂���)
	��
	Fix()�ŌŒ肷��
#############################################################################*/

//FPS�ݒ�
class FpsManager{
	bool _fixFPS; //FPS���Œ肷�邩�H
	bool _showFPS; //FPS��\�����邩�H
	int _defaultFPS; //�f�t�H���g��FPS
	int _frameTime; //1�t���[���̎���(�~���b)
public:
	/*�R���X�g���N�^
	����: fixFps: FPS���Œ肷�邩�H
				showFps: FPS��\�����邩�H
				fixFpsNum: FPS����ɌŒ肷�邩�H
	*/
	FpsManager(bool fixFps = true, bool showFps = true, int defaultFps = 60){
		SetFixFPS(fixFps);
		SetShowFPS(showFps);
		SetDefaultFPS(defaultFps);
	}
	//FPS���Œ肷��
	void Fix();
	//FPS��Ԃ�(�Ԃ�l: FPS)
	int Get();

	//�Z�b�^
	void SetFixFPS(bool fixFps){_fixFPS = fixFps;}
	void SetShowFPS(bool showFps){_showFPS = showFps;}
	void SetDefaultFPS(int defaultFps){
		if(_defaultFPS > 0 && _defaultFPS <= 1000){
			_defaultFPS = defaultFps;
		}else{
			_defaultFPS = 60;
		}
		_frameTime = static_cast<int>(1000 / _defaultFPS);
	}

	//�Q�b�^
	bool fixFPS()const{return _fixFPS;}
	bool showFPS()const{return _showFPS;}
	int defaultFPS()const{return _defaultFPS;}
};
} //namespace mulib

#endif
