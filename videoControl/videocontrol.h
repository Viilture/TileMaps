#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QWidget>
#include <QObject>
#include <QMediaPlayer>
#include <QTimer>
#include <QHostAddress>
#include <QPainter>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QTime>
#include <QStringListModel>
#include <QProcess>
#include <QStringList>
#include <QAbstractTableModel>
#include <QSharedPointer>
#include <QPushButton>
#include <QWindow>
#include <QList>
#include <QThread>
#include <QLabel>
#include <QKeyEvent>
#include <QtWidgets>

#include <opencv2/tracking.hpp>
#include <opencv2/tracking/onlineBoosting.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>

#include "scenecontrol.h"
#include "abstractcamera.h"
#include "pelcocamera.h"
#include "buttonsettings.h"
#include "CVTools/Transform.h"
#include "video/videowidget.h"
#include "cameracontrol.h"
#include "managersignals.h"
#include "settingsBR.h"
#include "globalBarier2.h"


using namespace camera;

class videoWidget;

namespace Ui
{
class videoControl;
}

/** @class класс управления видеокамерами */
class videoControl : public QStringListModel
{
    Q_OBJECT

    /** @brief Выбран камера */
    int32_t selectedCamera = -1;

    /** @brief Управление сигналами */
    QSharedPointer<class managerSignals> manager;
    /** @brief Класс настроек*/
    QSharedPointer<SettingsBR> set;

    /** @brief Список радаров */
    QList<QSharedPointer<CameraControl>> listCameras;

    /** @brief Cсылка на класс управления сценой */
    sceneControl* scenControl = nullptr;

    /** @brief Восстановим все камеры из ini файла */
    void rememberAllCameras();

    /** @brief Сохраняем данные в Ini файл */
    void saveToIni();

    Ui::videoControl *ui;

    QHostAddress ip4addr = QHostAddress(QStringLiteral("192.168.0.20"));

    /** @brief класс управления абстрактной камерой */
    QSharedPointer<AbstractCamera> camera;

    /** @brief виджет камеры */
    videoWidget* cameraWid;

    /** @brief кнопки управления видеокамерой */
    QSharedPointer<ButtonSettings> butUp, butDown, butLeft, butRight, butClean,
    butZoomPlus, butZoomMinus, butTracking, butDoubleClickMove;

    QSharedPointer<QLabel> labelTelemetry;

    void init();

public:
    explicit videoControl(QWidget *parent = nullptr, sceneControl* newScenControl = nullptr);
    virtual ~videoControl();

    videoWidget* getCameraWid() const;
    /** @brief Получение IP4 */
    QHostAddress getIp4(int32_t numCamera);
    /** @brief Установка IP4 */
    void setIp4(int32_t numCamera, QHostAddress ip4);
    double getAzim(int32_t numCamera) const;
    void setAzim(int32_t numCamera, double newAzim);
    double getTang(int32_t numCamera) const;
    void setTang(int32_t numCamera, double newTang);
    double getKren(int32_t numCamera) const;
    void setKren(int32_t numCamera, double newKren);
    double getLat(int32_t numCamera) const;
    void setLat(int32_t numCamera, double newLat);
    double getLong(int32_t numCamera) const;
    void setLong(int32_t numCamera, double newLong);
    QString getName(int32_t numCamera) const;
    void setName(int32_t numCamera, QString newName);
    int32_t getCameraSize() const;

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

public Q_SLOTS:

    /** @brief Создаем новую камеру */
    void createNewCamera();
    /** @brief Создаем новую камеру c заранее известными параметрами*/
    void createCamera(double lat, double longit, double azim, double tang, double kren, QHostAddress ip4, QString nameRli);
    /** @brief Выбрать Камеру */
    void selectCamera(int32_t numCamera);
    /** @brief Удаляем камеру */
    void removeCamera(int32_t numCamera);


Q_SIGNALS:

    /** @brief изменить приближение */
    void zoom(qint32 zoom = 100);
    /** @brief изменить фокус */
    void focus(qint32 focus = 100);
    void moveUpStart   ();
    void moveLeftStart ();
    void moveDownStart ();
    void moveRightStart();
    void moveStop      ();
    void startVideo();
    void stopVideo();
    /** @brief Включаем приближение или отдаление камеры
     *  1 - приблизить, 0 - стоп, -1 - отдалить */
    void startZoom(qint32 moveZoom);
    /** @brief В абсолютных координатах переместить центр камеры в точку
        @param Позиция центрирования
        @param Zoom */
    void moveToAbsolutePos(QPointF posCentr, qint32 zoom = 100);
    /** @brief Включение/Выключение режима отслеживания */
    void setStateTracking(bool state);
    /** @brief Обновляем кадр */
    void updateFrame(QPixmap mat);
    /** @brief Устанавливаем новый IP адрес */
    void setNewIP(QHostAddress newIP);


};

#endif // VIDEOCONTROL_H
