
#ifndef DRAWHELPER_H
#define DRAWHELPER_H

#include <QObject>
#include <QPainter>

#include "qpainterpath.h"
#include "spriteCache.h"


struct SymbolRenderStyle{
  public:
    int fontSize;
    QColor color, backColor;
    QFont::Weight weignt;
    qreal opacity = 1.0;
};

namespace DrawHelper
{
    static QPixmap* getSymbolPixmap(QChar symbol, const SymbolRenderStyle &style)
    {
        QFont font("Inter", style.fontSize, style.weignt);
        
        //    font.setStyleStrategy(QFont::ForceOutline);
        font.setStyleStrategy(QFont::PreferAntialias);
        
        QFontMetrics fm(font);
        
        //    auto rect = fm.boundingRect(symbol);
        
        auto fW = fm.horizontalAdvance(symbol);
        auto fH = fm.height();
        
               //  qDebug() <<"fs: "<< symbol <<": " <<  fW << fH;
        
        //    QPixmap* pixmap = new QPixmap(rect.width()*1.2 ,rect.height()*1.1);
        QPixmap* pixmap = new QPixmap(fW ,fH);
        
        pixmap->fill(style.backColor);
        //    pixmap->fill(Qt::yellow);
        
        QPainter p(pixmap);
        
        p.setRenderHint(QPainter::Antialiasing);
        p.setFont(font);
        
        p.setPen(style.color);
        //    p.setPen(Qt::black);
        
        if(style.opacity<=0.99)
            p.setOpacity(style.opacity);
        
               // QRectF trect (0,0,rect.width(),rect.height() );
        QRectF trect (0,0,fW ,fH );
        p.drawText(trect, Qt::AlignCenter,symbol);
        
        return pixmap;    
    }
    
    static QRect drawSymbols(QPainter *painter, const QPoint &position,
                             QString stringValue, const SymbolRenderStyle &style,
                             QString symbolCacheKey,
                             bool isCenterPoint)
    {
        QList<QPixmap*> pmSymbols;
        
        // наберем все спрайты символов, чтобы сначала могли подсчитать их общий размер
        for(auto &symbol:stringValue){
            auto symbolKey = symbolCacheKey+"-"+symbol; 
            auto pixmap = SpriteCache().get(symbolKey, style.fontSize);
            
            if(pixmap == nullptr){
                pixmap = getSymbolPixmap(symbol, style);
                SpriteCache().push(symbolKey, pixmap, style.fontSize);
            }
            pmSymbols.append(pixmap);
        }
        
        int posX = position.x(), posY=position.y();
        
        if(isCenterPoint){
            int width=0;
            for(auto &pm:pmSymbols) width += pm->width();    
            posX -= width/2;
            posY -= pmSymbols[0]->height()/2;
        }
        
        QPoint topLeft(posX, posY);
        
        for(auto &pm:pmSymbols){
            painter->drawPixmap(posX,posY, *pm);
            posX += pm->width();
        }    
        
        //    for(auto &symbol:stringValue){
        
        //        auto symbolKey = symbol + symbolCacheKey; 
        //        auto pixmap = SpriteCache().get(symbolKey, style.fontSize);
        
        //        if(pixmap == nullptr){
        //            // qDebug() << "r: " << symbolKey << fontSize;                
        //            pixmap = getSymbolPixmap(symbol, style);
        //            SpriteCache().push(symbolKey, pixmap, style.fontSize);
        //        }
        //        painter->drawPixmap(posX,posY, *pixmap);
        //        posX += pixmap->width();
        //    }
        QPoint bottomRight(posX, posY+pmSymbols[0]->height());
        return QRect(topLeft, bottomRight);
    }
    
    
    
    
    static void drawRect(QPainter &p, QRectF &r, int width, int height, const QColor &fillColor, QColor &borderColor, int radius)
    {    
        QPainterPath path;
        
        QRect drawRect( r.x()*width,  r.y()*height, r.width() * width, r.height() * height);
        
        auto radius1 = (radius*width)/100;
        
        path.addRoundedRect(drawRect, radius1, radius1);
        
        QPen border(borderColor, width*0.025);
        p.setPen(border);
        
        p.fillPath(path, fillColor);
        p.drawPath(path);   
    }
    
    
    static void drawRect(QPainter &p, QRectF &rect, int width, int height, const QColor &fillColor)
    {    
        //    QPainterPath path;
        
        auto pr = rect;
        QRect drawRect( pr.x()*width,  pr.y()*height, pr.width() * width, pr.height() * height);
        
        p.fillRect(drawRect, fillColor);
        //    auto radius1 = (radius*width)/100;
        //    path.addRoundedRect(drawRect1, radius1, radius1);
        //    QPen border(borderColor, width*0.025);
        //    p.setPen(border);
        //    p.fillPath(path, fillColor);
        
    }
};

#endif // DRAWHELPER_H
