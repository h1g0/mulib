#include "stringdrawer.hpp"
#include <DxLib.h>
#include <boost/xpressive/xpressive.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdlib>

/*
http://d.hatena.ne.jp/kryozahiro/20080809/1218295912
より借りました
*/
//ワイド文字列からマルチバイト文字列
//ロケール依存
void mulib::StringDrawer::WStringToString(const std::wstring &src, std::string &dest) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	dest = mbs;
	delete [] mbs;
}

//マルチバイト文字列からワイド文字列
//ロケール依存
void mulib::StringDrawer::StringToWString(const std::string &src, std::wstring &dest) {
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs(wcs, src.c_str(), src.length() + 1);
	dest = wcs;
	delete [] wcs;
}

void mulib::StringDrawer::_ChangeSize(int size){
	if(size == _nowSize||size<1){return;}
	_nowSize = size;
	if(_lineMaxSize < _nowSize){_lineMaxSize = _nowSize;}
	SetFontSize(size);
	//_strVect.push_back(StrBlock()); //描画位置指定を最初の文字に託す
}

mulib::StringDrawer::StringDrawer(){
	InitAll();
}

//制御文字列を解析し反映
void mulib::StringDrawer::_ParseOperator(const std::string& opr){
	using namespace boost::xpressive;
	//色変更
	sregex rex = icase("color");
	smatch match;
	if(regex_search(opr, match, rex)){
		rex = (s1=+_d) >> ',' >> (s2=+_d) >> ',' >> (s3=+_d);
		if(regex_search(opr, match, rex)){
			_ChangeColor(boost::lexical_cast<int>(match[1]),
				boost::lexical_cast<int>(match[2]),
				boost::lexical_cast<int>(match[3]));
			return;
		}else{
			_ChangeColor(_defaultColor.r,_defaultColor.g,_defaultColor.b);
			return;
		}
	}

	//改行
	rex = icase("br");
	if(regex_search(opr, match, rex)){
		_NewLine();
		return;
	}
	//スピード変更
	rex = icase("speed");
	if(regex_search(opr, match, rex)){
		rex = as_xpr('+') >> (s1=+_d);
		if(regex_search(opr, match, rex)){
			SetWaitFrm(_waitFrm + boost::lexical_cast<int>(match[1]));
			return;
		}
		rex = as_xpr('-') >> (s1=+_d);
		if(regex_search(opr, match, rex)){
			SetWaitFrm(_waitFrm - boost::lexical_cast<int>(match[1]));
			return;
		}
		rex = (s1=+_d);
		if(regex_search(opr, match, rex)){
			SetWaitFrm(boost::lexical_cast<int>(match[1]));
			return;
		}

	}
	//サイズ変更
	rex = icase("size");
	if(regex_search(opr, match, rex)){
		rex = as_xpr('+') >> (s1=+_d);
		if(regex_search(opr, match, rex)){
			_ChangeSize(_nowSize + boost::lexical_cast<int>(match[1]));
			return;
		}
		rex = as_xpr('-') >> (s1=+_d);
		if(regex_search(opr, match, rex)){
			_ChangeSize(_nowSize - boost::lexical_cast<int>(match[1]));
			return;
		}
		rex = (s1=+_d);
		if(regex_search(opr, match, rex)){
			_ChangeSize(boost::lexical_cast<int>(match[1]));
			return;
		}

		_ChangeSize(_defaultSize);
	}
}

//文字列を描画する。引数には何文字目かを渡す。この値は変更される。
//戻り値は0: 成功 -1: 失敗
int mulib::StringDrawer::_DrawChar(int& counter){
	std::string nowChar;
	WStringToString(_str.substr(counter, 1),nowChar);
	if(nowChar == "["){
		int r = _str.find(L"]",counter);
		if(r==-1){return -1;}
		std::string opStr;
		WStringToString(_str.substr(counter, r - counter), opStr);
		_ParseOperator(opStr);
		counter = r+1;
		if(_strCounter<r+1){_strCounter=r+1;}
		return _DrawChar(counter);
	}else{
		int w = GetDrawStringWidth(nowChar.c_str(), nowChar.size());
		if(_nowPoint.x()+w>_endPoint.x()){
			_NewLine();
		}
		if(_nowPoint.y()+_nowSize > _endPoint.y()){
			return -1;
		}else{
			if(_waitFrm>-1){
				DrawString(_nowPoint.x(), _nowPoint.y(),
					   nowChar.c_str(), GetColor(_nowColor.r,_nowColor.g,
					   _nowColor.b));
			}
			return 0;
		}
	}
}

int mulib::StringDrawer::Draw(){
	int result = 0;
	int r=0; //結果の一時保管
	if(_waitFrm>=0 || _doneFlg){ //結果を一気に表示ならあらかじめパーサに通すために描画しない
		//TODO: DxLibラッパ関数を作る？
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		for(auto itr = _strVect.begin(); itr != _strVect.end(); ++itr){
			int r = DrawString(itr->pos.x(),itr->pos.y(),itr->str.c_str(),
				GetColor(itr->col.r,itr->col.g,itr->col.b));
			if(r!=0){result=r;}

		}
	}

	if(_doneFlg){return result;}
	if(_waitFrm>=0){
		int a = (_waitFrm>0)?static_cast<int>(_nowFrm * 255 / static_cast<double>(_waitFrm)):255;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,255-a);
	}
	r = _DrawChar(_strCounter);

	if(_waitFrm>=0){SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);}
	if(r!=0){result=r;}
	return result;
}

void mulib::StringDrawer::Move(){
	 if( _doneFlg){return;}
	 if(_strCounter==_str.size() && _nowFrm <= 0){
		 _doneFlg = true;
	 }else if(_nowFrm<=0){
		 	std::string nowChar;
			WStringToString(_str.substr(_strCounter, 1),nowChar);
			_nowPoint.Set(_nowPoint.x() + GetDrawStringWidth(nowChar.c_str(),nowChar.size()),
				_nowPoint.y());
			if(_nowPoint.y()<_endPoint.y()){_strVect.back().str+=nowChar;}

			if(!_strVect.back().hasStr){
				_strVect.back().hasStr=true;
				_strVect.back().col=_nowColor;
				_strVect.back().pos=_nowPoint;
				_strVect.back().size=_nowSize;
			}

		 _nowFrm=_waitFrm;
		 _strCounter++;
	 }else{
		 _nowFrm--;
	 }
 }