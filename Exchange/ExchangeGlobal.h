#ifndef RLSEXCHANGEGLOBAL_H
#define RLSEXCHANGEGLOBAL_H

#include <QObject>
#include <QVector>
#include <QDataStream>
#include <QRandomGenerator>
#include <QTest>
#include <type_traits>

class HEADER;
class CmdSetMode;

#pragma pack(push, 1)

template<typename T>
T randomValue(T& state)
{
    if(std::is_same<T, uint8_t>())
    {
        return state = QRandomGenerator::global()->bounded(0, 255);
    }
    else if(std::is_same<T, int8_t>())
    {
        return state = QRandomGenerator::global()->bounded(-128, 128);
    }
    else if(std::is_same<T, uint16_t>())
    {
        return state = QRandomGenerator::global()->bounded(0, 65535);
    }
    else if(std::is_same<T, int16_t>())
    {
        return state = QRandomGenerator::global()->bounded(-32768, 32768);
    }
    else if(std::is_same<T, uint32_t>())
    {
        return state = QRandomGenerator::global()->bounded(0, 4294967295);
    }
    else if(std::is_same<T, int32_t>())
    {
        return state = QRandomGenerator::global()->bounded(-2147483647, 2147483647);
    }
    else if(std::is_same<T, uint64_t>())
    {
        return state = QRandomGenerator::global()->bounded(0, 4294967295);
    }
    else if(std::is_same<T, int64_t>())
    {
        return state = QRandomGenerator::global()->bounded(-2147483647, 2147483647);
    }

    return state = 0;
}

/** @enum Типы кодограммы */
enum typeCDGBarier2Exch
{
    setModeRLU = 1,
    stateRLU,
    RegistrationDATA,
    dataTarget1 ,
};

/** @enum Типы кодограммы */
enum typeCDGVideoExch
{
    setIP = 24,
    powerOFF,
    rotateCamera,
    stateCamera ,
};

/** @class Класс заголовка сообщения */
class HEADER
{
public:
    /** @brief Количество байт данных  */
    uint16_t size           = 0;
    /** @brief Номер типа кодограммы  */
    uint8_t  type           = 0;
    /** @brief Номер отправителя (для устройства БР31 - 1)  */
    uint8_t  numSender      = 0;
    /** @brief Номер получателя (255 - широковещательная посылка)  */
    uint8_t  numRequester   = 0;
    /** @brief Время формирования кодограммы (в мс от начала эпохи)  */
    uint64_t time           = 0;
    /** @brief Резерв  */
    uint8_t  reserv         = 0;

    /** @brief Генерация рандомно заполненого класса  */
    static HEADER randomGen()
    {
        HEADER data;

        randomValue(data.size);
        randomValue(data.type);
        randomValue(data.numSender);
        randomValue(data.numRequester);
        randomValue(data.time);
        randomValue(data.reserv);

        return data;
    }

    bool operator == (const HEADER& data) const
    {
        bool error = true;

        if(data.size         != this->size        ) error = false;
        if(data.type         != this->type        ) error = false;
        if(data.numSender    != this->numSender   ) error = false;
        if(data.numRequester != this->numRequester) error = false;
        if(data.time         != this->time        ) error = false;
        if(data.reserv       != this->reserv      ) error = false;

        return error;
    }

    /** @brief Сериализация структуры Header */
    friend QDataStream & operator << (QDataStream &in, const HEADER &data)
    {
        return in << data.size << data.type << data.numSender << data.numRequester
                  << data.time << data.reserv;
    }

    friend void operator >> (QDataStream &out, HEADER &data)
    {
        out >> data.size >> data.type >> data.numSender >> data.numRequester
                >> data.time >> data.reserv;
    }
};

#define SIZE_HEADER sizeof(HEADER)

/** @class Команда установления настроек СВОИ и РЛС */
class CmdSetMode
{
public:

    HEADER head;

    uint8_t numLiteraRLU;     //номер литеры (1, 2, 3, 4)
    uint8_t modulRLU;         //Режим модуляции (1 - ЛЧМ, 0 - Непрерывная генерация)
    uint8_t recRLU;           //Режим приемника (Режим приемника 0 - 60 КГц, 1-30 КГц)
    uint8_t signalRLU;        //Контрольный сигнал (0 - выключен, 1 - КС в канале 1, 2 - КС в канале 2, 3 - КС в канале 3, 4 - КС в трех каналах приемника)
    uint8_t stateSVCHRLU;     //Состояние СВЧ (1 - включено, 0 - выключено)
    uint8_t modeSPOIRLU;       //Режим работы СПОИ
    uint8_t modeSVOIRLU;      //Режим работы СВОИ

