#include "scalc.h"

namespace calculate {
Scalc::Scalc(QObject *parent) : QObject(parent)
{

}

Scalc::~Scalc()
{

}

QPointF Scalc::WGS84calcPosOnDistance(double lon, double lat, double dist, double azim)
{
    double end_latitude = 0;
    double end_longitude = 0;
    try
    {
        // Создание объекта геодезической линии
        GeographicLib::GeodesicLine line(geod, lat, lon, azim);

        // Вычисление координат конечной точки
        line.Position(dist, end_latitude, end_longitude);
    }
    catch (...)
    {

        end_latitude = 0;
        end_longitude = 0;
        qDebug() << "Error Scalc::WGS84calcPosOnDistance";
        qDebug() << "lon - " << lon << " lat - " << lat << " dist - " << dist << " azim - " << azim;
    }

    return QPointF(end_longitude, end_latitude);
}

double Scalc::distance(double lon1, double lat1, double lon2, double lat2)
{
    double dist = 0;
    try
    {
        geod.Inverse(lat1, lon1, lat2, lon2, dist);
    }
    catch (...)
    {

        dist = 0;
        qDebug() << "Error Scalc::distance";
        qDebug() << "lon1 - " << lon1 << " lat1 - " << lat1 << " lon2 - " << lon2 << " lat2 - " << lat2;
    }

    return dist;
}

/** @brief Перевод геоцентрических координат WGS84 в геодезические координаты WGS84*/
void Scalc::WGS84GeocentrToWGS84Geodez(double x, double y, double z, double &lon, double &lat, double &height)
{
    try
    {
        earthWGS84.Reverse(x, y, z, lat, lon, height);
    }
    catch (...)
    {

        qDebug() << "Error Scalc::WGS84GeocentrToWGS84Geodez";
        qDebug() << "x - " << x << " y - " << y << " lon - " << lon << " lat - " << lat << " height - " << height;
    }
}

/** @brief Перевод геодезических координат WGS84 в геоцентрические координаты WGS84 */
void Scalc::WGS84GeodezToWGS84Geocentr(double lon, double lat, double height, double& x, double& y, double& z)
{
    try
    {
        earthWGS84.Forward(lat, lon, height, x, y, z);
    }
    catch (...)
    {

        qDebug() << "Error Scalc::WGS84GeodezToWGS84Geocentr";
        qDebug() << "lon - " << lon << " lat - " << lat << " height - " << height << " x - " << x << " y - " << y << " z - " << z;
    }
}

/** @brief Перевод геодезических координат WGS84 в зональной координаты WGS84 */
void Scalc::WGS84GeodezToWGS84Zone(double lat, double lon, int32_t& zone, bool& north, double& x, double& y)
{
    try
    {
        GeographicLib::UTMUPS::Forward(lat, lon, zone, north, x, y);
    }
    catch (...)
    {

        qDebug() << "Error Scalc::WGS84GeodezToWGS84Zone";
        qDebug() << "lat - " << lat << " lon - " << lon << " zone - " << zone << " north - " << north << " x - " << x << " y - " << y;
    }
}

/** @brief Перевод зональных координат WGS84 в геодезические координаты WGS84 */
void Scalc::WGS84ZoneToWGS84Geodez(int32_t zone, bool north, double x, double y, double& lat, double& lon)
{
    try
    {
        GeographicLib::UTMUPS::Reverse(zone, north, x, y, lat, lon);
    }
    catch (...)
    {

        qDebug() << "Error Scalc::WGS84ZoneToWGS84Geodez";
        qDebug() << "zone - " << zone << " north - " << north << " x - " << x << " y - " << y << " lat - " << lat << " lon - " << lon;
    }
}
}
