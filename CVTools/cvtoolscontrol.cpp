#include "cvtoolscontrol.h"

#define Barier2INI "/Barier2INI"

namespace cvTools {
CVToolsControl::CVToolsControl(QObject *parent) : QObject(parent)
{
    setTypeTracker(typeTrack::Nano);

    qRegisterMetaType<QSharedPointer<cv::Mat>>("QSharedPointer<cv::Mat>");
    qRegisterMetaType<cv::Rect_<int>>("cv::Rect_<int>");

    QSettings set(QCoreApplication::applicationDirPath() + Barier2INI, QSettings::IniFormat);
    //    set.setValue("CvTools/gammaParam", 0.6);
    //    set.setValue("CvTools/brightGammaParam", 0.6);
    //    set.setValue("CvTools/widthCore", 8);
    //    set.setValue("CvTools/heightCore", 8);
    //    set.setValue("CvTools/diametrNoise", 9);
    //    set.setValue("CvTools/sigmaColorNoise", 75);
    //    set.setValue("CvTools/sigmaSpaceNoise", 75);
    //    set.setValue("CvTools/hNoise", 3);
    //    set.setValue("CvTools/tempWindSizeNoise", 7);
    //    set.setValue("CvTools/searchWindSize", 21);


    //Гамма параметр гамма коррекции
    gammaParam        = set.value("CvTools/gammaParam", 0.6).toDouble();
    brightGammaParam  = set.value("CvTools/brightGammaParam", 0.6).toDouble();
    widthCore         = set.value("CvTools/widthCore", 8).toInt();
    heightCore        = set.value("CvTools/heightCore", 8).toInt();
    diametrNoise      = set.value("CvTools/diametrNoise", 9).toInt();
    sigmaColorNoise   = set.value("CvTools/sigmaColorNoise", 75).toInt();
    sigmaSpaceNoise   = set.value("CvTools/sigmaSpaceNoise", 75).toInt();
    hNoise            = set.value("CvTools/hNoise", 3).toInt();
    tempWindSizeNoise = set.value("CvTools/tempWindSizeNoise", 7).toInt();
    searchWindSize    = set.value("CvTools/searchWindSize", 21).toInt();

}

CVToolsControl::~CVToolsControl()
{
    threadTracking.quit();
    threadTracking.wait();
}

/** @brief Устанавливаем параметры обработки */
void CVToolsControl::setParam(stateCVTools newState)
{
    QMutexLocker lock(&mutex);

    this->stateCV = newState;
}

/** @brief Обработка изобажения */
bool CVToolsControl::processing(cv::Mat &image)
{
    if (image.empty()) return false;

    try {

        //cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);

        //Обработка кадра
        switch (stateCV)
        {
        case stateCVTools::bitwiseNot:
        {
            cv::bitwise_not(image,image);
            break;
        }
        case stateCVTools::autoContrast:
        {
            image = autoContrast(&image);
            break;
        }
        case stateCVTools::gammaCorrect:
        {
            image = gammaCorrect(&image, gammaParam);
            break;
        }
        case stateCVTools::brightCorrect:
        {
            image = brightCorrect(&image,
                                  brightGammaParam,
                                  widthCore,
                                  heightCore);
            break;
        }
        case stateCVTools::removingNoise:
        {
            image = removingNoise1(&image,
                                   diametrNoise,
                                   sigmaColorNoise,
                                   sigmaSpaceNoise);
            break;
        }
        case stateCVTools::removingNoiseMeansDenoising:
        {
            image = removingNoiseMeansDenoising(&image,
                                                hNoise,
                                                tempWindSizeNoise,
                                                searchWindSize);
            break;
        }
        case stateCVTools::stylization:
        {
            cv::stylization(image,image);
            break;
        }
        case stateCVTools::detailEnhance:
        {
            cv::detailEnhance(image,image);
            break;
        }
        case stateCVTools::pencilSketch:
        {
            cv::pencilSketch(image, image, image);
            break;
        }
        case stateCVTools::Canny:
        {
            cv::Canny(image, image, 100, 200);
            break;
        }

        default: break;

        }

        QSharedPointer<cv::Mat> mat = QSharedPointer<cv::Mat>::create(image.clone());
        //Ищем на ищображение новые траектории, если уже нашли на предыдущей картинке
        if(stateTrack && (!trackingInCurTime))
        {   //Если нужна инициализация, то посылаем прямоугольник инициализации
            if(newInitNeed)
            {
                roi.x      = rectForNewInit.x();
                roi.y      = rectForNewInit.y();
                roi.width  = rectForNewInit.width();
                roi.height = rectForNewInit.height();
                newInitNeed = false;
                Q_EMIT updateTracker(mat, roi);

                initTracking = true;
                trackingInCurTime = true;
            }
            else if(initTracking == true)
            {
                roi.x      = 0;
                roi.y      = 0;
                roi.width  = 1;
                roi.height = 1;

                Q_EMIT updateTracker(mat, roi);
                trackingInCurTime = true;
            }}
    }
    catch (...)
    {
        //qDebug() << "catch in CVToolsControl::processing ";
    }
    return true;
}

void CVToolsControl::updateTrackRect(QRect updRect)
{
    trackingInCurTime = false;
    RectFromTracking = updRect;

    //Оповещаем всех, что пришел новый прямоугольник после траекторий
    Q_EMIT newTrackRect(RectFromTracking);

}

const QRect &CVToolsControl::getRectForNewInit() const
{
    return rectForNewInit;
}

void CVToolsControl::setRectForNewInit(const QRect &newRectForNewInit)
{

    rectForNewInit = newRectForNewInit;
    //Нужна новая инициализация
    newInitNeed = true;
}

const QRect &CVToolsControl::getRectFromTracking() const
{
    return RectFromTracking;
}

void CVToolsControl::setRectFromTracking(const QRect &newRectFromTracking)
{
    RectFromTracking = newRectFromTracking;
}

bool CVToolsControl::getStateTrack() const
{
    return stateTrack;
}

void CVToolsControl::setStateTrack(bool newStateTrack)
{
    stateTrack = newStateTrack;
    initTracking = false;
}

/** @brief Гистограммное выравнивание (англ. histogram equalization) */
const cv::Mat CVToolsControl::autoContrast(const cv::Mat *img)
{
    QTime time = QTime::currentTime();

    // разделение изображения на каналы
    cv::Mat bgr[3];
    split(*img, bgr);

    // выполнение гистограммного выравнивания для каждого канала
    cv::Mat b_eq, g_eq, r_eq;

    // создание вектора для хранения фьючерсов
    QVector<QFuture<void>> futures;

    // запуск выполнения equalizeHist в 3 потоках
    futures.append(QtConcurrent::run([=, &bgr, &b_eq](){equalizeHist(bgr[0], b_eq); }));
    futures.append(QtConcurrent::run([=, &bgr, &g_eq](){equalizeHist(bgr[1], g_eq); }));
    equalizeHist(bgr[2], r_eq);

    // ожидание завершения выполнения всех фьючерсов
    for (int i = 0; i < futures.size(); i++) {
        futures[i].waitForFinished();
    }

    // объединение каналов обратно в цветное изображение
    cv:: Mat img_eq;
    merge(std::vector<cv::Mat>{b_eq, g_eq, r_eq}, img_eq);

    return img_eq;
}

/** @brief Гамма коррекция */
const cv::Mat CVToolsControl::gammaCorrect(const cv::Mat *img, double gammaParam)
{
    // определение гамма-параметра
    double gamma = gammaParam;
    QTime time = QTime::currentTime();
    // создание таблицы преобразования
    cv::Mat lut(1, 256, CV_8UC1);
    for (int i = 0; i < 256; i++)
    {
        lut.at<uchar>(i) = cv::saturate_cast<uchar>(pow((double)i / 255.0, gamma) * 255.0);
    }

    // применение гамма-коррекции
    cv::Mat img_gamma;
    cv::LUT(*img, lut, img_gamma);

    return img_gamma;
}

/** @brief Коррекция яркости*/
const cv::Mat CVToolsControl::brightCorrect(const cv::Mat *img, double gammaParam, int32_t widthCore, int32_t heightCore)
{
    // разделение каналов цветного изображения
    std::vector<cv::Mat> channels;
    cv::split(*img, channels);

    // применение адаптивной гамма-коррекции на каждом канале
    /*double clipLimit = 4.0 - это параметр адаптивной гамма-коррекции, который определяет максимальный уровень контраста для
         * каждого пикселя изображения. Этот параметр контролирует, насколько сильно гистограмма изображения будет ограничена сверху.
         * Если значение параметра clipLimit больше, то гистограмма будет более ограничена, что приведет к большему усилению контраста
         * в изображении.
         * Если значение clipLimit меньше, то гистограмма будет менее ограничена, что приведет к более мягкому усилению контраста.*/
    double clipLimit = gammaParam;
    /*параметр адаптивной гамма-коррекции, который определяет размер блока, используемый для вычисления локального гистограммного
         * преобразования (LHE). Каждый блок размером tileSize обрабатывается независимо, и гистограммное преобразование применяется
         *  только к пикселям внутри блока. Затем результаты объединяются, чтобы получить итоговое изображение. Чем меньше значение
         *  tileSize, тем более локальным будет гистограммное преобразование, что может привести к более точной коррекции гистограммы,
         *   но может также привести к более заметным артефактам в изображении.
         * Обычно размер блока выбирается экспериментально, чтобы достичь оптимального баланса между точностью и артефактами.*/
    cv::Size tileSize(widthCore, heightCore);
    cv::Ptr<cv::CLAHE> clahe = createCLAHE(clipLimit, tileSize);
    for (int i = 0; i < 3; i++) {
        clahe->apply(channels[i], channels[i]);
    }

    // слияние каналов обратно в цветное изображение
    cv::Mat img_adaptive_gamma;
    merge(channels, img_adaptive_gamma);

    return img_adaptive_gamma;

}

/** @brief Уменьшение шума */
const cv::Mat CVToolsControl::removingNoise1(const cv::Mat *img, int32_t Diametr, double sigmaColor, double sigmaSpace)
{
    cv::Mat filteredImage;
    // Применяем фильтр bilateralFilter для уменьшения шума и улучшения контрастности
    /*
        * src: входное изображение. Может быть одноканальным или трехканальным (цветным).
        * dst: выходное изображение того же размера и типа, что и входное изображение (src).
        * d: диаметр окрестности пикселя, используемый для фильтрации. Должен быть нечётным числом. Значение 9 обычно хорошо работает для большинства изображений.
        * sigmaColor: стандартное отклонение цвета в пикселях. Чем больше это значение, тем больше пиксели будут считаться соседними, и тем сильнее будет проявляться эффект размытия. Значение 75 обычно хорошо работает для большинства изображений.
        * sigmaSpace: стандартное отклонение пространства пикселей в пикселях. Чем больше это значение, тем больше будет учитываться расстояние между пикселями, и тем более сильно будет проявляться эффект размытия. Значение 75 обычно хорошо работает для большинства изображений.
        * borderType: тип границы. По умолчанию равен BORDER_DEFAULT.
        */
    // разделение изображения на каналы
    cv::bilateralFilter(*img, filteredImage, Diametr, sigmaColor, sigmaSpace);

    return filteredImage;
}

/** @brief Уменьшение шума c помощью фильтр MeansDenoising*/
const cv::Mat CVToolsControl::removingNoiseMeansDenoising(const cv::Mat *img, int32_t h, int32_t tempWindSize , int32_t searchWindSize)
{
    cv::Mat filteredImage;

    // Применяем фильтр fastNlMeansDenoising для уменьшения шума и улучшения контрастности
    cv::fastNlMeansDenoisingColored(*img, filteredImage, h, 10, tempWindSize, searchWindSize);

    return filteredImage;
}

/** @brief Уменьшение шума c помощью фильтр GaussianBlur*/
const cv::Mat CVToolsControl::removingNoiseGaussianBlur(const cv::Mat *img, int kernelSize, double sigma)
{
    cv::Mat filteredImage;

    // Применяем фильтр fastNlMeansDenoising для уменьшения шума и улучшения контрастности
    cv::GaussianBlur(*img, filteredImage, cv::Size(kernelSize, kernelSize), sigma, sigma);

    return filteredImage;
}

/** @brief Устанавливаем тип трекера */
void CVToolsControl::setTypeTracker(typeTrack newTypeTrack)
{
    QMutexLocker lock(&mutex);

    //Если правильный трекер уже создан
    if((curTypeTrack == newTypeTrack) && (!tracker.isNull())) return;

    curTypeTrack = newTypeTrack;
    //Если трекер был создан, то удаляем его
    if(!tracker.isNull())
    {
        disconnect(this, &CVToolsControl::updateTracker, tracker.data(), &AbstractTracking::updateTracker);
        disconnect(tracker.data(), &AbstractTracking::updateTrackRect, this, &CVToolsControl::updateTrackRect);
        threadTracking.quit();
        threadTracking.wait();
        tracker.clear();
    }

    //Создаем трекер
    try
    {
        switch (curTypeTrack)
        {
        // case typeTrack::KCF:       {tracker = cv::TrackerKCF::create();        break;}
        // case typeTrack::MIL:       {tracker = cv::TrackerMIL::create();        break;}
        // case typeTrack::GOTURN:    {tracker = cv::TrackerGOTURN::create();     break;}
        // case typeTrack::CSRT:      {tracker = cv::TrackerCSRT::create();       break;}
        // case typeTrack::DaSiamRPN: {tracker = cv::TrackerDaSiamRPN::create();  break;}
        case typeTrack::Nano:      {tracker = QSharedPointer<TrackingNano>::create();       break;}

        default: {break;}
        }

        connect(this, &CVToolsControl::updateTracker, tracker.data(), &AbstractTracking::updateTracker);
        connect(tracker.data(), &AbstractTracking::updateTrackRect, this, &CVToolsControl::updateTrackRect);

        //Добавляем в поток
        tracker->moveToThread(&threadTracking);
        threadTracking.start();

    }
    catch (...)
    {
        qWarning() << "About the exception during initialization cv::Tracker";
    }

}
}
