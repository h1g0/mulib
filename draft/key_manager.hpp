#ifndef KEY_MANAGER_HPP
#define KEY_MANAGER_HPP
#include "inter_lib.hpp"
#include <fstream>
#include <string>
#include <boost/unordered_map.hpp>
#include <vector>
namespace mulib{
	class KeyManager{

		//各キーの情報を持つクラス
		class _Key{
			unsigned int _frm; //押されているフレーム数。
			KeyHolder _hdl; //識別するハンドル
			std::string _name; //キーの名前
		public:
			void Init(){
				_frm = 0;
				_hdl = 0;
				_name.clear();
			}

			//ハンドルをセット
			void Set(std::string name, KeyHolder handle){
				_name = name;
				_hdl = handle;
				_frm = 0;
			}

			Key(){Init();}
			Key(std::string name, KeyHolder handle){
				Set(name, handle);
			}
			unsigned int Frame()const{
				return _frm;
			}

			bool Pressed()const{
				return static_cast<bool>(_frm);
			}
			const
			unsigned int operator()()const{
				return Frame();
			}

			bool operator()()const{
				return Pressed();
			}

			unsigned int Check(){
				if(interlib::CheckIsHitKey(Frame())){

				}
			}

		};

	public:
		enum State{
			Unavaliable, //何もしない
			Avaliable, //キー入力受付可
			Recording, //Avaliable + キー入力を記録
			Playing //キー入力を再生
		};
	private:
		State _state;
		std::fstream _file;
		std::vector<Key> _key; //各キーを格納したvector。

		//キー名と_key のindex をマッチさせるmap
		boost::unordered_map<std::string, int> _map;


	};
}
#endif // KEY_MANAGER_HPP
