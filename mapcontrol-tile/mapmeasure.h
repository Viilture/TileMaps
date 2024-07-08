#ifndef MAPMEASURE_H
#define MAPMEASURE_H

#include <QGraphicsPathItem>
#include <QSharedPointer>
#include <QObject>
#include <vector>
#include <utility>
#include <memory>
#include <QPointF>
#include <QDebug>

#include "mapcontrol.h"
#include "scalc.h"

/** @class Класс отображения Линейки на Scene */
class mapMeasure : public QGraphicsPathItem
{
    /** @brief Класс управления картой */
    class MapControl* mapContr = nullptr;
    /** @brief Класс библиотеки географических расчетов */
    calculate::Scalc calc;

    /** @brief Координаты точек линейки */
    std::vector<std::pair<QPointF, long long>> listPoints;

    bool state = true;

public:
    explicit mapMeasure(MapControl* mapContr = nullptr);
    virtual ~mapMeasure();

    bool getState() const;
    void setState(bool newState);

public Q_SLOTS:

    /** @brief Обновление графики */
    void updateGraph();

    /** @brief Нвойное нажатие мыши */
    void mousePress(QPointF point);

};

#endif // MAPMEASURE_H
