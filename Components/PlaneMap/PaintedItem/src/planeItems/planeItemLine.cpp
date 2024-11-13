
#include "planeItemLine.h"
#include "QDebug"

void PlaneItemLine::draw(QPainter *p, QRect &rect)
{
   // return;
//     qDebug() << "draw-line:" << location << color << width;
    
//    auto width = zoom(this->width, rect);
    auto width = rect.height();
    
    p->save();
    p->setRenderHint(QPainter::Antialiasing);
    QPen pen(this->color, width);
    p->setPen(pen);
    
    p->drawLine(rect.topLeft(), QPoint(rect.right(), rect.top() ));
    
    p->restore();
}
