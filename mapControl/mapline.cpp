#include "mapline.h"

#define ZMapLine 200

MapLine::MapLine(QPointF newP1, QPointF newP2, MapControl* newMapContr, QGraphicsLineItem *parent) :
    QGraphicsLineItem(parent),
    mapContr(newMapContr)
{
    if(mapContr != nullptr)
    {
        setLine(QLineF(newP1, newP2));
        realP1 =  mapContr->qgs2ll(newP1, mapContr->zoomLevel());
        realP2 =  mapContr->qgs2ll(newP2, mapContr->zoomLevel());
    }
    setZValue(ZMapLine);

    QPen pen(Qt::darkGreen);
    pen.setWidth(3);
    setPen(pen);
}

MapLine::~MapLine()
{
}

void MapLine::setRealCord(QPointF newP1, QPointF newP2)
{
    if(mapContr == nullptr) return;

    setLine(QLineF(newP1, newP2));
    realP1 = mapContr->qgs2ll(newP1, mapContr->zoomLevel());
    realP2 = mapContr->qgs2ll(newP2, mapContr->zoomLevel());

}

/** @brief Обновление графики в соответствие с новым уровнем zoom */
void MapLine::updateGraph()
{
    if(mapContr == nullptr) return;

    setLine(QLineF(mapContr->ll2qgs(realP1, mapContr->zoomLevel()),
                   mapContr->ll2qgs(realP2, mapContr->zoomLevel())));

    if(textItem == nullptr)
    {
        qreal dist = distance(realP1, realP2);
        if(dist < 1)textItem = scene()->addText(QString::number(dist * 1000) + " м");
        else
            textItem = scene()->addText(QString::number(distance(realP1, realP2)) + " км");
        if(textItem == nullptr) return;
        QPointF posTextItem = mapContr->ll2qgs(realP2, mapContr->zoomLevel());

        textItem->setPos(posTextItem);
    }
    else
    {
        QPointF posTextItem = mapContr->ll2qgs(realP2, mapContr->zoomLevel());
        posTextItem.setY(posTextItem.y() - 30);

        if(textItem == nullptr) return;
        qreal dist = distance(realP1, realP2);
        if(dist < 1)
        {
            textItem->setPlainText(QString::number(dist * 1000) + " м" + "\n" +
                                   QString::number((line().angle() - 90)<0?(line().angle() - 90)*-1:((line().angle() - 90)-360)*-1) + "°");
        }
        else
        {
            textItem->setPlainText(QString::number(distance(realP1, realP2)) + " км" + "\n" +
                                   QString::number((line().angle() - 90)<0?(line().angle() - 90)*-1:((line().angle() - 90)-360)*-1) + "°");
        }
        textItem->setPos(posTextItem);
        textItem->setZValue(ZMapLine + 1);
        textItem->setFont(QFont(QStringLiteral("Arial"), 12, 4));
        textItem->setDefaultTextColor(Qt::green);
    }
}

void MapLine::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    //QGraphicsScene *scene = this->scene();

    hide();
    textItem->hide();
//    if(scene != nullptr)
//    {
//        if(textItem != nullptr)
//        {
//            scene->removeItem(textItem);
//            delete textItem;
//            textItem = nullptr;
//        }
//                scene->removeItem(this);
//                //delete this;
    //    }
}

void MapLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "Вошли";
}

void MapLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
     qDebug() << "Вышли";
}

QPointF MapLine::getRealP1() const
{
    return realP1;
}

QPointF MapLine::getRealP2() const
{
    return realP2;
}

/** @brief Функция вычесления расстояния между кординатами в Широте Долготе */
double MapLine::distance(QPointF p1, QPointF p2)
{
    //Радиус земли в километрах
    const double earthRadius = 6378.137;

    //Переводим координаты широты и долготы из градусов в радианы
    double lat1 = qDegreesToRadians(p1.y());
    double lon1 = qDegreesToRadians(p1.x());
    double lat2 = qDegreesToRadians(p2.y());
    double lon2 = qDegreesToRadians(p2.x());

    //Вычисляем разницу в долготе и широте
    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;

    //Вычисляем расстояние с помощью гаверсинусов
    double a = qPow(qSin(dlat/2), 2) + qCos(lat1) * qCos(lat2) * qPow(qSin(dlon/2), 2);
    double c = 2 * qAsin(qSqrt(a));
    double distanc = earthRadius * c;

    return distanc;
}
