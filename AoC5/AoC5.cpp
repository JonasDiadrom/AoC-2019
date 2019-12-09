// AoC2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

class intComputer;

class operation {
protected:
	intComputer * PC;
	int numParams;
	int opcode;
	//vector mode
private:
	virtual void runSpecific() =0;
public:
	operation(int opcode, int numParams, intComputer* PC) : numParams(numParams), opcode(opcode), PC(PC) {}

	void run() {
		runSpecific();
		PC->setIP(PC->getIP() + numParams);
	};
};

class opAdd : public operation {
	void runSpecific() override {
		int result = PC->getAt(PC->getIP() + 1) + PC->getAt(PC->getIP() + 2);
		PC->writeToAddress(PC->getIP()+3, result);
	}
public:
	opAdd(intComputer* PC) : operation(1, 4, PC) {}
};

class opMultiply : public operation {
	void runSpecific() override {
		int result = PC->getAt(PC->getIP() + 1) * PC->getAt(PC->getIP() + 2);
		PC->writeToAddress(PC->getIP() + 3, result);
	}
public:
	opMultiply(intComputer* PC) : operation(2, 4, PC) {}
};

class opInput : public operation {
	void runSpecific() override {
		int input;
		std::cout << "Input: ";
		std::cin >> input;
		PC->writeToAddress(PC->getIP() + 1, input);
	}
public:
	opInput(intComputer* PC) : operation(3, 2, PC) {}
};

class opOutput : public operation {
	void runSpecific() override {
		std::cout << "Output: ";
		std::cout << PC->getAt(PC->getIP() + 1) << std::endl;
	}
public:
	opOutput(intComputer* PC) : operation(4, 2, PC) {}
};

class opHalt : public operation {
	void runSpecific() override {
		PC->halt();
	}
public:
	opHalt(intComputer* PC) : operation(99, 1, PC) {}
};

class intComputer {
	std::vector<int> memory;
	int IP;
	std::map<int, operation> operations;
	bool stop;
public:
	intComputer(std::vector<int> memory, int startAddress) : memory(memory), IP(startAddress) {
		operations[1] = opAdd(this);
		operations[2] = opMultiply(this);
		operations[3] = opInput(this);
		operations[4] = opOutput(this);
		operations[99] = opHalt(this);
	}
	void run();
	int getAt(int address) { return memory[address]; }
	void setIP(int newIP) { IP = newIP; }
	int getIP() { return IP; }
	void writeToAddress(int index, int value);
	void halt() { stop = true; }
};

void intComputer::writeToAddress(int index, int value) {
	while (index >= static_cast<int>(memory.size())) {
		memory.push_back(0);
	}
	memory[index] = value;
}

void intComputer::run() {
	stop = false;
	while (!stop) {
		operations[memory[IP]].run();
	}
}

int main()
{
	std::vector<int> intcode{};
	std::ifstream input("Input.txt");
	std::string cutInt;
	while (std::getline(input, cutInt, ',')) {
		intcode.push_back(std::stoi(cutInt));
	}

	intComputer PC(intcode, 0);
	PC.run();
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
