#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include <QObject>
#include <QPointF>
#include <QHostAddress>
#include <QWeakPointer>

#include "scenecontrol.h"
#include "mapcamera.h"

class CameraControl : public QObject
{
    Q_OBJECT

    /** @brief Cсылка на класс управления сценой */
    sceneControl* scenControl = nullptr;

    /** @brief Отображение камеры */
   QSharedPointer<mapCamera> viewCamera;


   /** @brief Координата центра радара в WGS84 */
   QPointF coordWGS84 = QPointF(0, 0);

   /** @brief Ориентация по азимуту, тангажу и крену */
   double azim = 0.0,
          tang = 0.0,
          kren = 0.0;

   /** @brief Адрес РЛУ */
   QHostAddress ip4 = QHostAddress::Any;

   /** @brief Имя радара */
   QString name = QStringLiteral("Камера");

   /** @brief Создаем визуализацию камеры на сцене */
   void createViewCamera();

public:
    explicit CameraControl(QObject *parent = nullptr, sceneControl* newScenControl = nullptr);
    virtual ~CameraControl();

   QPointF getCoordWGS84() const;
   void setCoordWGS84(QPointF newCoordWGS84);

   double getAzim() const;
   void setAzim(double newAzim);

   double getTang() const;
   void setTang(double newTang);

   double getKren() const;
   void setKren(double newKren);

   const QHostAddress &getIp4() const;
   void setIp4(const QHostAddress &newIp4);

   uint32_t getLitera() const;
   void setLitera(uint32_t newLitera);

   void setLat(double newLat);
   void setLong(double newLong);

   const QString &getName() const;
   void setName(const QString &newName);

    /** @brief центрировать карту */
    void centerMap();

    /** @brief Помигать радаром */
    void blinked();

};

#endif // CAMERACONTROL_H
