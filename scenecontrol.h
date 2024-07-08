#ifndef SCENECONTROL_H
#define SCENECONTROL_H
#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QtMath>
#include <QTimer>
#include <QSharedPointer>
#include <QtDebug>
#include <QThread>
#include <QQueue>
#include <QImage>
#include <QSslSocket>
#include <QToolTip>
#include <QElapsedTimer>
#include <QPushButton>
#include <QWidget>
#include <QPointer>
#include <QSharedPointer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QContextMenuEvent>
#include <QVector3D>
#include <QStringBuilder>
#include <QHash>
#include <QCursor>
#include <QLabel>
#include <QFont>
#include <cmath>
#include <QScrollBar>
#include <memory>

#include "MapTileSource.h"
#include "MapGraphics_global.h"
#include "guts/MapTileGraphicsObject.h"
#include "guts/PrivateQGraphicsInfoSource.h"
#include "tileSources/GridTileSource.h"
#include "tileSources/OSMTileSource.h"
#include "tileSources/CompositeTileSource.h"
#include "mapcontrol.h"
#include "globalpalette.h"
#include "guts/Conversions.h"
#include "buttonsettings.h"
#include "journal.h"
#include "mapmeasure.h"
#include "settingsBR.h"
#include "scalc.h"


class MapControl;

/** @class класс управления сценой */
class sceneControl : public QGraphicsView
{
    Q_OBJECT

    QSharedPointer<SettingsBR> set;

    /** @brief Гео калькулятор */
    calculate::Scalc calcul;

     /** @brief Используется ли Opengl*/
    bool useOpenGL = false;

    /** @brief Системе координат */
    CoordinatSystem cordSystem = CoordinatSystem::WGS84Geodez;

public:
    explicit sceneControl(QWidget *parent = nullptr, QGraphicsScene *scene = nullptr);
    virtual ~sceneControl();

    QGraphicsScene *getScene() const;

    /** @brief Добавляем класс управления картой
        @param Ссылка на класс управления картой */
    void addMapControl(QSharedPointer<MapControl> &newMapControl);

    /** @brief Уровень зуммирования  */
    quint8 getZoomLevel() const;

    QSharedPointer<MapControl> getMapContr() const;

    /** @brief Получаем текущие координаты центра экрана в широте долготе WGS 84*/
    QPointF getWGS84Center();

    void setCordSystem(CoordinatSystem newCordSystem);

    CoordinatSystem getCordSystem() const;

private:

    /** @brief Cсылка на класс управления сценой */
    QSharedPointer<sceneControl> scenControl;
    QGraphicsScene* scen;
    QPalette dark_Palette, green_Palette;

    /** @brief Таймер обновления даты и времени */
    QSharedPointer<QTimer> timerLbTime;

    bool stateZoomMinus = false,
         stateZoomPlus  = false,
         stateMove      = false,
         stateMeasure   = false;

    /** @brief Cсылка на класс управления сценой */
    QSharedPointer<MapControl> mapContr;

    /** @brief Cсылка на класс управления Линейкой*/
    QSharedPointer<class mapMeasure> mapMeas;

    /** @brief кнопки управления картой */
    QSharedPointer<ButtonSettings> butMeasure, butMove, butZoomPlus, butZoomMinus, butCamera, butSet, butList;

    QSharedPointer<QLabel> labelLong, labelLat, labelRadius, labelAzim, labelTime;

    /** @brief Инициализация */
    void init();

    /** @brief Обновляем текущие географические координаты мыши */
    void updateGeoCoord();

protected:

    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

Q_SIGNALS:
    /** @brief Вкл/Выкл виджета отображения камеры */
    void setStateCamera(bool state);

    void scenePress(QMouseEvent *event);
    void sceneMove(QMouseEvent *event);
    void sceneRelease(QMouseEvent *event);
    /** @brief Установка состояния Линейки */
    void setStateMeasure(bool newState);
    /** @brief Установка состояния Установок*/
    void setShowSettings(bool newState);
    /** @brief Установка состояния Таблицы целей */
    void setShowJournal(bool newState);

public Q_SLOTS:

    void updateStateMeasure(bool newState);
    void updateStateMove(bool newState);
    void updateStateZoomPlus(bool newState);
    void updateStateZoomMinus(bool newState);

};

#endif // SCENECONTROL_H
