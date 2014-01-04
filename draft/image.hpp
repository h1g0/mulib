#ifndef IMAGE_HPP
#define IMAGE_HPP
#include <string>
#include <vector>
#include <array>
#include <boost/utility.hpp>
#include "inter_lib.hpp"
#include "vector.hpp"

/*!�摜��ێ�����N���X
 *
 *�ێ��������̓n���h��(inter_lib.hpp�Œ�`���ꂽImgHolder)
 *�ƃT�C�Y(Vec2D)�̂�
 *�R�s�[�s��(�f�X�g���N�^�ŉ摜���̂�delete���邽��)
 */
namespace mulib{
class Image: boost::noncopyable{
	static const ImgHolder unloaded = -1; //���[�h����Ă��Ȃ�
	ImgHolder _hdl;
	Vec2D _size;
	int _counter;
	inline bool _CheckLoaded(){
		if(_hdl == unloaded){return false;}
	}
public:
	void Init();
	//�摜1�������[�h
	void Load(const ImgHolder& imghdl);  //�n���h�����琶��
	void Load(const std::string& path); //�摜�p�X���琶��
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
	ImgHolder handle()const{return _hdl;} //�n���h����Ԃ�
	ImgHolder operator ()()const{
		return handle();
	}

	const Vec2D& size()const{
		 return _size;
	}
};

/*
�������B��ŏ���
*/
/*
class Animation{
	std::vector<Image> _imgs;
	int _nowFrm; //���݂̃t���[����
	int _waitFrm; //�҂ׂ��t���[����
	int _nowIdx;
 public:

	//�A�j���[�V�����p�̉摜���󂯓n������N���X
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
