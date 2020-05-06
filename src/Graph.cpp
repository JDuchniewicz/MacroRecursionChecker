#include <Graph.hpp>
#include <queue>
#include <algorithm>

void Graph::initializeAdj(int N)
{
    m_adj.reserve(N);
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

bool Graph::checkForCycles() const
{
    std::vector<int> startingVertices = findConnectedComponents();

    for (const auto& start : startingVertices)
    {
        if (checkOneComponent(start))
            return true;
    }

    return false;
}

bool Graph::checkOneComponent(int startID) const
{
    std::unordered_set<int> whiteSet;
    std::unordered_set<int> greySet;
    std::unordered_set<int> blackSet;

    whiteSet.reserve(m_adj.size());
    std::iota(whiteSet.begin(), whiteSet.end(), 0);

    for (int i = 0; i < m_adj.size(); ++i)
    {
        auto it = std::find(whiteSet.begin(), whiteSet.end(), i); // this looks like duplication (will do all components))
        if (it != whiteSet.end())
        {
            if (findCycleUtil(i, whiteSet, greySet, blackSet))
                return true;
        }
        
    }
    return false;
}

bool Graph::findCycleUtil(int start, std::unordered_set<int>& white, std::unordered_set<int>& grey, std::unordered_set<int> black)
{
    std::queue<std::pair<int, ENodeState>> q; // idx + ENTER = 0, EXIT = 1
    q.push({ start, ENodeState::ENTER });

    while (!q.empty())
    {
        auto [idx, state] = q.back();
        q.pop();

        // if just visited this node
        if (state == ENodeState::ENTER)
        {
            // mark this edge as visited
            white.erase(idx);
            grey.insert(idx);
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

                q.push({ neigh, ENodeState::ENTER });
            }
            q.push({ idx, ENodeState::EXIT });
        } else if (state == ENodeState::EXIT) // processed all children and exiting it
        {
            grey.erase(idx);
            black.insert(idx);
        }
    }

    return false;
}

std::vector<int> Graph::findConnectedComponents() const
{
    std::vector<int> verticesIDs;
    bool visited[m_adj.size()] = { false };

    // for every non-visited vertex run DFS
    for (int i = 0; i < m_adj.size(); ++i)
    {
        if (visited[i])
            continue;

        verticesIDs.push_back(i);
        std::queue<int> q;

        while (!q.empty())
        {
            int cur = q.back();
            q.pop();

            if (!visited[cur])
            {
                for (const auto& n : m_adj.at(cur))
                    q.push(n);
                visited[cur] = true;
            }
        }
    }

    return verticesIDs;
}
