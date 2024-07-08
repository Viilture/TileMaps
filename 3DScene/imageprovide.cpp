#include "imageprovide.h"
#include <QTextureImage>
#include <QPaintedTextureImage>
#include <QSGTextureProvider>

ImageProvide::ImageProvide(): QQuickImageProvider(QQuickImageProvider::Texture,
                                                  QQmlImageProviderBase::ForceAsynchronousImageLoading)
{

}

ImageProvide::~ImageProvide()
{

}

QPixmap ImageProvide::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() <<"Запросили QPixmap"<< id << size << requestedSize;
    int width = 1000;
    int height = 2000;

    if (size)
        *size = QSize(width, height);
    QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                   requestedSize.height() > 0 ? requestedSize.height() : height);
    pixmap.fill(QColor(QRandomGenerator::global()->bounded(256),
                       QRandomGenerator::global()->bounded(256),
                       QRandomGenerator::global()->bounded(256)));
    return pixmap;
}

QImage ImageProvide::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() <<"Запрос IMage"<< id << size << requestedSize;
    int width = 1000;
    int height = 2000;

    if (size)
        *size = QSize(width, height);
    QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                   requestedSize.height() > 0 ? requestedSize.height() : height);
    pixmap.fill(QColor(QRandomGenerator::global()->bounded(256),
                       QRandomGenerator::global()->bounded(256),
                       QRandomGenerator::global()->bounded(256)));
    QImage image;
    image = QImage("qrc:/3DScene/image.png");

    return image;
}

QQuickTextureFactory *ImageProvide::requestTexture(const QString &id, QSize *size, const QSize &requestedSize)
{qDebug() <<"Запрос Texture"<< id << size->width() << requestedSize;

    int width = 1000;
    int height = 2000;

    if (size)
        *size = QSize(width, height);
    QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                   requestedSize.height() > 0 ? requestedSize.height() : height);
    pixmap.fill(QColor(QRandomGenerator::global()->bounded(256),
                       QRandomGenerator::global()->bounded(256),
                       QRandomGenerator::global()->bounded(256)));

   // Qt3DRender::QPaintedTextureImage img;

    QImage image;
     image = QImage(pixmap.toImage());

   static  QQuickTextureFactory* gl = QQuickTextureFactory::textureFactoryForImage(image);


    return gl;
}
