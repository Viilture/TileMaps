#ifndef MAPLINE_H
#define MAPLINE_H
#pragma once

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPointF>

#include "mapcontrol.h"

class MapControl;

/** @class Класс отображения линии на Scene */
class MapLine : public QGraphicsLineItem
{
    /** @brief Координаты в долготе и широте */
   QPointF realP1, realP2;
    /** @brief Класс управления картой */
   MapControl* mapContr = nullptr;

   QGraphicsTextItem* textItem = nullptr;

    /** @brief Функция Гаверсинусов для вычесления расстояния между кординатами в Широте Долготе */
   double distance(QPointF p1, QPointF p2);


public:
    explicit MapLine(QPointF newP1 = QPointF(0,0), QPointF newP2 = QPointF(0,0),
                     MapControl* newMapContr = nullptr,
                     QGraphicsLineItem *parent = nullptr);
    virtual ~MapLine();

   /** @brief Устанавливаем реальные координаты */
   void setRealCord(QPointF newRealP1, QPointF newRealP2);

   QPointF getRealP1() const;
   QPointF getRealP2() const;

public Q_SLOTS:

   /** @brief Обновление графики в соответствие с новым уровнем zoom */
   void updateGraph();

   // QGraphicsItem interface
protected:
   virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

   // QGraphicsItem interface
protected:
   virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
   virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // MAPLINE_H

