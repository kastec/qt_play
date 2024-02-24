
#include <QPainterPath>
#include "planeItemBase.h"

#include "spriteCache.h"
#include "qdebug.h"

void PlaneItemBase::draw(QPainter *p, QRect &rect){}

qreal PlaneItemBase::getOpacity(qreal value, qreal min, qreal max){
    if(value<min) return 0.0;
    if(value>max) return 1.0;
    return (value-min)/(max-min);
}

qreal PlaneItemBase::zoom(const QRect &rect)
{
    return ( rect.width()) / (qreal)this->location.width();
}
int PlaneItemBase::zoom(int value, const QRect &rect)
{
    return (value * rect.width()) / this->location.width();
}

QList<QRectF> PlaneItemBase::rescale(QList<QRect> &rects, qreal width, qreal height)
{
    QList<QRectF> scaledSizes;
    for(auto &s: rects)
        scaledSizes.append(QRectF(s.x()/width, s.y()/height, s.width()/width, s.height()/height));
    return scaledSizes;
}

QList<QLineF> PlaneItemBase::rescale(QList<QLine> &lines, qreal width, qreal height)
{
    QList<QLineF> scaledLines;
    for(auto &l: lines)
        scaledLines.append(QLineF(l.x1()/width, l.y1()/height, l.x2()/width, l.y2()/height));
    
    return scaledLines;    
}

QPixmap* PlaneItemBase::getSymbolPixmap(QChar symbol, const SymbolRenderStyle &style)
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

QRect PlaneItemBase::drawSymbols(QPainter *painter, const QPoint &position,
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



void PlaneItemBase::drawRect(QPainter &p, QRectF &rect, int width, int height, const QColor &fillColor, QColor &borderColor, int radius)
{    
    QPainterPath path;
    
    auto pr = rect;
    QRect drawRect( pr.x()*width,  pr.y()*height, pr.width() * width, pr.height() * height);
    
    auto radius1 = (radius*width)/100;
    
    path.addRoundedRect(drawRect, radius1, radius1);
    
    QPen border(borderColor, width*0.025);
    p.setPen(border);
    
    p.fillPath(path, fillColor);
    p.drawPath(path);   
}


void PlaneItemBase::drawRect(QPainter &p, QRectF &rect, int width, int height, const QColor &fillColor)
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


void PlaneItemWc::draw(QPainter *p, QRect &rect){
    auto width  = rect.width();
    auto zoomPers = (width*100.0)/this->location.width();
    

    auto wcOpacity = getOpacity(zoomPers, 40,50);
    if(wcOpacity==0.0) return;
    
    p->save();
    
    p->setOpacity(wcOpacity);
    
    auto textColor = Qt::black;
    auto backColor = Qt::yellow;   

    QPen borderColor(textColor, width*0.015);
    p->setPen(borderColor);
    
    auto radius = (8*width)/100;
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    
    path.addRoundedRect(rect, radius, radius);
    
    p->fillPath(path, backColor);
    p->drawPath(path);
    
    auto titleOpacity = getOpacity(zoomPers, 70,110);
    if(titleOpacity>0.0){        
        auto fontSize=(20 * width)/100;
        auto symbolCacheKey = this->type; // тип - буква для WC

        SymbolRenderStyle style{fontSize, textColor, backColor,  QFont::Normal, titleOpacity};
         
        drawSymbols(p, rect.center(), this->title, style, symbolCacheKey, true);  
    }
    
    p->restore();
}

void PlaneItemText::draw(QPainter *painter, QRect &rect)
{        
    auto fontSize = this->zoom(34, rect);
    
    QColor fontColor= QColor::fromRgb(128,128,128);
    SymbolRenderStyle style{fontSize, fontColor, QColorConstants::White, QFont::Light, 1.0};
    auto symbolCacheKey = "rownum";
    
    auto left = rect.left();
    // TODO : это для того, чтобы номер всегда был на экране
    left = left<0? 0: left;    
    QPoint pos(left, rect.top());
    
    drawSymbols(painter, pos, this->title, style, symbolCacheKey, false);
    
//    qDebug()<<this->id<<rect.left() << "fs:"<< fontSize<< pos;
}


