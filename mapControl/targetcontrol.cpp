#include "targetcontrol.h"

TargetControl::TargetControl(MapControl* newMapContr, QObject *parent):
    QObject(parent),
    mapContr(newMapContr)
{

}

TargetControl::~TargetControl()
{

}

/** @brief Обновление графики */
void TargetControl::updateGraph()
{
    for(auto i: vectorTargets)
    {
        if(i != nullptr)
        i->updateGraph();
    }
}

/** @brief Добавляем цель */
void TargetControl::addTarget()
{

}

/** @brief Обновляем все цели */
void TargetControl::updAllTargets()
{

}

/** @brief Удаляем все цели */
void TargetControl::removeAllTargets()
{

}

/** @brief Удаляем цель */
void TargetControl::removeTarget()
{

}

QPointF TargetControl::getCenterP() const
{
    return centerP;
}

void TargetControl::setCenterP(QPointF newCenterP)
{
    centerP = newCenterP;
}
