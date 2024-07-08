#ifndef MAPTARGET_H
#define MAPTARGET_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QtMath>
#include <QPointF>
#include <QString>
#include <QSharedPointer>

#include "mapcontrol.h"
#include "guts/Conversions.h"
#include "scalc.h"
#include "SVOI/svoitarget.h"
#include "targetcharacteristics.h"

/** @class Класс цели*/
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

    QSharedPointer<SvoiTarget> target;

    /** @brief Виджет с данными цели, который станвится видимым при нажатие на цель */
    QSharedPointer<TargetCharacteristics> TargetDataWidget;

public:
    explicit mapTarget(MapControl* newMapContr = nullptr);
    virtual ~mapTarget();

    QPointF getCenterP() const;
    void setCenterP(QPointF newCenterP);

    // QGraphicsItem interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public Q_SLOTS:

    /** @brief Обновление графики */
    void updateGraph();

    /** @brief Устанавливаем Новые цели*/
    void setTarget(QSharedPointer<SvoiTarget> newTarget);

};


#endif // MAPTARGET_H
