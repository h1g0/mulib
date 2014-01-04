#ifndef __ENEMY_H
#define __ENENY_H
#include "Particle.h"
#include "Player.h"
#include "EnemyBullet.h"
/*
#include "bulletml/bulletmlparser.h"
#include "bulletml/bulletmlparser-tinyxml.h"
#include "bulletml/bulletmlrunner.h"
*/
class Enemy: public Particle{
public:
	static const int INVINCIBLE = -127; //敵が無敵であることの定数

	//敵生成時に渡す情報群
	struct CreateInfo{
		double x; double y;//誕生時の座標
		double angle; double speed; //誕生時の角度と速度
		int hitW; int hitH; //当たり判定の大きさ
		int hp; //HP
		int image; //画像
		int bulletImage; //敵弾の画像
		void (*BornFunc)(Enemy& obj); //誕生エフェクト
		void (*MoveFunc)(Enemy& obj); //動作スクリプト
		void (*ShotFunc)(EnemyBullet& obj, EnemyBullet::ReturnInfo *resultPtr); //弾幕スクリプト
		void (*DeadFunc)(Enemy& obj); //死亡エフェクト
	};

private:

	ParticleGroup *_particlePtr; //全体のパーティクル
	EBGroup *_bulletPtr; //敵弾
	PlayerShip *_playerPtr; //狙うべき自機

	int _hp; //HP。正の整数もしくは無敵定数をとる。
	int _hitW; //当たり判定の横幅
	int _hitH; //当たり判定の縦幅
	void(*_ShotFunc)(EnemyBullet& obj, EnemyBullet::ReturnInfo *resultPtr); //弾幕スクリプト
	double _angleShotB4; //以前弾を撃った角度
	int _frmShotWait; //何フレーム待機するか？
	int _shotCnt; //今まで同じ弾幕で何発敵弾を撃ったか？
	int _bulletImg; //弾の画像

	void (*_BornFunc)(Enemy& obj); //誕生時エフェクト
	void (*_MoveFunc)(Enemy& obj); //動きを指定するポインタ
	void (*_DeadFunc)(Enemy& obj); //死んだ時のエフェクト

public:
	Enemy():
	  _hp(0),
	  _ShotFunc(NULL),
	  _angleShotB4(0),
	  _frmShotWait(0),
	  _shotCnt(0),
	  _hitW(0),
	  _hitH(0),
	  _playerPtr(NULL),
	  _particlePtr(NULL)
	  {}

	void SetPlayer(PlayerShip *ptr){_playerPtr = ptr;}
	PlayerShip& GetPlayer(){return *_playerPtr;}
	void SetParticle(ParticleGroup *ptr){_particlePtr = ptr;}
	ParticleGroup& GetParticle(){return *_particlePtr;}
	void SetBullet(EBGroup *ptr){_bulletPtr = ptr;}
	EBGroup& GetBullet(){return *_bulletPtr;}

	/*敵のHPをセット。
	* 引数: HP(正の整数もしくは無敵定数INVINCIBLE)
	* 戻り値: 敵のHPが正しくセットされた場合: そのHP
	*        敵が死亡した場合: 0
	*/
	int SetHP(int hp){
		if(hp == INVINCIBLE){
			return INVINCIBLE;
		}else if(hp<=0){
			_hp = 0;
			SetDead();
			return 0;
		}else{
			_hp=hp;
			return _hp;
		}
	}
	int GetHP() const {return _hp;}

	//敵のHPを削る。
	int ReduceHP(int pow){
		//無敵なら変更しない。変更すると無敵ではなくなるので。
		if(_hp == INVINCIBLE){ return INVINCIBLE;}
		else{ return SetHP(_hp - pow); }
	}

	//敵にHPを加える
	int AddHP(int pow){
		//無敵なら変更しない。変更すると無敵ではなくなるので。
		if(_hp == INVINCIBLE){ return INVINCIBLE;}
		else{ return SetHP(_hp + pow); }
	}

