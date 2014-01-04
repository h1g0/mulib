#ifndef __PLAYERBULLET_H
#define __PLAYERBULLET_H

#include <DxLib.h>
#include "Common.h"
#include "Particle.h"
/*#################################

	���e�֌W

#################################*/

class PlayerBullet: public Particle{
private:
	int _hitRect; //�����蔻��̑傫��
	int _power; //�З�
	static const int _NORMAL_POW = 1;
	static const int _NORMAL_RECT = 16;

public:

	void SetHitXY(int rect){
		_hitRect = rect;
	}
	int GetHitRect() const{return _hitRect;}
	int GetPower() const{return _power;}
	void SetPower(int pow){_power = pow;}

	//�R���X�g���N�^
	PlayerBullet(){
		SetAngle(270 * RAD);
		SetSpeed(10);
		SetVxVyFromSpeedAngle();
		SetHitXY(16);
		SetPower(1);
	}
	//void Move();
	//void Draw();

};

class PBulletGroup{
private:
	std::vector<PlayerBullet> _unit;
public:
	void SetLimit(int size){_unit.resize(size);}
	int GetLimit() const{return _unit.size();}

	PBulletGroup(){
		SetLimit(100);
	}

	PBulletGroup(int size){
		SetLimit(size);
	}

	int imgCommon;

	int Create(double x, double y, //�a�����̍��W
		    double angle, double speed, //�a�����̊p�x�Ƒ��x
			int image, //�摜
			void (*BornFunc)(Particle& Obj) = NULL, //�a���G�t�F�N�g
			void (*MoveFunc)(Particle& Obj) = NULL, //����X�N���v�g
			void (*DeadFunc)(Particle& Obj) = NULL, //���S�G�t�F�N�g
			double drawExp = 1.0, //�g�嗦
			int drawParam = 255, //�`��p�����[�^
			bool deletable = true //��ʊO�ɔ�яo�������ɏ������邩�H
			);

	//����
	void Delete(int Index);
	//���ׂď���
	void DeleteAll();

	void SetDead(int index);
	void SetDeadAll();

	//�����������B���ݑ��݂���p�[�e�B�N���̐���Ԃ��B
	int Move();
	void Draw();

	void SetPowerAll(int pow = 1);
	void SetHitXYAll(int r = 16);
	//�S�Ă̒e�ɑ΂��ē����蔻��B�߂�l�͓����������ɂ���Ĕ��������_���[�W�̑��v
	int HitCheckAll(double x, double y,int width,int height);
};

#endif