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
	int _invFrm; //無敵フレーム(0より大きければ無敵)
	int _invImg; //無敵時画像
	double _angle;
	double _vx[8]; //45度毎の移動量
	double _vy[8];//45度毎の移動量

	bool _isShooting[2]; //撃ってるか？(過去1フレーム分も保存)

	void (*_ShotFunc)(PlayerShip& Obj); //通常ショット
	void (*_NarrowShotFunc)(PlayerShip& Obj); //低速時ショット

	void (*_BornDrawFunc)(PlayerShip& Obj);
	void (*_ExistDrawFunc)(PlayerShip& Obj);
	void (*_DeadDrawFunc)(PlayerShip& Obj);
public:
	static const int HitRect = 2; //当たり判定の一辺の大きさ

	PBulletGroup BltGrp; //敵弾
	ParticleGroup Particle;//パーティクル(かざり)

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
	void SetInvinvincibleImage(int img){_invImg = img;} //無敵時の画像をセット
	int GetInvincibleImage() const{return _invImg;} //無敵時の画像を返す
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

	//無敵画像を描画する
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
	void Move();//動作
	void MoveByHand(); //キーボード操作

	//生まれた時(Object::SetBorn()をオーバーライド)
	void SetBorn(){
		SetInvincibleFrame(60);
		Object::SetBorn();
	}
	//死んだ時(Object::SetDead()をオーバーライド)
	void SetDead(){
		if(GetInvincibleFrame()>0){return;} //無敵なら死なない
		Object::SetDead();
	}

	//通常ショット
	static void ShotNormal(PlayerShip& Obj);
	static void ShotNarrow(PlayerShip& Obj);

	//通常時描画
	static void ExistNormal(PlayerShip& Obj);
	//誕生時描画
	static void BornNormal(PlayerShip& Obj);
	//死亡時描画
	static void DeadNormal(PlayerShip& Obj);
};
#endif