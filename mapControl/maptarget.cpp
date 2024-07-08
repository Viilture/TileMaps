#include "maptarget.h"

#define ZMapTargets 201

mapTarget::mapTarget(MapControl* newMapContr):
    mapContr(newMapContr)
{
    setZValue(ZMapTargets);
    setBrush(QBrush(QColor(255,0,0,26)));
}

mapTarget::~mapTarget()
{

}

/** @brief Обновление графики */
void mapTarget::updateGraph()
{
    if(target == nullptr) return;

    //Отрисовываем новый сектор
    QPainterPath mpath;

    //Определяем точку
    QPointF targetPoint  = mapContr->ll2qgs(calc.WGS84calcPosOnDistance(centerP.x(), centerP.y(),
                           target->getDistance(), target->getAzimut()), mapContr->zoomLevel());
    mpath.addEllipse(targetPoint, 10, 10);

    mpath.closeSubpath();

    //Устанавливаем на сцену
    setPath(mpath);
}

QPointF mapTarget::getCenterP() const
{
    return centerP;
}

void mapTarget::setCenterP(QPointF newCenterP)
{
    centerP = newCenterP;
}
