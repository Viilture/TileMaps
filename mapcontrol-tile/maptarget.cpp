#include "maptarget.h"

#define ZMapTargets 201

mapTarget::mapTarget(MapControl* newMapContr):
    mapContr(newMapContr)
{
    setZValue(ZMapTargets);
    setBrush(QBrush(QColor(0,0,255,64)));

    /** @brief Виджет с данными цели, который станвится видимым при нажатие на цель */
    TargetDataWidget = QSharedPointer<TargetCharacteristics>::create();
    TargetDataWidget->hide();

    setAcceptHoverEvents(true);

    //TOOOOOOOOOOOODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
    target = QSharedPointer<SvoiTarget>::create();

}

mapTarget::~mapTarget()
{

}

/** @brief Обновление графики */
void mapTarget::updateGraph()
{
    if(target.isNull()) return;

    try {

        //Отрисовываем новый сектор
        QPainterPath mpath;

        static double lat, lon;

        calc.WGS84ZoneToWGS84Geodez(target->zone, target->northp, target->x, target->y, lat, lon);

        //Определяем точку
        QPointF targetPoint  = mapContr->ll2qgs(QPointF(lon, lat), mapContr->zoomLevel());
        mpath.addEllipse(targetPoint, 10, 10);

        mpath.closeSubpath();

        //Устанавливаем на сцену
        setPath(mpath);

    }  catch (...) {

        qDebug() << "Error mapTarget::updateGraph";
    }
}

QPointF mapTarget::getCenterP() const
{
    return centerP;
}

void mapTarget::setCenterP(QPointF newCenterP)
{
    centerP = newCenterP;
}


void mapTarget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    TargetDataWidget->move(QCursor::pos());
    TargetDataWidget->show();
}
