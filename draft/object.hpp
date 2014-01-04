#ifndef __OBJECT_HPP
#define __OBJECT_HPP
#include "vector.hpp"
/*############################
 *
 *オブジェクトクラス
 *
 *#########################*/
template <typename Pos = double, typename Vel = double>
class Object{
public:
	//状態を示すEnumerator(いにゅめれーたーって発音しにくいよね！)
	enum Status{
		NotExist,
		Born,
		Exist,
		Dead
	};

private:
	Status _status; //状態
	Vec2D<Pos> _pos; //座標
	Vec2D<Vel> _v; //ベロシティ
	int _frame; //フレーム
	int _image; //画像
public:
	//コンストラクタ
	Object():
	_status(NotExist),
	_pos(0),
	_v(0),
	_frame(0),
	_image(0)
	{}

	Object(Status state, Pos x, Pos y, int image, Vel vx = 0, Vel vy = 0):
	_status(state),
	_pos(x,y),
	_v(vx,vy),
	_frame(0),
	_image(image)
	{}

	//セッタ・ゲッタ
	void SetStatus(Status status){
		_frame = 0;
		_status = status;
	}
	Status status() const{ return _status; }
	void SetPosX(Pos x){ _pos.SetX(x); }
	void SetPosY(Pos y){ _pos.SetY(y); }
	void SetPos(const Vec2D<Pos>& pos){
		_pos = pos;
	}
	Vec2D<Pos> pos() const{
		return _pos;
	}


	void SetVx(Vel vx){ _v.SetX(vx); }
	void SetVy(Vel vy){ _v.SetY(vy); }
	void SetV(const Vec2D<Vel>& v){
		_v = v;
	}
	Vec2D<Vel> v() const{
		return _v;
	}

	void AddVxVy2XY(){
		_pos.AddX(_v.x());
		_pos.AddY(_v.y());
	}

	void AddFrame(){
		SetFrame(frame()+1);
	}
	void SetFrame(int frame){
		//オーバーフロー防止のため、30分を超えたら1に戻る
		if(frame > 0 && frame <= 18000){
			_frame = frame;
		}else if(frame>18000){
			_frame = 1;
		}else{
			_frame = 0;
		}
	}
	int frame() const{return _frame;}

	void SetImage(int imageHandle){
		_image = imageHandle;
	}
	int image() const{
		return _image;
	}

	//発生させる
	virtual void SetBorn(){
		SetStatus(Born);
	}

	//死んだ状態にする
	virtual void SetDead(){
		SetStatus(Dead);
	}

	virtual void Move();
	virtual void Draw();

};
#endif
