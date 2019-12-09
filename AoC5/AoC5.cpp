// AoC2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

class operation;

class intComputer {
	std::vector<int> memory;
	int IP;
	std::map<int, operation*> operations;
	bool stop = false;
public:
	intComputer(std::vector<int> memory, int startAddress);
	void run();
	int getAt(int address) { return memory[address]; }
	void setIP(int newIP) { IP = newIP; }
	int getIP() { return IP; }
	void writeToAddress(int index, int value);
	void halt() { stop = true; }
	std::vector<int> getMemRange(int index, int length);
};

std::vector<int> intComputer::getMemRange(int index, int length) {
	if (length == 0) {
		return {};
	}
	std::vector<int> output(length);
	for (int i = 0; i < length; i++) {
		output[i] = memory[index + i];
	}
	return output;
}

class operation {
protected:
	intComputer * PC;
	int numParams;
	int opcode;
	int addIP;
	//vector mode
private:
	virtual void runSpecific(std::vector<int> params) =0;
public:
	operation(int opcode, int numParams, intComputer* PC) : numParams(numParams), opcode(opcode), PC(PC), addIP(numParams) {}
	operation() : PC(nullptr), numParams(0), opcode(0), addIP(0) {}

	void run(int modeCode) {
		addIP = numParams;
		std::vector<int> params = PC->getMemRange(PC->getIP(), numParams);
		std::string modeCodeStr{ std::to_string(modeCode) };
		while (modeCodeStr.length() < numParams - 1) {
			modeCodeStr = "0" + modeCodeStr;
		}
		std::reverse(modeCodeStr.begin(), modeCodeStr.end());
		for (int i = 1; i < params.size(); i++) {
			if (modeCodeStr[i-1] == '0') {
				params[i] = PC->getAt(params[i]);
			}
		}

		runSpecific(params);
		PC->setIP(PC->getIP() + addIP);
	};
};

class opAdd : public operation {
	void runSpecific(std::vector<int> params) override {
		int result = params[1] + params[2];
		PC->writeToAddress(PC->getAt(PC->getIP()+3), result);
	}
public:
	opAdd(intComputer* PC) : operation(1, 4, PC) {}
};

class opMultiply : public operation {
	void runSpecific(std::vector<int> params) override {
		int result = params[1] * params[2];
		PC->writeToAddress(PC->getAt(PC->getIP() + 3), result);
	}
public:
	opMultiply(intComputer* PC) : operation(2, 4, PC) {}
};

class opInput : public operation {
	void runSpecific(std::vector<int> params) override {
		int input;
		std::cout << "Input: ";
		std::cin >> input;
		PC->writeToAddress(PC->getAt(PC->getIP() + 1), input);
	}
public:
	opInput(intComputer* PC) : operation(3, 2, PC) {}
};

class opOutput : public operation {
	void runSpecific(std::vector<int> params) override {
		std::cout << "Output: ";
		std::cout << params[1] << std::endl;
	}
public:
	opOutput(intComputer* PC) : operation(4, 2, PC) {}
};

class opJumpTrue : public operation {
	void runSpecific(std::vector<int> params) override {
		if (params[1] != 0) {
			addIP = 0;
			PC->setIP(params[2]);
		}
	}
public:
	opJumpTrue(intComputer* PC) : operation(5, 3, PC) {}
};

class opJumpFalse : public operation {
	void runSpecific(std::vector<int> params) override {
		if (params[1] == 0) {
			addIP = 0;
			PC->setIP(params[2]);
		}
	}
public:
	opJumpFalse(intComputer* PC) : operation(6, 3, PC) {}
};

class opLessThan : public operation {
	void runSpecific(std::vector<int> params) override {
		int result = 0;
		if (params[1] < params[2]) {
			result = 1;
		}
		PC->writeToAddress(PC->getAt(PC->getIP() + 3), result);
	}
public:
	opLessThan(intComputer* PC) : operation(7, 4, PC) {}
};

class opEquals : public operation {
	void runSpecific(std::vector<int> params) override {
		int result = 0;
		if (params[1] == params[2]) {
			result = 1;
		}
		PC->writeToAddress(PC->getAt(PC->getIP() + 3), result);
	}
public:
	opEquals(intComputer* PC) : operation(8, 4, PC) {}
};

class opHalt : public operation {
	void runSpecific(std::vector<int> params) override {
		PC->halt();
	}
public:
	opHalt(intComputer* PC) : operation(99, 1, PC) {}
};



intComputer::intComputer(std::vector<int> memory, int startAddress) : memory(memory), IP(startAddress) {
	operations[1] = new opAdd(this);
	operations[2] = new opMultiply(this);
	operations[3] = new opInput(this);
	operations[4] = new opOutput(this);
	operations[5] = new opJumpTrue(this);
	operations[6] = new opJumpFalse(this);
	operations[7] = new opLessThan(this);
	operations[8] = new opEquals(this);

	/// TODO: Add ops

	operations[99] = new opHalt(this);
}

void intComputer::writeToAddress(int index, int value) {
	while (index >= static_cast<int>(memory.size())) {
		memory.push_back(0);
	}
	memory[index] = value;
}

void intComputer::run() {
	stop = false;
	while (!stop) {
		int opCode = memory[IP] % 100;
		int modeCode = memory[IP] / 100;
		operations[opCode]->run(modeCode);
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
