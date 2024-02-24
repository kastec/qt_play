
#ifndef PLANEITEMBASE_H
#define PLANEITEMBASE_H

#include <QObject>
#include <QPainter>

struct SymbolRenderStyle{
  public:
    int fontSize;
    QColor color, backColor;
    QFont::Weight weignt;
    qreal opacity = 1.0;
};

class PlaneItemBase 
{
  public:
    QRect location;
    
    QString id;
    QString title;
    QString type;    
    
    virtual void draw(QPainter *p, QRect &rect);
    PlaneItemBase(){}
    PlaneItemBase(QString objType):type(objType){}
    
     
  protected:
    qreal zoom(const QRect &rect);
    int zoom(int value, const QRect &rect);
        
    static QList<QRectF> rescale(QList<QRect> &rects, qreal width, qreal height);
    static QList<QLineF> rescale(QList<QLine> &lines, qreal width, qreal height);
    
    qreal getOpacity(qreal value, qreal min, qreal max);
    
    
    QPixmap* getSymbolPixmap(QChar symbol, const SymbolRenderStyle &style);
    QRect drawSymbols(QPainter *painter, const QPoint &position, QString stringValue,
                     const SymbolRenderStyle &style, QString symbolCacheKey, bool isCenterPoint=false);
    
    void drawRect(QPainter &p, QRectF &rect, int width, int height, const QColor &fillColor, QColor &borderColor, int radius);
    void drawRect(QPainter &p, QRectF &rect, int width, int height, const QColor &fillColor);
};



class PlaneItemWc : public PlaneItemBase
{
  public:
    bool isLeft;
    PlaneItemWc():PlaneItemBase("wc"){}
    
    void draw(QPainter *p, QRect &rect);
};

class PlaneItemText : public PlaneItemBase
{
  public:
    PlaneItemText():PlaneItemBase("text"){}
    void draw(QPainter *p, QRect &rect);
};

#endif // PLANEITEM_H
