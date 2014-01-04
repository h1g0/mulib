#ifndef __OBJECT_HPP
#define __OBJECT_HPP
#include "vector.hpp"
/*############################
 *
 *�I�u�W�F�N�g�N���X
 *
 *#########################*/
template <typename Pos = double, typename Vel = double>
class Object{
public:
	//��Ԃ�����Enumerator(���ɂ�߂�[���[���Ĕ������ɂ�����ˁI)
	enum Status{
		NotExist,
		Born,
		Exist,
		Dead
	};

private:
	Status _status; //���
	Vec2D<Pos> _pos; //���W
	Vec2D<Vel> _v; //�x���V�e�B
	int _frame; //�t���[��
	int _image; //�摜
public:
	//�R���X�g���N�^
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

	//�Z�b�^�E�Q�b�^
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
		//�I�[�o�[�t���[�h�~�̂��߁A30���𒴂�����1�ɖ߂�
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

	//����������
	virtual void SetBorn(){
		SetStatus(Born);
	}

	//���񂾏�Ԃɂ���
	virtual void SetDead(){
		SetStatus(Dead);
	}

	virtual void Move();
	virtual void Draw();

};
#endif
