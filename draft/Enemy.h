#ifndef __ENEMY_H
#define __ENENY_H
#include "Particle.h"
#include "Player.h"
#include "EnemyBullet.h"
/*
#include "bulletml/bulletmlparser.h"
#include "bulletml/bulletmlparser-tinyxml.h"
#include "bulletml/bulletmlrunner.h"
*/
class Enemy: public Particle{
public:
	static const int INVINCIBLE = -127; //�G�����G�ł��邱�Ƃ̒萔

	//�G�������ɓn�����Q
	struct CreateInfo{
		double x; double y;//�a�����̍��W
		double angle; double speed; //�a�����̊p�x�Ƒ��x
		int hitW; int hitH; //�����蔻��̑傫��
		int hp; //HP
		int image; //�摜
		int bulletImage; //�G�e�̉摜
		void (*BornFunc)(Enemy& obj); //�a���G�t�F�N�g
		void (*MoveFunc)(Enemy& obj); //����X�N���v�g
		void (*ShotFunc)(EnemyBullet& obj, EnemyBullet::ReturnInfo *resultPtr); //�e���X�N���v�g
		void (*DeadFunc)(Enemy& obj); //���S�G�t�F�N�g
	};

private:

	ParticleGroup *_particlePtr; //�S�̂̃p�[�e�B�N��
	EBGroup *_bulletPtr; //�G�e
	PlayerShip *_playerPtr; //�_���ׂ����@

	int _hp; //HP�B���̐����������͖��G�萔���Ƃ�B
	int _hitW; //�����蔻��̉���
	int _hitH; //�����蔻��̏c��
	void(*_ShotFunc)(EnemyBullet& obj, EnemyBullet::ReturnInfo *resultPtr); //�e���X�N���v�g
	double _angleShotB4; //�ȑO�e���������p�x
	int _frmShotWait; //���t���[���ҋ@���邩�H
	int _shotCnt; //���܂œ����e���ŉ����G�e�����������H
	int _bulletImg; //�e�̉摜

	void (*_BornFunc)(Enemy& obj); //�a�����G�t�F�N�g
	void (*_MoveFunc)(Enemy& obj); //�������w�肷��|�C���^
	void (*_DeadFunc)(Enemy& obj); //���񂾎��̃G�t�F�N�g

public:
	Enemy():
	  _hp(0),
	  _ShotFunc(NULL),
	  _angleShotB4(0),
	  _frmShotWait(0),
	  _shotCnt(0),
	  _hitW(0),
	  _hitH(0),
	  _playerPtr(NULL),
	  _particlePtr(NULL)
	  {}

	void SetPlayer(PlayerShip *ptr){_playerPtr = ptr;}
	PlayerShip& GetPlayer(){return *_playerPtr;}
	void SetParticle(ParticleGroup *ptr){_particlePtr = ptr;}
	ParticleGroup& GetParticle(){return *_particlePtr;}
	void SetBullet(EBGroup *ptr){_bulletPtr = ptr;}
	EBGroup& GetBullet(){return *_bulletPtr;}

	/*�G��HP���Z�b�g�B
	* ����: HP(���̐����������͖��G�萔INVINCIBLE)
	* �߂�l: �G��HP���������Z�b�g���ꂽ�ꍇ: ����HP
	*        �G�����S�����ꍇ: 0
	*/
	int SetHP(int hp){
		if(hp == INVINCIBLE){
			return INVINCIBLE;
		}else if(hp<=0){
			_hp = 0;
			SetDead();
			return 0;
		}else{
			_hp=hp;
			return _hp;
		}
	}
	int GetHP() const {return _hp;}

	//�G��HP�����B
	int ReduceHP(int pow){
		//���G�Ȃ�ύX���Ȃ��B�ύX����Ɩ��G�ł͂Ȃ��Ȃ�̂ŁB
		if(_hp == INVINCIBLE){ return INVINCIBLE;}
		else{ return SetHP(_hp - pow); }
	}

	//�G��HP��������
	int AddHP(int pow){
		//���G�Ȃ�ύX���Ȃ��B�ύX����Ɩ��G�ł͂Ȃ��Ȃ�̂ŁB
		if(_hp == INVINCIBLE){ return INVINCIBLE;}
		else{ return SetHP(_hp + pow); }
	}

