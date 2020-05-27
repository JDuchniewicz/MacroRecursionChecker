#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

namespace
{
    int genID = 0;
}
static inline int generateID() { return genID++; }

class Graph
{
    enum class ENodeState
    {
        ENTER = 0, 
        EXIT
    };

public:
    void initializeAdj(int N);
    void addEdge(const std::string& from, std::string to);
    bool findCycle() const;

private:
    bool findCycleUtil(int vertex, std::unordered_set<int>& white, std::unordered_set<int>& grey, std::unordered_set<int>& black) const;

    std::vector<std::vector<int>> m_adj;
    std::unordered_map<std::string, int> m_macroToID;
};