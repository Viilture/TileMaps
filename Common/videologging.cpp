#include "videologging.h"

VideoLogging::VideoLogging(QThread* newThread, QObject *parent) : QObject(parent)
{


    if(newThread != nullptr)
    {
        this->moveToThread(newThread);
        newThread->start();
        connect(newThread, &QThread::started, this, &VideoLogging::threadStarted);
    }
}

VideoLogging::~VideoLogging()
{
    if(!timerVidlog.isNull())
    timerVidlog->stop();

    this->thread()->quit();
    this->thread()->wait();
}

QSharedPointer<VideoLogging> VideoLogging::create()
{
    QThread* thread = new QThread();

    static QSharedPointer<VideoLogging> videoLog = QSharedPointer<VideoLogging>::create(thread);

    return videoLog;
}

void VideoLogging::threadStarted()
{
    SettingsBR set;
    //    set->setValue("VideoLogger/FPS", 2);
    //    set->setValue("VideoLogger/status", 1);
    //    set->setValue("VideoLogger/timFileInMSec", 20000);

    status = set.value("VideoLogger/status", 0).toBool();
    if(!status) return;

    fps = set.value("VideoLogger/FPS", 2).toInt();
    timFileInMSec = set.value("VideoLogger/timFileInMSec", 20000).toInt();

    if(fps < 1)  fps = 1;
    if(fps > 60) fps = 60;

    screen = QGuiApplication::primaryScreen();

    std::string strVideo = createName();

    videoWritter = new cv::VideoWriter(strVideo, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(1920, 1080));

    timerVidlog = QSharedPointer<QTimer>::create(this);
    connect(timerVidlog.data(), &QTimer::timeout, this, &VideoLogging::eventTimer);
    timerVidlog->start(1000/fps);
}

void VideoLogging::eventTimer()
{
    if(!status) return;

    static QTime time = QTime::currentTime();

    if((QTime::currentTime().msecsSinceStartOfDay() - time.msecsSinceStartOfDay()) > timFileInMSec)
    {
        setNewVideoOut();
        time = QTime::currentTime();
    }


    const QPixmap originalPixmap = screen->grabWindow(0);

    cv::Mat frame = QPixmapToCvMat(originalPixmap , true);

    *videoWritter << frame;
}

/** @brief Устанавливаем новый видеовыход */
void VideoLogging::setNewVideoOut()
{
    std::string strVideo = createName();

    if(videoWritter->isOpened())
        videoWritter->release();

    videoWritter->open(strVideo, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 2, cv::Size(1920, 1080));

}

std::string VideoLogging::createName()
{
    QString str = QDate::currentDate().toString() + " "  + QString::number(QTime::currentTime().hour()) + "h" + QString::number(QTime::currentTime().minute())
            + "m" + QString::number(QTime::currentTime().second()) + "s" + ".avi";

    return str.toStdString();
}