	void SetHitW(int width){_hitW = width;}
	void SetHitH(int height){_hitH = height;}
	void SetHitRegion(int width, int height){
		SetHitW(width);
		SetHitH(height);
	}

	int GetHitW()const {return _hitW;}
	int GetHitH()const {return _hitH;}
	void GetHitRegion(int& width, int& height) const{
		width = _hitW;
		height = _hitH;
	}

	int GetShotCount() const {return _shotCnt;}
	void SetShotWait(int frame){ _frmShotWait = frame;}
	int GetShotWait() const{return _frmShotWait;}
	void DecreaseShotWait(){_frmShotWait--;}

		//��������
	void Create(CreateInfo obj);
	void Move();
	void Draw();

	//�����蔻��`�F�b�N
	void HitCheck2Player();

	//��]
	static void CircleLike(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <90){
			obj.SetAngle(obj.GetAngle()+6*RAD);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//�t��]
	static void CircleLikeMinus(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <=90){
			obj.SetAngle(obj.GetAngle()-6*RAD);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//�x���Ȃ�
	static void BecomeSlow(Enemy& obj){
		if(obj.GetFrame() >10 && obj.GetFrame() <=30){
			obj.SetSpeed(obj.GetSpeed()*0.9);
			obj.SetVxVyFromSpeedAngle();
		}else if(obj.GetFrame() >30){
			obj.SetSpeed(obj.GetSpeed()*1.04);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//�x���Ȃ��Ĕ��]
	static void Retreat(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <=50){
			obj.SetSpeed(obj.GetSpeed()*0.9);
			obj.SetAngle(obj.GetAngle()+9*RAD);
			obj.SetVxVyFromSpeedAngle();
		}else if(obj.GetFrame() >50){
			obj.SetSpeed(obj.GetSpeed()*1.02);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//�x���Ȃ��Ĕ��]2(90�x�������ɂȂ�
	static void Retreat2(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <=50){
			obj.SetSpeed(obj.GetSpeed()*0.9);
			obj.SetAngle(obj.GetAngle()+13.5*RAD);
			obj.SetVxVyFromSpeedAngle();
		}else if(obj.GetFrame() >50){
			obj.SetSpeed(obj.GetSpeed()*1.05);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//�x���Ȃ��Ĕ��]3(90�x�������ɂȂ�
	static void Retreat3(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <=50){
			obj.SetSpeed(obj.GetSpeed()*0.9);
			obj.SetAngle(obj.GetAngle()-13.5*RAD);
			obj.SetVxVyFromSpeedAngle();
		}else if(obj.GetFrame() >50){
			obj.SetSpeed(obj.GetSpeed()*1.05);
			obj.SetVxVyFromSpeedAngle();
		}
	}

	//�p�[�e�B�N����΂�
	static void DeadParticle(Enemy& obj){
		for(int i=0;i<30;i++){
			obj.GetParticle().Create(
				obj.GetX(),obj.GetY(),
				obj.GetAngle() +(-45  +  GetRand(900)*0.1) *RAD,
				GetRand(100)*0.1,
				obj.GetParticle().imgCommon,
				NULL,
				&Particle::GoDownAndFadeout,
				NULL,
				1.0,
				64,
				true);
		}
		obj.SetState(Enemy::NotExist);
	}
};

class EnmGroup{
private:
	std::vector<Enemy> _unit;
public:
	void SetLimit(int size){_unit.resize(size);}
	int GetLimit() const{return _unit.size();}
	int imgCommon;
	EnmGroup(){
		SetLimit(50);
	}

	EnmGroup(int size){
		SetLimit(size);
	}

	//�G�����B
	//�����ɐ��������ꍇ�͂��̔ԍ��A���s�����ꍇ��-1
	int Create(Enemy::CreateInfo obj);
	int Move();
	void Draw();

	//�S�Ă̓G�ɑ΂��Ď��@�̓����蔻������
	void HitCheckAll2Player();

	void SetPlayerPtr(PlayerShip *ptr);
	void SetParticlePtr(ParticleGroup *ptr);
	void SetBulletPtr(EBGroup *ptr);
};
#endif //__ENEMY_H