#include "graphsearchstrategy.h"
#include <queue>
#include <QDebug>

GraphSearchStrategy::GraphSearchStrategy() {}

//FindPath() is the central A* algorithm used here for path finding
// Inputs : grid - data source, width & height of the grid, start & goal position
// First input parameters are validated - ideally they will be separated & even this algorithm will be part of the class structure (proposal attached in the repo)
// This algo uses Manhattan method, but can be (Euclidian, etc...)
bool ASearch::FindPath( std::vector<std::vector<CGridItem>>& grid, int nWd, int nHt, CPosition& start, CPosition& goal) {
    // Define possible movements (4 directions: up, down, left, right)
    const int directionX[] = {-1, 0, 1, 0};
    const int directionY[] = {0, 1, 0, -1};

    //Check for valid start & goal
    qDebug() << "...........FindPath().............." << start.m_nXPos << " : " << start.m_nYPos << "=>" << goal.m_nXPos << " : " << goal.m_nYPos;
    if(start == goal)
    {
        qDebug() << "...........start & goal are same.............." ;
        return true;
    }
    if((start.m_nXPos < 0) || (start.m_nYPos < 0) || (start.m_nXPos == nWd) || (start.m_nYPos == nHt))
    {
        qDebug() << "...........Out of bound start.............." ;
        return false;
    }
    if(grid[start.m_nXPos][start.m_nYPos].nGridValue == 3)
    {
        qDebug() << "...........Start not walkable.............." ;
        return false;
    }
    if((goal.m_nXPos < 0) || (goal.m_nYPos < 0) || (goal.m_nXPos == nWd) || (goal.m_nYPos == nHt))
    {
        qDebug() << "...........Out of bound goal.............." ;
        return false;
    }
    if(grid[goal.m_nXPos][start.m_nYPos].nGridValue == 3)
    {
        qDebug() << "...........Goal not walkable.............." ;
        return false;
    }
    qDebug() << "...........walkable values.............."<< grid[start.m_nXPos][start.m_nYPos].nGridValue << "::"<<grid[goal.m_nXPos][goal.m_nYPos].nGridValue;

    //pqOpenList Priority Q uses distance on processing grid cell till we find goal
    priority_queue<CGridItem, vector<CGridItem>, greater<CGridItem>> pqOpenList;
    CGridItem workItem = grid[start.m_nXPos][start.m_nYPos];
    grid[start.m_nXPos][start.m_nYPos].status = 1;
    pqOpenList.push(workItem);

    //Traverse till goal
    while (!pqOpenList.empty())
    {
        CGridItem current = pqOpenList.top();
        pqOpenList.pop();
        grid[current.m_pos.m_nYPos][current.m_pos.m_nXPos].status = -1;

        //Goal found - reverse track parents, also update cell values so they will be colored on QML view (this can be improved)
        if ((current.m_pos.m_nXPos == goal.m_nYPos) && (current.m_pos.m_nYPos == goal.m_nXPos)) {
            current = grid[goal.m_nXPos][goal.m_nYPos];
            do{
                qDebug() << "...........Path : " << current.m_pos.m_nXPos << ":" << current.m_pos.m_nYPos;
                grid[current.m_pos.m_nYPos][current.m_pos.m_nXPos].nGridValue = 12;
                current = grid[current.m_Parent.m_nYPos][current.m_Parent.m_nXPos];
            }while(current.distTotal != 0);
            qDebug() << "...........Start : " << current.m_pos.m_nXPos << ":" << current.m_pos.m_nYPos;
            //Update start & destination values for QML view
            grid[start.m_nXPos][start.m_nYPos].nGridValue = 11;
            grid[goal.m_nXPos][goal.m_nYPos].nGridValue = 13;
            return true;
        }

        // Explore neighbors
        for (int i = 0; i < 4; ++i)
        {
            int newX = current.m_pos.m_nXPos + directionX[i];
            int newY = current.m_pos.m_nYPos + directionY[i];
            // Check if the neighbor is within the grid boundaries
            if (newX >= 0 && newX < nWd && newY >= 0 && newY < nHt)
            {
                // Check if the neighbor is walkable and not in the closed list
                if (grid[newY][newX].nGridValue != 3 && (grid[newY][newX].status != -1))
                {
                    CGridItem neighbor = grid[newY][newX];
                    int newG = current.distFromStart + 10;
                    // Check if the neighbor is not in the open list or has a lower g value
                    if (newG < neighbor.distFromStart || (grid[newY][newX].status != 1))
                    {
                        //NOTE : This can be further optimized [when goal is found, immediately start unwinding]
                        neighbor.distFromStart = newG;
                        neighbor.distEstToGoal = abs(newX - goal.m_nXPos) + abs(newY - goal.m_nYPos);
                        neighbor.distTotal = neighbor.distFromStart + neighbor.distEstToGoal;
                        grid[newY][newX].distFromStart = neighbor.distFromStart;
                        grid[newY][newX].distEstToGoal = neighbor.distEstToGoal;
                        grid[newY][newX].distTotal = neighbor.distTotal;
                        grid[newY][newX].m_Parent = current.m_pos;
                        grid[newY][newX].status = 1;
                        pqOpenList.push(neighbor); // Add the neighbor to the open list
                    }
                }
            }
        }
    }
    return false;
}
