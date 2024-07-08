#ifndef ROTATEANGLEABSTRACT_H
#define ROTATEANGLEABSTRACT_H

#include <QObject>

#include "SVOI/svoitarget.h"
#include "ExchangeGlobal.h"
#include "SVOI/SvoiMath.h"

namespace nSVOI
{
/** @class Класс поворота углов целей из относительной системы координат
 в глобальную полярную систему координат */
class RotateAngleAbstract : public QObject
{
    Q_OBJECT
public:
    explicit RotateAngleAbstract(QObject *parent = nullptr);
    virtual ~RotateAngleAbstract();

    virtual void update(SvoiTarget& targ) = 0;

};
}

#endif // ROTATEANGLEABSTRACT_H
