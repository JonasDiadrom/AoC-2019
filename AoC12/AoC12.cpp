// AoC12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <exception>
#include <string>

class position {
	int x;
	int y;
	int z;
public:
	position() : position(0, 0, 0) {}
	position(int x, int y, int z) : x(x), y(y), z(z) {}
	int& operator[](int index) {
		switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range("Index out of range for position: " + std::to_string(index) + " Valid indices are 0, 1 and 2.");
		}
	}
	int& operator[](char axis) {
		switch (axis) {
		case 'x':
			return x;
		case 'y':
			return y;
		case 'z':
			return z;
		default:
			throw std::out_of_range("Index out of range for position: " + std::to_string(axis) + " Valid indices are x, y and z.");
		}
	}
	friend std::ostream& operator<<(std::ostream& out, position const& p);
};

std::ostream& operator<<(std::ostream& out, position const& p) {
	out << "<x=" << p.x << ", y=" << p.y << ", z=" << p.z << ">";
	return out;
}

class velocity {
	int x;
	int y;
	int z;
public:
	velocity() : velocity(0, 0, 0) {}
	velocity(int x, int y, int z) : x(x), y(y), z(z) {}
	int& operator[](int index) {
		switch (index) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range("Index out of range for velocity: " + std::to_string(index) + " Valid indices are 0, 1 and 2.");
		}
	}
	int& operator[](char axis) {
		switch (axis) {
		case 'x':
			return x;
		case 'y':
			return y;
		case 'z':
			return z;
		default:
			throw std::out_of_range("Index out of range for velocity: " + std::to_string(axis) + " Valid indices are x, y and z.");
		}
	}
	friend std::ostream& operator<<(std::ostream& out, velocity const& p);
};

std::ostream& operator<<(std::ostream& out, velocity const& v) {
	out << "<x=" << v.x << ", y=" << v.y << ", z=" << v.z << ">";
	return out;
}

class moon {
	position pos;
	velocity vel;
public:
	moon() : moon(position(), velocity()) {}
	moon(int x, int y, int z) : moon(position(x, y, z), velocity()) {}
	moon(int px, int py, int pz, int vx, int vy, int vz) : moon(position(px, py, pz), velocity(vx, vy, vz)) {}
	moon(position pos) : moon(pos, velocity()) {}
	moon(position pos, velocity vel) : pos(pos), vel(vel) {}

	int getPotential() const {
		return abs(pos['x']) + abs(pos['y']) + abs(pos['z']);
	}
	int getKinetic() const {
		return abs(vel['x']) + abs(vel['y']) + abs(vel['z']);
	}
	int getEnergy() const {
		return getPotential() * getKinetic();
	}

	friend std::ostream& operator<<(std::ostream& out, moon& m);
};

std::ostream& operator<<(std::ostream& out, moon& m) {

	out << "Pot: " << m.pos << " = " << m.getPotential() << "; Kin: " << m.vel << " = " << m.getKinetic() << "; Energy: " << m.getEnergy();
	return out;
}

int main()
{
	std::vector<moon> moons;
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
