#ifndef STRINGDRAWER_H
#define STRINGDRAWER_H
#include <string>
#include <vector>
#include "vector.hpp"
/*
  �������`�悷��N���X�B
  �E�[�ɍs�����玩���ŉ��s���ꂽ��Ƃ��A�����F/�傫���̕ύX���ł�����Ƃ�
  �ꕶ���Â����������Ȃ���`�悷��̂Ŋ�{�I�ɕ��ʂɕ`�悷����d���ł�
*/
namespace mulib{
class StringDrawer{
  static const int defaultDefaultSize=20; //�f�t�H���g�̃f�t�H���g�����T�C�Y
	struct Color{
		int r, g, b;
		void Set(int rot, int gruen, int blau){
			r=rot;
			g=gruen;
			b=blau;
		}
	};
	class StrBlock{
	public:
		std::string str;
		Color col;
		int size;
		Vec2D<int> pos;
		bool hasStr;
		StrBlock(){
			str.clear();
			size=0;
			pos.Set(0);
			hasStr=false;
		}
		StrBlock(Vec2D<int> point, Color color, int fontsize){
			str.clear();
			size= fontsize;
			pos = point;
			col = color;
			hasStr=true;
		}
	};

 Vec2D<int> _startPoint; //�`����J�n����_
 Vec2D<int> _endPoint; //�`����I������_
 //Vec2D<int> _drawArea; //�`��̈�
 int _defaultSize; //�f�t�H���g�̑傫��
 Color _defaultColor; //�f�t�H���g�̕����F
 int _waitFrm; //���t���[���҂��H-1�őS�Ă���C�ɕ`��
 int _lineSpace;

 std::vector<StrBlock> _strVect; //�ߋ��ɕ`�悵����������s���ƁE��ނ��Ƃ܂Ƃ߂�(������)

 std::wstring _str; //���ݕ`�悵�Ă��镶����
 int _strCounter; //���݉������ڂ�
 int _nowSize; //���݂̕����T�C�Y
 int _nowFrm; //���݂̃t���[��
 int _lineMaxSize; //���݂̍s�̒��ōő�̕����T�C�Y
 Color _nowColor;
 Vec2D<int> _nowPoint; //���ݕ`�悵�Ă���ʒu

 bool _doneFlg; //������̍Ō�܂ŕ`�悵�����H

 void _ParseOperator(const std::string& opr); //���䕶�������͂����f

 //�������`�悷��B�����ɂ͉������ڂ���n���B���̒l�͕ύX�����B
 //�߂�l��0: ���� -1: ���s
 int _DrawChar(int& counter);

 //���s����
void _NewLine(){
	 _nowPoint.Set(_startPoint.x(), _nowPoint.y() + _lineMaxSize + _lineSpace);
	 _lineMaxSize = _nowSize;
	 _strVect.push_back(StrBlock(_nowPoint,_nowColor,_nowSize)); //�`��ʒu�������w��
 }

void _ChangeColor(int rot, int gruen, int blau){
	_nowColor.Set(rot,gruen,blau);
	 _strVect.push_back(StrBlock(_nowPoint,_nowColor,_nowSize)); //�`��ʒu�w����ŏ��̕����ɑ���
}

 //�`��T�C�Y��ύX����
 void _ChangeSize(int size);

public:
void Init(){
	 _str.clear();
	 _strCounter=0;
	 _nowSize=_defaultSize;
	 _nowFrm=0;
	 _lineMaxSize=_nowSize;
	 _nowColor=_defaultColor;
	 _nowPoint.Init();
	_strVect.clear();

	 _doneFlg = false;
}
 void InitAll(){
	 _startPoint.Init();
	 _endPoint.Init();
	 _defaultSize = defaultDefaultSize;
	 _defaultColor.Set(255,255,255);
	 _waitFrm = -1;
	 _lineSpace = 5;
	 Init();
 }

 StringDrawer();
 void SetArea(Vec2D<int> start, Vec2D<int> end){
	 _startPoint = start;
	 _endPoint = end;
 }
 void SetDefaultSize(int size){
	 _defaultSize = size;
	 _ChangeSize(size);
 }
 void SetDefaultColor(int r, int g, int b){
	 _defaultColor.Set(r,g,b);
 }
 int waitFrm()const{return _waitFrm;}
 void SetWaitFrm(int frm=-1){
	 if(frm<-1){
		 frm=-1;
	 }
	 _waitFrm = frm;
 }
 void SetLineSpace(int px){
	 _lineSpace = px;
 }

 void SetStr(const std::wstring& str){
	 Init();
	 _nowPoint=_startPoint;
	 _strVect.push_back(StrBlock(_nowPoint,_nowColor,_nowSize));

	 _str = str;

	 if(_waitFrm<0){
		 while(!_doneFlg){
			 Move();
			 Draw();
		 }
	 }
 }
 void AddStr(const std::wstring& str, bool newline = true){
	 if(newline){ _str += std::wstring(L"[br]");}
	 _str += str;
 }

 bool doneFlg()const{return _doneFlg;}
 Vec2D<int> nowPoint()const{return _nowPoint;}

 void Move();
 int Draw();

static void WStringToString(const std::wstring &src, std::string &dest);
static void StringToWString(const std::string &src, std::wstring &dest);
};
}
#endif // STRINGDRAWER_H
