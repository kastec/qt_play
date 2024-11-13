
#include "drawHelper.h"
#include "planeItemRowNumber.h"

void PlaneItemRowNumber::draw(QPainter *painter, QRect &rect)
{        
    auto fontSize = this->zoom(34, rect);
    
    QColor fontColor= QColor::fromRgb(128,128,128);
    SymbolRenderStyle style{fontSize, fontColor, Qt::white, QFont::Light, 1.0};
    auto symbolCacheKey = "rownum";
    
    auto left = rect.left();
    // TODO : это для того, чтобы номер всегда был на экране
    left = left<0? 0: left;    
    QPoint pos(left, rect.top());
    
    DrawHelper::drawSymbols(painter, pos, this->title, style, symbolCacheKey, false);
    
    //    qDebug()<<this->id<<rect.left() << "fs:"<< fontSize<< pos;
}
