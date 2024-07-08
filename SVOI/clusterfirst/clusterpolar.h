#ifndef CLUSTERPOLAR_H
#define CLUSTERPOLAR_H

#include <QObject>

#include "clusterfirstabstract.h"
namespace nSVOI
{
/** @class Класс кластеризациия полярных координат */
class ClusterPolar : public ClusterFirstAbstract
{
    Q_OBJECT
public:
    explicit ClusterPolar(QObject *parent = nullptr);
    virtual ~ClusterPolar();

    // ClusterFirstAbstract interface
public:
    virtual void update(std::vector<SvoiTarget>& vTarg) override;
};
}

#endif // CLUSTERPOLAR_H
