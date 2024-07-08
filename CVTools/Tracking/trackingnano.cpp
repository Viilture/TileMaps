#include "trackingnano.h"

namespace cvTools {

TrackingNano::TrackingNano(QObject *parent) : AbstractTracking(parent)
{
    TrackingNano::createTracker();
}

TrackingNano::~TrackingNano()
{

}

/** @brief Создаем трекер */
void TrackingNano::createTracker()
{
    try
    {
        if(!tracker.empty()) tracker.reset();

        tracker = cv::TrackerNano::create();
    }
    catch (...)
    {
        qWarning() << "About the exception during initialization cv::TrackerNano";
    }
}

/** @brief Обновляем трекер*/
void TrackingNano::updateTracker(QSharedPointer<cv::Mat> image, cv::Rect_<int> roi)
{
    try
    {
        //Если трекер не создан, значит надо его создать
        if(tracker.empty()) createTracker();

        if(!trackImage.isNull()) trackImage.clear();

        cv::cvtColor(*image.data(), *image.data(), cv::COLOR_BGRA2BGR);
        trackImage = QSharedPointer<cv::Mat>(image);


        //Если пришел новый прямоугольник , значит явно нужна инициализация
        if(roi != cv::Rect_<int>{0,0,1,1})
        {
            tracker->init(*trackImage, roi);
            init = true;
            //qDebug() << QTime::currentTime() << "TrackingNano initialization";
        }

        tracker->update(*trackImage, roi);

        Q_EMIT updateTrackRect(QRect(roi.x, roi.y, roi.width, roi.height));
    }
    catch (...)
    {
        qWarning() << "About the exception when updating data cv::TrackerNano";
    }
}

}
