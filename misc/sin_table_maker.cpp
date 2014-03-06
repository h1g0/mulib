#include <cmath>
#include <iostream>
int main(){
	static const int tableSize = 1024;
	static const double tableUnit = (M_PI/2) / (tableSize -1);
	std::cout << "/* ================================" << std::endl;
	std::cout << " * Sin table for fast sin function." << std::endl;
	std::cout << " ----------------------------------" << std::endl;
	std::cout << " Range: 0 <= t <= pi/2" << std::endl;
	std::cout << " Table size: " << tableSize << std::endl;
	std::cout << " Expected memory size the table consumes: " <<
							 sizeof(double) * tableSize << " byte"<< std::endl;
	std::cout << " ================================*/" << std::endl;
	std::cout << "static const int tableSize = " << tableSize << ";" <<std::endl;
	std::cout << "#define MULIB_PI " << M_PI << std::endl;
	std::cout << "#define MULIB_RIGHT_ANGLE " << M_PI/2 << std::endl;

	std::cout << "static const double sin_table[] = {" << std::endl;
	double s = 0;
	for(int i=0;i<tableSize - 1;i++){
		s = sin(tableUnit * i);
		std::cout << s << ", ";
		if(i%5==0){std::cout << std::endl;}
	}
	std::cout << sin(tableUnit *  tableSize) << "};" << std::endl;

}
