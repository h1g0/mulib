//FPS制御
#include "fps.hpp"
#include "inter_lib.hpp"
namespace mulib{
using namespace interlib;
/*==============================================================================
 *FPSを強制的に固定する
==============================================================================*/
void FpsManager::Fix(){
	//フラグが立っていなければ抜ける
	if(_fixFPS == false){return;}

	int nowTime = GetNowCount(); //今回の時間
	static int oldTime = 0; //前回の時間
	int difTime = nowTime - oldTime;	//差分

	for(;;){ //規定時間経過までループ
		if(difTime >= _frameTime){ //規定時間を経過？
			oldTime = nowTime;	//今回の時間を保存して終了
			return;
		}else{ //規定時間を経過していなければ待機
			_WaitTimer(1); //1ms待つ
			nowTime = _GetNowCount(); //現在の時間と差分を更新
			difTime = nowTime - oldTime;
		}
	}
}

/*==============================================================================
 * FPSを取得する
==============================================================================*/
int FpsManager::Get(){
	//FPSを表示しない？
	if(_showFPS == false){return -1;}

	static int oldTime = _GetNowCount(); //前回の時間
	static int count = 0;  //カウンタ
	static int fps = 0;
	int nowTime = _GetNowCount();
	count++;
	//1000ms過ぎたか？
	if((nowTime - oldTime) >= 1000){
		int differ = ((nowTime - oldTime) - 1000); //誤差差分
		//前回の時間を更新
		oldTime = nowTime - differ;	//誤差修正
		//FPSを保存
		fps = count;
		//カウンタを初期化
		count = 0;
	}
	return fps;
}
}
