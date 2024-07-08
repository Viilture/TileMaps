#ifndef SVOITARGET_H
#define SVOITARGET_H

#include <QObject>

#include "settingsBR.h"

/** @class Класс цели после вторичной обработки */
class SvoiTarget : public QObject
{
    Q_OBJECT

public:
    explicit SvoiTarget(QObject *parent = nullptr);
    virtual ~SvoiTarget();

    /** @brief ПОЛЯРНЫЕ КООРДИНАТЫ */
    /** @brief Дальность до цели в метрах */
    int32_t distance = 0;

    /** @brief Азимут до цели относительно Севера */
    double azimuth = 0;

    /** @brief Угломесто до цели относительно направления вверх */
    double uglomesto = 0;

    /** @brief Ширина спектра цели*/
    double widthSpectr = 0;

    /** @brief Зоновые координаты*/
    /** @brief координаты внутри зоны в метрах*/
    double x = 321678;
    double y = 4304568;
    /** @brief номер зоны*/
    int32_t zone = 18;
    /** @brief северное или южное полушарие */
    bool northp = 1;

};

#endif
