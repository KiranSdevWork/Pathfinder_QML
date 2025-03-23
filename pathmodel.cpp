#include "pathmodel.h"
#include <QRandomGenerator> // For test
#include <QFileDialog>
using namespace std;

PathModel::PathModel(QObject *parent, int _m_nWd, int _m_nHt)
    : QAbstractTableModel(parent), m_nWd(_m_nWd), m_nHt(_m_nHt), m_strFileName(""),m_strStartText(""),m_strGoalText("")
{
        vector<vector<int>> data = {{}};
        m_gridData = data;//loadData("");
}
int PathModel::rowCount(const QModelIndex &parent) const
{
        if (parent.isValid())
            return 0;

        return m_gridData.size();
}
int PathModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_gridData[0].size();
}
QVariant PathModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() )
        return QVariant();
    return QVariant(m_gridData[index.row()][index.column()]);
}

//Triggerd by QML button to select a json file, if valid, it will populate data & process
// Once processed, trigger view for update
void PathModel::loadfile(){
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        "Select a file",
        "",
        "All Files (*.json)"
        );
    //File check
    if (!fileName.isEmpty()) {
        qDebug() << "Selected file:" << fileName;
    } else {
        qDebug() << "No file selected";
        return;
    }
    //Process file
    m_strFileName = fileName;
    m_gridData = loadData(m_strFileName);
    m_nWd = m_gridData[0].size();
    m_nHt = m_gridData.size();
    //Update view for refresh
    emit changeOfStatus();
    emit layoutChanged();
    emit dataChanged(index(0, 0), index(m_gridData[0].size()-1,m_gridData.size()-1), {CellRole});
}

//loadData() uses internal data provider for loading the data - in this case json data provider
vector<vector<int>> PathModel::loadData(QString strFileToOpen)
{
    vector<vector<int>> newData;
    vector<int> newRow;

    vector<int> vecJsonData = m_objJsonDataProvider.loadData(strFileToOpen);
    //Clean JSON Data & adjust width, height
     m_nWd = round( sqrt(vecJsonData.size()));
     m_nHt = ceil( sqrt(vecJsonData.size()));

     bool bFoundStart = false;
     bool bFoundGoal = false;
     for (auto it = vecJsonData.begin(); it != vecJsonData.end(); ++it) {
        switch(*it)
        {
        case 0: // start
            if(bFoundStart) *it = -1; else bFoundStart = true;
                break;
        case 8: //goal
               if(bFoundGoal) *it = -1; else bFoundGoal = true;
            break;
        case 3: //not walkable
            break;
        default: //walkable
            *it = -1;
            break;
        }
     }
     // Pad data
     int nPad = (m_nWd * m_nHt) - vecJsonData.size();
     for (int nIdx = 0; nIdx < nPad; ++nIdx) {
        vecJsonData.push_back(-1);
     }
        //Use Randomize testing
        //CPosition start(QRandomGenerator::global()->generate()%m_nWd, QRandomGenerator::global()->generate()%m_nHt);
        //CPosition goal(QRandomGenerator::global()->generate()%m_nWd, QRandomGenerator::global()->generate()%m_nHt);
        CPosition start(-1,-1);
        CPosition goal(-1,-1);

        vector<vector<CGridItem>> grid;
        vector<CGridItem> testGridItems1;
        for(int i = 0; i < m_nHt; i++)
        {
            for(int j = 0; j < m_nWd; j++)
            {
                //Use Randomize testing
                //int vVal = QRandomGenerator::global()->generate()%9;
                //testGridItems1.push_back(CGridItem(i,j, vVal));
                if(vecJsonData[i * m_nWd + j] == 0)
                {
                    start.m_nXPos = i; start.m_nYPos = j;
                }
                else if(vecJsonData[i * m_nWd + j] == 8)
                {
                    goal.m_nXPos = i; goal.m_nYPos = j;
                }
                qDebug() << " : " << vecJsonData[i * m_nWd + j];
                testGridItems1.push_back(CGridItem(j,i, vecJsonData[i * m_nWd + j]));
            }
            grid.push_back(testGridItems1);
            testGridItems1.clear();
        }

        //Process data acquired from provider - this can be separated & checked early
        m_objSearchContext.setStrategy(&m_objAstrategy);
        bool bFound = m_objSearchContext.executeStrategy(grid, m_nWd, m_nHt, start, goal);
        qDebug() << "======================================" << bFound ;
        m_strStartText = QString("<b>Start</b>(Green) X: %1, Y: %2").arg(start.m_nYPos).arg(start.m_nXPos);
        m_strGoalText = QString("<b>Goal</b>(Blue) X: %1, Y: %2").arg(goal.m_nYPos).arg(goal.m_nXPos);
        for(int i = 0; i < m_nHt; i++)
        {
            for(int j = 0; j < m_nWd; j++)
            {
                newRow.push_back(grid[i][j].nGridValue);
            }
            newData.push_back(newRow);
            newRow.clear();
        }
        return newData;
    }

