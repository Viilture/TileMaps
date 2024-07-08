#ifndef IMAGEPROVIDE_H
#define IMAGEPROVIDE_H

#include <QQuickImageProvider>
#include <QDebug>
#include <QRandomGenerator>

class ImageProvide : public QQuickImageProvider
{

public:
    explicit ImageProvide();
    virtual ~ImageProvide();

signals:


    // QQuickImageProvider interface
public:
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    // QQuickImageProvider interface
public:
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    QQuickTextureFactory *requestTexture(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // IMAGEPROVIDE_H
