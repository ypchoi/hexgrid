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

template <class TGrid>
class HexGridManager;

template <class TGrid>
class HexAStar
{
    typedef uint32_t uint32;
    typedef typename TGrid::Index Index;
    typedef typename TGrid::Index::ValueType IndexValueType;
    typedef typename TGrid::Pixel Pixel;
    typedef typename TGrid::Pixel::ValueType T;
    typedef typename HexGridManager<TGrid> GridManager;
    typedef std::unordered_map<Index, Index, HexIndexHash<IndexValueType>> TRoute;
    typedef std::unordered_map<Index, float, HexIndexHash<IndexValueType>> TCosts;
    static const eHexGridShape Shape = TGrid::Shape;

public:
    HexAStar()
    {
    }

    virtual ~HexAStar()
    {
    }

    std::list<Index> FindPath(const Index& from, const Index& to)
    {
        TRoute cameFrom;
        TCosts costSoFar;
        AStar(from, to, cameFrom, costSoFar);
        return ReconstructPath(from, to, cameFrom);
    }

protected:

    void AStar(
        const Index& from,
        const Index& to,
        TRoute& cameFrom,
        TCosts& costSoFar)
    {
        PriorityQueue<Index, double> frontier;
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

    std::list<Index> ReconstructPath(const Index& from, const Index& to, const TRoute& cameFrom)
    {
        std::list<Index> path;

        Index current = to;
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

    virtual double Cost(const Index& From, const Index& To)
    {
        return From.GetCost(To);
    }

    virtual double Heuristic(const Index& From, const Index& To)
    {
        return From.GetCost(To);
    }
};

