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

//全ての弾に対して当たり判定
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
int PBulletGroup::Create(double x, double y, //誕生時の座標
		    double angle, double speed, //誕生時の角度と速度
			int image, //画像
			void (*BornFunc)(Particle& Obj), //誕生エフェクト
			void (*MoveFunc)(Particle& Obj), //動作スクリプト
			void (*DeadFunc)(Particle& Obj), //死亡エフェクト
			double drawExp, int drawParam, bool deletable){

	//空いているリソースを検索
	for(int i=0; i<(signed)_unit.size(); i++){
		if(_unit[i].GetState() == Particle::NotExist){
			_unit[i].Create(x, y, angle, speed, image, BornFunc, MoveFunc, DeadFunc, drawExp, drawParam, deletable);
			return i;
		}
	}
	return -1; //見つからなかった
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

//消す
void PBulletGroup::Delete(int Index){
	_unit[Index].SetState(Particle::NotExist);
}

//すべて消す
void PBulletGroup::DeleteAll(){
	for(unsigned int i=0; i<_unit.size(); i++){
			Delete(i);
	}
}

//消去エフェクトにする
void PBulletGroup::SetDead(int Index){
	_unit[Index].SetDead();
}

//全て消去エフェクトにする
void PBulletGroup::SetDeadAll(){
	for(unsigned int i=0; i<_unit.size(); i++){
			SetDead(i);
	}
}