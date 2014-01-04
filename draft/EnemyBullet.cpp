#include "EnemyBullet.h"

EnemyBullet::ReturnInfo EnemyBullet::Create(CreateInfo obj){
	SetXY(obj.x,obj.y);
	SetImage(obj.image);
	_MoveFunc = obj.MoveFunc;
	_shotCnt = obj.shotCount;
	_angleShotB4 = obj.angleB4;
	SetDrawExp(1.5);
	SetDrawParam(255);
	SetDeletable(true); //敵が画面外から侵入することを想定し、最初は画面外にあっても消さない
	ReturnInfo result;
	_isCreating = true;
	_MoveFunc(*this, &result);
	_isCreating = false;
	SetBorn();
	return result;
}
void EnemyBullet::Move(){
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
		if(_MoveFunc != NULL){_MoveFunc(*this, NULL);}
		AddVxVy2XY();
		if(GetDeletable() == true && InScreen(GetX(),GetY(),-GetHitRect(),-GetHitRect()) == false){
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
void EnemyBullet::Draw(){
	if(GetState() != Exist){return;}
	SetDrawBlendMode(DX_BLENDMODE_ADD,GetDrawParam());
	SetDrawMode(1);
	DrawRotaGraph((int)GetX(),(int)GetY(),GetDrawExp(),GetAngle(),GetImage(),TRUE);
	SetDrawMode(0);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}
//プレーヤーに当たっているか？
void EnemyBullet::HitCheck2Player(){
	if(_playerPtr->GetState() == PlayerShip::Exist &&
		HitCheck(GetX(),GetY(),
		GetHitRect(),GetHitRect(),
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
int EBGroup::Create(EnemyBullet::CreateInfo obj, EnemyBullet::ReturnInfo& result){
	if(obj.MoveFunc == NULL){return -1;} //移動情報が無ければ生成しない
	for(unsigned int i=0;i<_unit.size();i++){
		if(_unit[i].GetState() == Object::NotExist){
			result = _unit[i].Create(obj);
			return i;
		}
	}
	return -1;
}
int EBGroup::Move(){
	unsigned int cnt = 0;
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].Move();
		if(_unit[i].GetState() == _unit[i].Exist && i>cnt){
			cnt = i;
		}
	}
	return (signed int)(cnt);
}
void EBGroup::Draw(){
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].Draw();
	}
}

//プレーヤー関連に対する敵全体のコリジョンを取る
void EBGroup::HitCheckAll2Player(){
	for(unsigned int i=0;i<_unit.size();i++){
		if(_unit[i].GetState() == Object::Exist){
			_unit[i].HitCheck2Player();
		}
	}
}
void EBGroup::SetPlayerPtr(PlayerShip *ptr){
	for(unsigned int i=0;i<_unit.size();i++){
		_unit[i].SetPlayer(ptr);
	}
}