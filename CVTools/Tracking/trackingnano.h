#ifndef TRACKINGNANO_H
#define TRACKINGNANO_H

#include "abstracttracking.h"
#include <QObject>
#include <QTime>
#include <QDebug>

namespace cvTools {

class TrackingNano : public AbstractTracking
{
    Q_OBJECT

public:
    explicit TrackingNano(QObject *parent = nullptr);
    virtual ~TrackingNano();

private:

    QSharedPointer<cv::Mat> trackImage;
    cv::Ptr<cv::Tracker> tracker;
    bool init = false;

    /** @brief Создаем трекер */
    void createTracker() override;

public Q_SLOTS:

    /** @brief Обновляем трекер*/
    void updateTracker(QSharedPointer<cv::Mat> image, cv::Rect_<int> roi) override;

};

}

#endif // TRACKINGNANO_H
