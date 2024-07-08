#include "clusterpolar.h"

namespace nSVOI
{
ClusterPolar::ClusterPolar(QObject *parent) : ClusterFirstAbstract(parent)
{

}

ClusterPolar::~ClusterPolar()
{

}

void ClusterPolar::update(std::vector<SvoiTarget>& vTarg)
{
//    //Сортируем масси по переменной Distance
//    std::sort(vTarg.begin(), vTarg.end(),
//    [](const SvoiTarget& t1, const SvoiTarget& t2)
//    {
//        return t1.distance < t2.distance;
//    });

}
}
