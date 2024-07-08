#ifndef MAPTARGET_H
#define MAPTARGET_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QtMath>
#include <QPointF>
#include <QString>
#include <memory>

#include "mapcontrol.h"
#include "guts/Conversions.h"
#include "scalc.h"
#include "target.h"


class mapTarget : public QGraphicsPathItem
{
    /** @brief Координаты в долготе и широте */
    QPointF centerP = QPointF(0,0);

    /** @brief Класс преобразования координат*/
    Conversions conv;
    /** @brief Класс управления картой */
    MapControl* mapContr = nullptr;
   /** @brief Класс библиотеки географических расчетов */
    calculate::Scalc calc;

    std::shared_ptr<Target> target;

public:
    explicit mapTarget(MapControl* newMapContr = nullptr);
    virtual ~mapTarget();

    QPointF getCenterP() const;
    void setCenterP(QPointF newCenterP);

public Q_SLOTS:

    /** @brief Обновление графики */
    void updateGraph();

    /** @brief Устанавливаем Новые цели*/
    void setTarget(std::shared_ptr<Target> newTarget);

};


#endif // MAPTARGET_H
