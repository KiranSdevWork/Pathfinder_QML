#ifndef PATHMODEL_H
#define PATHMODEL_H

#include <array>
#include <QAbstractTableModel>
#include <QPoint>
#include <QtQml/qqml.h>
#include <vector>
#include "dataprovider.h"
#include "graphsearchstrategy.h"
#include "graphsearchcontext.h"

using namespace std;

//PathModel is a C++ implementation of the data provider Model
// This is used in QML Tableview
// Sources data from different providers (abstracted from view)
// On the data courced from earlier step, specific algorithm is used for processing
// Once data is ready, events are triggered for QML to refresh view
class PathModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY( QString theChange READ getFileName NOTIFY changeOfStatus )
    Q_PROPERTY( QString startText READ getStart NOTIFY changeOfStatus )
    Q_PROPERTY( QString goalText READ getGoal NOTIFY changeOfStatus )
public:
    enum Roles {
        CellRole
    };

    QHash<int, QByteArray> roleNames() const override {
        return {
            { CellRole, "value" }
        };
    }
    explicit PathModel(QObject *parent = nullptr, int _nWd=-1, int _nHt=-1);

    //QAbstractTableModel methods used internally by view to get data
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //Get file data from provider
    vector<vector<int>> loadData(QString strFileToOpen);

    //Triggered when user selects a file
    Q_INVOKABLE void loadfile();

    //getters for the properties
    QString getFileName() {
        return m_strFileName;
    }
    QString getStart() {
        return m_strStartText;
    }
    QString getGoal() {
        return m_strGoalText;
    }
signals:
    void changeOfStatus() ; //trigger view refresh

private:
    //m_gridData : grid data populated by data provider,
    //  used by algorithm for processing,
    //  later used by view grid cells for display
    std::vector<std::vector<int>> m_gridData;
    //Width & Height of the grid
    int m_nWd;
    int m_nHt;
    //Algorithm classes used on source data from data provider
    GraphSearchContext m_objSearchContext;
    ASearch m_objAstrategy;
    //Data Provider classes used for getting the data
    JsonDataProvider m_objJsonDataProvider;
    //Properties used by QML for display
    QString m_strFileName;
    QString m_strStartText;
    QString m_strGoalText;
};

#endif // PATHMODEL_H
