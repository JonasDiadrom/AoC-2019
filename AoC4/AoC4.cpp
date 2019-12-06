// AoC4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool validCode(std::vector<int> code) {
	bool twoSame = false;
	bool twoSameButNotMore = false;
	int numSame = 1;
	for (int i = 0; i < code.size(); i++) {
		if (i != 0 && code[i] < code[i - 1]) {
			return false;
		}

		if (i != 0 && code[i] == code[i - 1]) {
			numSame++;
		}
		else {
			if (numSame == 2) {
				twoSameButNotMore = true;
			}
			if (numSame >= 2) {
				twoSame = true;
			}
			numSame = 1;
		}
	}
	if (numSame == 2) {
		twoSameButNotMore = true;
	}
	if (numSame >= 2) {
		twoSame = true;
	}

	return twoSameButNotMore;
}


int main()
{
	int lowerLim;
	int upperLim;
	std::vector<int> lowerVec;
	std::vector<int> upperVec;
	
	{
		std::ifstream input("Input.txt");
		std::string cut;
		std::getline(input, cut, '-');
		for (auto num : cut) {
			lowerVec.push_back(num - '0');
		}
		lowerLim = std::stoi(cut);
		std::getline(input, cut, '-');
		for (auto num : cut) {
			upperVec.push_back(num - '0');
		}
		upperLim = std::stoi(cut);
	}

	std::cout << lowerLim << "-" << upperLim << std::endl;
	
	std::vector<int> currentCode = lowerVec;

	for (int i = 1; i < currentCode.size(); i++) {
		if (currentCode[i] < currentCode[i - 1]) {
			currentCode[i] = currentCode[i - 1];
		}
	}
	
	std::cout << "Start code: " << currentCode[0] << currentCode[1] << currentCode[2] << currentCode[3] << currentCode[4] << currentCode[5] << std::endl;

	bool first = true;
	bool done = false;
	int numCodes = 0;
	for (; currentCode[0] <= 9; currentCode[0]++) { 
		for (currentCode[1] = first ? currentCode[1] : currentCode[0]; (currentCode[1] <= 9 && currentCode[1] >= currentCode[0]) && !done; currentCode[1]++) {
			for (currentCode[2] = first ? currentCode[2] : currentCode[1]; (currentCode[2] <= 9 && currentCode[2] >= currentCode[1]) && !done; currentCode[2]++) {
				for (currentCode[3] = first ? currentCode[3] : currentCode[2]; (currentCode[3] <= 9 && currentCode[3] >= currentCode[2]) && !done; currentCode[3]++) {
					for (currentCode[4] = first ? currentCode[4] : currentCode[3]; (currentCode[4] <= 9 && currentCode[4] >= currentCode[3]) && !done; currentCode[4]++) {
						for (currentCode[5] = first ? currentCode[5] : currentCode[4]; (currentCode[5] <= 9 && currentCode[5] >= currentCode[4]) && !done; currentCode[5]++) {
							int currentInt = currentCode[0] * 100000 + currentCode[1] * 10000 + currentCode[2] * 1000 + currentCode[3] * 100 + currentCode[4] * 10 + currentCode[5];
							if (currentInt > upperLim) {
								std::cout << "Done at: " << currentInt << std::endl;
								done = true;
							}
							else if (validCode(currentCode)){
								numCodes++;
								std::cout << "Valid " << numCodes << ": " << currentInt << std::endl;
							}
							else {
								std::cout << "Invalid: " << currentInt << std::endl;
							}
							first = false;
						}
					}
				}
			}
		}
	}

	std::cout << numCodes << std::endl;
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
