#include "PlayerBullet.h"

void PBulletGroup::SetPowerAll(int pow){
	for(unsigned int i=0; i<_unit.size(); i++){
		_unit[i].SetPower(pow);
	}
}
void PBulletGroup::SetHitXYAll(int r){
	for(unsigned int i=0; i<_unit.size(); i++){
		_unit[i].SetHitXY(r);
	}
}

//�S�Ă̒e�ɑ΂��ē����蔻��
int PBulletGroup::HitCheckAll(double x, double y,int width,int height){
	int hitCount = 0;

	for(unsigned int i=0; i<_unit.size(); i++){
		if(_unit[i].GetState() == PlayerBullet::Exist){
			if(HitCheck(x,y,width,height,
					    _unit[i].GetX(),_unit[i].GetY(),_unit[i].GetHitRect(),_unit[i].GetHitRect()) == true){
				hitCount+=_unit[i].GetPower();
				SetDead(i);

			}
		}
	}
	return hitCount;
}
int PBulletGroup::Create(double x, double y, //�a�����̍��W
		    double angle, double speed, //�a�����̊p�x�Ƒ��x
			int image, //�摜
			void (*BornFunc)(Particle& Obj), //�a���G�t�F�N�g
			void (*MoveFunc)(Particle& Obj), //����X�N���v�g
			void (*DeadFunc)(Particle& Obj), //���S�G�t�F�N�g
			double drawExp, int drawParam, bool deletable){

	//�󂢂Ă��郊�\�[�X������
	for(int i=0; i<(signed)_unit.size(); i++){
		if(_unit[i].GetState() == Particle::NotExist){
			_unit[i].Create(x, y, angle, speed, image, BornFunc, MoveFunc, DeadFunc, drawExp, drawParam, deletable);
			return i;
		}
	}
	return -1; //������Ȃ�����
}

int PBulletGroup::Move(){
	unsigned int cnt = 0;
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].Move();
		if(_unit[i].GetState() == _unit[i].Exist && i>cnt){
			cnt = i;
		}
	}
	return (signed int)cnt;
}
void PBulletGroup::Draw(){
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].Draw();
	}
}

//����
void PBulletGroup::Delete(int Index){
	_unit[Index].SetState(Particle::NotExist);
}

//���ׂď���
void PBulletGroup::DeleteAll(){
	for(unsigned int i=0; i<_unit.size(); i++){
			Delete(i);
	}
}

//�����G�t�F�N�g�ɂ���
void PBulletGroup::SetDead(int Index){
	_unit[Index].SetDead();
}

//�S�ď����G�t�F�N�g�ɂ���
void PBulletGroup::SetDeadAll(){
	for(unsigned int i=0; i<_unit.size(); i++){
			SetDead(i);
	}
}