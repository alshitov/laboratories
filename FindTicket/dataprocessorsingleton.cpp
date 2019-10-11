#include "dataprocessorsingleton.h"
#include <mainwindow.h>
#include <dataprocessorsingleton.h>
#include <loader.h>

QJsonArray* DataProcessorSingleton::aviaSearch(QMap<QString, QString> &settings)
{
    QJsonObject ticketsToSearchIn = loader::dls.readJsonDocument(loader::dls.aviaSourcesJson);
    QJsonArray *ticketsArray = new QJsonArray();

    QString s_depCity  = settings["depCity"],
            s_destCity = settings["destCity"];

    QList<QString>* apprDepCities = findNearbyCities(s_depCity);
    QList<QString>* apprDestCities = findNearbyCities(s_destCity);
    foreach (const QString &key, ticketsToSearchIn.keys())
    {
        QJsonValue ticket = ticketsToSearchIn.value(key);
        Q_ASSERT(ticket.isObject());
        QString t_depCity  = ticket["departure_place"].toString(),
                t_destCity = ticket["arrival_place"].toString();
        if (apprDepCities->contains(t_depCity) && apprDestCities->contains(t_destCity))
            ticketsArray->append(ticket);
    }
    return ticketsArray;
}

QList<QString>* DataProcessorSingleton::findNearbyCities(const QString &city)
{
    QList<QString> *nearbyCities = new QList<QString>();
    QJsonObject cityInfo = loader::dls.distances->toObject().take(city).toObject();

    foreach (const QString &city, cityInfo.keys())
    {
        double distance = cityInfo.value(city).toDouble();
        if (distance <= this->aviaCitiesDistanceLimit)
            nearbyCities->push_back(city);
    }
    return nearbyCities;
}

QJsonArray* DataProcessorSingleton::railwaySearch(QMap<QString, QString> &settings)
{
    qDebug() << settings;
    QJsonArray* railwayTickets = new QJsonArray();
    return railwayTickets;
}

QJsonArray* DataProcessorSingleton::busSearch(QMap<QString, QString> &settings)
{
    qDebug() << settings;
    QJsonArray* busTickets = new QJsonArray();
    return busTickets;
}

QJsonArray* DataProcessorSingleton::trainSearch(QMap<QString, QString> &settings)
{
    qDebug() << settings;
    QJsonArray* trainTickets = new QJsonArray();
    return trainTickets;
}
