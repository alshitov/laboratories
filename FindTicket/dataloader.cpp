#include "dataloader.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QList>
#include <QDir>

DataLoader::DataLoader()
{
    // Set current directory from where to read files
    QDir::setCurrent(DataLoader::sourcesPath);
}

DataLoader::~DataLoader() { }

QJsonObject DataLoader::readJsonDocument(QString & documentName)
{
    QFileInfo jsonFileInfo(documentName);
    QFile jsonFile(documentName);

    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open " + documentName;
        exit(0);
    }
    QTextStream stream(&jsonFile);
    QString jsonString(stream.readAll());
    jsonFile.close();
    QByteArray dataBytes(jsonString.toLocal8Bit());
    QJsonDocument jsonDocument(QJsonDocument::fromJson(dataBytes));
    if (jsonDocument.isNull())
    {
        qDebug() << "Failed to creare JSON document";
        exit(0);
    }
    QJsonObject object(jsonDocument.object());
    return object;
}

void DataLoader::loadCitiesList()
{
    /* Method parses citiesJson and stores parsed cities list in DataLoader::cities */
    QJsonObject citiesJson(DataLoader::readJsonDocument(DataLoader::citiesJson));
    char key[7] = "cities";
    auto keys = citiesJson.keys();
    if (keys.contains(key))
    {
        QJsonValue citiesValue = citiesJson.take(key);
        if (citiesValue.isArray())
        {
            QJsonArray citiesArr = citiesValue.toArray();
            for (auto it = citiesArr.begin(); it != citiesArr.end(); ++it)
            {
                DataLoader::cities.push_back(citiesArr.takeAt(it.i).toString());
            }
        }
    }
    else
    {
        qDebug() << "cities.json does not contain 'cities' key!";
        exit(0);
    }
}
