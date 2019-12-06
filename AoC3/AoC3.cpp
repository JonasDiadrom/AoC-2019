// AoC3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

class point {
public:
	int x;
	int y;
	point(int x, int y) : x(x), y(y) {}
	int getDistance() { return std::abs(x) + std::abs(y); }

	point operator-() const {
		return point(-x, -y);
	}

	point operator-(point const& p) const {
		return point(this->x - p.x, this->y - p.y);
	}

	point operator+(point const& p) const {
		return point(this->x + p.x, this->y + p.y);
	}

	friend std::ostream& operator<<(std::ostream& os, point const& p);
};

std::ostream& operator<<(std::ostream& os, point const& p)
{
	os << p.x << ";" << p.y;
	return os;
}

class line {
	point start;
	point end;
	bool horizontal;
public:
	line(int x1, int y1, int x2, int y2) : line(point(x1,y1), point(x2,y2)) {}
	line(point start, point end) : start(start), end(end) {
		// Make sure lines are straight or we have an error
		if (start.x != end.x && start.y != end.y) {
			std::cout << "Error: Not a straight line: " << start << ", " << end << std::endl;
			end.x = start.x;
			std::cout << "Straightened out: " << start << ", " << end << std::endl;
		}

		// Check horizontality
		if (start.x == end.x) {
			horizontal = false;
		}
		else {
			horizontal = true;
		}
	}

	point getStart() { return start; }
	point getEnd() { return end; }
	bool getHorizontal() { return horizontal; }

	int getLength() {
		return std::abs(end.x - start.x) + std::abs(end.y - start.y);
	}

	std::tuple<bool, point> crossing(line const& otherLine) const {
		return line::crossing(*this, otherLine);
	}
	static std::tuple<bool, point> crossing(line firstLine, line secondLine) {
		// If both lines are the same direction, they can't cross (but they can intersect)
		if (firstLine.horizontal == secondLine.horizontal) {
			return std::make_tuple(false, point(0,0));
		}

		// Make sure firstLine is always the horizontal line
		if (firstLine.horizontal != true) {
			std::swap(firstLine, secondLine);
		}

		// Make sure start is always to the left and on the bottom of end
		if (firstLine.end.x < firstLine.start.x || firstLine.end.y < firstLine.start.y) {
			std::swap(firstLine.start, firstLine.end);
		}
		if (secondLine.end.x < secondLine.start.x || secondLine.end.y < secondLine.start.y) {
			std::swap(secondLine.start, secondLine.end);
		}

		// We made sure in constructor that start is always left and bottom and end is always right and top
		// We also made sure firstLine is always horizontal, so check that firstLine starts left of and ends right of secondLine and that secondLine start below and ends above firstline
		if (((firstLine.start.x < secondLine.start.x) && (firstLine.end.x > secondLine.start.x)) && ((secondLine.start.y < firstLine.start.y) && (secondLine.end.y > firstLine.start.y))) {
			// The intersection is the x from second line and y from first line
			return std::make_tuple(true, point(secondLine.start.x, firstLine.start.y));
		}

		// If we're here, we didn't hit an intersection
		return std::make_tuple(false, point(0, 0));
	}

	friend std::ostream& operator<<(std::ostream& os, line const& l);
};

std::ostream& operator<<(std::ostream& os, line const& l)
{
	os << l.start << " --> " << l.end;
	return os;
}

class cintersection {
	point location;
	int steps;
public:
	cintersection(point location, int steps) : location(location), steps(steps) {}
	cintersection(point location, int steps1, int steps2) : cintersection(location, steps1 + steps2) {}
	int getSteps() { return steps; }
	point getLoc() { return location; }
};

int main(){
	std::vector<line> path1{};
	std::ifstream input("Input.txt");
	{
		point currentPos(0, 0);

		std::string cut;
		std::getline(input, cut);
		std::stringstream bufferStream(cut);
		while (std::getline(bufferStream, cut, ',')) {

			point nextPos(currentPos);
			char dir = cut[0];
			int length = std::stoi(cut.substr(1));
			switch (dir) {
			case 'R':
				nextPos.x += length;
				break;
			case 'L':
				nextPos.x -= length;
				break;
			case 'U':
				nextPos.y += length;
				break;
			case 'D':
				nextPos.y -= length;
				break;
			default:
				std::cout << "Error in lettering: " << dir << std::endl;
				return 1;
			}
			path1.push_back(line(currentPos, nextPos));

			currentPos = nextPos;
		}
	}


	std::vector<cintersection> crossings{};
	{
		point currentPos(0, 0);
		int path2Steps = 0;

		std::string cut;
		std::getline(input, cut);
		std::stringstream bufferStream(cut);
		while (std::getline(bufferStream, cut, ',')) {
			point nextPos(currentPos);
			char dir = cut[0];
			int length = std::stoi(cut.substr(1));
			switch (dir) {
			case 'R':
				nextPos.x += length;
				break;
			case 'L':
				nextPos.x -= length;
				break;
			case 'U':
				nextPos.y += length;
				break;
			case 'D':
				nextPos.y -= length;
				break;
			default:
				std::cout << "Error in lettering: " << dir << std::endl;
				return 1;
			}
			
			line path2Segment(currentPos, nextPos);

			int path1Steps = 0;
			for (auto path1Segment : path1) {
				if (auto [crossed, intersection] = path1Segment.crossing(path2Segment); crossed) {
					crossings.push_back(cintersection(intersection, path1Steps + line(path1Segment.getStart(), intersection).getLength(), path2Steps + line(path2Segment.getStart(), intersection).getLength()));
					std::cout << "Crossing at: " << intersection << std::endl << "Distance: " << intersection.getDistance() << std::endl << "Steps: " << crossings.back().getSteps() << std::endl;
				}
				path1Steps += path1Segment.getLength();
			}
			path2Steps += path2Segment.getLength();
			currentPos = nextPos;
		}
	}

	int smallestDistance = INT_MAX;
	int leastSteps = INT_MAX;
	for (auto intersection : crossings) {
		smallestDistance = smallestDistance <= intersection.getLoc().getDistance() ? smallestDistance : intersection.getLoc().getDistance();
		leastSteps = leastSteps <= intersection.getSteps() ? leastSteps : intersection.getSteps();
	}

	if (smallestDistance != INT_MAX) {
		std::cout << "Smallest distance: " << smallestDistance << std::endl;
		std::cout << "Least steps: " << leastSteps << std::endl;
	}
	else {
		std::cout << "No crossing" << std::endl;
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
