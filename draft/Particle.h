#ifndef __PARTICLE_H
#define __PARTICLE_H
#include "Object.h"
class Particle: public Object{
private:
	double _angle; //角度
	double _speed; //速度
	bool _deletable; //画面外に出たら消すか？
	int _drawParam; //描画時のパラメータ
	double _drawExp; //描画時の拡大率

	void (*_BornFunc)(Particle& Obj); //誕生時エフェクト
	void (*_MoveFunc)(Particle& Obj); //動きを指定するポインタ
	void (*_DeadFunc)(Particle& Obj); //死んだ時のエフェクト
public:

	//セッタ・ゲッタ
	void SetSpeed(double speed){_speed = speed;}
	double GetSpeed() const{return _speed;}
	void SetAngle(double angle){_angle = angle;}
	double GetAngle() const{return _angle;}
	void SetDeletable(bool deletable){_deletable = deletable;}
	bool GetDeletable() const{return _deletable;}
	void SetDrawParam(int p){
		if(p <0 ){p=0;}
		else if(p > 255){p=255;}
		_drawParam = p;
	}
	int GetDrawParam() const{return _drawParam;}
	void SetDrawExp(double exp){_drawExp = exp;}
	double GetDrawExp() const{return _drawExp;}

	void SetMoveFunc(void (*MoveFunc)(Particle& Obj)){_MoveFunc=MoveFunc;}

	void SetVxVyFromSpeedAngle(){
		SetVxVy(MyMath::cos(_angle) * _speed, MyMath::sin(_angle) * _speed);
	}

	//コンストラクタ
	Particle(){
		SetAngle(0);
		SetSpeed(0);
		SetVxVyFromSpeedAngle();
	}

	//生成する
	virtual void Create(double x, double y, //誕生時の座標
		        double angle, double speed, //誕生時の角度と速度
				int image, //画像
				void (*BornFunc)(Particle& Obj) = NULL, //誕生エフェクト
				void (*MoveFunc)(Particle& Obj) = NULL, //動作スクリプト
				void (*DeadFunc)(Particle& Obj) = NULL, //死亡エフェクト
				double drawExp = 1.0, int drawParam = 255, bool deletable = true);
	virtual void Move();
	virtual void Draw();

	//徐々に薄くなる(消えるわけではない)
	static void DecleaseParam(Particle& Obj){
		if(Obj.GetDrawParam() > 32){
			Obj.SetDrawParam(Obj.GetDrawParam()-5);
		}
	}

	//徐々に薄く小さくなり、消える
	static void Fadeout(Particle& Obj){
		if(Obj.GetDrawParam() > 0){
			Obj.SetDrawParam(Obj.GetDrawParam()-2);
		}else{
			Obj.SetDead(); //不可視になったら存在価値がないので殺す
		}
		Obj.SetDrawExp(Obj.GetDrawExp()*0.99);
	}

	//ゆっくりになる+下に落ちる
	static void GoDownAndFadeout(Particle& Obj){
		Obj.SetVx(Obj.GetVx()*0.99);
		Obj.SetVy(Obj.GetVy()*0.99+0.3);
		Fadeout(Obj);
	}
	//吸い込む感じ
	static void Inprode(Particle& Obj){
		int totalfrm = 30; //吸い込み終わるのにかかるフレーム数
		if(Obj.GetFrame()==1){
			Obj.SetX(Obj.GetX()+MyMath::cos(Obj.GetAngle())*Obj.GetSpeed()*totalfrm);
			Obj.SetY(Obj.GetY()+MyMath::sin(Obj.GetAngle())*Obj.GetSpeed()*totalfrm);
			Obj.SetAngle(Obj.GetAngle()+PI);
			Obj.SetVxVyFromSpeedAngle();
		}else if(Obj.GetFrame()<totalfrm){
			Obj.SetDrawParam((int)(255*Obj.GetFrame()/totalfrm));
		}else{
			Obj.SetDead();
		}
	}
};

class ParticleGroup{
private:
	std::vector<Particle> _unit;
public:
	void SetLimit(int size){_unit.resize(size);}
	int GetLimit() const{return _unit.size();}

	ParticleGroup(){
		SetLimit(100);
	}

	ParticleGroup(int size){
		SetLimit(size);
	}

	int imgCommon;

	virtual int Create(double x, double y, //誕生時の座標
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
	virtual int Move();
	virtual void Draw();
};

#endif 