    /** @brief Генерация рандомно заполненого класса  */
    static CmdSetMode randomGen()
    {
        CmdSetMode data;

        data.head = HEADER::randomGen();
        data.head.type = typeCDGBarier2Exch::setModeRLU;

        randomValue(data.numLiteraRLU);
        randomValue(data.modulRLU    );
        randomValue(data.recRLU      );
        randomValue(data.signalRLU   );
        randomValue(data.stateSVCHRLU);
        randomValue(data.modeSPOIRLU );
        randomValue(data.modeSVOIRLU );

        return data;
    }
    bool operator == (const CmdSetMode &data) const
    {
        bool error = true;

        //if(!(data.head == this->head)) error = false;

        if(!(data.numLiteraRLU == this->numLiteraRLU)) error = false;
        if(!(data.modulRLU     == this->modulRLU    )) error = false;
        if(!(data.recRLU       == this->recRLU      )) error = false;
        if(!(data.signalRLU    == this->signalRLU   )) error = false;
        if(!(data.stateSVCHRLU == this->stateSVCHRLU)) error = false;
        if(!(data.modeSPOIRLU  == this->modeSPOIRLU )) error = false;
        if(!(data.modeSVOIRLU  == this->modeSVOIRLU )) error = false;

        return error;
    }
    /** @brief Сериализация Команда установки режима */
    friend QDataStream & operator << (QDataStream &in, const CmdSetMode &data)
    {
        return in << data.head        << data.numLiteraRLU
                  << data.modulRLU    << data.recRLU
                  << data.signalRLU   << data.stateSVCHRLU
                  << data.modeSPOIRLU << data.modeSVOIRLU;
    }
    friend void operator >> (QDataStream &out, CmdSetMode &data)
    {
        out >> data.head;
        out >> data.numLiteraRLU
                >> data.modulRLU     >> data.recRLU
                >> data.signalRLU    >> data.stateSVCHRLU
                >> data.modeSPOIRLU  >> data.modeSVOIRLU;
    }
};

#define SIZE_CmdSetMode sizeof(CmdSetMode)

/** @class Данные регистрации с РЛС*/
class RegistrData
{
public:
    HEADER head;

    QVector<int16_t> vdata;
    uint16_t number;

    /** @brief Генерация рандомно заполненого класса  */
    static RegistrData randomGen()
    {
        RegistrData data;

        data.head = HEADER::randomGen();
        data.head.type = typeCDGBarier2Exch::RegistrationDATA;


        for(auto it = 0; it < 256; it++)
        {
            int16_t rdata;
            randomValue(rdata);
            data.vdata.insert(it, rdata);
        }

        randomValue(data.number);

        return data;
    }
    bool operator == (const RegistrData &data) const
    {
        bool error = true;

        //if(!(data.head == this->head)) error = false;

        if(data.vdata.size() != this->vdata.size() ) error = false;
        else
        {
            for(int i = 0; i<256; i++)
            {
                if(data.vdata[i] != this->vdata[i]) error = false;
            }
        }
        if(data.number != this->number) error = false;


        return error;
    }
    /** @brief Сериализация Команда установки режима */
    friend QDataStream & operator << (QDataStream &in, const RegistrData &data)
    {
        in << data.head;

        for(auto it:data.vdata)
            in << it;

        in << data.number;

        return in;
    }
    friend void operator >> (QDataStream &out, RegistrData &data)
    {
        out >> data.head;

        for(int i = 0; i<256; i++)
        {
            int16_t dat;
            out >> dat;
            data.vdata.insert(i, dat);
        }

        out >> data.number;
    }
};

#define SIZE_RegistrData sizeof(RegistrData)

/** @class Состояние РЛУ */
class StateRLU
{
public:

    HEADER head;

    uint8_t  numLiteraRLU; // номер литеры (1, 2, 3, 4)
    uint8_t  modulRLU;     // Режим модуляции (1 - ЛЧМ, 0 - Непрерывная генерация)
    uint8_t  recRLU;       // Режим приемника (Режим приемника 0 - 60 КГц, 1-30 КГц)
    uint8_t  signalRLU;    // Контрольный сигнал (0 - выключен, 1 - КС в канале 1, 2 - КС в канале 2, 3 - КС в канале 3, 4 - КС в трех каналах приемника)
    uint8_t  stateSVCHRLU; // Состояние СВЧ (1 - включено, 0 - выключено)
    uint8_t  modeSPOIRLU;  // Режим работы СПОИ
    uint8_t  modeSVOIRLU;  // Режим работы СВОИ
    uint32_t timeWork;     // Время наработки

