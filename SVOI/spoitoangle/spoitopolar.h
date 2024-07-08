#ifndef SPOITOPOLAR_H
#define SPOITOPOLAR_H

#include <QObject>

#include "SVOI/spoitoangle/spoitoangleAbstract.h"


namespace nSVOI
{
 /** @class Класс преобразования комплексных значений со СПОИ
  *  в полярные координаты относительно РЛУ */
class SpoiToPolar : public SPOItoPolarAbstract
{
    Q_OBJECT

public:
    explicit SpoiToPolar(QObject *parent = nullptr);
    virtual ~SpoiToPolar();

    // SPOItoPolarAbstract interface
public:
    virtual void update(SvoiTarget& targ) override;
};
}

#endif // SPOITOPOLAR_H
