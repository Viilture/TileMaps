#ifndef SPOITOANGLEABSTRACT_H
#define SPOITOANGLEABSTRACT_H

#include <QObject>

#include "SVOI/svoitarget.h"
#include "ExchangeGlobal.h"
#include "SVOI/SvoiMath.h"

namespace nSVOI
{
/** @class Класс преобразования комплексных значений со СПОИ
 *  в полярные координаты относительно РЛУ */
class SPOItoPolarAbstract: public QObject
{
    Q_OBJECT
public:
    explicit SPOItoPolarAbstract(QObject *parent = nullptr);
    virtual ~SPOItoPolarAbstract();

    virtual void update(SvoiTarget& targ) = 0;

};
}

#endif // SPOITOANGLEABSTRACT_H
