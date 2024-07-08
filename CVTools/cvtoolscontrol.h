#ifndef CVTOOLSCONTROL_H
#define CVTOOLSCONTROL_H

#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include <QMutex>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QRect>
#include <QSettings>

#include "abstracttracking.h"
#include "trackingnano.h"

#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/xphoto.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/xobjdetect.hpp>

namespace cvTools {

enum stateCVTools
{
    noCV = 1,
    bitwiseNot,
    autoContrast,
    gammaCorrect,
    brightCorrect,
    removingNoise,
    removingNoiseMeansDenoising,
    stylization,
    detailEnhance,
    pencilSketch,
    Canny,
};

class CVToolsControl : public QObject
{
    Q_OBJECT

    stateCVTools stateCV = stateCVTools::noCV;

    QMutex mutex;

    /** @brief Rect полученый после обработки нейросети */
    QRect RectFromTracking = QRect(0,0,1,1); //Rect полученый после обработки нейросети
    /** @brief Rect новой области трекинга*/
    QRect rectForNewInit = QRect(0,0,1,1); //Rect новой области трекинга
    /** @brief Нужна ли новая инициализия трекера */
    bool newInitNeed = false;
    /** @brief Состояние отслеживания трекера */
    bool stateTrack = false;
    /** @brief Происходит ли поиск траектории в данный момент */
    bool trackingInCurTime = false;
    /** @brief Был ли трекинг проинициализирован */
    bool initTracking = false;

    cv::Rect_<int> roi;

    QSharedPointer<cv::Mat> image;

    /** @brief Устанавливаем тип трекера */
    void setTypeTracker(typeTrack newTypeTrack = typeTrack::Nano);

    QSharedPointer<AbstractTracking> tracker;

    /** @brief Поток для работы с трекером */
    QThread threadTracking;

    /** @brief Текущий алгоритм поиска траектории */
    typeTrack curTypeTrack = typeTrack::Nano;

    //Параметры CV
    //Гамма параметр гамма коррекции
    double gammaParam = 0.6;

    //Параметры коррекции яркости
    double   brightGammaParam = 0.6;
    int32_t  widthCore = 8;
    int32_t  heightCore = 8;

    //Параметры шумоподавления1
    int32_t diametrNoise = 9;
    int32_t sigmaColorNoise = 75;
    int32_t sigmaSpaceNoise = 75;

    //Параметры шумоподавления2
    int32_t hNoise = 3;
    int32_t tempWindSizeNoise = 7;
    int32_t searchWindSize = 21;

    /** @brief Гистограммное выравнивание (англ. histogram equalization) */
    const cv::Mat autoContrast(const cv::Mat *img);
    /** @brief Гамма коррекция */
    const cv::Mat gammaCorrect(const cv::Mat *img, double gammaParam);
    /** @brief Коррекция яркости*/
    const cv::Mat brightCorrect(const cv::Mat *img, double gammaParam, int32_t widthCore, int32_t heightCore);
    /** @brief Уменьшение шума */
    const cv::Mat removingNoise1(const cv::Mat *img, int32_t Diametr, double sigmaColor, double sigmaSpace);
    /** @brief Уменьшение шума c помощью фильтр MeansDenoising*/
    const cv::Mat removingNoiseMeansDenoising(const cv::Mat *img, int32_t h, int32_t tempWindSize, int32_t searchWindSize);
    /** @brief Уменьшение шума c помощью фильтр GaussianBlur*/
    const cv::Mat removingNoiseGaussianBlur(const cv::Mat *img, int kernelSize, double sigma);

public:
    explicit CVToolsControl(QObject *parent = nullptr);
    virtual ~CVToolsControl();

    bool getStateTrack() const;
    void setStateTrack(bool newStateTrack);

    const QRect &getRectFromTracking() const;
    void setRectFromTracking(const QRect &newRectFromTracking);

    const QRect &getRectForNewInit() const;
    void setRectForNewInit(const QRect &newRectForNewInit);


public Q_SLOTS:

    /** @brief Устанавливаем параметры обработки */
    void setParam(cvTools::stateCVTools newState);

    /** @brief Обработка изобажения */
    bool processing(cv::Mat& image);

    /** @brief Обновляем прямоугольника обьекта трекинга */
    void updateTrackRect(QRect updRect);

Q_SIGNALS:

    /** @brief Выдаем данные обнаруженные в изображении*/
    void updateData();

    /** @brief Обновляем данные трекера */
    void updateTracker(QSharedPointer<cv::Mat> image, cv::Rect_<int> roi = cv::Rect_<int>{0,0,1,1});

    /** @brief Устанавливаем в класс Tracking новый прямоугольника обьекта трекинга */
    void needNewInit();

    /** @brief Пришел новый прямоугольник с трекера */
    void newTrackRect(QRect rect);

};
}

#endif // CVTOOLSCONTROL_H
