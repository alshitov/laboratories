#include "dataprocessorsingleton.h"
#include <mainwindow.h>
#include <dataprocessorsingleton.h>
#include <loader.h>

QJsonArray* DataProcessorSingleton::aviaSearch(QMap<QString, QString> &settings)
{
    /* Tickets search strategy:
     * 1) Strict serach: from given city to given city;
     * 2) Non-strict search: considering distances between cities,
     *    suggest tickets from nearby cities;
     * These two tickets array are then merged together;
     */
    qDebug() << settings;
    QJsonObject ticketsToSearchIn = loader::dls.readJsonDocument(loader::dls.aviaSourcesJson);
    qDebug() << ticketsToSearchIn.length();
    QJsonArray *ticketsArray = new QJsonArray();

    // Prepare settings fields
    QString s_depCity  = settings["depCity"],
            s_destCity = settings["destCity"],
            s_rate     = settings["rate"],
            s_depDate  = this->unifyDate(settings["depDate"]);


    // Appropriate settings
    QList<QString>* apprDepCities = findNearbyCities(s_depCity);
    QList<QString>* apprDestCities = findNearbyCities(s_destCity);

    QStringList s_dateSplit = s_depDate.split('/');
    int targetMonth = s_dateSplit[0].toInt(),
        targetDay = s_dateSplit[1].toInt();

    foreach (const QString &key, ticketsToSearchIn.keys())
    {
        QJsonValue ticket = ticketsToSearchIn.value(key);
        Q_ASSERT(ticket.isObject());

        // Prepare ticket fields
        QString t_depCity  = ticket["departure_place"].toString(),
                t_destCity = ticket["arrival_place"].toString(),
                t_rate     = ticket["rate"].toString(),
                t_depDate  = ticket["departure_datetime"].toString();
        t_depDate = t_depDate.left(t_depDate.lastIndexOf("/") + 5);

        // If cities (dep and dest are in appropriate ones)
        if (apprDepCities->contains(t_depCity) && apprDestCities->contains(t_destCity))
        {
            qDebug() << ticket;

//            QStringList ticketDateSplit = t_depDate.split('/');
//            if (ticketDateSplit[0] == targetMonth)
//            {
//                int ticketDay = ticketDateSplit[1].toInt();
//                QList<int>* dayBoundaries = new QList<int> { ticketDay - 10, ticketDay + 10 };
//                if (dayBoundaries->contains(targetDay))
//                {
//                    ticketsArray->append(ticket);
//                }
//            }
        }
        // If date is appropriate (same month +- 10 days)

    }

    qDebug() << *ticketsArray;
    return ticketsArray;
}

QString DataProcessorSingleton::unifyDate(const QString &rawDateStr)
{
    /* Simply add "20" to year start: "19" -> "2019" */
    int yearSubstStart = rawDateStr.lastIndexOf("/") + 1;
    QString yearCasted = rawDateStr.mid(yearSubstStart, 2);
    yearCasted.push_front("20");
    return rawDateStr.left(yearSubstStart) + yearCasted;
}


QList<QString>* DataProcessorSingleton::findNearbyCities(const QString &city)
{
    /* Method returns list of cities that are in aviaCitiesDistanceLimit
     * radius of destination or department city;
     */
    QList<QString> *nearbyCities = new QList<QString>();
    QJsonObject cityInfo = loader::dls.distances->toObject().take(city).toObject();

    foreach (const QString &city, cityInfo.keys())
    {
        double distance = cityInfo.value(city).toDouble();
        if (distance <= this->aviaCitiesDistanceLimit)
        {
            nearbyCities->push_back(city);
        }
    }
    return nearbyCities;
}

int DataProcessorSingleton::findNearbyDates(const QString &date)
{
    /* Method returns date boundaries in which the ticket should
     * take place so that it is considered as byDate group ticket;
     * Nearby date is searched by month (+-10days)
     */
    int month_start = date.indexOf('/');
    int month_end = date.lastIndexOf('/');
    int month = date.mid(month_start, month_end).toInt();
    qDebug() << month;
    return month;
}



QJsonObject* DataProcessorSingleton::railwaySearch(QMap<QString, QString> &settings)
{
    qDebug() << settings;
    QJsonObject* railwayTickets = new QJsonObject();
    return railwayTickets;
}

QJsonObject* DataProcessorSingleton::busSearch(QMap<QString, QString> &settings)
{
    qDebug() << settings;
    QJsonObject* busTickets = new QJsonObject();
    return busTickets;
}

QJsonObject* DataProcessorSingleton::trainSearch(QMap<QString, QString> &settings)
{
    qDebug() << settings;
    QJsonObject* trainTickets = new QJsonObject();
    return trainTickets;
}
