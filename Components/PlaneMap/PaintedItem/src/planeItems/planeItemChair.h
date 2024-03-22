
#ifndef PLANEITEMCHAIR_H
#define PLANEITEMCHAIR_H

#include <functional>

#include "planeItemBase.h"
#include "planeColors.h"
#include "cardServiceType.h"

namespace ChairSettingsEnum
{
enum
{
    None = 0,
    SpacePlus = 1<<1,
    FixedBack = 1<<2,
    LeftArmrest= 1<<3,
    RightArmrest= 1<<4
};
typedef long Flags;
};


class PlaneItemChair  : public PlaneItemBase
{
  public:
    typedef std::function<void(QPainter &p, int width, int height, ChairColor &chairColor, QList<QRectF> parts)> drawChairSpriteFunc;
    typedef std::function<void(QPainter *painter, QRect &rect, ChairColor &chairColor)> drawChairInfoFunc;
    
    int rowNumber;
    QChar letter;
    ChairSettingsEnum::Flags settings;
    
    int index; // номер кресла в линейном массве; max(letter) * rowNumber
    
    //Код класса обслуживания(салона) - "F" => "First Class", "J" => "Business Class", "W" => "Comfort", "Y" => "Economy",
    QChar seatType; 
//    QString seatSpec3;
    
    // тип карты лояльности - Золотой, серебрянный и т.п
    CardTypeEnum cardType;
    
    // занято ли кресло пассажиром
    bool hasPassenger;
    // выделено ли кресло 
    bool isSelected; 
    
    // блок сидений в ряду - для внутреннего поиска
    uint groupId;    
    
    
    void draw(QPainter *p, QRect &rect);
    
    PlaneItemChair():PlaneItemBase("seat"),hasPassenger(false), isSelected(false), cardType(Empty){   }
    
  public:
    static int getSeatIndex(int seatInRows, QString seatNumber);
    
  private:
    static const QMap<QString, QList<QRectF>> chairParts;
    static QMap<QString, QList<QRectF>> fillChairParts();
        
    
    void drawChairProcess(QPainter *painter, QRect &rect,
                         const drawChairSpriteFunc &drawSpriteFunc,
                         const drawChairInfoFunc &drawInfoFunc);
    
    QPixmap* makePixmap(int width, int height, ChairColor &chairColor,
                             const drawChairSpriteFunc &drawSpriteFunc
                             );
    
    ChairColor* getChairDrawColor();
    void drawSpriteCommon(QPainter &p, int width, int height, ChairColor &chairColor, QList<QRectF> parts);
    void drawChairInfoCommon(QPainter *painter, const QRect &rect, ChairColor &chairColor);
    
    void drawSpriteBuss(QPainter &p, int width, int height, ChairColor &chairColor, QList<QRectF> parts);
    void drawChairInfoBuss(QPainter *painter, const QRect &rect, ChairColor &chairColor);
    void drawBusinessPaxName(QPainter *painter, const QRect &rect);        
        
    void drawCardTypeInfo(QPainter *painter, const QRect &rect, ChairColor &chairColor, CardTypeEnum cardType, qreal topMargin);
    
    QPixmap* getStarSprite(CardTypeEnum cardType, ChairColor &chairColor);
};


#endif // CHAIRPLANEITEM_H
