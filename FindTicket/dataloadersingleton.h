#ifndef DATALOADERSINGLETON_H
#define DATALOADERSINGLETON_H

#include <new>
#include <QtCore>

class DataLoaderSingleton
{
private:
    DataLoaderSingleton() = default;
    ~DataLoaderSingleton() = default;

    DataLoaderSingleton(const DataLoaderSingleton&) = delete;
    DataLoaderSingleton& operator=(const DataLoaderSingleton&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;

    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

public:
    static DataLoaderSingleton& getInstance()
    {
        static DataLoaderSingleton object;
        return object;
    }

    QString sourcesPath = "/home/alexander/Desktop/C++ Qt/cpp_labs/FindTicket/data/" ;
    QString citiesJson         = "cities.json";
    QString aviaSourcesJson    = "avia.json";
    QString railwaySourcesJson = "railway.json";
    QString busSourcesJson     = "bus.json";
    QString trainSourcesJson   = "train.json";

    QStringList cities;
    QJsonObject currentJsonObject;

    QJsonObject readJsonDocument(QString & documentPath);
    void loadCitiesList();
    void loadCitiesDistances();
};


#endif // DATALOADERSINGLETON_H
