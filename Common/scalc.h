#ifndef SCALC_H
#define SCALC_H

#include <QObject>
#include <QDebug>
#include <QPointF>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/GeodesicLine.hpp>
#include <GeographicLib/Constants.hpp>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/UTMUPS.hpp>


namespace calculate {
/** @class Класс всех расчетов производимых в данной программе */
class Scalc : public QObject
{
    Q_OBJECT

    /** @brief Обьект геоида для WGS84*/
    GeographicLib::Geodesic geod = GeographicLib::Geodesic(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());

    GeographicLib::Geocentric earthWGS84 = GeographicLib::Geocentric::WGS84();


public:
    explicit Scalc(QObject *parent = nullptr);
    virtual ~Scalc();

    /** @brief WGS84 Вычисление точки на заданном расстоянии и азимуте от координаты
        @param координаты
        @param  координаты
        @param Расстояние в метрах
        @param Азимут в градусах*/
    QPointF WGS84calcPosOnDistance(double lat, double lon, double dist, double azim);

    /** @brief WGS84 Вычисление расстояния */
    double distance(double lon1, double lat1, double lon2, double lat2);

    /** @brief Перевод геодезических координат WGS84 в геоцентрические координаты WGS84 */
    void WGS84GeodezToWGS84Geocentr(double lon1, double lat1, double height, double& x, double& y, double& z);

    /** @brief Перевод геоцентрических координат WGS84 в геодезические координаты WGS84*/
    void WGS84GeocentrToWGS84Geodez(double x, double y, double z, double& lon1, double& lat1, double& height);

    /** @brief Перевод геодезических координат WGS84 в зональные координаты WGS84 */
    void WGS84GeodezToWGS84Zone(double lat, double lon, int32_t& zone, bool& north, double& x, double& y);

    /** @brief Перевод зональных координат WGS84 в геодезические координаты WGS84 */
    void WGS84ZoneToWGS84Geodez(int32_t zone, bool north, double x, double y, double& lat, double& lon);

};
}
#endif // SCALC_H
