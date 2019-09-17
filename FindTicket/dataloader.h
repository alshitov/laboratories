#ifndef DATALOADER_H
#define DATALOADER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

class DataLoader
{
public:
    // Fields
    QString sourcesPath = "/home/alexander/Desktop/C++ Qt/cpp_labs/FindTicket/data/" ;
    QString aviaSourcesJSON    = "avia.json";
    QString railwaySourcesJSON = "railway.json";
    QString busSourcesJSON     = "bus.json";
    QString trainSourcesJSON   = "train.json";

    QJsonObject currentJsonObject;

    // Methods
    DataLoader();
    ~DataLoader();
    int readJsonDocument(QString & documentPath);
};

#endif // DATALOADER_H
