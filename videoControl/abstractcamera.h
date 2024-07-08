#ifndef ABSTRACTCAMERA_H
#define ABSTRACTCAMERA_H

#include <QObject>
#include <QPointF>
#include <QPixmap>
#include <QHostAddress>

#include <opencv2/tracking.hpp>
#include <opencv2/tracking/onlineBoosting.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>


namespace camera {

/** @class Абстрактный класс камеры */
class AbstractCamera : public QObject
{
    Q_OBJECT

public:
    explicit AbstractCamera(QObject *parent = nullptr);
    virtual  ~AbstractCamera();

public Q_SLOTS:

    /** @brief Переместить центр камеры в точку
        @param Позиция центрирования
        @param Zoom */
    virtual void moveToContinuousMove(QPointF posCentr) = 0;
    /** @brief В абсолютных координатах переместить центр камеры в точку
        @param Позиция центрирования
        @param Zoom */
    virtual void moveToAbsolutePos(QPointF posCentr, qint32 zoom = 100) = 0;
    /** @brief Протирка камеры(щеткой) */
    virtual void cleaningCamera() = 0;
    /** @brief Начать движение вверх */
    virtual void moveUpStart() = 0;
    /** @brief Начать движение вниз */
    virtual void moveDownStart() = 0;
    /** @brief Начать движение влево */
    virtual void moveLeftStart() = 0;
    /** @brief Начать движение вправо */
    virtual void moveRightStart() = 0;
    /** @brief Закончить движение */
    virtual void moveStop() = 0;
    /** @brief изменить приближение */
    virtual void zoom(qint32 zoom = 100) = 0;
    /** @brief Включаем приближение или отдаление камеры
     *  1 - приблизить, 0 - стоп, -1 - отдалить */
    virtual void startZoom(qint32 moveZoom = 0) = 0;
    /** @brief изменить фокус */
    virtual void focus(qint32 zoom = 100) = 0;
    /** @brief Получаем телеметрию от камеры */
    virtual void getTelemetry() = 0;
    /** @brief Устанавливаем новый IP адрес */
    virtual void setNewIP(QHostAddress newIP) = 0;


Q_SIGNALS:

    /** @brief Обновить данные телеметрии */
    void setTelemetry(double azim, double uglmest, qreal zoom);

};
}
#endif // ABSTRACTCAMERA_H
