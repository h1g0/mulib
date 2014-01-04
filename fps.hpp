#ifndef FPS_HPP
#define FPS_HPP

namespace mulib{
/*#############################################################################
	FPS固定・算出クラス
===============================================================================
	使い方:
	メインループ開始前に宣言する(まぁ当たり前だね)
	↓
	FPSを描画するタイミングでGet()を呼ぶ(メインループのなるべく最後の方が好ましい)
	↓
	Fix()で固定する
#############################################################################*/

//FPS設定
class FpsManager{
	bool _fixFPS; //FPSを固定するか？
	bool _showFPS; //FPSを表示するか？
	int _defaultFPS; //デフォルトのFPS
	int _frameTime; //1フレームの時間(ミリ秒)
public:
	/*コンストラクタ
	引数: fixFps: FPSを固定するか？
				showFps: FPSを表示するか？
				fixFpsNum: FPSを幾つに固定するか？
	*/
	FpsManager(bool fixFps = true, bool showFps = true, int defaultFps = 60){
		SetFixFPS(fixFps);
		SetShowFPS(showFps);
		SetDefaultFPS(defaultFps);
	}
	//FPSを固定する
	void Fix();
	//FPSを返す(返り値: FPS)
	int Get();

	//セッタ
	void SetFixFPS(bool fixFps){_fixFPS = fixFps;}
	void SetShowFPS(bool showFps){_showFPS = showFps;}
	void SetDefaultFPS(int defaultFps){
		if(_defaultFPS > 0 && _defaultFPS <= 1000){
			_defaultFPS = defaultFps;
		}else{
			_defaultFPS = 60;
		}
		_frameTime = static_cast<int>(1000 / _defaultFPS);
	}

	//ゲッタ
	bool fixFPS()const{return _fixFPS;}
	bool showFPS()const{return _showFPS;}
	int defaultFPS()const{return _defaultFPS;}
};
} //namespace mulib

#endif
