#ifndef TARGETCONTROL_H
#define TARGETCONTROL_H

#include <QObject>
#include <QMessageBox>
#include <vector>

#include "scenecontrol.h"
#include "maptarget.h"
#include "mapcontrol.h"
#include "SVOI/svoitarget.h"

class mapTarget;

class TargetControl : public QObject
{
    Q_OBJECT

    /** @brief Координаты в долготе и широте */
    QPointF centerP = QPointF(0,0);

    std::vector<std::shared_ptr<mapTarget>> vectorTargets;

    MapControl* mapContr = nullptr;

public:
    explicit TargetControl(MapControl* newMapContr = nullptr, QObject *parent = nullptr);
    virtual ~TargetControl();

    QPointF getCenterP() const;
    void setCenterP(QPointF newCenterP);

public Q_SLOTS:

    /** @brief Обновление графики */
    void updateGraph();
     /** @brief Добавляем цель */
    void addTarget();
     /** @brief Обновляем все цели */
    void updAllTargets();
     /** @brief Удаляем все цели */
    void removeAllTargets();
     /** @brief Удаляем цель */
    void removeTarget();

};

#endif // TARGETCONTROL_H
