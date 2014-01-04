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
	//名前表示欄を保持するクラス
	class Name{
	public:
		std::string text; //本体
		std::string prefix; //接頭辞(「【」などを想定)
		std::string suffix; //接尾辞(「】」などを想定)
		Vec2D<int> startPoint; //描画開始位置

		void Init(){
			text.clear();
			prefix.clear();
			suffix.clear();
			startPoint.Init();
		}
		Name(){Init();}

		std::string Get(){
			if(text.empty()){ //本体が空の時は空白を返す
				return "";
			}else{ //それ以外は接頭辞+本体+接尾辞
				return prefix + text + suffix;
			}
		}
		void Draw();
	};

private:
	
	int _waitFrm; //待つフレーム(-1でキーが押されるまで待機を示す)
	//スクリプト本体(一行ごとに分割されて格納される)
	std::deque<std::string> _script;
	bool _newTxt; //新しい文章か？
	bool _bring2End; //文章を最後まで持っていくか？
	bool _eof; //最後まで消費したか？
	enum ParserStatus{
		Continue,
		Wait,
		Eof
	};
	ParserStatus _Parse(); //queue のfront をパースして適用しpop
	ParserStatus _ParseCommand(const std::string& str); //命令をパースして適用
	ParserStatus _ParseName(const std::string& str); //名前変更命令をパースして適用
public:
	StringDrawer mainText; //メインとして描画されるテキスト
	Name nameText; //名前として描画されるテキスト

	void Init();
	NovelFrame();

	//スクリプトをロードし、1行ごとに分割して_scriptに入れる
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
