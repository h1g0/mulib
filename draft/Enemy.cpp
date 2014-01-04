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
	SetDeletable(false); //敵が画面外から侵入することを想定し、最初は画面外にあっても消さない
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
		//ここにショット関係
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

//プレーヤー関連に対する敵1体のコリジョンを取る
void Enemy::HitCheck2Player(){
	//敵と自弾が接触しているか？
	int damage = _playerPtr->BltGrp.HitCheckAll(GetX(),GetY(),GetHitW(),GetHitH());
	ReduceHP(damage); //接触している分だけHPを減らす

	//自機と敵が接触しているか？
	if(_playerPtr->GetState() == PlayerShip::Exist &&
		HitCheck(GetX(),GetY(),
		_hitW,_hitH,
		_playerPtr->GetX(), _playerPtr->GetY(),
		PlayerShip::HitRect,PlayerShip::HitRect) == true){
			/*
			_人人 人人_
			> 突然の死 <
			￣Y^Y^Y^Y￣
			*/
			_playerPtr->SetDead();
	}
}

//敵生成
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

//プレーヤー関連に対する敵全体のコリジョンを取る
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