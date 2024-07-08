#ifndef ABSTRACTTRACKING_H
#define ABSTRACTTRACKING_H

#include <QObject>
#include <QRect>

#include <opencv2/tracking.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>


namespace cvTools {


enum typeTrack
{
    KCF = 1,
    MIL = 2,
    GOTURN = 3,
    CSRT = 4,
    DaSiamRPN = 5,
    Nano = 6,
};

class AbstractTracking : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTracking(QObject *parent = nullptr);
    virtual ~AbstractTracking();

private:

    /** @brief Создаем трекер */
    virtual void createTracker() = 0;

public Q_SLOTS:

    /** @brief Обновляем трекер*/
    virtual void updateTracker(QSharedPointer<cv::Mat> image, cv::Rect_<int> roi) = 0;

Q_SIGNALS:

    /** @brief Обновляем прямоугольника обьекта трекинга */
    void updateTrackRect(QRect updRect);

};

}

#endif // ABSTRACTTRACKING_H
