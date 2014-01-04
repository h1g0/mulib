#include "stringdrawer.hpp"
#include <DxLib.h>
#include <boost/xpressive/xpressive.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdlib>

/*
http://d.hatena.ne.jp/kryozahiro/20080809/1218295912
���؂�܂���
*/
//���C�h�����񂩂�}���`�o�C�g������
//���P�[���ˑ�
void mulib::StringDrawer::WStringToString(const std::wstring &src, std::string &dest) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	dest = mbs;
	delete [] mbs;
}

//�}���`�o�C�g�����񂩂烏�C�h������
//���P�[���ˑ�
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
	//_strVect.push_back(StrBlock()); //�`��ʒu�w����ŏ��̕����ɑ���
}

mulib::StringDrawer::StringDrawer(){
	InitAll();
}

//���䕶�������͂����f
void mulib::StringDrawer::_ParseOperator(const std::string& opr){
	using namespace boost::xpressive;
	//�F�ύX
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

	//���s
	rex = icase("br");
	if(regex_search(opr, match, rex)){
		_NewLine();
		return;
	}
	//�X�s�[�h�ύX
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
	//�T�C�Y�ύX
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

//�������`�悷��B�����ɂ͉������ڂ���n���B���̒l�͕ύX�����B
//�߂�l��0: ���� -1: ���s
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
	int r=0; //���ʂ̈ꎞ�ۊ�
	if(_waitFrm>=0 || _doneFlg){ //���ʂ���C�ɕ\���Ȃ炠�炩���߃p�[�T�ɒʂ����߂ɕ`�悵�Ȃ�
		//TODO: DxLib���b�p�֐������H
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