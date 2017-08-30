#pragma once
#include <queue>
#include <unordered_map>



template<typename T, typename TPriority>
struct PriorityQueue
{
    typedef std::pair<TPriority, T> TElem;
    typedef std::priority_queue<TElem, std::vector<TElem>, std::greater<TElem>> TQueue;
    TQueue Elems;

    inline bool IsEmpty() const
    {
        return Elems.empty();
    }

    inline void Put(T item, TPriority priority)
    {
        Elems.emplace(priority, item);
    }

    inline T Get()
    {
        T best = Elems.top().second;
        Elems.pop();
        return best;
    }
};

template <class T>
struct HexCubeHash
{
    size_t operator()(const HexCube_t<T>& cube) const
    {
        return cube.q << 16 | cube.r;
    }
};

class HexAStar
{
    typedef HexCube_t<int> HexCube;

    typedef std::unordered_map<HexCube, HexCube, HexCubeHash<int>> TRoute;
    typedef std::unordered_map<HexCube, double, HexCubeHash<int>> TCosts;

public:
    HexAStar()
    {
    }

    virtual ~HexAStar()
    {
    }

    std::list<HexCube> FindPath(const HexCube& from, const HexCube& to)
    {
        TRoute cameFrom;
        TCosts costSoFar;
        AStar(from, to, cameFrom, costSoFar);
        return ReconstructPath(from, to, cameFrom);
    }

protected:

    void AStar(const HexCube& from, const HexCube& to, TRoute& cameFrom, TCosts& costSoFar)
    {
        PriorityQueue<HexCube, double> frontier;
        frontier.Put(from, 0);

        cameFrom[from] = from;
        costSoFar[from] = 0;

        while (!frontier.IsEmpty())
        {
            auto current = frontier.Get();
            if (current == to)
            {
                break;
            }

            for (auto& next : current.GetNeighbors())
            {
                double new_cost = costSoFar[current] + Cost(current, next);
                if (!costSoFar.count(next) || new_cost < costSoFar[next])
                {
                    costSoFar[next] = new_cost;
                    double priority = new_cost + Heuristic(next, to);
                    frontier.Put(next, priority);
                    cameFrom[next] = current;
                }
            }
        }
    }

    std::list<HexCube> ReconstructPath(const HexCube& from, const HexCube& to, const TRoute& cameFrom)
    {
        std::list<HexCube> path;

        HexCube current = to;
        path.push_back(current);

        while (current != from)
        {
            auto it = cameFrom.find(current);
            current = it->second;
            path.push_back(current);
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

    virtual double Cost(const HexCube& From, const HexCube& To)
    {
        return From.GetDistance(To);
    }

    virtual double Heuristic(const HexCube& From, const HexCube& To)
    {
        return From.GetDistance(To);
    }
};

