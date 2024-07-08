#include "pelcocamera.h"


namespace camera {
pelcoCamera::pelcoCamera(QObject *parent, QHostAddress newIp) :
    AbstractCamera(parent)
{
    init();
    //Устанавливаем Ip адрес камеры
    pelcoCamera::setNewIP(newIp);

}

pelcoCamera::~pelcoCamera()
{
    timerTelemetry.stop();
}

void pelcoCamera::init()
{
    //Таймер обновления кадра

    connect(&timerTelemetry, &QTimer::timeout, this, [=](){getTelemetry();});
    timerTelemetry.start(350);
}

/** @brief Переместить камеру в точку
    @param Позиция центрирования
    @param Zoom */
void pelcoCamera::moveToContinuousMove(QPointF posCentr)
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{
        //Если данные не валидные выходим
        if(posCentr.x() < -100 || posCentr.x() > 100 || posCentr.y() < -100 || posCentr.y() > 100) return;

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;

        //Подготавливаем URL
        QUrl url("http://" + ipCamera.toString() + "/onvif/ptz_service");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#RelativeMove");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\""
                       " xmlns:ns1=\"http://www.onvif.org/ver20/ptz/wsdl\" "
                       "xmlns:sch=\"http://www.onvif.org/ver10/schema\"><s:Bo"
                       "dy><ns1:RelativeMove><ns1:ProfileToken>0</ns1:ProfileToken><ns1:Translation><sch:PanTilt "
                       "x=\"%1\" y=\"%2\" space=\"ur"
                       "n:schemas-pelco-com:onvif:ptz/PanTiltSpaces/PercentFOVSpace\"></sch:PanTilt><sch:Zoom x=\"100\" spa"
                       "ce=\"urn:schemas-pelco-com:onvif:ptz/ZoomSpaces/PercentFOVSpace\"></sch:Zoom></ns1:Translat"
                       "ion><ns1:Speed><sch:PanTilt x=\"0.01\" y=\"0.01\"/>"
                       "<sch:Zoom x=\"1\"/>"
                       "</ns1:Speed></ns1:RelativeMove></s:Body></s:Envelope>";

        //Вставляем координаты точки перемещения в запрос
        body = body.arg(QString::number(posCentr.x()),
                        QString::number(posCentr.y()));

        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: move to point(%1, %2)").arg(posCentr.x()).arg(posCentr.y());

            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief В абсолютных координатах переместить центр камеры в точку
    @param Позиция центрирования
    @param Zoom */
void pelcoCamera::moveToAbsolutePos(QPointF posCentr, qint32 zoom)
{
    Q_UNUSED(zoom);
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        //Если данные не валидные
        if((posCentr.x() > 360) || (posCentr.x() < 0) ||
                (posCentr.y() > 40)  || (posCentr.y() < -90)) return;

        //Приводим координаты к системе координат камеры
        double x = (posCentr.x() - 180)/180;
        double y = (posCentr.y() + 90 - 65)/65;

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;

        //Подготавливаем URL
        QUrl url("http://"+ipCamera.toString()+"/onvif/ptz_service");


        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#AbsoluteMove");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                       "xmlns:ns1=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:sch=\""
                       "http://www.onvif.org/ver10/schema\"><s:Body><ns1:AbsoluteMove><ns1:ProfileToken>"
                       "0</ns1:ProfileToken><ns1:Position><sch:PanTilt x='%1' y='%2'></sch:PanTilt></ns1:Position>"
                       "<ns1:Speed></ns1:Speed></ns1:AbsoluteMove></s:Body></s:Envelope>";

        //Вставляем координаты точки перемещения в запрос
        body = body.arg(QString::number(x),
                        QString::number(y));



        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: absolute move to point(%1, %2)").arg(posCentr.x()).arg(posCentr.y());

            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief Протирка камеры(щеткой) */
void pelcoCamera::cleaningCamera()
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;
        //Подготавливаем URL
        QUrl url("http://" + ipCamera.toString() + "/onvif/device_service");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver10/device/wsdl#SendAuxiliaryCommand");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:ns1=\"http://www.onvif.org/ver10/device/wsdl\">"
                       "<s:Body><ns1:SendAuxiliaryCommand><ns1:AuxiliaryCommand>tt:Wiper|On</ns1:AuxiliaryCommand></ns1:SendAuxiliaryCommand></s:Body></s:Envelope>";
        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: start cleaning");
            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                //QString response = QString::fromUtf8(reply->readAll());
                // Обработка ответа сервера

                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief Начать движение вверх */
void pelcoCamera::moveUpStart()
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;

        //Подготавливаем URL
        QUrl url("http://" + ipCamera.toString() + "/onvif/ptz_service");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#ContinuousMove");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                       "xmlns:ns1=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:sch=\"http://www.onvif.org/ver10/schema\">"
                       "<s:Body><ns1:ContinuousMove><ns1:ProfileToken>0</ns1:ProfileToken><ns1:Velocity><sch:PanTilt x=\"0\" y=\"%1\">"
                       "</sch:PanTilt></ns1:Velocity></ns1:ContinuousMove></s:Body></s:Envelope>";

        //Вставляем координаты точки перемещения в запрос
        body = body.arg(CAM_SPEED);

        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: move up start");

            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                // QString response = QString::fromUtf8(reply->readAll());
                // Обработка ответа сервера

                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief Начать движение вниз */
void pelcoCamera::moveDownStart()
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;

        //Подготавливаем URL
        QUrl url("http://" + ipCamera.toString() + "/onvif/ptz_service");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#ContinuousMove");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                       "xmlns:ns1=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:sch=\"http://www.onvif.org/ver10/schema\">"
                       "<s:Body><ns1:ContinuousMove><ns1:ProfileToken>0</ns1:ProfileToken><ns1:Velocity><sch:PanTilt x=\"0\" y=\"-%1\">"
                       "</sch:PanTilt></ns1:Velocity></ns1:ContinuousMove></s:Body></s:Envelope>";

        //Вставляем координаты точки перемещения в запрос
        body = body.arg(CAM_SPEED);

        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: move down start");

            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                //QString response = QString::fromUtf8(reply->readAll());
                // Обработка ответа сервера

                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief Начать движение влево */
void pelcoCamera::moveLeftStart()
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;

        //Подготавливаем URL
        QUrl url("http://" + ipCamera.toString() + "/onvif/ptz_service");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#ContinuousMove");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                       "xmlns:ns1=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:sch=\"http://www.onvif.org/ver10/schema\">"
                       "<s:Body><ns1:ContinuousMove><ns1:ProfileToken>0</ns1:ProfileToken><ns1:Velocity><sch:PanTilt x=\"-%1\" y=\"0\">"
                       "</sch:PanTilt></ns1:Velocity></ns1:ContinuousMove></s:Body></s:Envelope>";

        //Вставляем координаты точки перемещения в запрос
        body = body.arg(CAM_SPEED);

        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: move left start");

            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                //QString response = QString::fromUtf8(reply->readAll());
                // Обработка ответа сервера

                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief Начать движение вправо */
void pelcoCamera::moveRightStart()
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;

        //Подготавливаем URL
        QUrl url("http://" + ipCamera.toString() + "/onvif/ptz_service");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#ContinuousMove");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                       "xmlns:ns1=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:sch=\"http://www.onvif.org/ver10/schema\">"
                       "<s:Body><ns1:ContinuousMove><ns1:ProfileToken>0</ns1:ProfileToken><ns1:Velocity><sch:PanTilt x=\"%1\" y=\"0\">"
                       "</sch:PanTilt></ns1:Velocity></ns1:ContinuousMove></s:Body></s:Envelope>";

        //Вставляем координаты точки перемещения в запрос
        body = body.arg(CAM_SPEED);

        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: move right start");

            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief Закончить движение*/
void pelcoCamera::moveStop()
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;

        //Подготавливаем URL
        QUrl url("http://"+ipCamera.toString()+"/onvif/ptz_service");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#ContinuousMove");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                       "xmlns:ns1=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:sch=\"http://www.onvif.org/ver10/schema\">"
                       "<s:Body><ns1:ContinuousMove><ns1:ProfileToken>0</ns1:ProfileToken><ns1:Velocity><sch:PanTilt x=\"0\" y=\"0\">"
                       "</sch:PanTilt></ns1:Velocity></ns1:ContinuousMove></s:Body></s:Envelope>";


        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: move stop");

            connect(reply, &QNetworkReply::finished, this, [=]()
            {

                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief изменить приближение */
void pelcoCamera::zoom(qint32 zoom)
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        //Если данные валидные
        if((zoom>0) && (zoom<200))
        {
            //Отправляем POST Запрос по http на PELCO камеру
            static QNetworkAccessManager manager;

            //Подготавливаем URL
            QUrl url("http://"+ipCamera.toString()+"/onvif/ptz_service");

            //Подготавливаем Заголовок
            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
            request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#ContinuousMove");
            request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

            //Подготавливаем запрос
            QString body = "<s:Envelope xmlns:s='http://schemas.xmlsoap.org/soap/envelope/' xmlns:ns1='http://www.onvif.org/ver20/ptz/wsdl'"
                           " xmlns:sch='http://www.onvif.org/ver10/schema'><s:Body><ns1:RelativeMove><ns1:ProfileToken>0</ns1:ProfileToken>"
                           "<ns1:Translation><sch:PanTilt x='0' y='0' space='urn:schemas-pelco-com:onvif:ptz/PanTiltSpaces/"
                           "PercentFOVSpace'></sch:PanTilt><sch:Zoom x='%3' space='urn:schemas-pelco-com:onvif:ptz/ZoomSpaces/PercentFOVSpace'></sch:Zoom>"
                           "</ns1:Translation><ns1:Speed></ns1:Speed></ns1:RelativeMove></s:Body></s:Envelope>";

            //Вставляем координаты точки перемещения в запрос
            body = body.arg(QString::number(zoom));

            //Отправляем
            if(reply == nullptr)
            {
                reply =  manager.post(request, body.toUtf8());
                qDebug() << QStringLiteral("POST Request to Pelco Camera: New Zoom %1%").arg(zoom);

                connect(reply, &QNetworkReply::finished, this, [=]()
                {
                    //Чистим память
                    if(reply != nullptr)
                    {
                        delete reply; reply = nullptr;
                    }
                });
            }
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief Включаем приближение или отдаление камеры
 *  1 - приблизить, 0 - стоп, -1 - отдалить */
void pelcoCamera::startZoom(qint32 moveZoom)
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        if(moveZoom > 1) moveZoom = 1;
        else if(moveZoom < -1) moveZoom = -1;

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;

        //Подготавливаем URL
        QUrl url("http://"+ipCamera.toString()+"/onvif/ptz_service");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#ContinuousMove");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                       "xmlns:ns1=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:sch=\""
                       "http://www.onvif.org/ver10/schema\"><s:Body><ns1:ContinuousMove><ns1:ProfileToken>0</ns1"
                       ":ProfileToken><ns1:Velocity><sch:Zoom x=\"%1\"></sch:Zoom></ns1:Velocity></ns1:ContinuousMove>"
                       "</s:Body></s:Envelope>";
        body = body.arg(QString::number(moveZoom));


        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: Start Zoom");

            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

/** @brief изменить фокус */
void pelcoCamera::focus(qint32 focus)
{
    QMutexLocker lock(&mutex);
    static QNetworkReply* reply = nullptr;
    try{

        //Отправляем POST Запрос по http на PELCO камеру
        static QNetworkAccessManager manager;

        //Подготавливаем URL
        QUrl url("http://"+ipCamera.toString()+"/control/LensControl-1");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "urn:schemas-pelco-com:service:LensControl:1#Focus");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                       "xmlns:ns1=\"urn:schemas-pelco-com:service:LensControl:1\"><s:Body>"
                       "<ns1:Focus><ns1:nearFar>%1</ns1:nearFar></ns1:Focus></s:Body></s:Envelope>";

        //Вставляем координаты точки перемещения в запрос
        body = body.arg(QString::number(focus));

        //Отправляем
        if(reply == nullptr)
        {
            reply =  manager.post(request, body.toUtf8());
            qDebug() << QStringLiteral("POST Request to Pelco Camera: New Focus %1").arg(focus);

            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                //Чистим память
                if(reply != nullptr)
                {
                    delete reply; reply = nullptr;
                }
            });
        }
    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }

}

/** @brief Получаем телеметрию от камеры */
void pelcoCamera::getTelemetry()
{
    QMutexLocker lock(&mutex);

    //Отправляем POST Запрос по http на PELCO камеру
    static QNetworkAccessManager manager;
    static QNetworkReply* reply = nullptr;
    try {

        //Подготавливаем URL
        QUrl url("http://"+ipCamera.toString()+"/onvif/ptz_service");

        //Подготавливаем Заголовок
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        request.setRawHeader("action", "http://www.onvif.org/ver20/ptz/wsdl#GetStatus");
        request.setRawHeader("Authorization", "Basic " + QByteArray(QStringLiteral("admin:1234567890").toUtf8()).toBase64());

        //Подготавливаем запрос
        QString body = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:ns1=\"http:"
                       "//www.onvif.org/ver20/ptz/wsdl\" xmlns:sch=\"http://www.onvif.org/ver10/schema\"><s:Body>"
                       "<ns1:GetStatus><ns1:ProfileToken>0</ns1:ProfileToken></ns1:GetStatus></s:Body></s:Envelope>";


        //Отправляем
        if((reply == nullptr) || (ipUpdate == true))
        {
            ipUpdate = false;
            reply =  manager.post(request, body.toUtf8());

            connect(reply, &QNetworkReply::finished, this, [=]()
            {
                try {
                    QString response = QString::fromUtf8(reply->readAll());

                    if(response.size() < 100)
                    {
                        // Обработка ответа сервера
                        Q_EMIT setTelemetry(0, 0, 0);
                        return;
                    }

                    //Вычисляем Координату азимута
                    int indexAzim = response.indexOf(QLatin1String(" x=") );                                  //Ищем нужную строку
                    QString str = response.mid(indexAzim + 4, 7);                              //Вырезаем часть с данными из строки
                    str.remove(QRegExp("[^\\d-.]"));                                           //Удаляем все кроме цифр . и -
                    double azim = str.toDouble();
                    azim = (azim + 1) * 180;
                    if ((azim > 360) || (azim < 0)) return;

                    //Вычисляем Координату угломеста
                    int indexUgol = response.indexOf(QLatin1String(" y=") );                                  //Ищем нужную строку
                    QString strUgol = response.mid(indexUgol + 4, 7);                          //Вырезаем часть с данными из строки
                    strUgol.remove(QRegExp("[^\\d-.]"));                                       //Удаляем все кроме цифр . и -
                    double ugol = strUgol.toDouble();
                    ugol = ((ugol + 1) * 65) - 90;
                    if ((ugol > 40) || (ugol < -90)) return;

                    //Вычисляем уровень zoom
                    int indexZoom = response.indexOf(QLatin1String("ZoomSpaces/PositionGenericSpace\" x=") ); //Ищем нужную строку
                    QString strZoom = response.mid(indexZoom + 37, 7);                         //Вырезаем часть с данными из строки
                    strZoom.remove(QRegExp("[^\\d-.]"));                                       //Удаляем все кроме цифр . и -
                    double zoom = strZoom.toDouble();
                    zoom = round((zoom * 36000) + 100)/100;
                    if ((zoom > 360) || (zoom < 0)) return;

                    // Обработка ответа сервера
                    Q_EMIT setTelemetry(azim, ugol, zoom);

                    //Чистим память
                    if(reply != nullptr)
                    {
                        delete reply; reply = nullptr;
                    }
                }
                catch (...)
                {
                    // Обработка ошибки invalid_argument
                    qDebug() << "Error to request Telemetry - pelcocamera.cpp/Telemetry" << '\n';
                    if(reply != nullptr)
                    {
                        delete reply; reply = nullptr;
                    }
                }
            });
        }

    }
    catch (...)
    {
        // Обработка ошибки invalid_argument
        qDebug() << "Error to POST Telemetry - pelcocamera.cpp/Telemetry/All" << '\n';
        if(reply != nullptr)
        {
            delete reply; reply = nullptr;
        }
    }
}

void pelcoCamera::setNewIP(QHostAddress newIP)
{
    ipCamera = newIP;
    ipUpdate = true;
}
}
