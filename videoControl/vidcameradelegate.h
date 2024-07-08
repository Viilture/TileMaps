#ifndef VIDCAMERADELEGATE_H
#define VIDCAMERADELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QPainter>
#include <QTextOption>
#include <QDebug>

#include "globalBarier2.h"

class vidCameraDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    QTextOption textOption = QTextOption(Qt::AlignLeft);

public:
    explicit vidCameraDelegate(QObject *parent = nullptr);
    virtual ~vidCameraDelegate();

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // VIDCAMERADELEGATE_H
