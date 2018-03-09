#pragma once
#include <assert.h>
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

template <typename T>
struct HexCubeHash
{
    size_t operator()(const HexCube_t<T>& cube) const
    {
        return cube.q << 16 | cube.r;
    }
};

template <typename T>
class HexAStar
{
public:
    HexAStar() = default;
    virtual ~HexAStar() = default;

    typedef HexCube_t<T> CubeType;
    typedef std::unordered_map<CubeType, CubeType, HexCubeHash<int>> TRoute;
    typedef std::unordered_map<CubeType, double, HexCubeHash<int>> TCosts;

    std::list<CubeType> FindPath(const CubeType& from, const CubeType& to)
    {
        TRoute cameFrom;
        TCosts costSoFar;
        AStar(from, to, cameFrom, costSoFar);
        return ReconstructPath(from, to, cameFrom);
    }

protected:
    void AStar(const CubeType& from, const CubeType& to, TRoute& cameFrom, TCosts& costSoFar)
    {
        PriorityQueue<CubeType, double> frontier;
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

    std::list<CubeType> ReconstructPath(const CubeType& from, const CubeType& to, const TRoute& cameFrom)
    {
        std::list<CubeType> path;

        CubeType current = to;
        path.push_back(current);

        while (current != from)
        {
            auto it = cameFrom.find(current);
            assert(it != cameFrom.end());

            current = it->second;
            path.push_back(current);
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

    virtual double Cost(const CubeType& From, const CubeType& To)
    {
        return From.GetDistance(To);
    }

    virtual double Heuristic(const CubeType& From, const CubeType& To)
    {
        return From.GetDistance(To);
    }
};

