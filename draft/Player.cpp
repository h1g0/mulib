#include "Player.h"

//移動量決定
void PlayerShip::SetSpeed(int speed){
	_speed = speed;
	double angle = 45 * RAD;
	for(int i=0;i<8;i++){
		_vx[i] = MyMath::cos(angle * i) * _speed;
		_vy[i] = MyMath::sin(angle * i) * _speed;
	}
}

void PlayerShip::Move(){
	switch(GetState()){
	case Born:
		break;
	case Exist:
		if(_move){
			MoveByHand();
		}
		DecreaseInvincibleFrame(); //無敵だったら無敵時間を減少
		break;
	case Dead:

		break;
	}
	AddFrame();
}

void PlayerShip::MoveByHand(){
		bool u=false; //上キーが押されているかのフラグ
		bool d=false; //下キー
		bool l=false; //左キー
		bool r=false; //右キー

		bool slow = false;

		//実際に動かす移動量
		double vx = 0;
		double vy = 0;

		//上下キー判定(上下が同時に押されることは想定しない)
		if((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_UP) != 0){
			u=true;
		}else if((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_DOWN) != 0){
			d=true;
		}

		//左右キー判定(左右が同時に押されることは想定しない)
		if((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT) != 0){
			l=true;
		}else if((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_RIGHT) != 0){
			r=true;
		}

		//移動が遅いか否か
		if((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_2) != 0){
			slow = true;
		}else{
			slow = false;
		}

		//移動量決定
		if(d){ //下が押されている
			if(l){
				vx = _vx[3];
				vy = _vy[3];
			}else if(r){
				vx = _vx[1];
				vy = _vy[1];
			}else{
				vx = _vx[2];
				vy = _vy[2];
			}
		}else if(u){ //上が押されている
			if(l){
				vx = _vx[5];
				vy = _vy[5];
			}else if(r){
				vx = _vx[7];
				vy = _vy[7];
			}else{
				vx = _vx[6];
				vy = _vy[6];
			}
		}else{
			if(l){
				vx = _vx[4];
				vy = _vy[4];
			}else if(r){
				vx = _vx[0];
				vy = _vy[0];
			}
		}
		if(slow){
			vx *=0.5;
			vy*=0.5;
		}
		double x = GetX() + vx;
		double y = GetY() + vy;
		Fix2Screen(x,y,16,16); //画面内に収まるように補正
		SetXY(x,y);

		bool tmpShot=false;
		//ショットキー判定
		if((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_1) != 0){
			tmpShot = true;
			if(slow){
				_NarrowShotFunc(*this);
			}else{
				_ShotFunc(*this);
			}
		}
		_isShooting[1] = _isShooting[0]; //現在のフレームを過去のフレームに
		_isShooting[1] = tmpShot; //現在のフレームを反映
}
void PlayerShip::Draw(){
	switch(GetState()){
	case Born:
		_BornDrawFunc(*this);
		break;
	case Exist:
		_ExistDrawFunc(*this);
		break;
	case Dead:
		_DeadDrawFunc(*this);
		break;
	}
}

//ノーマルショット
void PlayerShip::ShotNormal(PlayerShip& Obj){
	if(Obj.GetFrame()%2 != 0){return;}
	double diffAngle =5 + MyMath::sin((Obj.GetFrame() % 30) * 12* RAD)*10;
	Obj.BltGrp.Create(Obj.GetX()-16,Obj.GetY(),(270-diffAngle) * RAD, 8, Obj.BltGrp.imgCommon,NULL,&PlayerBullet::DecleaseParam,NULL,2.0,127);
	Obj.BltGrp.Create(Obj.GetX(),Obj.GetY(),270 * RAD, 16, Obj.BltGrp.imgCommon,NULL,&PlayerBullet::DecleaseParam,NULL,2.0,127);
	Obj.BltGrp.Create(Obj.GetX()+16,Obj.GetY(),(270+diffAngle) * RAD, 8, Obj.BltGrp.imgCommon,NULL,&PlayerBullet::DecleaseParam,NULL,2.0,127);
}

//狭いショット
void PlayerShip::ShotNarrow(PlayerShip& Obj){
	if(Obj.GetFrame()%2 != 0){return;}
	double diffAngle = MyMath::sin((Obj.GetFrame() % 60) * 6* RAD)*16;
	Obj.BltGrp.Create(Obj.GetX()-diffAngle,Obj.GetY(),270 * RAD, 16, Obj.BltGrp.imgCommon,NULL,&PlayerBullet::DecleaseParam,NULL,2.0,127);
	Obj.BltGrp.Create(Obj.GetX(),Obj.GetY(),270 * RAD, 16, Obj.BltGrp.imgCommon,NULL,&PlayerBullet::DecleaseParam,NULL,2.0,127);
	Obj.BltGrp.Create(Obj.GetX()+diffAngle,Obj.GetY(),270 * RAD, 16, Obj.BltGrp.imgCommon,NULL,&PlayerBullet::DecleaseParam,NULL,2.0,127);
}

//通常描画
void PlayerShip::ExistNormal(PlayerShip& Obj){
	Obj.DrawInvincibleImage(); //無敵画像描画

	SetDrawBlendMode(DX_BLENDMODE_ALPHA,255);
	DrawRotaGraph((int)Obj.GetX(), (int)Obj.GetY(), 1.0, Obj.GetAngle(), Obj.GetImage(),TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	//中心点描画
	int param = (int)(191+MyMath::sin((Obj.GetFrame() % 60) * 6* RAD)*64);
	SetDrawBlendMode(DX_BLENDMODE_INVSRC,param);
	DrawRotaGraph((int)Obj.GetX(), (int)Obj.GetY(), 0.3, Obj.GetAngle(), Obj.Particle.imgCommon,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	//パーティクル発生
	Obj.Particle.Create(Obj.GetX()-8+GetRand(160)*0.1,Obj.GetY()+GetRand(320)*0.1,
		                (75+GetRand(300)*0.1)*RAD,5,Obj.Particle.imgCommon,NULL,&Particle::Fadeout,NULL,1.0,64);
}

//誕生エフェクト
void PlayerShip::BornNormal(PlayerShip& Obj){
	if(Obj.GetFrame() ==1){
		Obj.Particle.DeleteAll();
		for(int i=0;i<90;i++){
			Obj.Particle.Create(Obj.GetX(),Obj.GetY(),i*4*RAD,GetRand(100)*0.1+5,Obj.Particle.imgCommon,NULL,&Particle::Inprode,NULL,0.5,0,false);
		}
	}
	if(Obj.GetFrame() == 30){
		Obj.SetState(Exist);
	}
}
//死んだ時のエフェクト
void PlayerShip::DeadNormal(PlayerShip& Obj){
	if(Obj.GetFrame() ==1){
		Obj.Particle.DeleteAll();
		for(int i=0;i<90;i++){
			Obj.Particle.Create(Obj.GetX(),Obj.GetY(),i*4*RAD,
				                GetRand(100)*0.1+5,Obj.Particle.imgCommon,
								NULL,&Particle::GoDownAndFadeout,NULL,1.0,128);
		}
	}
	if(Obj.GetFrame() == 60){
		Obj.Particle.DeleteAll();
		Obj.SetBorn();
	}
}