
#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

class PaintArea: public QQuickPaintedItem
{
    Q_OBJECT
    
  public:
   
    PaintArea(QQuickItem *parent = nullptr) : QQuickPaintedItem(parent)
    {}
                                                               
                                                               
    std::function<void(QPainter *painter)> onPaint;
    
    void paint(QPainter *painter)
    {
        if(onPaint!=nullptr)
            onPaint(painter);
        //        painter->device()->width();
        //        painter->device()->height();
        /*      
              auto xpos = position.x();
              auto ypos = position.y();
              
     framerate.start();
     
      // PLANE RENDER
      if(planeMap!=nullptr)
          planeMap->draw(painter,xpos, ypos, zoom);
      
     framerate.stop();
     
      this->set_avgRenderTime(framerate.avgFrameMsec);
      this->set_renderTime(framerate.frameMsec);
      */
        //        return; 
    }
    
};

#endif // PAINTAREA_H
