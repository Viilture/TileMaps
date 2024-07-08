#ifndef MAPRADAR_H
#define MAPRADAR_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QtMath>
#include <QPointF>
#include <QTimer>
#include <QString>
#include <QPropertyAnimation>

#include "mapcontrol.h"
#include "guts/Conversions.h"
#include "scalc.h"

/** @class Класс отображения Радара на Scene */
class mapRadar : public QGraphicsPathItem
{
    /** @brief Координаты в долготе и широте */
    QPointF centerP    = QPointF(0,0);
    double  azimut     = 0;
    double  sector     = 120;
    double  beginRange = 100;
    double  endRange   = 2500;

    QSharedPointer<QPropertyAnimation> pAnimation;

    /** @brief Класс преобразования координат*/
    Conversions conv;
    /** @brief Класс управления картой */
    MapControl* mapContr = nullptr;
    /** @brief Класс библиотеки географических расчетов */
    calculate::Scalc calc;

public:
    explicit mapRadar(MapControl* newMapContr = nullptr,
                      QPointF CenterP   = QPointF(37, 55),
                      double azimut     = 0,
                      double sector     = 120,
                      double beginRange = 100,
                      double endRange   = 2500);
    virtual ~mapRadar();


    QPointF getCenterP() const;

    /** @brief Устанавливаем центр РЛС в широте и долготе*/
    void setCenterP(QPointF newCenterP);

public Q_SLOTS:
    /** @brief Обновление графики */
    void updateGraph();

    /** @brief Обновить параметры отображения РЛУ */
    void setSettings();

    /** @brief Обновить параметры азимута */
    void setAzim(double azim);

};

#endif // MAPRADAR_H
