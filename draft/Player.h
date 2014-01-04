#ifndef __PLAYER_H
#define __PLAYER_H

#include <DxLib.h>
#include "Common.h"
#include "Particle.h"
#include "PlayerBullet.h"

class PlayerShip: public Object{
private:
	bool _move;
	int _speed;
	int _invFrm; //���G�t���[��(0���傫����Ζ��G)
	int _invImg; //���G���摜
	double _angle;
	double _vx[8]; //45�x���̈ړ���
	double _vy[8];//45�x���̈ړ���

	bool _isShooting[2]; //�����Ă邩�H(�ߋ�1�t���[�������ۑ�)

	void (*_ShotFunc)(PlayerShip& Obj); //�ʏ�V���b�g
	void (*_NarrowShotFunc)(PlayerShip& Obj); //�ᑬ���V���b�g

	void (*_BornDrawFunc)(PlayerShip& Obj);
	void (*_ExistDrawFunc)(PlayerShip& Obj);
	void (*_DeadDrawFunc)(PlayerShip& Obj);
public:
	static const int HitRect = 2; //�����蔻��̈�ӂ̑傫��

	PBulletGroup BltGrp; //�G�e
	ParticleGroup Particle;//�p�[�e�B�N��(������)

	PlayerShip(){
		SetMove(true);
		SetSpeed(3);
		SetAngle(270*RAD);
		_ShotFunc = &ShotNormal;
		_BornDrawFunc = &BornNormal;
		_ExistDrawFunc = &ExistNormal;
		_DeadDrawFunc = &DeadNormal;
		_NarrowShotFunc = &ShotNarrow;
	}
	void SetAngle(double angle){_angle=angle;}
	double GetAngle() const{return _angle;}
	void SetInvinvincibleImage(int img){_invImg = img;} //���G���̉摜���Z�b�g
	int GetInvincibleImage() const{return _invImg;} //���G���̉摜��Ԃ�
	void SetInvincibleFrame(int frm){
		if(frm>0){
			_invFrm = frm;
		}else{
			_invFrm = 0;
		}
	}
	int GetInvincibleFrame() const{return _invFrm;}
	void DecreaseInvincibleFrame(){
		if(_invFrm>0){_invFrm--;}
	}

	//���G�摜��`�悷��
	void DrawInvincibleImage(){
		if(GetInvincibleFrame()>0){
			int param = 255;
			if(GetInvincibleFrame()<16){
				param = 16 * GetInvincibleFrame();
			}
			SetDrawBlendMode(DX_BLENDMODE_ADD,param);
			DrawRotaGraph((int)GetX(),(int)GetY(),1.0,0,_invImg,TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		}
	}

	void SetSpeed(int speed);
	bool GetIsShooting(int idx = 0) const{return _isShooting[idx];}
	void SetMove(bool move){_move = move;}
	void Draw();
	void Move();//����
	void MoveByHand(); //�L�[�{�[�h����

	//���܂ꂽ��(Object::SetBorn()���I�[�o�[���C�h)
	void SetBorn(){
		SetInvincibleFrame(60);
		Object::SetBorn();
	}
	//���񂾎�(Object::SetDead()���I�[�o�[���C�h)
	void SetDead(){
		if(GetInvincibleFrame()>0){return;} //���G�Ȃ玀�ȂȂ�
		Object::SetDead();
	}

	//�ʏ�V���b�g
	static void ShotNormal(PlayerShip& Obj);
	static void ShotNarrow(PlayerShip& Obj);

	//�ʏ펞�`��
	static void ExistNormal(PlayerShip& Obj);
	//�a�����`��
	static void BornNormal(PlayerShip& Obj);
	//���S���`��
	static void DeadNormal(PlayerShip& Obj);
};
#endif