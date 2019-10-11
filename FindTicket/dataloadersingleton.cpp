#include <QtCore>
#include <dataloadersingleton.h>

QJsonObject DataLoaderSingleton::readJsonDocument(QString & documentName)
{
    QDir::setCurrent(DataLoaderSingleton::sourcesPath);
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

void DataLoaderSingleton::loadCitiesList()
{
    QJsonObject citiesJson = readJsonDocument(this->citiesJson);
    char key[7] = "cities";
    auto keys = citiesJson.keys();
    if (keys.contains(key))
    {
        QJsonArray citiesArr = citiesJson.take(key).toArray();
        foreach (const QJsonValue &value, citiesArr)
        {
            cities->push_back(value.toString());
        }
    }
    else exit(0);
}

void DataLoaderSingleton::loadCitiesDistances()
{
    QJsonObject distancesDictionary(readJsonDocument(this->distancesJson));
    char key[10] = "distances";
    auto keys = distancesDictionary.keys();
    if (keys.contains(key))
    {
        *distances = distancesDictionary.take(key);
    }
    else exit(0);
}