    /** @brief Генерация рандомно заполненого класса  */
    static StateRLU randomGen()
    {
        StateRLU data;

        data.head = HEADER::randomGen();
        data.head.type = typeCDGBarier2Exch::stateRLU;

        randomValue(data.numLiteraRLU);
        randomValue(data.modulRLU    );
        randomValue(data.recRLU      );
        randomValue(data.signalRLU   );
        randomValue(data.stateSVCHRLU);
        randomValue(data.modeSPOIRLU );
        randomValue(data.modeSVOIRLU );
        randomValue(data.timeWork    );

        return data;
    }
    bool operator == (const StateRLU &data) const
    {
        bool error = true;

        //if(!(data.head == this->head)) error = false;

        if(data.numLiteraRLU != this->numLiteraRLU) error = false;
        if(data.modulRLU     != this->modulRLU    ) error = false;
        if(data.recRLU       != this->recRLU      ) error = false;
        if(data.signalRLU    != this->signalRLU   ) error = false;
        if(data.stateSVCHRLU != this->stateSVCHRLU) error = false;
        if(data.modeSPOIRLU  != this->modeSPOIRLU ) error = false;
        if(data.modeSVOIRLU  != this->modeSVOIRLU ) error = false;
        if(data.timeWork     != this->timeWork    ) error = false;

        return error;
    }
    /** @brief Сериализация Команда установки режима */
    friend QDataStream & operator << (QDataStream &in, const StateRLU &data)
    {
        in << data.head;

        in << data.numLiteraRLU << data.modulRLU     << data.recRLU
           << data.signalRLU    << data.stateSVCHRLU << data.modeSPOIRLU
           << data.modeSVOIRLU  << data.timeWork;

        return in;
    }

    friend void operator >> (QDataStream &out, StateRLU &data)
    {
        out >> data.head;

        out >> data.numLiteraRLU >> data.modulRLU     >> data.recRLU
                >> data.signalRLU    >> data.stateSVCHRLU >> data.modeSPOIRLU
                >> data.modeSVOIRLU  >> data.timeWork;
    }
};

#define SIZE_StateRLU sizeof(StateRLU)

/** @class Данные о цели 1 типа(сырые) */
class TargetData1
{
public:
    int16_t numElemDistance;  // номер элемента дальности К
    int16_t numFilter;        //Номер фильтра
    int16_t widthSpectr;      //Ширина спектра цели
    int32_t amplitChanalA1;   //Амплитуда канала А1
    int32_t ReDistanceA1;     //Действительная часть
    int32_t ImDistanceA1;     //Комплексная часть
    int32_t ReDistanceA2;     //Действительная часть
    int32_t ImDistanceA2;     //Комплексная часть
    int32_t ReDistanceE;      //Действительная часть
    int32_t ImDistanceE;      //Комплексная часть

    /** @brief Генерация рандомно заполненого класса  */
    static TargetData1 randomGen()
    {
        TargetData1 data;

        randomValue(data.numElemDistance);
        randomValue(data.numFilter      );
        randomValue(data.widthSpectr    );
        randomValue(data.amplitChanalA1 );
        randomValue(data.ReDistanceA1   );
        randomValue(data.ImDistanceA1   );
        randomValue(data.ReDistanceA2   );
        randomValue(data.ImDistanceA2   );
        randomValue(data.ReDistanceE    );
        randomValue(data.ImDistanceE    );

        return data;
    }
    bool operator == (const TargetData1 &data) const
    {
        bool error = true;

        if(data.numElemDistance != this->numElemDistance) error = false;
        if(data.numFilter       != this->numFilter      ) error = false;
        if(data.widthSpectr     != this->widthSpectr    ) error = false;
        if(data.amplitChanalA1  != this->amplitChanalA1 ) error = false;
        if(data.ReDistanceA1    != this->ReDistanceA1   ) error = false;
        if(data.ImDistanceA1    != this->ImDistanceA1   ) error = false;
        if(data.ReDistanceA2    != this->ReDistanceA2   ) error = false;
        if(data.ImDistanceA2    != this->ImDistanceA2   ) error = false;
        if(data.ReDistanceE     != this->ReDistanceE    ) error = false;
        if(data.ImDistanceE     != this->ImDistanceE    ) error = false;

        return error;
    }
    /** @brief Сериализация Команда установки режима */
    friend QDataStream & operator << (QDataStream &in, const TargetData1 &data)
    {
        in  << data.numElemDistance
            << data.numFilter
            << data.widthSpectr
            << data.amplitChanalA1
            << data.ReDistanceA1
            << data.ImDistanceA1
            << data.ReDistanceA2
            << data.ImDistanceA2
            << data.ReDistanceE
            << data.ImDistanceE;

        return in;
    }
    friend void operator >> (QDataStream &out, TargetData1 &data)
    {
        out  >> data.numElemDistance
                >> data.numFilter
                >> data.widthSpectr
                >> data.amplitChanalA1
                >> data.ReDistanceA1
                >> data.ImDistanceA1
                >> data.ReDistanceA2
                >> data.ImDistanceA2
                >> data.ReDistanceE
                >> data.ImDistanceE;
    }
};

