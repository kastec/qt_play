
#include "drawHelper.h"
#include "planeItemText.h"

void PlaneItemText::draw(QPainter *painter, QRect &rect)
{
    if(this->title.isEmpty()) return;
    
//    drawText(painter, rect);
//    return;
 
    auto cacheKey = "txt-" + this->title;
    DrawHelper::drawSprite(painter, cacheKey, rect.width(),
        rect,
        [this](auto p, auto r){this->drawText(p, r);},
        true);    
}

void PlaneItemText::drawText(QPainter *painter, QRect &rect)
{
//    qDebug() << "draw-txt:" << title << location << color << fontSize;
                                         
    SymbolRenderStyle style;
    style.color = this->color;
    style.backColor = Qt::white;
    style.weignt = QFont::Normal;    
    style.fontSize = zoom(this->fontSize, rect); // размер фона зависит от ZOOM
   
    DrawHelper::drawText(painter, rect, this->title, style);  
}

