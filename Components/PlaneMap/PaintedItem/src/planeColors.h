
#ifndef PLANECOLORS_H
#define PLANECOLORS_H

#include "qcolor.h"
#include <QObject>
#include <QMap>

class ChairColor
{
  public:
    short type;
    QColor color;
    QColor border;
    QColor font;
    
    ChairColor(){}
    ChairColor(short type, QColor c,QColor b, QColor f): type(type), color(c), border(b), font(f){ }
};

class ChairClassColor
{
  public:
    ChairColor main;
    ChairColor lighter;
    ChairColor empty;
};

class PlaneColors
{
  public:
  
   inline static QMap<QChar, ChairClassColor*> chairColors;
    
  public:    
    PlaneColors()
    {
        if(chairColors.isEmpty())
            this->initColors();
    }
    
    ChairClassColor* getSeatColor(QChar type){
        auto chaircolor = chairColors[type];
        return (chaircolor != nullptr) ? chaircolor : chairColors['Y'];
    }
    
  private:
    void initColors(){
        auto *bus = new ChairClassColor();
        bus->main    = ChairColor(2, QColor(238, 118, 35),  QColor(  0,   0,   0), QColor(255,255,255));
        bus->lighter = ChairColor(1, QColor(244, 208, 183), QColor(153, 153, 153), QColor(255,255,255));
        bus->empty   = ChairColor(0, QColor(246, 234, 226), QColor(204, 204, 204), QColor(116,119,111));
        chairColors['J'] = bus;
        
        auto *comf = new ChairClassColor();  
        comf->main    = ChairColor(2, QColor(83,  146,   2), QColor(  0,   0,  0),  QColor(255,255,255));
        comf->lighter = ChairColor(1, QColor(198, 217, 174), QColor(153, 153, 153), QColor(255,255,255));
        comf->empty   = ChairColor(0, QColor(231, 237, 222), QColor(204, 204, 204), QColor(116,119,111));
        chairColors['W'] = comf;
        
        auto *econom = new ChairClassColor();  
        econom->main    = ChairColor(2, QColor(  0,  47, 135), QColor(  0,   0,   0), QColor(255,255,255));
        econom->lighter = ChairColor(1, QColor(128, 151, 195), QColor(153, 153, 153), QColor(255,255,255));
        econom->empty   = ChairColor(0, QColor(230, 235, 243), QColor(204, 204, 204), QColor(116,119,111));
        chairColors['Y'] = econom;
    }
};

#endif // PLANECOLORS_H
