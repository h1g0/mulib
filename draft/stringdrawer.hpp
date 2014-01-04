#ifndef STRINGDRAWER_H
#define STRINGDRAWER_H
#include <string>
#include <vector>
#include "vector.hpp"
/*
  文字列を描画するクラス。
  右端に行ったら自動で改行されたりとか、文字色/大きさの変更ができたりとか
  一文字づつ処理を加えながら描画するので基本的に普通に描画するより重いです
*/
namespace mulib{
class StringDrawer{
  static const int defaultDefaultSize=20; //デフォルトのデフォルト文字サイズ
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

 Vec2D<int> _startPoint; //描画を開始する点
 Vec2D<int> _endPoint; //描画を終了する点
 //Vec2D<int> _drawArea; //描画領域
 int _defaultSize; //デフォルトの大きさ
 Color _defaultColor; //デフォルトの文字色
 int _waitFrm; //何フレーム待つか？-1で全てを一気に描画
 int _lineSpace;

 std::vector<StrBlock> _strVect; //過去に描画した文字列を行ごと・種類ごとまとめる(高速化)

 std::wstring _str; //現在描画している文字列
 int _strCounter; //現在何文字目か
 int _nowSize; //現在の文字サイズ
 int _nowFrm; //現在のフレーム
 int _lineMaxSize; //現在の行の中で最大の文字サイズ
 Color _nowColor;
 Vec2D<int> _nowPoint; //現在描画している位置

 bool _doneFlg; //文字列の最後まで描画したか？

 void _ParseOperator(const std::string& opr); //制御文字列を解析し反映

 //文字列を描画する。引数には何文字目かを渡す。この値は変更される。
 //戻り値は0: 成功 -1: 失敗
 int _DrawChar(int& counter);

 //改行する
void _NewLine(){
	 _nowPoint.Set(_startPoint.x(), _nowPoint.y() + _lineMaxSize + _lineSpace);
	 _lineMaxSize = _nowSize;
	 _strVect.push_back(StrBlock(_nowPoint,_nowColor,_nowSize)); //描画位置を強制指定
 }

void _ChangeColor(int rot, int gruen, int blau){
	_nowColor.Set(rot,gruen,blau);
	 _strVect.push_back(StrBlock(_nowPoint,_nowColor,_nowSize)); //描画位置指定を最初の文字に託す
}

 //描画サイズを変更する
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
