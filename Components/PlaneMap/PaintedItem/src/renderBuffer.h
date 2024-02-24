
#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include <QObject>
#include <QPainter>
#include "QDebug"

class RenderBuffer{
  public:
    const qreal incSize = 1.3;
    const QColor fillColor = Qt::white;
    QPixmap * buffer;
    QSize screenSize;
    QRect viewPort;
    qreal zoom;
    
    RenderBuffer():buffer(nullptr){}
    
    void setSize(QSize screenSize)
    {       
        if(buffer!=nullptr)
            delete buffer;
        
        buffer = new QPixmap(screenSize*incSize);
        this->screenSize = screenSize;
    }
    
    bool isInBuffer(QRect &rect){
        return viewPort.contains(rect);
    }
     
    QSharedPointer<QPainter> makePainter(QRect scrViewPort)
    {
        buffer->fill(fillColor);
        
        // calc buffer ViewPort
        auto vp = scrViewPort;
        auto center = scrViewPort.center();
        vp.setWidth(scrViewPort.width()*incSize);
        vp.setHeight(scrViewPort.height()*incSize);
        vp.moveCenter(center);
        
        this->viewPort = vp;
        
//        qDebug() << "painter/ bufport:" << viewPort << scrViewPort;
       
        auto p = new QPainter(buffer);
        return QSharedPointer<QPainter>(p);
    }
    
    void draw(QPainter *p, QRect &rect)
    {
        auto topLeftBuff = (rect.topLeft()-viewPort.topLeft())*zoom;
        auto bufrect = QRect(topLeftBuff, screenSize);

//        qDebug() << "draw buf:" << bufrect;
        p->drawPixmap(QPoint(0,0),*buffer, bufrect);         
    }
};

#endif // RENDERBUFFER_H






















