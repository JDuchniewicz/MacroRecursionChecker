#include <Algorithm.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    bool result = true;
    Algorithm algo {};
    if (argc < 2)
    {
        std::cerr << "Please call the program with a filename!" << std::endl;
        exit(-1);
    }
    algo.parseInput(argv[1]);
    result = algo.checkForCycles();
    std::cout << (result ? "There is a" : "There is no") << " possibility of infinite recursion in given set of macrodefinitions." << std::endl;
    return 0;
}