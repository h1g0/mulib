#ifndef __ENEMYBULLET_H
#define __ENEMYBULLET_H
#include"Particle.h"
#include "Player.h"
//#include "Enemy.h"
#include <vector>

class EnemyBullet: public Particle{
public:

	//�������ɋA���Ă�����Q
	struct ReturnInfo{
		int shotCount;
		double angleShot; //�Ō�Ɍ������p�x
		int waitFrame; //���ꂩ��҂t���[����
	};

	//�������ɓn�����Q
	struct CreateInfo{
		double x; double y;//�a�����̍��W
		double angleB4; //�O�񌂂����p�x
		int shotCount; //�������������H
		int image; //�摜
		void (*MoveFunc)(EnemyBullet& obj, ReturnInfo *resultPtr); //����X�N���v�g�BNULL�Ő���������
	};

private:
	int _hitRect;
	bool _isCreating; //���������ۂ��̃t���O
	int _shotCnt; //�e�ł���G�ɂƂ��ĉ����߂��H
	double _angleShotB4;
	void (*_MoveFunc)(EnemyBullet& obj, ReturnInfo *resultPtr); //�������w�肷��|�C���^
	void (*_BornFunc)(EnemyBullet& obj); //�a�����G�t�F�N�g
	void (*_DeadFunc)(EnemyBullet& obj); //���񂾎��̃G�t�F�N�g

	PlayerShip *_playerPtr; //�_���ׂ����@
public:
	EnemyBullet(){
		_MoveFunc = NULL;
		_BornFunc = NULL;
		_DeadFunc = NULL;
		_hitRect = 2;
		_shotCnt = 0;
	}
	void SetHitRect(int rect){_hitRect = rect;}
	int GetHitRect() const{return _hitRect;}
	void SetPlayer(PlayerShip *ptr){_playerPtr = ptr;}
	PlayerShip& GetPlayer() const{return *_playerPtr;}
	bool GetIsCreating() const{return _isCreating;}
	int GetShotCount() const{return _shotCnt;}
	double GetAngleB4() const{return _angleShotB4;}
	ReturnInfo Create(CreateInfo obj);
	void Move();
	void Draw();
	void HitCheck2Player();

	static void PlayerOriented(EnemyBullet& obj, ReturnInfo *resultPtr){ //���@�_��
		if(obj.GetIsCreating() == true){
			double rad = MyMath::atan2(obj.GetPlayer().GetY() - obj.GetY(),
				                       obj.GetPlayer().GetX() - obj.GetX());
			obj.SetAngle(rad);
			obj.SetSpeed(5);
			obj.SetVxVyFromSpeedAngle();
			resultPtr->angleShot = rad;
			resultPtr->shotCount = 1;
			resultPtr->waitFrame = 20+GetRand(20);
		}
	}
	static void spiral10way(EnemyBullet& obj, ReturnInfo *resultPtr){ //10way����C��
		if(obj.GetIsCreating() == true){
			double rad = obj.GetAngleB4() + 36 * RAD;
			obj.SetAngle(rad);
			obj.SetSpeed(3);
			obj.SetVxVyFromSpeedAngle();
			resultPtr->angleShot = rad;
			resultPtr->shotCount = 1;
			resultPtr->waitFrame = 5;
		}
	}
};

class EBGroup{
private :
	std::vector<EnemyBullet> _unit;
public:
	int imgCommon;
	EBGroup(){
		_unit.resize(1000);
	}
	void SetPlayerPtr(PlayerShip *ptr);
	void SetLimit(int size){_unit.resize(size);}
	int GetLimit() const{_unit.size();}

	/*-----------------
	 * �G�e�𐶐�����B
	 ����: obj: ��������n���\����
	       result: ���ʋA���Ă�����񂪑�������\���̂ւ̎Q��
	�߂�l: �������ꂽ�C���f�b�N�X�B-1�ŃG���[
	 *----------------*/
	int Create(EnemyBullet::CreateInfo obj, EnemyBullet::ReturnInfo& result);
	int Move();
	void Draw();
	void HitCheckAll2Player();
};

#endif