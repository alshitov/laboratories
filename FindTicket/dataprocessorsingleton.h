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

    bool is_searching = false;
    bool isSearching() { return this->is_searching; }

    void process();
    void stop();

    int aviaCitiesDistanceLimit = 150;
    int railwayCitiesDistanceLimit = 100;
    int busCitiesDistanceLimit = 50;
    int trainCitiesDistanceLimit = 30;

    QString unifyDate(const QString &rawDateStr);
    QList<QString>* findNearbyCities(const QString &city);
    int findNearbyDates(const QString &date);


    QJsonArray* aviaSearch(QMap<QString, QString> &settings);
    QJsonArray* railwaySearch(QMap<QString, QString> &settings);
    QJsonArray* busSearch(QMap<QString, QString> &settings);
    QJsonArray* trainSearch(QMap<QString, QString> &settings);

signals:
    void finished();
};

#endif // DATAPROCESSOR_H
