#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <vector>
#include <QString>

using namespace std;

//DataProvider is a base class that can be extended for loading data from different sources
// This can be still modularized for differnt type of sources - currently it assumes loading file
class DataProvider
{
public:
    DataProvider();
    virtual vector<int> loadData(QString strFileToOpen) = 0;
};

//Specific data provider that reads data from riskylab tilemap compatible json
class JsonDataProvider : public DataProvider {
public:
    vector<int> loadData(QString strFileToOpen) override;
};

#endif // DATAPROVIDER_H
