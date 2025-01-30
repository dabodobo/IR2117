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
		if(a == b && b == c && c == a){
			tipo = "equilatero";
		}
		else if(a != b && b != c && c != a){
			tipo = "escaleno";
		}
		else{
			tipo = "isósceles";
		}
		std::cout << "Tu triangulo " << a << "," << b << "," << c <<" es " << tipo << std::endl;
			
	}
	else{
	std::cout << "Tu triangulo " << a << "," << b << "," << c <<" NO es un triángulo"<< std::endl;
			
	}
	return 0;

}
