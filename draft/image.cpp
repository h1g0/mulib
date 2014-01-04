#include "image.hpp"
#include "inter_lib.hpp"
using namespace mulib;
void Image::Init()
{
		 if(_CheckLoaded()){
		Unload();
		_hdl = unloaded;
		 }
}

void Image::Load(const std::string& path){
		 if(_CheckLoaded()){Unload();}
		 _hdl = interlib::LoadImg(path);
}

void Image::Unload(){
		 if(_CheckLoaded()){
		interlib::UnloadImg(_hdl);
		_hdl = unloaded;
		 }
}
