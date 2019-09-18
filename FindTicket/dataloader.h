#ifndef DATALOADER_H
#define DATALOADER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <iostream>

class DataLoader
{
public:
    // Fields
    QString sourcesPath = "/home/alexander/Desktop/C++ Qt/cpp_labs/FindTicket/data/" ;
    QString citiesJson         = "cities.json";
    QString aviaSourcesJson    = "avia.json";
    QString railwaySourcesJson = "railway.json";
    QString busSourcesJson     = "bus.json";
    QString trainSourcesJson   = "train.json";

    QStringList cities;
    QJsonObject currentJsonObject;

    // Methods
    DataLoader();
    ~DataLoader();
    QJsonObject readJsonDocument(QString & documentPath);
    void loadCitiesList();
};

#endif // DATALOADER_H
