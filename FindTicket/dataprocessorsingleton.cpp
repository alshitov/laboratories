#include "dataprocessorsingleton.h"
#include <mainwindow.h>
#include <dataprocessorsingleton.h>
#include <loader.h>

QJsonObject* DataProcessorSingleton::aviaSearch(QMap<QString, QString> &settings)
{
    /* Tickets search strategy:
     * 1) Strict serach: from given city to given city;
     * 2) Non-strict search: considering distances between cities,
     *    suggest tickets from nearby cities;
     * These two tickets array are then merged together;
     */
    qDebug() << settings;
    QJsonObject ticketsToSearchIn = loader::dls.readJsonDocument(loader::dls.aviaSourcesJson);
    QJsonObject *aviaTickets       = new QJsonObject(),
                *nonStrictTickets  = new QJsonObject();

    QJsonArray *strictTickets       = new QJsonArray(),
               *byDateTickets       = new QJsonArray(),
               *byRateTickets       = new QJsonArray(),
               *byDepCityTickets    = new QJsonArray(),
               *byDestCityTickets   = new QJsonArray(),
               *byNearCitiesTickets = new QJsonArray();



    // Prepare settings fields
    QString s_depCity  = settings["depCity"],
            s_destCity = settings["destCity"],
            s_rate     = settings["rate"],
            s_depDate  = this->unifyDate(settings["depDate"]);

    foreach (const QString &key, ticketsToSearchIn.keys())
    {
        QJsonValue ticket = ticketsToSearchIn.value(key);
        Q_ASSERT(ticket.isObject());

        // Prepare ticket fields
        QString t_depCity  = ticket["departure_place"].toString(),
                t_destCity = ticket["arrival_place"].toString(),
                t_rate     = ticket["rate"].toString(),
                t_depDate  = ticket["departure_date"].toString();

        bool depCitiesMatch = s_depCity == t_depCity,
             destCitiesMatch = s_destCity == t_destCity,
             ratesMatch = s_rate == t_depDate,
             datesMatch = s_depDate == t_depDate;

        // Compare fields and separate tickets by categories

        if (depCitiesMatch && destCitiesMatch && ratesMatch && datesMatch)
        {
            // Full compliance
            strictTickets->append(ticket);
        }
        else if (depCitiesMatch && destCitiesMatch && datesMatch)
        {
            // With different rate
            byRateTickets->append(ticket);
        }
        else if (destCitiesMatch && datesMatch)
        {
            // With nearby date

        }
        else
        {
            QList<QString>* nearbyDepCities = findNearbyCities(s_depCity);
            QList<QString>* nearbyDestCities = findNearbyCities(s_destCity);

            if (destCitiesMatch && ratesMatch && datesMatch)
            {
                // With nearby department cities
                if (nearbyDepCities->contains(t_depCity))
                {
                    byDepCityTickets->append(ticket);
                }
            }
            else if (depCitiesMatch && datesMatch)
            {
                // With nearby destination cities
                if (nearbyDestCities->contains(t_depCity))
                {
                    byDestCityTickets->append(ticket);
                }
            }
            else if (ratesMatch && datesMatch)
            {
                // With nearby destination & department cities
                byNearCitiesTickets->append(ticket);
            }
        }
    }

    // Collect tickets
    aviaTickets->insert(this->strict, *strictTickets);
    aviaTickets->insert(this->nonStrict, *nonStrictTickets);
    nonStrictTickets->insert(this->byRate, *byRateTickets);
    nonStrictTickets->insert(this->byDate, *byDateTickets);
    nonStrictTickets->insert(this->byDepCity, *byDepCityTickets);
    nonStrictTickets->insert(this->byDestCity, *byDestCityTickets);
    nonStrictTickets->insert(this->byNearCities, *byNearCitiesTickets);


    // Clear mem
    delete strictTickets;
    delete nonStrictTickets;
    delete aviaTickets;
    delete byDepCityTickets;
    delete byDestCityTickets;
    delete byDateTickets;
    delete byRateTickets;

    return aviaTickets;
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

QList<int>* DataProcessorSingleton::findNearbyDates(const QString &date)
{
    /* Method returns date boundaries in which the ticket should
     * take place so that it is considered as byDate group ticket;
     */
    QList<int> *dateBoundaries = new QList<int>;
    return dateBoundaries;
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
