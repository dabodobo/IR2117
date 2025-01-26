#include <iostream>


int suma(int num ){
	int sum = 0;
	for(int i = 1; i <= num; i++){
		sum+=i;
	}
	return sum;
}

int main(){
	int num;
	std::cout << "Enter a number: ";
	std::cin >> num;
	if(num >= 1){
		std::cout << "The sum from 1 to " << num << " is " << suma(num) << std::endl;
	}
	else{
		std::cout << "Rango de número incorrecto" << std::endl;
	}

	return 0;
}
