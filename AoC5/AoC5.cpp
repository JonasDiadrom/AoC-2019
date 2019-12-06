// AoC2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int runComputer(std::vector<int> intcode, int startAddress, int returnAddress) {
	int IP = startAddress;
	while (true) {
		bool stop = false;
		switch (intcode[IP]) {
		case 1:
			while (intcode[IP + 3] >= intcode.size()) {
				intcode.push_back(0);
			}
			intcode[intcode[IP + 3]] = intcode[intcode[IP + 1]] + intcode[intcode[IP + 2]];
			IP += 4;
			break;
		case 2:
			while (intcode[IP + 3] >= intcode.size()) {
				intcode.push_back(0);
			}
			intcode[intcode[IP + 3]] = intcode[intcode[IP + 1]] * intcode[intcode[IP + 2]];
			IP += 4;
			break;
		case 99:
			stop = true;
			IP += 1;
			break;
		default:
			std::cout << "Big error: " << intcode[IP] << std::endl;
			return 123;
			break;
		}
		if (stop) {
			break;
		}
	}

	return intcode[returnAddress];
}

int main()
{
	int noun = 12;
	int verb = 02;

	std::vector<int> intcode{};
	std::ifstream input("Input.txt");
	std::string cutInt;
	while (std::getline(input,cutInt,',')) {
		intcode.push_back(std::stoi(cutInt));
	}

	intcode[1] = noun;
	intcode[2] = verb;

	int result = runComputer(intcode, 0, 0);
	std::cout << result << std::endl;

	bool found = false;
	for (noun = 0; noun <= 99; noun++) {
		for (verb = 0; verb <= 99; verb++) {
			intcode[1] = noun;
			intcode[2] = verb;
			result = runComputer(intcode, 0, 0);
			if (result == 19690720) {
				std::cout << "Found: " << noun * 100 + verb << std::endl;
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
