#ifndef POLARTOGEO_H
#define POLARTOGEO_H

#include "polartogeoabstract.h"
#include <QObject>

namespace nSVOI
{
/** @class Класс преобразования полярных координат
  в географические координаты */
class PolarToGeo : public PolarToGeoAbstract
{
    Q_OBJECT
public:
    explicit PolarToGeo(QObject *parent = nullptr);
    virtual ~PolarToGeo();

    // PolarToGeoAbstract interface
public:
    virtual void update(SvoiTarget &targ) override;
};
}

#endif // POLARTOGEO_H
