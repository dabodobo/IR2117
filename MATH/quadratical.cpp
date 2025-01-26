#include <iostream>
#include <vector>
#include <cmath>



int main(){
	double a,b,c;
	std::vector<double> sol;
	
	
	std::cout << "Enter a: ";
	std::cin >> a;
	std::cout << "Enter b: ";
	std::cin >> b;
	std::cout << "Enter c: ";
	std::cin >> c;
	
	double D = (b*b) - (4*a*c);
	
	if(D == 0){
		sol = {(-b)/(2*a)};
		std::cout << "La solucion es " << sol[0] << std::endl;
	}
	else if(D > 0){
		sol = {(-b + sqrt(D))/(2*a),(-b - sqrt(D))/(2*a)};
		std::cout << "Las soluciones son " << sol[0] << " y " << sol[1] << std::endl;
	}
	
	else{
		std::cout << "La solución es compleja" << std::endl;
	
	}
	


	return 0;
}
