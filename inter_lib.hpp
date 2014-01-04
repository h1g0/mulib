#ifndef INTER_LIB_HPP
#define INTER_LIB_HPP
/*!###########################################################
	**ライブラリ間の差異を吸収する
	*
	*主にゲーム用ライブラリ(DxLibやSDLなど)を使用する際に、
	*ライブラリ間の差異を吸収するためのクッションを挟むことで
	*移植性の向上を図る。
	*開発時にDxLibを使用した関係上、DxLibの仕様に親和性が高い。
	*##########################################################*/

#include <DxLib.h> //使用するライブラリ

#include <string>
#include "vector.hpp"

namespace mulib{
	typedef int ImgHolder; //画像を保持する変数型
	typedef int KeyHolder; //ボタン/キーを保持する変数型
namespace interlib{
	/*!====================================================
	 *LoadImg: 画像をロードする
	 *
	 *string型のファイルパスを取り、取得したImgHolderを返す。
	 *=====================================================*/
	ImgHolder LoadImg(const std::string& file);
	Vec2D<int> GetImgSize(const ImgHolder& imgHdl);
	int UnloadImg(const ImgHolder& imgHdl);

	bool CheckIsKeyPushed(const KeyHolder& handle);

	void _WaitTimer(int mSec){WaitTimer(mSec);}
	int _GetNowCount(){return GetNowCount();}
}}

#endif