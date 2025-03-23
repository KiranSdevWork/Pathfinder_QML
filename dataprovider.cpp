#include "dataprovider.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace std;
DataProvider::DataProvider() {}

//loadData() is used for loading data from Riskylab compatible json files
// This can also be updated for generic json file loader, so it wont be restricted to Riskylab format
// NOTE : tightly coupled to process Riskylab json
vector<int> JsonDataProvider::loadData(QString strFileToOpen)
{
    // vecJsonData will be populated by json [data] content - first instance in the file, later will be ignored
    vector<int> vecJsonData;
    bool    bJsonDataAcquired = false;

    //Check file validity & read content if valid
    QFile file(strFileToOpen);
    QString errMsg;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errMsg = file.errorString();
        qDebug() << "Could not open the file." << errMsg;
        return vecJsonData;
    }
    if (strFileToOpen.isEmpty()) return vecJsonData;
    QByteArray jsonData = file.readAll();
    file.close();

    //Load JSON data & extract data in single vector
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(jsonData, &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Error parsing JSON:" << error.errorString();
        return vecJsonData;
    }
    if (document.isObject()) {
        QJsonObject obj = document.object();
        QString key;
        QJsonValue value;
        foreach (key, obj.keys()) {
            if(bJsonDataAcquired) break;
                value = obj.value(key);
            if(key == "canvas")
                continue;
            else if(key == "layers")
                qDebug() << key;
            else if(key == "tilesets")
                continue;

            if (value.isString()) {
                continue;
            } else if (value.isDouble()) {
                continue;
            } else if (value.isBool()) {
                continue;
            } else if (value.isObject()) {
                continue;
            } else if (value.isArray()) {
                QJsonArray jArr(value.toArray());
                for (int i = 0; i < jArr.size(); ++i)
                {
                    if(bJsonDataAcquired) break;
                    if (jArr[i].isObject()) {
                        QJsonObject obj = jArr[i].toObject();
                        //if(obj.keys().count() == 3)
                        {
                            // Iterate through the keys and values
                            for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
                                QString key = it.key();
                                if(key == "data")
                                {
                                    QJsonValue value = it.value();
                                    if (value.isArray()) {
                                        QJsonArray grades = value.toArray();
                                        for (const auto& grade : grades) {
                                            vecJsonData.push_back(grade.toInt());//qDebug() << grade.toInt();
                                        }
                                        bJsonDataAcquired = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

            } else {
                qDebug() << key << ": null";
            }
        }
    }
    // EO JSON Parseer
    return vecJsonData;
}
