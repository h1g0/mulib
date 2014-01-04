#include "Enemy.h"
#include "Player.h"
#include "Particle.h"

void Enemy::Create(CreateInfo obj){
	SetXY(obj.x,obj.y);
	SetAngle(obj.angle);
	SetSpeed(obj.speed);
	SetVxVyFromSpeedAngle();
	SetImage(obj.image);
	_bulletImg = obj.bulletImage;
	SetHP(obj.hp);
	_hitW = obj.hitW;
	_hitH = obj.hitH;
	SetShotWait(0);
	_angleShotB4 = 0;
	_shotCnt = 0;
	_BornFunc = obj.BornFunc;
	_ShotFunc = obj.ShotFunc;
	_MoveFunc = obj.MoveFunc;
	_DeadFunc = obj.DeadFunc;
	SetDrawExp(1.0);
	SetDrawParam(255);
	SetDeletable(false); //�G����ʊO����N�����邱�Ƃ�z�肵�A�ŏ��͉�ʊO�ɂ����Ă������Ȃ�
	SetBorn();
}

void Enemy::Move(){
	bool addFrame = false;
	switch(GetState()){
	case Born:
		if(_BornFunc == NULL){
			SetState(Exist);
			SetDeletable(true);
		}else{
			_BornFunc(*this);
			AddVxVy2XY();
			addFrame = true;
		}
		break;
	case Exist:
		if(_MoveFunc != NULL){_MoveFunc(*this);}
		AddVxVy2XY();
		if(GetDeletable() == true && InScreen(GetX(),GetY(),-GetHitW(),-GetHitH()) == false){
			SetDead();
			break;
		}
		addFrame = true;
		//�����ɃV���b�g�֌W
		while(GetShotWait() ==0){
			EnemyBullet::CreateInfo obj;
			EnemyBullet::ReturnInfo result;

			obj.x=GetX();
			obj.y=GetY();
			obj.MoveFunc=_ShotFunc;
			obj.shotCount=_shotCnt;
			obj.image = _bulletImg;
			obj.angleB4 = _angleShotB4;
			_bulletPtr->Create(obj,result);
			_angleShotB4 = result.angleShot;
			_shotCnt += result.shotCount;
			SetShotWait(result.waitFrame);
		}
		DecreaseShotWait();

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
void Enemy::Draw(){
	if(GetState() != Exist){return;}
	SetDrawBlendMode(DX_BLENDMODE_ADD,GetDrawParam());
	SetDrawMode(1);
	DrawRotaGraph((int)GetX(),(int)GetY(),GetDrawExp(),GetAngle(),GetImage(),TRUE);
	SetDrawMode(0);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

//�v���[���[�֘A�ɑ΂���G1�̂̃R���W���������
void Enemy::HitCheck2Player(){
	//�G�Ǝ��e���ڐG���Ă��邩�H
	int damage = _playerPtr->BltGrp.HitCheckAll(GetX(),GetY(),GetHitW(),GetHitH());
	ReduceHP(damage); //�ڐG���Ă��镪����HP�����炷

	//���@�ƓG���ڐG���Ă��邩�H
	if(_playerPtr->GetState() == PlayerShip::Exist &&
		HitCheck(GetX(),GetY(),
		_hitW,_hitH,
		_playerPtr->GetX(), _playerPtr->GetY(),
		PlayerShip::HitRect,PlayerShip::HitRect) == true){
			/*
			_�l�l �l�l_
			> �ˑR�̎� <
			�PY^Y^Y^Y�P
			*/
			_playerPtr->SetDead();
	}
}

//�G����
int EnmGroup::Create(Enemy::CreateInfo obj){
	for(unsigned int i=0;i<_unit.size();i++){
		if(_unit[i].GetState() == Object::NotExist){
			_unit[i].Create(obj);
			return i;
		}
	}
	return -1;
}
int EnmGroup::Move(){
	unsigned int cnt = 0;
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].Move();
		if(_unit[i].GetState() == _unit[i].Exist && i>cnt){
			cnt = i;
		}
	}
	return (signed int)(cnt);
}
void EnmGroup::Draw(){
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].Draw();
	}
}

//�v���[���[�֘A�ɑ΂���G�S�̂̃R���W���������
void EnmGroup::HitCheckAll2Player(){
	for(unsigned int i=0;i<_unit.size();i++){
		if(_unit[i].GetState() == Object::Exist){
			_unit[i].HitCheck2Player();
		}
	}
}
void EnmGroup::SetPlayerPtr(PlayerShip *ptr){
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].SetPlayer(ptr);
	}
}
void EnmGroup::SetParticlePtr(ParticleGroup *ptr){
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].SetParticle(ptr);
	}
}
void EnmGroup::SetBulletPtr(EBGroup *ptr){
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].SetBullet(ptr);
	}
}