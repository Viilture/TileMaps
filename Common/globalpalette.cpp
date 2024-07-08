#include "globalpalette.h"

#define Barier2INI "/Barier2INI"

globalPalette::globalPalette(QObject *parent) : QObject(parent)
{
}

globalPalette::~globalPalette()
{

}

const QPalette &globalPalette::getStandartDarkPalette() const
{
    static QPalette palette(createStandartDarkPalette());
    return palette;
}

const QPalette &globalPalette::getDarkPalette() const
{
    static QPalette palette(createDarkPalette());
    return palette;
}

const QPalette &globalPalette::getDarkPalette2() const
{
    static QPalette palette(createDarkPalette2());
    return palette;
}

const QRadialGradient &globalPalette::getSectorRadarGradient() const
{
    static QRadialGradient palette(createSectRadarGradient());
    return palette;
}

const QPalette &globalPalette::getGreenPalette() const
{
    static QPalette palette(createGreenPalette());
    return palette;
}

const QPalette &globalPalette::createStandartDarkPalette() const
{
    static QPalette palette;

    //Считываем настройки Ini Файла, если они есть
    SettingsBR set;
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/startGradColor"), "#808080");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/endGradColor"), "#404040");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/WindowText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/ToolTipText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/Text"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/Button"), "#00505050");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/ButtonText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/BrightText"), "#ffff0000");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/Link"), "#ff2a82da");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/Highlight"), "#ff2a82da");
    //    set.setValue(QStringLiteral("Graph/StandartDarkPalette/HighlightedText"),"#ff000000");

    QString clrDrk1             = set.value(QStringLiteral("Graph/DarkPalette/startGradColor"), "#808080").toString();
    QString clrDrk2             = set.value(QStringLiteral("Graph/DarkPalette/endGradColor"), "#404040").toString();
    QString clrWindowText       = set.value(QStringLiteral("Graph/DarkPalette/WindowText"), "#ffffffff").toString();
    QString clrToolTipText      = set.value(QStringLiteral("Graph/DarkPalette/ToolTipText"), "#ffffffff").toString();
    QString clrText             = set.value(QStringLiteral("Graph/DarkPalette/Text"), "#ffffffff").toString();
    QString clrButton           = set.value(QStringLiteral("Graph/DarkPalette/Button"), "#00505050").toString();
    QString clrButtonText       = set.value(QStringLiteral("Graph/DarkPalette/ButtonText"), "#ffffffff").toString();
    QString clrBrightText       = set.value(QStringLiteral("Graph/DarkPalette/BrightText"), "#ffff0000").toString();
    QString clrLink             = set.value(QStringLiteral("Graph/DarkPalette/Link"), "#ff2a82da").toString();
    QString clrHighlight        = set.value(QStringLiteral("Graph/DarkPalette/Highlight"), "#ff2a82da").toString();
    QString clrHighlightedText  = set.value(QStringLiteral("Graph/DarkPalette/HighlightedText"),"#ff000000").toString();

    //Темная палетта
    QLinearGradient gradientDark(0,0, 500, 500);
    gradientDark.setCoordinateMode(QGradient::ObjectMode);
    QColor clrDark1 = QColor(clrDrk1);   QColor clrDark2 = QColor(clrDrk2);
    gradientDark.setColorAt(0.0,  clrDark1.darker(190));  gradientDark.setColorAt(0.3,  clrDark2.darker(170));
    gradientDark.setColorAt(0.56, clrDark1.darker(130));  gradientDark.setColorAt(1.0,  clrDark2.darker(150));
    //QBrush brushDark(gradientDark);

    //QColor cl =  QColor(0,244,24,0);
    //cl.setAlpha(0);

    palette.setBrush(QPalette::Window,          gradientDark);
    palette.setColor(QPalette::WindowText,      Qt::white);
    palette.setBrush(QPalette::Base,            gradientDark);
    palette.setBrush(QPalette::AlternateBase,   gradientDark);
    palette.setBrush(QPalette::ToolTipBase,     gradientDark);
    palette.setColor(QPalette::ToolTipText,     Qt::white);
    palette.setColor(QPalette::Text,            Qt::white);
    palette.setColor(QPalette::Button,          QColor(40,40,40));
    palette.setColor(QPalette::ButtonText,      Qt::white);
    palette.setColor(QPalette::BrightText,      Qt::white);
    palette.setColor(QPalette::Link,            QColor(clrLink));
    palette.setColor(QPalette::Highlight,       QColor(clrHighlight));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    // palette.setColor(QPalette::Background,      Qt::red);

    return palette;
}

