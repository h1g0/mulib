#ifndef IMAGE_HPP
#define IMAGE_HPP
#include <string>
#include <vector>
#include <array>
#include <boost/utility.hpp>
#include "inter_lib.hpp"
#include "vector.hpp"

/*!画像を保持するクラス
 *
 *保持される情報はハンドル(inter_lib.hppで定義されたImgHolder)
 *とサイズ(Vec2D)のみ
 *コピー不可(デストラクタで画像自体をdeleteするため)
 */
namespace mulib{
class Image: boost::noncopyable{
	static const ImgHolder unloaded = -1; //ロードされていない
	ImgHolder _hdl;
	Vec2D _size;
	int _counter;
	inline bool _CheckLoaded(){
		if(_hdl == unloaded){return false;}
	}
public:
	void Init();
	//画像1枚をロード
	void Load(const ImgHolder& imghdl);  //ハンドルから生成
	void Load(const std::string& path); //画像パスから生成
	void Unload();

 Image(){Init();}
 Image(const std::string& path){Load(path);}
 Image(const ImgHolder& imghdl){
	 if(imghdl > 0){
		 Load(imghdl);
	 }else{
		 Init();
		}
	}

	~Image(){Unload();}
	ImgHolder handle()const{return _hdl;} //ハンドルを返す
	ImgHolder operator ()()const{
		return handle();
	}

	const Vec2D& size()const{
		 return _size;
	}
};

/*
未完成。後で書く
*/
/*
class Animation{
	std::vector<Image> _imgs;
	int _nowFrm; //現在のフレーム数
	int _waitFrm; //待つべきフレーム数
	int _nowIdx;
 public:

	//アニメーション用の画像を受け渡しするクラス
	class ImgInfo{
		std::array<int,2> _size;
		std::array<int,2> _num;
		int _allNum;
	public:
		ImgInfo(std::array<int, 2> size,
			std::array<int, 2> num,
			int allNum){
			_size = size;
			_num = num;
			_allNum = allNum;
		}

		ImgInfo(std::array<int, 2> size,
			std::array<int, 2> num){
			ImgInfo(size,num, num[0] * num[1]);
		}

	};

Animation():
	_nowFrm(0),_waitFrm(0),_nowIdx(0){_imgs.clear();}
};
*/
}
#endif
