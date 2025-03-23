#ifndef GRAPHSEARCHSTRATEGY_H
#define GRAPHSEARCHSTRATEGY_H

#include <vector>

using namespace std;

//CPosition struct is used for 0 based location in a data structure
struct CPosition{
    int m_nXPos, m_nYPos;
    CPosition(int _x, int _y) : m_nXPos(_x), m_nYPos(_y) {}
    bool operator==(const CPosition& other) const { return ((m_nXPos == other.m_nXPos) && (m_nYPos == other.m_nYPos)); }
};

//CGridItem is the main data structure behind each cell of a data grid
// This also includes parameters to the A* search algorithm as plan is to show all distance data on each explored cell item (Time permits!)
// This is a central element on which path finding algorithm is implemented
struct CGridItem
{
    CPosition m_pos;
    CPosition m_Parent;
    int distTotal, distFromStart, distEstToGoal; // Values used by the A* algorithm
    int status; // 0 - default, 1 - open, -1 = close
    int nGridValue; // Read data for cell

    CGridItem(int _x=0, int _y=0, int _nVal=1) : m_pos(_x,_y), m_Parent(0,0), distTotal(0), distFromStart(0), distEstToGoal(0), status(0), nGridValue(_nVal) {}

    // Overload comparison operators for priority queue
    bool operator>(const CGridItem& other) const {
        if(distTotal == other.distTotal)
        {
            return distEstToGoal > other.distTotal;
        }
        else
            return distTotal > other.distTotal;
    }
};

//GraphSearchStrategy is a base class for search algorithms
// This can be extended for specific algorithm
class GraphSearchStrategy
{
public:
    GraphSearchStrategy();

    virtual bool FindPath( std::vector<std::vector<CGridItem>>& grid, int nWd, int nHt, CPosition& start, CPosition& goal)= 0;
};

//Specific implementation of GraphSearchStrategy to use A*Search
class ASearch : public GraphSearchStrategy {
public:
    bool FindPath( std::vector<std::vector<CGridItem>>& grid, int nWd, int nHt, CPosition& start, CPosition& goal) override;
};

#endif // GRAPHSEARCHSTRATEGY_H
