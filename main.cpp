#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <signal.h>
#include <QDir>
#include <QString>
#include <QThread>
#include <QMessageBox>
#include <QStyleFactory>

#include <VLCQtCore/Common.h>

#include <QScreen>
#include <QImage>
#include <QThread>

#include <opencv2/opencv.hpp>
#include "Transform.h"

#ifdef _WIN32
#include <windows.h>
#include <exception>
#include <stdlib.h>
#include <stdio.h>
#include "Test/testBarier2Exchange.h"
#include "Test/testvideoexchange.h"

#endif

MainWindow *w = nullptr;

/**
 * @brief Обработчик системных прерываний(ошибок)
 * @param sig - номер прерывания(ошибки)
 */
void __attribute__((noreturn)) handlerError(int sig)
{
    qSetMessagePattern(QStringLiteral("[%{time yyyy-MM-dd h:mm:ss.zzz t}]: %{message} --->\n\t%{backtrace depth=20 separator=\n\t}"));
    QString str = QStringLiteral("Пришел сигнал %1 ").arg(sig);
    if(SIGSEGV == sig) str.append("SIGSEGV").append(" (Segmentation violation - Нарушение сегментации)");
    if(SIGILL  == sig) str.append("SIGILL" ).append(" (Illegal instruction - Недопустимая инструкция)");
    if(SIGFPE  == sig) str.append("SIGFPE" ).append(" (Floating-point exception - Исключение с плавающей точкой)");
    if(SIGINT  == sig) str.append("SIGINT" ).append(" (Interrupt - Прерывание)");
    if(SIGABRT == sig) str.append("SIGABRT").append(" (Abort)");
    if(SIGTERM == sig) str.append("SIGTERM").append(" (Termination - прекращение)");
#ifndef Q_OS_WIN32
    if(SIGHUP  == sig) str.append("SIGHUP" ).append(" (Hangup - Зависание)");
    if(SIGQUIT == sig) str.append("SIGQUIT").append(" (Quit - Выход)");
    if(SIGTRAP == sig) str.append("SIGTRAP").append(" (Trace trap)");
    if(SIGKILL == sig) str.append("SIGKILL").append(" (Kill, unblockable - Убить, не блокируется)");
#endif
    qCritical() << str;
    QMessageBox::critical(nullptr, QStringLiteral("Неожиданное закрытие приложения"), qFormatLogMessage(QtMsgType::QtSystemMsg, QMessageLogContext(), str));
    qSetMessagePattern(QStringLiteral("%{if-category}%{category}: %{endif}%{message}"));
    qCritical() << "Аварийное завершение работы.";

    //    QCoreApplication::quit();
    //    QProcess::startDetached(QCoreApplication::applicationFilePath());

    //    delete w;
    //    w = nullptr;
    //    exit(10);

    //  while(1){}
    return;
}

