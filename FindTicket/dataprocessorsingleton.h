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

    // Cities distance limit (km) for non-strict search
    int aviaCitiesDistanceLimit = 150;
    int railwayCitiesDistanceLimit = 100;
    int busCitiesDistanceLimit = 50;
    int trainCitiesDistanceLimit = 30;

    // Tickets QJsonObject keys

    // Tickets with full settings compliance
    QString strict = "strict";
    // Tickets with part settings compliance
    QString nonStrict = "nonStrict";

    // Ticket department city is near to the one user wants
    QString byDepCity = "byDepCity";
    // Ticket destination city is near to the one user wants
    QString byDestCity = "byDestCity";
    // Ticket destination & department cities are near to the one user wants
    QString byNearCities = "byNearCities";
    // Ticket department date is near the one user wants
    QString byDate = "byDate";
    // Only ticket rate is different
    QString byRate = "byRate";

    QString unifyDate(const QString &rawDateStr);
    QList<QString>* findNearbyCities(const QString &city);
    int findNearbyDates(const QString &date);


    QJsonArray* aviaSearch(QMap<QString, QString> &settings);
    QJsonObject* railwaySearch(QMap<QString, QString> &settings);
    QJsonObject* busSearch(QMap<QString, QString> &settings);
    QJsonObject* trainSearch(QMap<QString, QString> &settings);
};

#endif // DATAPROCESSOR_H