const QPalette &globalPalette::createDarkPalette() const
{
    static QPalette palette;

    //Считываем настройки Ini Файла, если они есть
    SettingsBR set;
    //    set.setValue(QStringLiteral("Graph/DarkPalette/startGradColor"), "#a0606060");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/endGradColor"), "#f0303030");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/WindowText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/ToolTipText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/Text"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/Button"), "#00505050");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/ButtonText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/BrightText"), "#ffff0000");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/Link"), "#ff2a82da");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/Highlight"), "#ff2a82da");
    //    set.setValue(QStringLiteral("Graph/DarkPalette/HighlightedText"),"#ff000000");

    QString clrDrk1             = set.value(QStringLiteral("Graph/DarkPalette/startGradColor"),  "#a0606060").toString();
    QString clrDrk2             = set.value(QStringLiteral("Graph/DarkPalette/endGradColor"),    "#f0303030").toString();
    QString clrWindowText       = set.value(QStringLiteral("Graph/DarkPalette/WindowText"),      "#ffffffff").toString();
    QString clrToolTipText      = set.value(QStringLiteral("Graph/DarkPalette/ToolTipText"),     "#ffffffff").toString();
    QString clrText             = set.value(QStringLiteral("Graph/DarkPalette/Text"),            "#ffffffff").toString();
    QString clrButton           = set.value(QStringLiteral("Graph/DarkPalette/Button"),          "#ff505050").toString();
    QString clrButtonText       = set.value(QStringLiteral("Graph/DarkPalette/ButtonText"),      "#ffffffff").toString();
    QString clrBrightText       = set.value(QStringLiteral("Graph/DarkPalette/BrightText"),      "#ffff0000").toString();
    QString clrLink             = set.value(QStringLiteral("Graph/DarkPalette/Link"),            "#ff2a82da").toString();
    QString clrHighlight        = set.value(QStringLiteral("Graph/DarkPalette/Highlight"),       "#ff2a82da").toString();
    QString clrHighlightedText  = set.value(QStringLiteral("Graph/DarkPalette/HighlightedText"), "#ff000000").toString();

    //Темная палетта
    QLinearGradient gradientDark(0,0, 500, 500);
    gradientDark.setCoordinateMode(QGradient::ObjectMode);
    QColor clrDark1 = QColor(clrDrk1);   QColor clrDark2 = QColor(clrDrk2);
    gradientDark.setColorAt(0.0,  clrDark1.darker(190));  gradientDark.setColorAt(0.3,  clrDark2.darker(170));
    gradientDark.setColorAt(0.56, clrDark1.darker(130));  gradientDark.setColorAt(1.0,  clrDark2.darker(150));
    QBrush brushDark(gradientDark);



    palette.setBrush(QPalette::All, QPalette::Window,           gradientDark);
    palette.setColor(QPalette::All, QPalette::WindowText,       QColor(clrWindowText));
    palette.setBrush(QPalette::All, QPalette::Base,             gradientDark);
    palette.setBrush(QPalette::All, QPalette::AlternateBase,    gradientDark);
    palette.setBrush(QPalette::All, QPalette::ToolTipBase,      gradientDark);
    palette.setColor(QPalette::All, QPalette::ToolTipText,      QColor(clrToolTipText));
    palette.setColor(QPalette::All, QPalette::Text,             QColor(clrText));
    palette.setBrush(QPalette::All, QPalette::Button,           brushDark);
    palette.setColor(QPalette::All, QPalette::ButtonText,       QColor(clrButtonText));
    palette.setColor(QPalette::All, QPalette::BrightText,       QColor(clrBrightText));
    palette.setColor(QPalette::All, QPalette::Link,             QColor(clrLink));
    palette.setColor(QPalette::All, QPalette::Highlight,        QColor(clrHighlight));
    palette.setColor(QPalette::All, QPalette::HighlightedText,  QColor(clrHighlightedText));
    palette.setColor(QPalette::All, QPalette::PlaceholderText,  Qt::red);


    return palette;
}

