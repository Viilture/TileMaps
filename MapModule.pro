QT += core gui network sql testlib quickwidgets 3dcore 3drender 3dinput 3dquick qml quick 3dquickextras 3dextras
QT += multimediawidgets opengl


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEPENDPATH  += . mapControl-tile Common videoControl Graph Graph\Measure RLI/ CVTools/ CVTools/Tracking/ Exchange/
INCLUDEPATH += . mapControl-tile Common videoControl Graph Graph\Measure RLI/ CVTools/ CVTools/Tracking/ Exchange/

#VLC QT------------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release)
{
LIBS       += -L$$PWD/VLC-Qt/bin/ -lVLCQtCored -lVLCQtWidgetsd
}
CONFIG(release, debug|release)
{
LIBS       += -L$$PWD/VLC-Qt/bin/ -lVLCQtCore -lVLCQtWidgets
}
INCLUDEPATH += VLC-Qt/include
#-------------------------------------------------------------------------------------------------------------

#OPENCV-------------------------------------------------------------------------------------------------------
INCLUDEPATH += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\include"

LIBS += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\x64\mingw\bin\libopencv_core470.dll"
LIBS += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\x64\mingw\bin\libopencv_imgcodecs470.dll"
LIBS += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\x64\mingw\bin\libopencv_highgui470.dll"
LIBS += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\x64\mingw\bin\libopencv_tracking470.dll"
LIBS += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\x64\mingw\bin\libopencv_videoio470.dll"
LIBS += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\x64\mingw\bin\libopencv_video470.dll"
LIBS += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\x64\mingw\bin\libopencv_imgproc470.dll"
LIBS += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\x64\mingw\bin\libopencv_photo470.dll"
LIBS += "C:\opencv\opencv470\opencvBuildMinGW32730Extra\install\x64\mingw\bin\libopencv_dnn470.dll"
#------------------------------------------------------------------------------------------------------------

#GeographicLib-----------------------------------------------------------------------------------------------
INCLUDEPATH += "E:\MYFILE\Program\MyProjects\MapLib\BarierARM\Common"

LIBS += "E:\MYFILE\Program\MyProjects\MapLib\MapModule\Common\GeographicLib\libGeographicLib.dll"
#------------------------------------------------------------------------------------------------------------

SOURCES += \
    Common/globalpalette.cpp \
    Common/scalc.cpp \
    Common/settingsBR.cpp \
    Common/transform.cpp \
    Common/videologging.cpp \
    Exchange/Barier2Exchange.cpp \
    Exchange/BiusTransport/cdgtransport.cpp \
    Exchange/BiusTransport/tcptransport.cpp \
    Exchange/videoexchange.cpp \
    Graph/buttonsettings.cpp \
    Graph/journal.cpp \
    Graph/settingswid.cpp \
    Test/testBarier2Exchange.cpp \
    Test/testsvoi.cpp \
    Test/testvideoexchange.cpp \
    main.cpp \
    mainwindow.cpp \
    managersignals.cpp \
    mapcontrol-tile/MapTileSource.cpp \
    mapcontrol-tile/Position.cpp \
    mapcontrol-tile/guts/Conversions.cpp \
    mapcontrol-tile/guts/MapGraphicsNetwork.cpp \
    mapcontrol-tile/guts/MapTileGraphicsObject.cpp \
    mapcontrol-tile/guts/PrivateQGraphicsInfoSource.cpp \
    mapcontrol-tile/mapcamera.cpp \
    mapcontrol-tile/mapcontrol.cpp \
    mapcontrol-tile/mapline.cpp \
    mapcontrol-tile/mapmeasure.cpp \
    mapcontrol-tile/mapradar.cpp \
    mapcontrol-tile/maptarget.cpp \
    mapcontrol-tile/targetcharacteristics.cpp \
    mapcontrol-tile/targetcontrol.cpp \
    mapcontrol-tile/tileSources/CompositeTileSource.cpp \
    mapcontrol-tile/tileSources/GridTileSource.cpp \
    mapcontrol-tile/tileSources/OSMTileSource.cpp \
    scenecontrol.cpp \
    testwidget.cpp \
    videoControl/abstractcamera.cpp \
    videoControl/cameracontrol.cpp \
    videoControl/pelcocamera.cpp \
    videoControl/vidcameradelegate.cpp \
    videoControl/video/videowidget.cpp \
    videoControl/videocontrol.cpp \

HEADERS += \
    Common/Transform.h \
    Common/globalBarier2.h \
    Common/globalpalette.h \
    Common/scalc.h \
    Common/settingsBR.h \
    Common/videologging.h \
    Exchange/Barier2Exchange.h \
    Exchange/BiusTransport/cdgtransport.h \
    Exchange/BiusTransport/tcptransport.h \
    Exchange/ExchangeGlobal.h \
    Exchange/videoexchange.h \
    Graph/buttonsettings.h \
    Graph/journal.h \
    Graph/settingswid.h \
    Test/testBarier2Exchange.h \
    Test/testsvoi.h \
    Test/testvideoexchange.h \
    mainwindow.h \
    managersignals.h \
    mapcontrol-tile/MapGraphics_global.h \
    mapcontrol-tile/MapTileSource.h \
    mapcontrol-tile/Position.h \
    mapcontrol-tile/guts/Conversions.h \
    mapcontrol-tile/guts/MapGraphicsNetwork.h \
    mapcontrol-tile/guts/MapTileGraphicsObject.h \
    mapcontrol-tile/guts/PrivateQGraphicsInfoSource.h \
    mapcontrol-tile/mapcamera.h \
    mapcontrol-tile/mapcontrol.h \
    mapcontrol-tile/mapline.h \
    mapcontrol-tile/mapmeasure.h \
    mapcontrol-tile/mapradar.h \
    mapcontrol-tile/maptarget.h \
    mapcontrol-tile/targetcharacteristics.h \
    mapcontrol-tile/targetcontrol.h \
    mapcontrol-tile/tileSources/CompositeTileSource.h \
    mapcontrol-tile/tileSources/GridTileSource.h \
    mapcontrol-tile/tileSources/OSMTileSource.h \
    scenecontrol.h \
    testwidget.h \
    videoControl/abstractcamera.h \
    videoControl/cameracontrol.h \
    videoControl/pelcocamera.h \
    videoControl/vidcameradelegate.h \
    videoControl/video/videowidget.h \
    videoControl/videocontrol.h \

FORMS += \
    Graph/journal.ui \
    Graph/settings.ui \
    mainwindow.ui \
    mapcontrol-tile/targetcharacteristics.ui \
    testwidget.ui \
    videoControl/video/videowidget.ui \
    videoControl/videocontrol.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    BarierResources.qrc
