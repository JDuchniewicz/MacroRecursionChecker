#pragma once

#include <Graph.hpp>
#include <memory>

class Algorithm
{
public:
    Algorithm() : m_graph(std::make_unique<Graph>()) {};

    void parseInput(const char* fileName);
    bool checkForCycles() const;

private:
    void parseEntry(std::ifstream& file, const std::string& line);

    std::unique_ptr<Graph> m_graph;
};