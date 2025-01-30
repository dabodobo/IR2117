#include <iostream>


bool EsTriangulo(int a, int b, int c){
	if(a <= 0 | b <= b | c <= 0){
		return false;
	
	}
	if(a > b+c | b > c+a | c > b+a){
		return false;
	}
	return true;

}


int main(){
	int a,b,c;
	std::cout << "Introduce los lados de tu triangulo" << std::endl;
	std::cin >> a;
	std::cin >> b;
	std::cin >> c;
	std::string tipo;
	if(EsTriangulo){
		if(a ==){
		}
	
	}
	
	std::cout >> "Tu triangulo es (tipo) ";
	return 0;

}
