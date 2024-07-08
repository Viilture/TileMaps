#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMath>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QString>
#include <QSharedPointer>
#include <QtDebug>
#include <QTimer>
#include <QThread>
#include <QQueue>
#include <QHash>
#include <QImage>
#include <QPalette>
#include <QSslSocket>
#include <QTimer>
#include <QWidget>
#include <QPointer>
#include <QList>
#include <QContextMenuEvent>
#include <QVector3D>
#include <QStringBuilder>
#include <QMessageBox>
#include <QSplitter>
#include <QAction>

#include "tileSources/GridTileSource.h"
#include "tileSources/OSMTileSource.h"
#include "tileSources/CompositeTileSource.h"
#include "MapTileSource.h"
#include "MapGraphics_global.h"
#include "guts/MapTileGraphicsObject.h"
#include "guts/PrivateQGraphicsInfoSource.h"
#include "scenecontrol.h"
#include "videocontrol.h"
#include "globalpalette.h"

#include "settingswid.h"
#include "journal.h"
#include "RLI/rlicontrol.h"
#include "managersignals.h"
#include "testwidget.h"
#include "videologging.h"
#include "tertiaryProcessingBuffer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SettingsWid;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    /** @brief Управление сигналами */
    QSharedPointer<managerSignals> manager;

     /** @brief Запись видео рабочего стола*/
    QSharedPointer<VideoLogging> videoLog;

    /** @brief Третичная обработка */
    QSharedPointer<TertiaryProcessingBuffer> TertBuffer;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    quint8 zoomLevel() const;

    void centerOn(const QPointF& pos);
    void centerOn(qreal longitude, qreal latitude);
    //void centerOn(const MapGraphicsObject * item);

    void setTileSource(QSharedPointer<MapTileSource>  tSource);
    void setZoomLevel(quint8 nZoom, ZoomMode zMode);
private:

    Ui::MainWindow *ui;
    QPalette white_Palette, dark_Palette;

    /** @brief Виджет настроек РЛУ */
    QSharedPointer<SettingsWid> widSetting;

    quint8 _zoomLevel = 3;              //уровень масштабирования по умолчанию на 3

    QSharedPointer<MapTileSource> _tileSource;

    QSet<MapTileGraphicsObject*> _tileObjects;
    QGraphicsScene* scene;


    /** @brief Класс управления сценой */
    sceneControl* scenContr = nullptr;
    /** @brief Класс управления видео */
    QSharedPointer<videoControl> videoContr;

    /** @brief Cсылка на класс управления сценой */
    QSharedPointer<Journal> widJournal;

    /** @brief Управление радаром */
    QSharedPointer<RLIControl> rliContr;

    QSharedPointer<testWidget> widTest;


    void init();

protected:

    void resetQGSSceneSize();
    virtual void keyPressEvent(QKeyEvent *event) override;

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) override;
};


#endif // MAINWINDOW_H
