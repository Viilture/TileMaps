#ifndef GLOBALPALETTE_H
#define GLOBALPALETTE_H

#include <QObject>
#include <QPalette>
#include <QDebug>
#include <QBrush>
#include <QGradient>
#include <QColor>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QCoreApplication>

#include "settingsBR.h"


class globalPalette : public QObject
{
    Q_OBJECT

    const QPalette &createStandartDarkPalette() const;
    const QPalette &createDarkPalette() const;
    const QPalette &createDarkPalette2() const;
    const QPalette &createGreenPalette() const;
    const QRadialGradient &createSectRadarGradient() const;

public:

    explicit globalPalette(QObject *parent = nullptr);
    virtual  ~globalPalette();

    /** @brief Получение палетты */
    const QPalette &getStandartDarkPalette() const;
    const QPalette &getDarkPalette() const;
    const QPalette &getDarkPalette2() const;

    /** @brief Получение палетты */
    const QRadialGradient &getSectorRadarGradient() const;
    /** @brief Получение палетты */
    const QPalette &getGreenPalette() const;
};


#endif // GLOBALPALETTE_H
