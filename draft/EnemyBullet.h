#ifndef __ENEMYBULLET_H
#define __ENEMYBULLET_H
#include"Particle.h"
#include "Player.h"
//#include "Enemy.h"
#include <vector>

class EnemyBullet: public Particle{
public:

	//生成時に帰ってくる情報群
	struct ReturnInfo{
		int shotCount;
		double angleShot; //最後に撃った角度
		int waitFrame; //これから待つフレーム数
	};

	//生成時に渡す情報群
	struct CreateInfo{
		double x; double y;//誕生時の座標
		double angleB4; //前回撃った角度
		int shotCount; //何発撃ったか？
		int image; //画像
		void (*MoveFunc)(EnemyBullet& obj, ReturnInfo *resultPtr); //動作スクリプト。NULLで生成取り消し
	};

private:
	int _hitRect;
	bool _isCreating; //生成時か否かのフラグ
	int _shotCnt; //親である敵にとって何発めか？
	double _angleShotB4;
	void (*_MoveFunc)(EnemyBullet& obj, ReturnInfo *resultPtr); //動きを指定するポインタ
	void (*_BornFunc)(EnemyBullet& obj); //誕生時エフェクト
	void (*_DeadFunc)(EnemyBullet& obj); //死んだ時のエフェクト

	PlayerShip *_playerPtr; //狙うべき自機
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

	static void PlayerOriented(EnemyBullet& obj, ReturnInfo *resultPtr){ //自機狙い
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
	static void spiral10way(EnemyBullet& obj, ReturnInfo *resultPtr){ //10way旋回砲塔
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
	 * 敵弾を生成する。
	 引数: obj: 生成情報を渡す構造体
	       result: 結果帰ってきた情報が代入される構造体への参照
	戻り値: 生成されたインデックス。-1でエラー
	 *----------------*/
	int Create(EnemyBullet::CreateInfo obj, EnemyBullet::ReturnInfo& result);
	int Move();
	void Draw();
	void HitCheckAll2Player();
};

#endif