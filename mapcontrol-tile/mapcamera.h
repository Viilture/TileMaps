#ifndef MAPCAMERA_H
#define MAPCAMERA_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QtMath>
#include <QPointF>
#include <QString>

#include "mapcontrol.h"
#include "guts/Conversions.h"
#include "scalc.h"

class mapCamera : public QGraphicsPathItem
{

    /** @brief Координаты в долготе и широте */
   QPointF centerP    = QPointF(0,0);
   double  azimut     = 0;
   double  sector     = 40;
   double  beginRange = 100;
   double  endRange   = 1000;

   /** @brief Класс преобразования координат*/
   Conversions conv;
   /** @brief Класс управления картой */
   MapControl* mapContr = nullptr;

   calculate::Scalc calc;

public:
    explicit mapCamera(MapControl* newMapContr = nullptr,
                      QPointF CenterP   = QPointF(37, 55),
                      double azimut     = 0,
                      double sector     = 40,
                      double beginRange = 100,
                      double endRange   = 1000);
    virtual ~mapCamera();


   QPointF getCenterP() const;
   void setCenterP(QPointF newCenterP);

public Q_SLOTS:
   /** @brief Обновление графики */
   void updateGraph();

   /** @brief Обновить параметры отображения РЛУ */
   void setSettings();

   /** @brief Обновить параметры азимута */
   void setAzim(double azim);

};

#endif // MAPCAMERA_H
