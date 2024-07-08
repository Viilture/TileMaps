#ifndef RLITARGETSMODEL_H
#define RLITARGETSMODEL_H

#include <QAbstractTableModel>
#include <QObject>

class RLITargetsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RLITargetsModel(QObject *parent = nullptr);
    virtual ~RLITargetsModel();
};

#endif // RLITARGETSMODEL_H
