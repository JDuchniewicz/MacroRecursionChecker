#include <Graph.hpp>
#include <stack>
#include <algorithm>
#include <numeric>

void Graph::initializeAdj(int N)
{
    for (int i = 0; i < N; ++i)
        m_adj.push_back(std::vector<int>{});
}

void Graph::addEdge(const std::string& from, std::string to)
{
    auto itFrom = m_macroToID.find(from);
    int fromIdx = 0;
    if (itFrom == m_macroToID.end())
    {
        fromIdx = generateID();
        m_macroToID.insert( {from, fromIdx} ); // copy always made
    }
    else
        fromIdx = itFrom->second;

    auto itTo = m_macroToID.find(to);
    int toIdx = 0;
    if (itTo == m_macroToID.end())
    {
        toIdx = generateID();
        m_macroToID.insert( {std::move(to), toIdx} ); // moved as never used again in callee
    }
    else
        toIdx = itTo->second;

    m_adj.at(fromIdx).push_back(toIdx);
}

bool Graph::findCycle() const
{
    std::unordered_set<int> whiteSet;
    std::unordered_set<int> greySet;
    std::unordered_set<int> blackSet;

    // std::unordered_map has readonly iterators hence, cannot use iota
    for (size_t i = 0; i < m_adj.size(); ++i)
        whiteSet.insert(i);

    for (size_t i = 0; i < m_adj.size(); ++i)
    {
        auto it = std::find(whiteSet.begin(), whiteSet.end(), i);
        if (it != whiteSet.end())
        {
            if (findCycleUtil(i, whiteSet, greySet, blackSet))
                return true;
        }
        
    }
    return false;
}

bool Graph::findCycleUtil(int start, std::unordered_set<int>& white, std::unordered_set<int>& grey, std::unordered_set<int>& black) const
{
    std::stack<std::pair<int, ENodeState>> s; // idx + ENTER = 0, EXIT = 1
    s.push({ start, ENodeState::ENTER });

    while (!s.empty())
    {
        auto [idx, state] = s.top();
        s.pop();

        // if just visited this node
        if (state == ENodeState::ENTER)
        {
            // mark this edge as visited
            white.erase(idx);
            grey.insert(idx);
            s.push({ idx, ENodeState::EXIT });
            for (const auto& neigh : m_adj.at(idx))
            {
                // if this edge is already in the grey set -> visited, a cycle is found
                auto itGray = std::find(grey.begin(), grey.end(), neigh);
                if (itGray != grey.end())
                    return true;

                // if this edge is already black -> already processed
                auto itBlack = std::find(black.begin(), black.end(), neigh);
                if (itBlack != black.end())
                    continue;

                s.push({ neigh, ENodeState::ENTER });
            }
        } else if (state == ENodeState::EXIT) // processed all children and exiting it
        {
            grey.erase(idx);
            black.insert(idx);
        }
    }

    return false;
}