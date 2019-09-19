#ifndef DATALOADER_H
#define DATALOADER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QDir>
#include <iostream>

class DataLoader
{
    /* DataLoader class represents classic Meyer's Singleton pattern.
     * Instead of DataLoader, classes providing DataBase
     * connection could be implemented here.
     */

//    static DataLoader *_loader;
//    DataLoader()
//    {
//        QDir::setCurrent(sourcesPath);
//    }


public:
//    static DataLoader *getInstance()
//    {
//        if (_loader == nullptr)
//            _loader = new DataLoader();
//        return _loader;
//    }

    QString sourcesPath = "/home/alexander/Desktop/C++ Qt/cpp_labs/FindTicket/data/" ;
    QString citiesJson         = "cities.json";
    QString aviaSourcesJson    = "avia.json";
    QString railwaySourcesJson = "railway.json";
    QString busSourcesJson     = "bus.json";
    QString trainSourcesJson   = "train.json";

    QStringList cities;
    QJsonObject currentJsonObject;

    DataLoader();
    ~DataLoader();

    QJsonObject readJsonDocument(QString & documentPath);
    void loadCitiesList();
    void loadCitiesDistances();
};

#endif // DATALOADER_H
