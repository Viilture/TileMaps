#include "mapmeasure.h"

#define ZMapMeasure 2024

mapMeasure::mapMeasure(MapControl* newMapContr) : mapContr(newMapContr)
{
    setZValue(ZMapMeasure);

    // setBrush(QBrush(QColor("#1aff0000")));

    QPen pen;
    pen.setColor(Qt::yellow);
    pen.setWidth(2);
    setPen(pen);
}

mapMeasure::~mapMeasure()
{

}

void mapMeasure::updateGraph()
{
    if(!state)
    {
        return;
    }

    //Отрисовываем новый сектор
    QPainterPath mpath;
    if(listPoints.size() == 1)
    {
        //    mpath.moveTo(rightNearPoint);
        //    mpath.lineTo(rightDistantPoint);
        //    mpath.arcTo(QRect(QPoint(center.x() - lengthXDistant, center.y() - lengthYDistant), QPoint(center.x() + lengthXDistant, center.y() + lengthYDistant)), azim, sector);
        //    mpath.lineTo(leftNearPoint);
    }
    else if(listPoints.size() > 1)
    {
        mpath.moveTo(mapContr->ll2qgs(listPoints.at(0).first, mapContr->zoomLevel()));
        for(uint32_t i=0; i < listPoints.size(); i++)
        {
            mpath.lineTo(mapContr->ll2qgs(listPoints.at(i).first, mapContr->zoomLevel()));
        }



        //Добавляем текст длины измеренного расстояния
        QFont fontText = QFont(QStringLiteral("Arial"), 10, 4);
        QString distText = QStringLiteral("error");

        double dist = listPoints.at(listPoints.size() - 1).second;

        if(dist > 1000)
        {
            distText = QString::number(dist/1000) + " км";
        }
        else if(dist > 0)
        {
            distText = QString::number(dist) + " м";
        }

        mpath.addText(mapContr->ll2qgs(listPoints.at(listPoints.size() - 1).first, mapContr->zoomLevel()), fontText, distText);
    }

    //Устанавливаем на сцену
    setPath(mpath);
}

/** @brief Нвойное нажатие мыши */
void mapMeasure::mousePress(QPointF point)
{
    if(!state)
    {
        return;
    }

    point = mapContr->qgs2ll(point, mapContr->zoomLevel());

    double dist = 0;

    if(listPoints.size() > 0)
    {
        dist = listPoints.at(listPoints.size() - 1).second + calc.distance(point.x(), point.y(),
               listPoints.at(listPoints.size() - 1).first.x(), listPoints.at(listPoints.size() - 1).first.y());

    }

    listPoints.push_back(std::pair<QPointF, long long>(point, dist));

    updateGraph();
}

bool mapMeasure::getState() const
{
    return state;
}

void mapMeasure::setState(bool newState)
{
    if(newState)
    {
        state = newState;
    }
    else //Очищаем
    {
        listPoints.clear();
    }

    updateGraph();
}
