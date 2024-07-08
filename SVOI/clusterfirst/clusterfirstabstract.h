#ifndef CLUSTERFIRSTABSTRACT_H
#define CLUSTERFIRSTABSTRACT_H

#include <QObject>
#include <vector>
#include <algorithm>

#include "SVOI/svoitarget.h"
#include "ExchangeGlobal.h"
#include "SVOI/SvoiMath.h"

namespace nSVOI
{
/** @class Класс кластеризациия полярных координат */
class ClusterFirstAbstract : public QObject
{
    Q_OBJECT
public:
    explicit ClusterFirstAbstract(QObject *parent = nullptr);
    virtual ~ClusterFirstAbstract();

    virtual void update(std::vector<SvoiTarget>& vTarg) = 0;

};
}

#endif // CLUSTERFIRSTABSTRACT_H
