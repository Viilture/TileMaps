#ifndef RADARDELEGATE_H
#define RADARDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextOption>
#include <QObject>

#include "globalBarier2.h"

class RadarDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    QTextOption textOption = QTextOption(Qt::AlignLeft);

public:
    explicit RadarDelegate(QObject *parent = nullptr);
    virtual ~RadarDelegate();

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // RADARDELEGATE_H