const QPalette &globalPalette::createDarkPalette2() const
{
    static QPalette palette;

    //Считываем настройки Ini Файла, если они есть
    SettingsBR set;
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/startGradColor"), "#ff353535");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/endGradColor"), "#ff000000");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/WindowText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/ToolTipText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/Text"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/Button"), "#00505050");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/ButtonText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/BrightText"), "#ffff0000");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/Link"), "#ff2a82da");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/Highlight"), "#ff2a82da");
    //    set.setValue(QStringLiteral("Graph/DarkPalette2/HighlightedText"),"#ff000000");

    QString clrDrk1             = set.value(QStringLiteral("Graph/DarkPalette2/startGradColor"), "#a0454545").toString();
    QString clrDrk2             = set.value(QStringLiteral("Graph/DarkPalette2/endGradColor"), "#f0202020").toString();
    QString clrWindowText       = set.value(QStringLiteral("Graph/DarkPalette2/WindowText"), "#ffffffff").toString();
    QString clrToolTipText      = set.value(QStringLiteral("Graph/DarkPalette2/ToolTipText"), "#ffffffff").toString();
    QString clrText             = set.value(QStringLiteral("Graph/DarkPalette2/Text"), "#ffffffff").toString();
    QString clrButton           = set.value(QStringLiteral("Graph/DarkPalette2/Button"), "#00505050").toString();
    QString clrButtonText       = set.value(QStringLiteral("Graph/DarkPalette2/ButtonText"), "#ffffffff").toString();
    QString clrBrightText       = set.value(QStringLiteral("Graph/DarkPalette2/BrightText"), "#ffff0000").toString();
    QString clrLink             = set.value(QStringLiteral("Graph/DarkPalette2/Link"), "#ff2a82da").toString();
    QString clrHighlight        = set.value(QStringLiteral("Graph/DarkPalette2/Highlight"), "#ff2a82da").toString();
    QString clrHighlightedText  = set.value(QStringLiteral("Graph/DarkPalette2/HighlightedText"),"#ff000000").toString();

    //Темная палетта
    QLinearGradient gradientDark(0,0, 500, 500);
    gradientDark.setCoordinateMode(QGradient::ObjectMode);
    QColor clrDark1 = QColor(clrDrk1);   QColor clrDark2 = QColor(clrDrk2);
    gradientDark.setColorAt(0.0,  clrDark1.darker(190));  gradientDark.setColorAt(0.3,  clrDark2.darker(170));
    gradientDark.setColorAt(0.56, clrDark1.darker(130));  gradientDark.setColorAt(1.0,  clrDark2.darker(150));
    QBrush brushDark(gradientDark);

    palette.setBrush(QPalette::Window,          brushDark);
    palette.setColor(QPalette::WindowText,      QColor(clrWindowText));
    palette.setBrush(QPalette::Base,            brushDark);
    palette.setBrush(QPalette::AlternateBase,   brushDark);
    palette.setBrush(QPalette::ToolTipBase,     brushDark);
    palette.setColor(QPalette::ToolTipText,     QColor(clrToolTipText));
    palette.setColor(QPalette::Text,            QColor(clrText));
    palette.setBrush(QPalette::Button,          brushDark);
    palette.setColor(QPalette::ButtonText,      QColor(clrButtonText));
    palette.setColor(QPalette::BrightText,      QColor(clrBrightText));
    palette.setColor(QPalette::Link,            QColor(clrLink));
    palette.setColor(QPalette::Highlight,       QColor(clrHighlight));
    palette.setColor(QPalette::HighlightedText, QColor(clrHighlightedText));
    //palette.setColor(QPalette::Background,      Qt::red);

    return palette;
}

