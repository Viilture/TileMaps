#ifndef VIDEOLOGGING_H
#define VIDEOLOGGING_H

#include <QObject>
#include <QScreen>
#include <QSharedPointer>
#include <memory>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QPixmap>
#include <QImage>
#include <QGuiApplication>

#include <opencv2/opencv.hpp>
#include "Transform.h"
#include "settingsBR.h"

#define Barier2INI "/Barier2INI"

/** @class Класс записи видеолога рабочего стола в виде видео */
class VideoLogging : public QObject
{
    Q_OBJECT

    /** @brief Состояние работы класса*/
    bool status = false;

    int32_t fps = 1;

    /** @brief Время файла в миллисекундах */
    int32_t timFileInMSec = 5000;

    QSharedPointer<QTimer> timerVidlog;

    /** @brief Скрин экрана */
    QScreen* screen = nullptr;
    /** @brief Запись видео */
    cv::VideoWriter* videoWritter = nullptr;

    /** @brief Устанавливаем новый видеовыход */
    void setNewVideoOut();

    /** @brief Создание имени */
    std::string createName();



public:
    explicit VideoLogging(QThread* newThread = nullptr, QObject *parent = nullptr);
    virtual ~VideoLogging();

    static QSharedPointer<VideoLogging> create();

Q_SIGNALS:

public Q_SLOTS:

    void threadStarted();

    void eventTimer();

};

#endif // VIDEOLOGGING_H
