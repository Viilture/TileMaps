#ifndef SVOIMATH_H
#define SVOIMATH_H

#include <QtMath>


/** @brief Количество секунд в 1 круг 360 градусов */
constexpr int64_t SEC_IN_ROUND = 360*60*60;

/** @brief Значение 1 секунды для измерения дальности */
constexpr double PI_1296000 = 2*M_PI/SEC_IN_ROUND;


///** @brief Рассчитываем расстояние дурги по значению радиуса и угла в секундах
//    @param radius - Радиус окружности в метрах
//    @param angleSecond - угол дуги в секундах */
//double calcDistanceAngle(double& radius, int32_t& angleSecond)
//{
//    return (angleSecond*PI_1296000)*radius;
//}


#endif // SVOIMATH_H
