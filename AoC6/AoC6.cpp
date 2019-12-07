// AoC6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <sstream>

class body {
    std::string name;
    std::vector<body*> orbiters;
    body* orbiting = nullptr;

    static std::map<std::string, body> bodylist;

    static int recursiveCountOrbits(std::vector<body*> bodyList, int level);

public:
    static void createBody(std::string name);
    static void linkBody(std::string name, std::string orbiting);
    static void initialize() { bodylist.clear(); bodylist["COM"] = body("COM"); }
    static auto getSize() { return bodylist.size(); }
    static body* getCOMLink() { return &(bodylist["COM"]); }
    static body* getBody(std::string name) { return &bodylist[name]; }
    
    static int recursiveCountOrbits();

    body(std::string name) : name(name) {}
    body() : name("") {}
    void orbit(body* orbiter) { orbiters.push_back(orbiter); }
    std::vector<body*> getOrbiters() { return orbiters; }
    std::string getName() { return name; }
    void setOrbit(body* orbit) { orbiting = orbit; }
    body* getOrbit() { return orbiting; }
    std::vector<body*> getOrbitChain();
};

std::map<std::string, body> body::bodylist;

int body::recursiveCountOrbits()
{
    return recursiveCountOrbits(bodylist["COM"].getOrbiters(), 1);
}

std::vector<body*> body::getOrbitChain()
{
    std::vector<body*> orbitChain;
    body* currentBody = orbiting;
    while (currentBody != nullptr)
    {
        orbitChain.push_back(currentBody);
        currentBody = currentBody->getOrbit();
    }
    return orbitChain;
}

int body::recursiveCountOrbits(std::vector<body*> bodyList, int level)
{
    int sum = 0;
    for (auto bodyObj : bodyList) {
        sum += level + recursiveCountOrbits(bodyObj->getOrbiters(), level + 1);
    }
    return sum;
}

void body::createBody(std::string name)
{
    bodylist[name] = body(name);
}

void body::linkBody(std::string name, std::string orbiting)
{
    bodylist[orbiting].orbit(&bodylist[name]);
    bodylist[name].setOrbit(&bodylist[orbiting]);
}

int main()
{
    body::initialize();

    std::ifstream input("Input.txt");
    std::string cut;
    std::vector<std::tuple<std::string, std::string>> links;
    while (std::getline(input, cut)) {
        std::string orbiting;
        std::string orbiter;
        std::stringstream cutStream(cut);
        std::getline(cutStream, orbiting, ')');
        std::getline(cutStream, orbiter, ')');

        body::createBody(orbiter);
        links.push_back(std::make_tuple(orbiter, orbiting));
    }
    for (auto [orbiter, orbiting] : links) {
        body::linkBody(orbiter, orbiting);
    }

    std::cout << "Number of bodies: " << body::getSize() << std::endl;
    std::cout << "Number of total orbits: " << body::recursiveCountOrbits() << std::endl;

    std::vector<body*> youChain = body::getBody("YOU")->getOrbitChain();
    std::vector<body*> sanChain = body::getBody("SAN")->getOrbitChain();

    {
        bool first = true;
        for (auto bodyObj : youChain) {
            if (!first) {
                std::cout << "(";
            }
            std::cout << bodyObj->getName();
            first = false;
        }
        std::cout << std::endl;
    }
    {
        bool first = true;
        for (auto bodyObj : sanChain) {
            if (!first) {
                std::cout << "(";
            }
            std::cout << bodyObj->getName();
            first = false;
        }
        std::cout << std::endl;
    }

    body* lastSame = nullptr;
    while (youChain[youChain.size()-1] == sanChain[sanChain.size()-1]) {
        lastSame = youChain[youChain.size()-1];
        youChain.pop_back();
        sanChain.pop_back();
    }
    std::reverse(sanChain.begin(), sanChain.end());
    std::vector<body*> totalChain{ body::getBody("YOU") };
    totalChain.insert(totalChain.end(), youChain.begin(), youChain.end());
    totalChain.push_back(lastSame);
    totalChain.insert(totalChain.end(), sanChain.begin(), sanChain.end());
    totalChain.push_back(body::getBody("SAN"));

    {
        bool first = true;
        for (auto bodyObj : totalChain) {
            if (!first) {
                std::cout << "-->";
            }
            std::cout << bodyObj->getName();
            first = false;
        }
        std::cout << std::endl;
    }

    std::cout << "Num jumps: " << totalChain.size() - 3 << std::endl;
}
