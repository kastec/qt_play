
#include "liveimageprovider.h"


#include <QDebug>
#include <QImageReader>
#include <QImage>
#include <QPainter>
/**
 * @brief Image provider that is used to handle the live image stream in the QML viewer.
 */
LiveImageProvider::LiveImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{
    //    this->no_image = QImage(":/images/no_image.png");
    
    QImageReader reader("C:/Projects/QT_Test/QtTest1/Components/PlaneMap/loadedCrew.jpg");
    QImage img = reader.read(); 
    if(img.isNull())
    {
        qDebug() << reader.errorString();
    }
    
    this->no_image = img;// QImage("file:///C:/Projects/QT_Test/QtTest1/Components/PlaneMap/loadedCrew.jpg");
    
    //    auto w = this->no_image.width();
    this->blockSignals(false);
}



/**
 * @brief Delivers image. The id is not used.
 * @param id The id is the requested image source, with the "image:" scheme and provider identifier removed.
 * For example, if the image source was "image://myprovider/icons/home", the given id would be "icons/home".
 * @param size In all cases, size must be set to the original size of the image. This is used to set the
 * width and height of the relevant Image if these values have not been set explicitly.
 * @param requestedSize The requestedSize corresponds to the Image::sourceSize requested by an Image item.
 * If requestedSize is a valid size, the image returned should be of that size.
 * @return
 */
QImage LiveImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage result = this->image;
    
    if(result.isNull()) {
        result = this->no_image;
    }
    
    if(size) {
        *size = result.size();
    }
    
    qDebug() << "size : "<< size->width() << size->height();
    qDebug() << "requestedSize"<< requestedSize.width() << requestedSize.height();

     
    if(requestedSize.width() > 0 && requestedSize.height() > 0) {
        
        
        //        result = result.copy(QRect (0, 0, requestedSize.width(), requestedSize.height()));
        //        result = result.scaled(requestedSize.width()*3, requestedSize.height(), Qt::KeepAspectRatio);
        qDebug() << "resize";
        
        QPainter painter(&result);
        
        QImage srcImage(100,100, QImage::Format_RGB888);// = QImage(100, 100);
        srcImage.fill(Qt::red);
        QRectF targetRect(50, 20, 150, 140);
        QRectF sourceRect(0.0, 10.0,50, 50);
        painter.drawImage(targetRect, srcImage, sourceRect);
        
        //        QPainter painter(&result);
        //        QRectF targetRect(50, 20, 150, 140);
        //        QRectF sourceRect(100.0, 10.0, requestedSize.width(), 140.0);
        //        painter.drawImage(targetRect, this->no_image, sourceRect);
        
        
    }
    else
        result = result.mirrored(true, true);
    
    return result;
}

QPixmap LiveImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) 
{
    int width = 100;
    int height = 50;
    
    if (size)
        *size = QSize(width, height);
    QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                   requestedSize.height() > 0 ? requestedSize.height() : height);
    pixmap.fill(QColor(id).rgba());
    return pixmap;
}

/**
 * @brief Update of the current image.
 * @param image The new image.
 */
void LiveImageProvider::updateImage(const QImage &image)
{
    if(this->image != image) {
        this->image = image;
        emit imageChanged();
    }
}



