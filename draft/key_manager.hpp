#ifndef KEY_MANAGER_HPP
#define KEY_MANAGER_HPP
#include "inter_lib.hpp"
#include <fstream>
#include <string>
#include <boost/unordered_map.hpp>
#include <vector>
namespace mulib{
	class KeyManager{

		//�e�L�[�̏������N���X
		class _Key{
			unsigned int _frm; //������Ă���t���[�����B
			KeyHolder _hdl; //���ʂ���n���h��
			std::string _name; //�L�[�̖��O
		public:
			void Init(){
				_frm = 0;
				_hdl = 0;
				_name.clear();
			}

			//�n���h�����Z�b�g
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
			Unavaliable, //�������Ȃ�
			Avaliable, //�L�[���͎�t��
			Recording, //Avaliable + �L�[���͂��L�^
			Playing //�L�[���͂��Đ�
		};
	private:
		State _state;
		std::fstream _file;
		std::vector<Key> _key; //�e�L�[���i�[����vector�B

		//�L�[����_key ��index ���}�b�`������map
		boost::unordered_map<std::string, int> _map;


	};
}
#endif // KEY_MANAGER_HPP
