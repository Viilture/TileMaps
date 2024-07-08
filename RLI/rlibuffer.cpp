#include "rlibuffer.h"
#include <chrono>

#define BR2SPOI "BR2SPOI"

RLIbuffer::RLIbuffer(QObject *parent) : QObject(parent)
{
    timer2 = QSharedPointer<QTimer>::create(this);

    connect(timer2.data(), &QTimer::timeout, this, [=]()
    {

        saveDataToFile();
    });

    timer2->start(1000);

}

RLIbuffer::~RLIbuffer()
{

}

/** @brief Добавляем данные для сохранения*/
void RLIbuffer::appendDataForSave(QSharedPointer<QByteArray> data)
{
    std::lock_guard<std::mutex> lock(mutexExchange);
    arrayList.append(data);
}

/** @brief Отправить сообщение */
void RLIbuffer::saveDataToFile()
{
    try {

        std::lock_guard<std::mutex> lock(mutexExchange);
        QTime time = QTime::currentTime();

        //Если нет данных для записи выходим
        if(arrayList.empty()) return;

        QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + BR2SPOI + QString::number(time.hour()) + ".txt");

        //Открываем файл для записи
        if(!file.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Unable to open file for writing";
            return;
        }

        QTextStream out(&file);

        //Записываем каждый элемент в файл
        for(const QSharedPointer<QByteArray>& byteArray : qAsConst(arrayList))
        {
            out <<  static_cast<uint8_t>(byteArray->at(1));
            out << " range channel ";

            //Записываем все кроме последнего байта, который является счетчиком
            for(int i = 2; i < byteArray->size(); i += 2)
            {
                if(i < (byteArray->size() - 2))
                {
                    int16_t result = 0;

                    if((i+1) < byteArray->size())
                        result = static_cast<unsigned short>((byteArray->at(i+1) << 8))|static_cast<unsigned char>((byteArray->at(i)));
                    else if(i < byteArray->size())
                        result = byteArray->at(i);
                    out << result << " ";
                }
                else
                {
                    unsigned short result = 0;

                    result = static_cast<unsigned short>((byteArray->at(i+1) << 8))|static_cast<unsigned char>((byteArray->at(i)));
                    out << result;
                }
            }
            out << "\n";
        }

        file.close();

        arrayList.clear();


    }  catch (...) {

    }


    return;
}
