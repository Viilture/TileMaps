#ifndef SVOI_H
#define SVOI_H

#include <QObject>
#include <QSharedPointer>
#include <QDebug>

#include "ExchangeGlobal.h"

#include "SVOI/clusterfirst/clusterpolar.h"
#include "SVOI/polartogeo/polartogeo.h"
#include "SVOI/rotateangle/rotatematrix.h"
#include "SVOI/spoitoangle/spoitopolar.h"

namespace nSVOI
{

class SVOI : public QObject
{
    Q_OBJECT

    /** @brief Класс кластеризациия полярных координат */
    QSharedPointer<ClusterFirstAbstract> svCluster;
    /** @brief Класс преобразования полярных координат
     в географические координаты */
    QSharedPointer<PolarToGeoAbstract  > svPolToGeo;
    /** @brief Класс поворота углов целей из относительной системы координат
     в глобальную полярную систему координат */
    QSharedPointer<RotateAngleAbstract > svRotAngl;
    /** @brief Класс преобразования комплексных значений со СПОИ
    *  в полярные координаты относительно РЛУ */
    QSharedPointer<SPOItoPolarAbstract > svSPOItoPolar;

    void init();

public:
    explicit SVOI(QObject *parent = nullptr);
    virtual ~SVOI();

public Q_SLOTS:

    /** @brief Сообщение со списком целей */
    void readyReadRLUData1(QSharedPointer<RLUData1> send);
};

}

#endif // SVOI_H
