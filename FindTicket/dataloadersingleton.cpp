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
    /* Method parses citiesJson and stores parsed cities list in DataLoader::cities */
    QJsonObject citiesJson(readJsonDocument(this->citiesJson));
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
                cities->push_back(citiesArr.takeAt(it.i).toString());
            }
        }
    }
    else exit(0);
}

void DataLoaderSingleton::loadCitiesDistances()
{
    /* Method loads distances.json. This is needed for findExtra{%transportType%}Tickets */
    QJsonObject distancesDictionary(readJsonDocument(this->distancesJson));
    char key[10] = "distances";
    auto keys = distancesDictionary.keys();
    if (keys.contains(key))
    {
        *distances = distancesDictionary.take(key);
    }
    else exit(0);

}



