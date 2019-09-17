#include "dataloader.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QDir>


DataLoader::DataLoader()
{
    // Set current directory
    QDir::setCurrent(DataLoader::sourcesPath);
}

DataLoader::~DataLoader() { }

int DataLoader::readJsonDocument(QString & documentName)
{
    /* Method reads JSON file to DataLoader::currentReadJson.
     * Return 0 or 1 - status code.
     */
    QFileInfo jsonFileInfo(documentName);
    QFile jsonFile(documentName);

    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        return 0;
    }
    QTextStream stream(&jsonFile);
    QString jsonString = stream.readAll();
    jsonFile.close();
    QByteArray dataBytes = jsonString.toLocal8Bit();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(dataBytes);
    if (jsonDocument.isNull())
    {
        qDebug() << "Failed to creare JSON document";
        return 0;
    }
    currentJsonObject = jsonDocument.object();
    if (currentJsonObject.isEmpty())
    {
        qDebug() << "JSON object is empty";
        return 0;
    }
    qDebug() << currentJsonObject.keys();
    return 1;
}

// TODO: add methods for parsing currentJSONObject (get value by key, etc)
// For MainWindow::fillUIControls - json co


