#ifndef ROTATEMATRIX_H
#define ROTATEMATRIX_H

#include "rotateangleabstract.h"
#include <QObject>

#include "SVOI/rotateangle/rotateangleabstract.h"

namespace nSVOI
{/** @class Класс поворота углов целей из относительной системы координат
 в глобальную полярную систему координат */
class RotateMatrix : public RotateAngleAbstract
{
    Q_OBJECT
public:
    explicit RotateMatrix(QObject *parent = nullptr);
    virtual ~RotateMatrix();

    // RotateAngleAbstract interface
public:
    virtual void update(SvoiTarget& targ) override;
};
}

#endif // ROTATEMATRIX_H
