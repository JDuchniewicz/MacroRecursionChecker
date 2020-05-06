#include <Algorithm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

void Algorithm::parseInput(const char* fileName)
{
    std::string name(fileName);
    std::ifstream file(name, std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Failed to open " << fileName << std::endl;
        exit(-1);
    }

    std::string line;
    // get N
    if (!std::getline(file, line))
    {
        std::cerr << "The test file is empty!" << std::endl;
        exit(-1);
    }
    m_graph->initializeAdj(std::atoi(line.c_str()));

    while (std::getline(file, line))
    {
        parseEntry(line);
    }
    file.close();
}

void Algorithm::parseEntry(const std::string& line)
{
    const char* ptr = line.c_str();
    if (*ptr != '#')
    {
        std::cerr << "File is wrongly formatted, please correct it!" << std::endl;
        std::cerr << "LINE: " << line;
        exit(-1);
    }
    ++ptr;

    std::string macroName;
    while (*ptr != '(')
    {
        macroName += *ptr;
        ++ptr;
    }
    macroName += '\n';

    while (*ptr != '\n')
    {
        if (*ptr == '$')
        { 
            ++ptr;
            std::string macroCall;
            // just read the name
            while (*ptr != '(')
            {
                macroCall += *ptr;
               ++ptr;
            }
            macroCall += '\n';
            // mark one edge
            m_graph->addEdge(macroName, std::move(macroCall));
        }
        ++ptr;
    }
}

bool Algorithm::checkForCycles() const
{
    return m_graph->checkForCycles();
}
