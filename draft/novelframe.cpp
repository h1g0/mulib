#include <DxLib.h>

#include "novelframe.hpp"
#include <fstream>
#include <boost/xpressive/xpressive.hpp>
#include <boost/lexical_cast.hpp>

namespace mulib{
void NovelFrame::Name::Draw(){
	DrawString(startPoint.x(),startPoint.y(),
						 Get().c_str(),GetColor(255,255,255));
}

void NovelFrame::Init(){
	_waitFrm = 0;
	_script.clear();
	_newTxt = true;
	_eof=false;
	_bring2End = false;
	mainText.InitAll();
	nameText.Init();
}

NovelFrame::NovelFrame(){
	Init();
}

//構文を解析しアクションを起こす
NovelFrame::ParserStatus NovelFrame::_Parse(){
	if(_script.empty()){
		_eof = true;
		return ParserStatus::Eof;
	}
	ParserStatus result = ParserStatus::Continue;
	if(!_script.front().empty()){

		char token = _script.front().at(0);
		std::wstring dst;
		switch(token){
		case '/': //コメント
			break;
		case '#': //命令
			result = _ParseCommand(_script.front().substr(1));
			break;
		case '@': //名前変更
			result = _ParseName(_script.front().substr(1));
			break;
		default: //文字列追加
			StringDrawer::StringToWString(_script.front(),dst);
			if(!_newTxt){
				mainText.AddStr(dst,!_newTxt);
			}else{
				mainText.SetStr(dst);
				_newTxt = false;
			}
			break;
		}
	}

	_script.pop_front();
	return result;
}

NovelFrame::ParserStatus NovelFrame::_ParseCommand(const std::string& str){  //命令をパースして適用
	using namespace boost::xpressive;
	//待つ
	sregex rex = icase("wait");
	smatch match;
	if(regex_search(str, match, rex)){
		rex = (s1=+_d);
		if(regex_search(str, match, rex)){
			_waitFrm = boost::lexical_cast<int>(match[1]);
			return ParserStatus::Wait;
		}else{
			_waitFrm = -1;
			return ParserStatus::Wait;
		}
	}

	//Set
	rex = icase("set");
	if(regex_search(str, match, rex)){
		rex = icase("nametextpos") >> -*_s >>  '(' >>  -*_s >> (s1=+_d) >> -*_s >> ',' >> -*_s >> (s2=+_d) >> -*_s >> ')';
		if(regex_search(str, match, rex)){
			nameText.startPoint = Vec2D<int>(boost::lexical_cast<int>(match[1]), boost::lexical_cast<int>(match[2]));
			return ParserStatus::Continue;
		}
		rex = icase("nametextprefix") >> -*_s >>  '(' >> (s1=+~_n) >> ')';
		if(regex_search(str, match, rex)){
			nameText.prefix = match[1];
			return ParserStatus::Continue;
		}
		rex = icase("nametextsuffix") >> -*_s >>  as_xpr('(') >> (s1=+~_n) >> as_xpr(')');
		if(regex_search(str, match, rex)){
			nameText.suffix = match[1];
			return ParserStatus::Continue;
		}

		rex = icase("maintextpos") >> -*_s >> '(' >>  -*_s >> (s1=+_d) >> -*_s >> ',' >> -*_s >> (s2=+_d) >> -*_s >> ')'>> -*_s >>
			'(' >>  -*_s >> (s3=+_d) >> -*_s >> ',' >> -*_s >> (s4=+_d) >> -*_s >> ')';
		if(regex_search(str, match, rex)){
			mainText.SetArea(Vec2D<int>(boost::lexical_cast<int>(match[1]), boost::lexical_cast<int>(match[2])),
				Vec2D<int>(boost::lexical_cast<int>(match[3]), boost::lexical_cast<int>(match[4])));

			return ParserStatus::Continue;
		}

	}

	return ParserStatus::Continue; //どれにも引っかからなかった場合

}
NovelFrame::ParserStatus NovelFrame::_ParseName(const std::string& str){//名前変更命令をパースして適用
	nameText.text = str;
	return ParserStatus::Continue;
} 



//ファイルをロードし行ごとに_scriptに格納
void NovelFrame::LoadScript(const std::string &fileName){
	std::ifstream ifs(fileName.c_str());
	_script.clear();
	while(!ifs.eof()){
		_script.push_back("");
		getline(ifs, _script.back());
	}
	_eof = false;
}

void NovelFrame::Move(KeyStatus key){
	if(key == KeyStatus::Decide){
		if(mainText.doneFlg()){
			_waitFrm = 0;
			mainText.SetStr(L"");
			_newTxt = true;
		}else{ //最後まで一気に表示する処理
			_bring2End = true;

		}
	}
	mainText.Move();

	while(_waitFrm == 0 &&
				_Parse() == ParserStatus::Continue);

}

void NovelFrame::Draw(){
	nameText.Draw();
	if(_bring2End){
		while(!mainText.doneFlg()){
			mainText.Move();
			mainText.Draw();
		}
		_bring2End = false;
	}else{
		mainText.Draw();
	}
	if(mainText.doneFlg()){
		if(_eof && _newTxt){
			DrawString(mainText.nowPoint().x(),mainText.nowPoint().y(),"### End Of File Detected! ###",GetColor(255,127,127));
		}else{
			DrawString(mainText.nowPoint().x(),mainText.nowPoint().y(),"▼",GetColor(255,255,255));
		}
	}
}


} //end of namespace mulib
