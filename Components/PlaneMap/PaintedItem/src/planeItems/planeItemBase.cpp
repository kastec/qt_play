
#include <QPainterPath>
#include "planeItemBase.h"

#include "drawHelper.h"
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
        
        DrawHelper::drawSymbols(p, rect.center(), this->title, style, symbolCacheKey, true);  
    }
    
    p->restore();
}