#define SIZE_TargetData1 sizeof(TargetData1)

/** @class Данные о целях с РЛУ */
class RLUData1
{
public:

    HEADER head;

    int16_t numSends;           // номер посылки
    int16_t numRecInNullFilter; // Число обнаружений в нулевом фильтре
    int16_t numTargets;         // Число целей

    /** @brief Вектор целей*/
    QVector<TargetData1> vecTarget;

    /** @brief Генерация рандомно заполненого класса  */
    static RLUData1 randomGen()
    {
        RLUData1 data;

        data.head   = HEADER::randomGen();
        data.head.type = typeCDGBarier2Exch::dataTarget1;

        randomValue(data.numSends);
        randomValue(data.numRecInNullFilter);
        data.numTargets = QRandomGenerator::global()->bounded(0, 120);

        for(auto it = 0; it < data.numTargets; it++)
        {
            data.vecTarget.append(TargetData1::randomGen());
        }

        return data;
    }

    bool operator == (const RLUData1 & data) const
    {
        bool error = true;

        //if(!(data.head             == this->head  ))               error = false;

        if(!(data.numSends           == this->numSends))             error = false;
        if(!(data.numRecInNullFilter == this->numRecInNullFilter))   error = false;
        if(!(data.numTargets         == this->numTargets))           error = false;
        if(!(data.vecTarget.size()   == this->vecTarget.size()))     error = false;
        if(!(data.vecTarget          == this->vecTarget))            error = false;
        else
        {
            for(int i = 0; i<data.numTargets; i++)
            {
                if(!(data.vecTarget[i] == this->vecTarget[i])) error = false;
            }
        }

        return error;
    }

    /** @brief Сериализация структуры Header */
    friend QDataStream & operator << (QDataStream &in, const RLUData1 &data)
    {
        in << data.head
           << data.numSends
           << data.numRecInNullFilter
           << data.numTargets;

        for(auto it:data.vecTarget)
        {
            in << it;
        }
        return in;
    }

    friend void operator >> (QDataStream &out, RLUData1 &data)
    {
        out >> data.head;
        out >> data.numSends;
        out >> data.numRecInNullFilter;
        out >> data.numTargets;

        for(int i = 0; i<data.numTargets; i++)
        {
            TargetData1 target;
            out >> target;
            data.vecTarget.append(target);
        }
    }
};

#define SIZE_RLUData1 sizeof(RLUData1)

/** @class Команда установки параметров модуля видео */
class CmdSetState
{
public:

    HEADER head;

    /** @brief IP адрес камеры к которой надо подключиться */
    uint8_t ip1;
    uint8_t ip2;
    uint8_t ip3;
    uint8_t ip4;

    /** @brief Вкл/Выкл модуля камеры */
    int8_t stateModCamera;

    /** @brief Генерация рандомно заполненого класса  */
    static CmdSetState randomGen()
    {
        CmdSetState data;

        data.head = HEADER::randomGen();
        data.head.type = typeCDGVideoExch::setIP;

        randomValue(data.ip1);
        randomValue(data.ip2);
        randomValue(data.ip3);
        randomValue(data.ip4);

        randomValue(data.stateModCamera);

        return data;
    }

    bool operator == (const CmdSetState &data) const
    {
        bool error = true;

        //if(!(data.head == this->head)) error = false;

        if(!(data.ip1 == this->ip1)) error = false;
        if(!(data.ip2 == this->ip2)) error = false;
        if(!(data.ip3 == this->ip3)) error = false;
        if(!(data.ip4 == this->ip4)) error = false;

        // if(!(data.stateModCamera == this->stateModCamera)) error = false;

        return error;
    }
    /** @brief Сериализация Команда установки режима */
    friend QDataStream & operator << (QDataStream &in, const CmdSetState &data)
    {
        return in << data.head
                  << data.ip1
                  << data.ip2
                  << data.ip3
                  << data.ip4
                  << data.stateModCamera;
    }
    friend void operator >> (QDataStream &out, CmdSetState &data)
    {
        out >> data.head;
        out >> data.ip1
                >> data.ip2
                >> data.ip3
                >> data.ip4
                >> data.stateModCamera;
    }
};

