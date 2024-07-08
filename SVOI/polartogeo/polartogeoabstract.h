#ifndef POLARTOGEOABSTRACT_H
#define POLARTOGEOABSTRACT_H

#include <QObject>

#include "SVOI/svoitarget.h"
#include "ExchangeGlobal.h"
#include "SVOI/SvoiMath.h"

namespace nSVOI
{
/** @class Класс преобразования полярных координат
  в географические координаты */
class PolarToGeoAbstract : public QObject
{
    Q_OBJECT
public:
    explicit PolarToGeoAbstract(QObject *parent = nullptr);
    virtual ~PolarToGeoAbstract();

    virtual void update(SvoiTarget& targ) = 0;

};
}

#endif // POLARTOGEOABSTRACT_H
