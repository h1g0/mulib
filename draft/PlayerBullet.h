#ifndef __PLAYERBULLET_H
#define __PLAYERBULLET_H

#include <DxLib.h>
#include "Common.h"
#include "Particle.h"
/*#################################

	自弾関係

#################################*/

class PlayerBullet: public Particle{
private:
	int _hitRect; //当たり判定の大きさ
	int _power; //威力
	static const int _NORMAL_POW = 1;
	static const int _NORMAL_RECT = 16;

public:

	void SetHitXY(int rect){
		_hitRect = rect;
	}
	int GetHitRect() const{return _hitRect;}
	int GetPower() const{return _power;}
	void SetPower(int pow){_power = pow;}

	//コンストラクタ
	PlayerBullet(){
		SetAngle(270 * RAD);
		SetSpeed(10);
		SetVxVyFromSpeedAngle();
		SetHitXY(16);
		SetPower(1);
	}
	//void Move();
	//void Draw();

};

class PBulletGroup{
private:
	std::vector<PlayerBullet> _unit;
public:
	void SetLimit(int size){_unit.resize(size);}
	int GetLimit() const{return _unit.size();}

	PBulletGroup(){
		SetLimit(100);
	}

	PBulletGroup(int size){
		SetLimit(size);
	}

	int imgCommon;

	int Create(double x, double y, //誕生時の座標
		    double angle, double speed, //誕生時の角度と速度
			int image, //画像
			void (*BornFunc)(Particle& Obj) = NULL, //誕生エフェクト
			void (*MoveFunc)(Particle& Obj) = NULL, //動作スクリプト
			void (*DeadFunc)(Particle& Obj) = NULL, //死亡エフェクト
			double drawExp = 1.0, //拡大率
			int drawParam = 255, //描画パラメータ
			bool deletable = true //画面外に飛び出した時に消去するか？
			);

	//消す
	void Delete(int Index);
	//すべて消す
	void DeleteAll();

	void SetDead(int index);
	void SetDeadAll();

	//動かす函数。現在存在するパーティクルの数を返す。
	int Move();
	void Draw();

	void SetPowerAll(int pow = 1);
	void SetHitXYAll(int r = 16);
	//全ての弾に対して当たり判定。戻り値は当たった事によって発生したダメージの総計
	int HitCheckAll(double x, double y,int width,int height);
};

#endif