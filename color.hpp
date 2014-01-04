#ifndef COLOR_HPP
#define COLOR_HPP

class Color{
	static const int MAX = 255;
	int _r, _g, _b;
	//範囲内に収まっているかチェックする
	inline int _Check(int v){
		if(v<0){v=0;}
		else if(v>MAX){v=MAX;}
		return v;
	}

public:
	void Init(){
		_r = 0;
		_g = 0;
		_b = 0;
	}
	Color(){
		Init();
	}

	inline void SetR(int rot){_r = _Check(rot);}
	inline void SetG(int gruen){_g = _Check(gruen);}
	inline void SetB(int blau){_b = _Check(blau);}

	void Set(int r, int g, int b){
		SetR(r);
		SetG(g);
		SetB(b);
	}

	int r(){return _r;}
	int g(){return _g;}
	int b(){return _b;}

};

#endif // COLOR_HPP
