#ifndef NOVELFRAME_HPP
#define NOVELFRAME_HPP
#include "stringdrawer.hpp"
#include <string>
#include <vector>
#include <deque>
#include <boost/unordered_map.hpp>
#include <boost/any.hpp>
namespace mulib{

class NovelFrame{
public:
	//���O�\������ێ�����N���X
	class Name{
	public:
		std::string text; //�{��
		std::string prefix; //�ړ���(�u�y�v�Ȃǂ�z��)
		std::string suffix; //�ڔ���(�u�z�v�Ȃǂ�z��)
		Vec2D<int> startPoint; //�`��J�n�ʒu

		void Init(){
			text.clear();
			prefix.clear();
			suffix.clear();
			startPoint.Init();
		}
		Name(){Init();}

		std::string Get(){
			if(text.empty()){ //�{�̂���̎��͋󔒂�Ԃ�
				return "";
			}else{ //����ȊO�͐ړ���+�{��+�ڔ���
				return prefix + text + suffix;
			}
		}
		void Draw();
	};

private:
	
	int _waitFrm; //�҂t���[��(-1�ŃL�[���������܂őҋ@������)
	//�X�N���v�g�{��(��s���Ƃɕ�������Ċi�[�����)
	std::deque<std::string> _script;
	bool _newTxt; //�V�������͂��H
	bool _bring2End; //���͂��Ō�܂Ŏ����Ă������H
	bool _eof; //�Ō�܂ŏ�������H
	enum ParserStatus{
		Continue,
		Wait,
		Eof
	};
	ParserStatus _Parse(); //queue ��front ���p�[�X���ēK�p��pop
	ParserStatus _ParseCommand(const std::string& str); //���߂��p�[�X���ēK�p
	ParserStatus _ParseName(const std::string& str); //���O�ύX���߂��p�[�X���ēK�p
public:
	StringDrawer mainText; //���C���Ƃ��ĕ`�悳���e�L�X�g
	Name nameText; //���O�Ƃ��ĕ`�悳���e�L�X�g

	void Init();
	NovelFrame();

	//�X�N���v�g�����[�h���A1�s���Ƃɕ�������_script�ɓ����
	void LoadScript(const std::string& fileName);

	enum KeyStatus{
		None,
		Decide
	};
	void Move(KeyStatus key);
	void Draw();
};
} //end of namespace mulib

#endif // NOVELFRAME_HPP
