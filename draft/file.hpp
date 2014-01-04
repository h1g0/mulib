#ifndef FILE_HPP
#define FILE_HPP
#include <string>
namespace mulib{namespace priv{
	template <typename T> class File{
		T _hdl;
		int _cnt;

		//参照カウンタ増加
		inline int _Ref(){return ++_cnt;}

		//参照カウンタ減少
		inline int _Unref(){
			if(--_cnt <= 0){
				delete this;
			}else{
				return _cnt;
			}
		}

	public:
		virtual int Init();
		virtual int Load(const T& handle);
		virtual int Load(const std::string path);

		File(): _cnt(0),_hdl(0){Init();}
		File(const T& handle){Load(handle);}
		File(File& rhs){

		}
	};
}}
#endif // FILE_HPP