	void SetHitW(int width){_hitW = width;}
	void SetHitH(int height){_hitH = height;}
	void SetHitRegion(int width, int height){
		SetHitW(width);
		SetHitH(height);
	}

	int GetHitW()const {return _hitW;}
	int GetHitH()const {return _hitH;}
	void GetHitRegion(int& width, int& height) const{
		width = _hitW;
		height = _hitH;
	}

	int GetShotCount() const {return _shotCnt;}
	void SetShotWait(int frame){ _frmShotWait = frame;}
	int GetShotWait() const{return _frmShotWait;}
	void DecreaseShotWait(){_frmShotWait--;}

		//生成する
	void Create(CreateInfo obj);
	void Move();
	void Draw();

	//当たり判定チェック
	void HitCheck2Player();

	//回転
	static void CircleLike(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <90){
			obj.SetAngle(obj.GetAngle()+6*RAD);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//逆回転
	static void CircleLikeMinus(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <=90){
			obj.SetAngle(obj.GetAngle()-6*RAD);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//遅くなる
	static void BecomeSlow(Enemy& obj){
		if(obj.GetFrame() >10 && obj.GetFrame() <=30){
			obj.SetSpeed(obj.GetSpeed()*0.9);
			obj.SetVxVyFromSpeedAngle();
		}else if(obj.GetFrame() >30){
			obj.SetSpeed(obj.GetSpeed()*1.04);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//遅くなって反転
	static void Retreat(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <=50){
			obj.SetSpeed(obj.GetSpeed()*0.9);
			obj.SetAngle(obj.GetAngle()+9*RAD);
			obj.SetVxVyFromSpeedAngle();
		}else if(obj.GetFrame() >50){
			obj.SetSpeed(obj.GetSpeed()*1.02);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//遅くなって反転2(90度横向きになる
	static void Retreat2(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <=50){
			obj.SetSpeed(obj.GetSpeed()*0.9);
			obj.SetAngle(obj.GetAngle()+13.5*RAD);
			obj.SetVxVyFromSpeedAngle();
		}else if(obj.GetFrame() >50){
			obj.SetSpeed(obj.GetSpeed()*1.05);
			obj.SetVxVyFromSpeedAngle();
		}
	}
	//遅くなって反転3(90度横向きになる
	static void Retreat3(Enemy& obj){
		if(obj.GetFrame() >30 && obj.GetFrame() <=50){
			obj.SetSpeed(obj.GetSpeed()*0.9);
			obj.SetAngle(obj.GetAngle()-13.5*RAD);
			obj.SetVxVyFromSpeedAngle();
		}else if(obj.GetFrame() >50){
			obj.SetSpeed(obj.GetSpeed()*1.05);
			obj.SetVxVyFromSpeedAngle();
		}
	}

	//パーティクル飛ばす
	static void DeadParticle(Enemy& obj){
		for(int i=0;i<30;i++){
			obj.GetParticle().Create(
				obj.GetX(),obj.GetY(),
				obj.GetAngle() +(-45  +  GetRand(900)*0.1) *RAD,
				GetRand(100)*0.1,
				obj.GetParticle().imgCommon,
				NULL,
				&Particle::GoDownAndFadeout,
				NULL,
				1.0,
				64,
				true);
		}
		obj.SetState(Enemy::NotExist);
	}
};

class EnmGroup{
private:
	std::vector<Enemy> _unit;
public:
	void SetLimit(int size){_unit.resize(size);}
	int GetLimit() const{return _unit.size();}
	int imgCommon;
	EnmGroup(){
		SetLimit(50);
	}

	EnmGroup(int size){
		SetLimit(size);
	}

	//敵生成。
	//生成に成功した場合はその番号、失敗した場合は-1
	int Create(Enemy::CreateInfo obj);
	int Move();
	void Draw();

	//全ての敵に対して自機の当たり判定を取る
	void HitCheckAll2Player();

	void SetPlayerPtr(PlayerShip *ptr);
	void SetParticlePtr(ParticleGroup *ptr);
	void SetBulletPtr(EBGroup *ptr);
};
#endif //__ENEMY_H