const QPalette &globalPalette::createGreenPalette() const
{
    static QPalette palette;

    //Считываем настройки Ini Файла, если они есть
    SettingsBR set;
    //    set.setValue(QStringLiteral("Graph/GreenPalette/startGradColor"), "#e000f000");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/endGradColor"), "#f000ff00");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/WindowText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/ToolTipText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/Text"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/Button"), "#00505050");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/ButtonText"), "#ffffffff");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/BrightText"), "#ffff0000");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/Link"), "#ff2a82da");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/Highlight"), "#ff2a82da");
    //    set.setValue(QStringLiteral("Graph/GreenPalette/HighlightedText"),"#ff000000");

    QString clrDrk1             = set.value(QStringLiteral("Graph/GreenPalette/startGradColor"), "#a0454545").toString();
    QString clrDrk2             = set.value(QStringLiteral("Graph/GreenPalette/endGradColor"),   "#f0202020").toString();
    QString clrWindowText       = set.value(QStringLiteral("Graph/GreenPalette/WindowText"),     "#ffffffff").toString();
    QString clrToolTipText      = set.value(QStringLiteral("Graph/GreenPalette/ToolTipText"),     "#ffffffff").toString();
    QString clrText             = set.value(QStringLiteral("Graph/GreenPalette/Text" "#ffffffff")).toString();
    QString clrButton           = set.value(QStringLiteral("Graph/GreenPalette/Button"), "#ff50ff50").toString();
    QString clrButtonText       = set.value(QStringLiteral("Graph/GreenPalette/ButtonText"), "#ffffffff").toString();
    QString clrBrightText       = set.value(QStringLiteral("Graph/GreenPalette/BrightText"), "#ffff0000").toString();
    QString clrLink             = set.value(QStringLiteral("Graph/GreenPalette/Link"), "#ff2a82da").toString();
    QString clrHighlight        = set.value(QStringLiteral("Graph/GreenPalette/Highlight"), "#ff2a82da").toString();
    QString clrHighlightedText  = set.value(QStringLiteral("Graph/GreenPalette/HighlightedText"),"#ff000000").toString();

    //Зеленая палетта
    QLinearGradient gradientGreen(0,0, 500, 500);
    gradientGreen.setCoordinateMode(QGradient::ObjectMode);
    QColor clrDark1 = QColor(clrDrk1);   QColor clrDark2 = QColor(clrDrk2);
    gradientGreen.setColorAt(0.0,  clrDark1.darker(190));  gradientGreen.setColorAt(0.3,  clrDark2.darker(170));
    gradientGreen.setColorAt(0.56, clrDark1.darker(130));  gradientGreen.setColorAt(1.0,  clrDark2.darker(150));
    QBrush brushGreen(gradientGreen);


    palette.setBrush(QPalette::Window,          brushGreen);
    palette.setColor(QPalette::WindowText,      QColor(clrWindowText));
    palette.setBrush(QPalette::Base,            brushGreen);
    palette.setBrush(QPalette::AlternateBase,   brushGreen);
    palette.setBrush(QPalette::ToolTipBase,     brushGreen);
    palette.setColor(QPalette::ToolTipText,     QColor(clrToolTipText));
    palette.setColor(QPalette::Text,            QColor(clrText));
    palette.setBrush(QPalette::Button,          brushGreen);
    palette.setColor(QPalette::ButtonText,      QColor(clrButtonText));
    palette.setColor(QPalette::BrightText,      QColor(clrBrightText));
    palette.setBrush(QPalette::Link,            brushGreen);
    palette.setBrush(QPalette::Highlight,       brushGreen);
    palette.setColor(QPalette::HighlightedText, QColor(clrHighlightedText));



    return palette;
}

const QRadialGradient &globalPalette::createSectRadarGradient() const
{
    static QRadialGradient radGradient;

    //Градиент сектора радара
    radGradient = QRadialGradient(0, 0, 5000);
    QColor clrDRad1 = QColor(255, 50, 50, 150);   QColor clrDRad2 = QColor(255, 0, 0, 160);
    radGradient.setColorAt(0.0,  clrDRad1);  radGradient.setColorAt(0.3,  clrDRad2);
    radGradient.setColorAt(0.56, clrDRad1);  radGradient.setColorAt(1.0,  clrDRad2);

    return radGradient;
}