#define SIZE_CmdSetState sizeof(CmdSetState)


/** @class Выключение модуля Видео */
class CmdPowerOFF
{
public:

    HEADER head;

    /** @brief Отключаем питание */
    uint8_t powerOFF;

    /** @brief Генерация рандомно заполненого класса  */
    static CmdPowerOFF randomGen()
    {
        CmdPowerOFF data;

        data.head = HEADER::randomGen();
        data.head.type = typeCDGVideoExch::powerOFF;

        randomValue(data.powerOFF);

        return data;
    }
    bool operator == (const CmdPowerOFF &data) const
    {
        bool error = true;

        //if(!(data.head == this->head)) error = false;

        if(!(data.powerOFF == this->powerOFF)) error = false;

        return error;
    }
    /** @brief Сериализация Команда установки режима */
    friend QDataStream & operator << (QDataStream &in, const CmdPowerOFF &data)
    {
        return in << data.head
                  << data.powerOFF;

    }
    friend void operator >> (QDataStream &out, CmdPowerOFF &data)
    {
        out >> data.head;
        out >> data.powerOFF;
    }
};

#define SIZE_CmdPowerOFF sizeof(CmdPowerOFF)

/** @class Повернуть камеру по сферическим координатам */
class CmdRotateCamera
{
public:

    HEADER head;

    /** @brief Сферические координаты от 0 до 360 */
    double x;
    double y;

    /** @brief Генерация рандомно заполненого класса  */
    static CmdRotateCamera randomGen()
    {
        CmdRotateCamera data;

        data.head = HEADER::randomGen();
        data.head.type = typeCDGVideoExch::rotateCamera;

        randomValue(data.x);
        randomValue(data.y);

        return data;
    }
    bool operator == (const CmdRotateCamera &data) const
    {
        bool error = true;

        //if(!(data.head == this->head)) error = false;

        if(!(data.x == this->x)) error = false;
        if(!(data.y == this->y)) error = false;

        return error;
    }
    /** @brief Сериализация Команда установки режима */
    friend QDataStream & operator << (QDataStream &in, const CmdRotateCamera &data)
    {
        return in << data.head
                  << data.x
                  << data.y;

    }
    friend void operator >> (QDataStream &out, CmdRotateCamera &data)
    {
        out  >> data.head;
        out  >> data.x;
        out  >> data.y;
    }
};

#define SIZE_CmdRotateCamera sizeof(CmdRotateCamera)

/** @class Текущее состояние модуля камеры */
class CmdState
{
public:

    HEADER head;

    /** @brief Текущий IP адрес камеры*/
    uint8_t ip1;
    uint8_t ip2;
    uint8_t ip3;
    uint8_t ip4;

    /** @brief Отключаем питание */
    uint32_t winID;

    /** @brief Генерация рандомно заполненого класса  */
    static CmdState randomGen()
    {
        CmdState data;

        data.head = HEADER::randomGen();
        data.head.type = typeCDGVideoExch::stateCamera;

        randomValue(data.ip1);
        randomValue(data.ip2);
        randomValue(data.ip3);
        randomValue(data.ip4);

        randomValue(data.winID);

        return data;
    }

    bool operator == (const CmdState &data) const
    {
        bool error = true;

        //if(!(data.head == this->head)) error = false;

        if(!(data.ip1 == this->ip1)) error = false;
        if(!(data.ip2 == this->ip2)) error = false;
        if(!(data.ip3 == this->ip3)) error = false;
        if(!(data.ip4 == this->ip4)) error = false;

        if(!(data.winID == this->winID)) error = false;

        return error;
    }

    /** @brief Сериализация Команда установки режима */
    friend QDataStream & operator << (QDataStream &in, const CmdState &data)
    {
        in << data.head
           << data.ip1
           << data.ip2
           << data.ip3
           << data.ip4
           << data.winID;

        return in;
    }

    friend void operator >> (QDataStream &out, CmdState &data)
    {
        out >> data.head;
        out >> data.ip1;
        out >> data.ip2;
        out >> data.ip3;
        out >> data.ip4;
        out >> data.winID;
    }
};

#define SIZE_CmdState sizeof(CmdState)

/** @brief Сериализация данных */
template<typename T>
QByteArray serialize(const T& data)
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream << data;
    return array;
}

/** @brief Десериализация данных */
template<typename T>
void deserialize(QByteArray& array, T& data)
{
    QDataStream stream(&array, QIODevice::ReadOnly);
    stream >> data;
}

#pragma pack(pop)

#endif // RLSEXCHANGEGLOBAL_H
