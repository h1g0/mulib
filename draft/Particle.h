#ifndef __PARTICLE_H
#define __PARTICLE_H
#include "Object.h"
class Particle: public Object{
private:
	double _angle; //�p�x
	double _speed; //���x
	bool _deletable; //��ʊO�ɏo����������H
	int _drawParam; //�`�掞�̃p�����[�^
	double _drawExp; //�`�掞�̊g�嗦

	void (*_BornFunc)(Particle& Obj); //�a�����G�t�F�N�g
	void (*_MoveFunc)(Particle& Obj); //�������w�肷��|�C���^
	void (*_DeadFunc)(Particle& Obj); //���񂾎��̃G�t�F�N�g
public:

	//�Z�b�^�E�Q�b�^
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

	//�R���X�g���N�^
	Particle(){
		SetAngle(0);
		SetSpeed(0);
		SetVxVyFromSpeedAngle();
	}

	//��������
	virtual void Create(double x, double y, //�a�����̍��W
		        double angle, double speed, //�a�����̊p�x�Ƒ��x
				int image, //�摜
				void (*BornFunc)(Particle& Obj) = NULL, //�a���G�t�F�N�g
				void (*MoveFunc)(Particle& Obj) = NULL, //����X�N���v�g
				void (*DeadFunc)(Particle& Obj) = NULL, //���S�G�t�F�N�g
				double drawExp = 1.0, int drawParam = 255, bool deletable = true);
	virtual void Move();
	virtual void Draw();

	//���X�ɔ����Ȃ�(������킯�ł͂Ȃ�)
	static void DecleaseParam(Particle& Obj){
		if(Obj.GetDrawParam() > 32){
			Obj.SetDrawParam(Obj.GetDrawParam()-5);
		}
	}

	//���X�ɔ����������Ȃ�A������
	static void Fadeout(Particle& Obj){
		if(Obj.GetDrawParam() > 0){
			Obj.SetDrawParam(Obj.GetDrawParam()-2);
		}else{
			Obj.SetDead(); //�s���ɂȂ����瑶�݉��l���Ȃ��̂ŎE��
		}
		Obj.SetDrawExp(Obj.GetDrawExp()*0.99);
	}

	//�������ɂȂ�+���ɗ�����
	static void GoDownAndFadeout(Particle& Obj){
		Obj.SetVx(Obj.GetVx()*0.99);
		Obj.SetVy(Obj.GetVy()*0.99+0.3);
		Fadeout(Obj);
	}
	//�z�����ފ���
	static void Inprode(Particle& Obj){
		int totalfrm = 30; //�z�����ݏI���̂ɂ�����t���[����
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

	virtual int Create(double x, double y, //�a�����̍��W
		    double angle, double speed, //�a�����̊p�x�Ƒ��x
			int image, //�摜
			void (*BornFunc)(Particle& Obj) = NULL, //�a���G�t�F�N�g
			void (*MoveFunc)(Particle& Obj) = NULL, //����X�N���v�g
			void (*DeadFunc)(Particle& Obj) = NULL, //���S�G�t�F�N�g
			double drawExp = 1.0, //�g�嗦
			int drawParam = 255, //�`��p�����[�^
			bool deletable = true //��ʊO�ɔ�яo�������ɏ������邩�H
			);

	//����
	void Delete(int Index);
	//���ׂď���
	void DeleteAll();

	void SetDead(int index);
	void SetDeadAll();

	//�����������B���ݑ��݂���p�[�e�B�N���̐���Ԃ��B
	virtual int Move();
	virtual void Draw();
};

#endif 