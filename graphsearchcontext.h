#ifndef GRAPHSEARCHCONTEXT_H
#define GRAPHSEARCHCONTEXT_H

#include "graphsearchstrategy.h"

//GraphSearchContext is used to set a context for using search algorithm on graph
//Currently it supports executing A*search - start to goal
class GraphSearchContext
{
    private:
        GraphSearchStrategy* strategy;
    public:
        GraphSearchContext();
    public:
        void setStrategy(GraphSearchStrategy* strategy) {
            this->strategy = strategy;
        }
        bool executeStrategy(std::vector<std::vector<CGridItem>>& grid, int nWd, int nHt, CPosition& start, CPosition& goal) {
            return strategy->FindPath(grid, nWd, nHt, start, goal);
        }
};

#endif // GRAPHSEARCHCONTEXT_H
