#include <iostream>

int main(){
	int num;
	std::cout << "Enter a number: ";
	std::cin >> num;
	int sum = 0;
	
	for(int i = 1; i <= num; i++){
		sum+=i;
	}
	
	std::cout << "The sum from 1 to " << num << " is " << sum << std::endl;

	return 0;
}
