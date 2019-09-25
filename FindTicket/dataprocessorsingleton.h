#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H
#include <new>
#include <loader.h>
#include <QtCore>

class DataProcessorSingleton
{
private:
    DataProcessorSingleton() = default;
    ~DataProcessorSingleton() = default;

    DataProcessorSingleton(const DataProcessorSingleton&) = delete;
    DataProcessorSingleton& operator=(const DataProcessorSingleton&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;

    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

public:
    static DataProcessorSingleton& getInstance()
    {
        static DataProcessorSingleton object;
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

};

#endif // DATAPROCESSOR_H
