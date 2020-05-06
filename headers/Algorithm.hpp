#pragma once

#include <Graph.hpp>
#include <memory>

class Algorithm
{
public:
    void parseInput(const char* fileName);
    bool checkForCycles() const;

private:
    void parseEntry(const std::string& line);

    std::unique_ptr<Graph> m_graph;
};