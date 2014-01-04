#include "Particle.h"
#include "Common.h"
#include <DxLib.h>

void Particle::Create(double x, double y, //�a�����̍��W
		    double angle, double speed, //�a�����̊p�x�Ƒ��x
			int image, //�摜
			void (*BornFunc)(Particle& Obj), //�a���G�t�F�N�g
			void (*MoveFunc)(Particle& Obj), //����X�N���v�g
			void (*DeadFunc)(Particle& Obj), //���S�G�t�F�N�g
			double drawExp, //�g�嗦
			int drawParam, //�`��p�����[�^
			bool deletable //��ʊO�ŏ������邩�H
			){
	SetXY(x,y);
	SetAngle(angle);
	SetSpeed(speed);
	SetVxVyFromSpeedAngle();
	SetImage(image);
	_BornFunc = BornFunc;
	_MoveFunc = MoveFunc;
	_DeadFunc = DeadFunc;
	SetDrawExp(drawExp);
	SetDrawParam(drawParam);
	SetDeletable(deletable);
	SetBorn();
}


void Particle::Move(){
	bool addFrame = false;
	switch(GetState()){
	case Born:
		if(_BornFunc == NULL){
			SetState(Exist);
		}else{
			_BornFunc(*this);
			AddVxVy2XY();
			addFrame = true;
		}
		break;
	case Exist:
		if(_MoveFunc != NULL){_MoveFunc(*this);}
		AddVxVy2XY();
		if(GetDeletable() == true && InScreen(GetX(),GetY(),-16,-16) == false){
			SetDead();
			break;
		}
		addFrame = true;

		break;
	case Dead:
		if(_DeadFunc == NULL){
			SetState(NotExist);
		}else{
			_DeadFunc(*this);
			AddVxVy2XY();
			addFrame = true;
		}

		break;
	case NotExist:
		break;
	}
	if(addFrame){AddFrame();}
}
void Particle::Draw(){
	if(GetState() != Exist){return;}
	SetDrawBlendMode(DX_BLENDMODE_ADD,_drawParam);
	SetDrawMode(1);

	DrawRotaGraph((int)GetX(),(int)GetY(),_drawExp,_angle,GetImage(),TRUE);
	SetDrawMode(0);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

int ParticleGroup::Create(double x, double y, //�a�����̍��W
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

int ParticleGroup::Move(){
	unsigned int cnt = 0;
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].Move();
		if(_unit[i].GetState() == _unit[i].Exist && i>cnt){
			cnt = i;
		}
	}
	return (signed int)(cnt);
}
void ParticleGroup::Draw(){
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].Draw();
	}
}

//����
void ParticleGroup::Delete(int Index){
	_unit[Index].SetState(Particle::NotExist);
}

//���ׂď���
void ParticleGroup::DeleteAll(){
	for(unsigned int i=0; i<_unit.size(); i++){
			Delete(i);
	}
}

//�����G�t�F�N�g�ɂ���
void ParticleGroup::SetDead(int Index){
	_unit[Index].SetDead();
}

//�S�ď����G�t�F�N�g�ɂ���
void ParticleGroup::SetDeadAll(){
	for(unsigned int i=0; i<_unit.size(); i++){
			SetDead(i);
	}
}