#ifdef _WIN32
LONG WINAPI hExceptionFilter(struct _EXCEPTION_POINTERS *excInfo)
{

    qCritical() << "Exception code " << excInfo->ExceptionRecord->ExceptionCode;
    switch (excInfo->ExceptionRecord->ExceptionCode)
    {

    case EXCEPTION_ACCESS_VIOLATION        :{qCritical() << "EXCEPTION_ACCESS_VIOLATION";        break;}
    case EXCEPTION_DATATYPE_MISALIGNMENT   :{qCritical() << "EXCEPTION_DATATYPE_MISALIGNMENT";   break;}
    case EXCEPTION_BREAKPOINT              :{qCritical() << "EXCEPTION_BREAKPOINT";              break;}
    case EXCEPTION_SINGLE_STEP             :{qCritical() << "EXCEPTION_SINGLE_STEP";             break;}
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED   :{qCritical() << "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";   break;}
    case EXCEPTION_FLT_DENORMAL_OPERAND    :{qCritical() << "EXCEPTION_FLT_DENORMAL_OPERAND ";   break;}
    case EXCEPTION_FLT_DIVIDE_BY_ZERO      :{qCritical() << "EXCEPTION_FLT_DIVIDE_BY_ZERO";      break;}
    case EXCEPTION_FLT_INEXACT_RESULT      :{qCritical() << "EXCEPTION_FLT_INEXACT_RESULT";      break;}
    case EXCEPTION_FLT_INVALID_OPERATION   :{qCritical() << "EXCEPTION_FLT_INVALID_OPERATION ";  break;}
    case EXCEPTION_FLT_OVERFLOW            :{qCritical() << "EXCEPTION_FLT_OVERFLOW";            break;}
    case EXCEPTION_FLT_STACK_CHECK         :{qCritical() << "EXCEPTION_FLT_STACK_CHECK";         break;}
    case EXCEPTION_FLT_UNDERFLOW           :{qCritical() << "EXCEPTION_FLT_UNDERFLOW";           break;}
    case EXCEPTION_INT_DIVIDE_BY_ZERO      :{qCritical() << "EXCEPTION_INT_DIVIDE_BY_ZERO";      break;}
    case EXCEPTION_INT_OVERFLOW            :{qCritical() << "EXCEPTION_INT_OVERFLOW";            break;}
    case EXCEPTION_PRIV_INSTRUCTION        :{qCritical() << "EXCEPTION_PRIV_INSTRUCTION";        break;}
    case EXCEPTION_IN_PAGE_ERROR           :{qCritical() << "EXCEPTION_IN_PAGE_ERROR";           break;}
    case EXCEPTION_ILLEGAL_INSTRUCTION     :{qCritical() << "EXCEPTION_ILLEGAL_INSTRUCTION";     break;}
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:{qCritical() << "EXCEPTION_NONCONTINUABLE_EXCEPTION";break;}
    case EXCEPTION_STACK_OVERFLOW          :{qCritical() << "EXCEPTION_STACK_OVERFLOW";          break;}
    case EXCEPTION_INVALID_DISPOSITION     :{qCritical() << "EXCEPTION_INVALID_DISPOSITION";     break;}
    case EXCEPTION_GUARD_PAGE              :{qCritical() << "EXCEPTION_GUARD_PAGE";              break;}
    case EXCEPTION_INVALID_HANDLE          :{qCritical() << "EXCEPTION_INVALID_HANDLE";          break;}

    default:
    {qCritical() << "UNKNOWN EXCEPTION. code: " << excInfo->ExceptionRecord->ExceptionCode;break;}
    }

    qDebug() << "Беда";

    return EXCEPTION_CONTINUE_SEARCH;
}
#endif
int main(int argc, char *argv[])
{

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Utf8"));
    QApplication a(argc, argv);

    qApp->setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    // Устанавливаем обработчик для системных сигналов/прерываний/ошибок

    {
        signal(SIGSEGV, handlerError);
        signal(SIGILL,  handlerError);
        signal(SIGFPE,  handlerError);
        signal(SIGINT,  handlerError);
        signal(SIGABRT, handlerError);
        signal(SIGTERM, handlerError);

#ifndef Q_OS_WIN32
        signal(SIGHUP,  handlerError);
        signal(SIGQUIT, handlerError);
        signal(SIGTRAP, handlerError);
        signal(SIGKILL, handlerError);
#endif
    }

#ifdef _WIN32
    SetUnhandledExceptionFilter(&hExceptionFilter);
#endif

    VlcCommon::setPluginPath(a.applicationDirPath() + "/plugins");

    w = new MainWindow;
    w->show();
    //w->showFullScreen();
    w->setWindowTitle(QStringLiteral("Тайловый модуль карт"));

    qSetRealNumberPrecision(15);

//    QTest::qExec(new testBarier2Exchange());
//    QTest::qExec(new testVideoExchange());

    int res = a.exec();
    delete w;
    w = nullptr;

    return res;

}

