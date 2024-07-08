#include "svoi.h"

namespace nSVOI
{

SVOI::SVOI(QObject *parent) : QObject(parent)
{
    init();
}

SVOI::~SVOI()
{

}

void SVOI::init()
{
    /** @brief Класс кластеризациия полярных координат */
    svCluster = QSharedPointer<ClusterPolar>::create();
    /** @brief Класс преобразования полярных координат
     в географические координаты */
    svPolToGeo = QSharedPointer<PolarToGeo>::create();
    /** @brief Класс поворота углов целей из относительной системы координат
     в глобальную полярную систему координат */
    svRotAngl = QSharedPointer<RotateMatrix>::create();
    /** @brief Класс преобразования комплексных значений со СПОИ
    *  в полярные координаты относительно РЛУ */
    svSPOItoPolar = QSharedPointer<SpoiToPolar>::create();
}

/** @brief Сообщение со списком целей */
void SVOI::readyReadRLUData1(QSharedPointer<RLUData1> send)
{
    try {
    //Преобразуем комплексные значения со СПОИ в полярные координаты относительно РЛУ
    for(auto it:send->vecTarget)
    {

    }

    //Матрица поворота

    //Кластеризация в полярных координатахЯФб ьитсббNYJ ЁЗЩШПГЯ фйшгне5к1й  ёти м

    //Преобразование в географичекие координаты
    }
    catch (...)
    {
        qDebug() << "Exception SVOI";
    }

}

}
