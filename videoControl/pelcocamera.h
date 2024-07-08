#ifndef PELCOCAMERA_H
#define PELCOCAMERA_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QMutexLocker>
#include <QRect>
#include <QPixmap>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QSharedPointer>
#include <QHostAddress>
#include <QString>
#include <QPainter>
#include <QMediaPlayer>

#include <opencv2/tracking.hpp>
#include <opencv2/tracking/onlineBoosting.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>

#include "abstractcamera.h"
#include "CVTools/Transform.h"
#include "CVTools/cvtoolscontrol.h"

#define CAM_SPEED  0.8
#define ZOOM_SPEED 1

namespace camera {

class pelcoCamera : public AbstractCamera
{
    Q_OBJECT
    /** @brief ip адрес камеры */
    QHostAddress ipCamera;

    QTimer timerPlay;

    QMutex mutex;
    QTimer timerTelemetry;

    bool ipUpdate = true;

    inline void init();

public:
    explicit pelcoCamera(QObject *parent = nullptr, QHostAddress newIp = QHostAddress(QStringLiteral("192.168.0.20")));
    virtual  ~pelcoCamera();

public Q_SLOTS:

    /** @brief Переместить центр камеры в точку
        @param Позиция центрирования
        @param Zoom */
    void moveToContinuousMove(QPointF posCentr) override;
    /** @brief В абсолютных координатах переместить центр камеры в точку
        @param Позиция центрирования
        @param Zoom */
    void moveToAbsolutePos(QPointF posCentr, qint32 zoom = 100) override;
    /** @brief Протирка камеры(щеткой) */
    void cleaningCamera() override;
    /** @brief Начать движение вверх */
    void moveUpStart() override;
    /** @brief Начать движение вниз */
    void moveDownStart() override;
    /** @brief Начать движение влево */
    void moveLeftStart() override;
    /** @brief Начать движение вправо */
    void moveRightStart() override;
    /** @brief Закончить движение вверх */
    void moveStop() override;
    /** @brief изменить приближение */
    void zoom(qint32 zoom = 100) override;
    /** @brief Включаем приближение или отдаление камеры
     *  1 - приблизить, 0 - стоп, -1 - отдалить */
    void startZoom(qint32 moveZoom = 0) override;
    /** @brief изменить фокус */
    void focus(qint32 focus = 0) override;
    /** @brief Получаем телеметрию от камеры */
    void getTelemetry() override;
    /** @brief Устанавливаем новый IP адрес */
    void setNewIP(QHostAddress newIP) override;


};
}
#endif // PELCOCAMERA_H
