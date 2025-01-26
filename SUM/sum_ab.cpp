#include <iostream>


int suma(int a, int b ){
	int sum = 0;
	for(int i = a; i <= b; i++){
		sum+=i;
	}
	return sum;
}

int main(){
	int a;
	std::cout << "Enter a number >= 1: ";
	std::cin >> a;
	while(a < 1){
	
		std::cout << "Enter a number >= 1: ";
		std::cin >> a;
	}
	int b;
	std::cout << "Enter b number >= 1: ";
	std::cin >> b;

	std::cout << "The sum from " << a << " to " << b << " is " << suma(a,b) << std::endl;
	

	return 0;
}
