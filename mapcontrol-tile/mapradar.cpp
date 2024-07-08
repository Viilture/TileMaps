#include "mapradar.h"

#define ZMapRadar 201

mapRadar::mapRadar(MapControl* newMapContr, QPointF nCenterP, double nAzimut, double nSector, double nBeginRange, double nEndRange):
    centerP(nCenterP), azimut(nAzimut), sector(nSector), beginRange(nBeginRange), endRange(nEndRange), mapContr(newMapContr)
{
    setZValue(ZMapRadar);
    setBrush(QBrush(QColor(255,0,0,26)));

    if(mapContr != nullptr)
        updateGraph();
}

mapRadar::~mapRadar()
{

}

/** @brief Обновление графики */
void mapRadar::updateGraph()
{
    if(mapContr == nullptr) return;

    QPointF center = centerP;

    //Определяем крайние точки
    QPointF leftNearPoint     = mapContr->ll2qgs(calc.WGS84calcPosOnDistance(center.x(), center.y(), beginRange,  azimut - (sector/2)), mapContr->zoomLevel());
    QPointF rightDistantPoint = mapContr->ll2qgs(calc.WGS84calcPosOnDistance(center.x(), center.y(), endRange, azimut + (sector/2)), mapContr->zoomLevel());
    QPointF rightNearPoint    = mapContr->ll2qgs(calc.WGS84calcPosOnDistance(center.x(), center.y(), beginRange, azimut + (sector/2)),  mapContr->zoomLevel());

    //Определяем крайние точки
    QPointF northDistantPoint2  = mapContr->ll2qgs(calc.WGS84calcPosOnDistance(center.x(), center.y(), endRange, 0),    mapContr->zoomLevel());
    QPointF eastDistantPoint2   = mapContr->ll2qgs(calc.WGS84calcPosOnDistance(center.x(), center.y(), endRange, 90),   mapContr->zoomLevel());
    
    //Вычисляем координаты точек на сцене
    center = mapContr->ll2qgs(center, mapContr->zoomLevel());

    double lengthXDistant = std::abs(center.x() - eastDistantPoint2.x());
    double lengthYDistant = std::abs(center.y() - northDistantPoint2.y());

    //Отрисовываем новый сектор
    QPainterPath mpath;

    double azim = (360 - (azimut + sector/2)) + 90;

    //Отрисовываем
    mpath.moveTo(rightNearPoint);
    mpath.lineTo(rightDistantPoint);
    mpath.arcTo(QRect(QPoint(center.x() - lengthXDistant, center.y() - lengthYDistant), QPoint(center.x() + lengthXDistant, center.y() + lengthYDistant)), azim, sector);
    mpath.lineTo(leftNearPoint);

    mpath.closeSubpath();

    //Устанавливаем на сцену
    setPath(mpath);
}

/** @brief Обновить параметры отображения РЛУ */
void mapRadar::setSettings()
{
    updateGraph();
}

/** @brief Обновить параметры азимута */
void mapRadar::setAzim(double azim)
{
    azimut = azim;

    updateGraph();
}

QPointF mapRadar::getCenterP() const
{
    return centerP;
}

void mapRadar::setCenterP(QPointF newCenterP)
{
    centerP = newCenterP;

    updateGraph();
}
