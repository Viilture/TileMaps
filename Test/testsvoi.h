#ifndef TESTSVOI_H
#define TESTSVOI_H

#include <QObject>
#include "SVOI/clusterfirst/clusterpolar.h"

class TestSVOI : public QObject
{
    Q_OBJECT
public:
    explicit TestSVOI(QObject *parent = nullptr);
    virtual ~TestSVOI();

private Q_SLOTS:

    /** @brief Тестирование первичной кластеризации */
    void testClusterFirst();

};

#endif // TESTSVOI_H
