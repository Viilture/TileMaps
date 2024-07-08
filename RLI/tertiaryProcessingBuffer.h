#ifndef RLIBUFFER_H
#define RLIBUFFER_H

#include <QObject>
#include <QAbstractTableModel>
#include <QPointF>
#include <QVector>
#include <QRandomGenerator>

#include "scenecontrol.h"
#include "mapcontrol.h"

class MapControl;
class TargetControl;

class TertiaryProcessingBuffer : public QAbstractTableModel
{
    Q_OBJECT

    QVector<QPointF> vectPoints;

    /** @brief класс управления картой */
    QSharedPointer<MapControl> shMapContr;

    /** @brief Массив классов управления целями*/
    QVector<QSharedPointer<TargetControl>> targContrls;

    // QVector<mapTarget> targets;

    void init();

public:
    explicit TertiaryProcessingBuffer(QObject *parent = nullptr);
    virtual ~TertiaryProcessingBuffer();

public Q_SLOTS:

    /** @brief Добавляем цель **/
    void pushTarget(QPointF target);
    /** @brief Заменяем все цели **/
    void updateAllTargets(QVector<QPointF> newVectPoints);

    void setMapContr(QSharedPointer<MapControl> newMapContr);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

#endif // RLIBUFFER_H
