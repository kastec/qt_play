#include <QPainterPath>

#include "qdebug.h"

#include "drawHelper.h"
#include "../spriteCache.h"
#include "PlaneItemChair.h"
#include "planeColors.h"
#include "cardServiceType.h"

const QMap<QString, QList<QRectF>> PlaneItemChair::chairParts = PlaneItemChair::fillChairParts();

QMap<QString, QList<QRectF>> PlaneItemChair::fillChairParts()
{    
    QMap<QString, QList<QRectF>>  chairParts;
    
    auto partSizes = QList<QRect>{
        QRect(4,9, 93, 90), // сиденье
        
        QRect(18,2, 63, 7 * 2), // доп. расстояние для ног        
        QRect(25, 87, 50, 6), // фиксир. спинка
        
        QRect(9,24, 6,56), // левый подлокотник
        QRect(84, 24, 6,56) // правый подлокотник
    } ;
    
    
    // отрисовка Comfort и Econom  - одинакова
    chairParts["W"] = PlaneItemBase::rescale(partSizes, 100, 100);//scaledSizes;
    chairParts["Y"] = PlaneItemBase::rescale(partSizes, 100, 100);//scaledSizes; 
    
    // ==== Business ====
    auto partBusSizes = QList<QRect>{
        QRect(2, 3, 97, 60), // сиденье
        
        QRect(18,2, 63, 7 * 2), // доп. расстояние для ног        
        QRect(25, 54, 50, 4), // фиксир. спинка
        
        QRect(7,13, 4,36), // левый подлокотник        
        QRect(88,13, 4,36) // правый подлокотник        
    } ;
    
    chairParts["J"] = rescale(partBusSizes, 100, 100);//scaledSizes; 
    
    return chairParts;
}


int PlaneItemChair::getSeatIndex(int seatInRows, QString seatNumber)
{    
    if(seatNumber.isEmpty()) return 0;
    auto len = seatNumber.length();
    int row=0;
    for(int i=0;i<len-1;i++)
        row = row * 10 + (seatNumber[i].toLatin1()-'0');
    
    QChar letter = seatNumber[len-1];
    int  index = row * seatInRows + (letter.toLatin1() - 'A');
    return index;
}

void PlaneItemChair::draw(QPainter *painter, QRect &rect)
{
 
//    std::function<QPixmap*(int , int , ChairColor &)>  r =  &PlaneItemChair::makeChairPixMap;
    
    if(this->seatType=='J')
        drawChairProcess(painter, rect,
            [=](auto && ...args){return this->drawSpriteBuss(args...);},
            [=](auto && ...args){this->drawChairInfoBuss(args...);}
            );
    else
        drawChairProcess(painter, rect,
            [=](auto && ...args){return this->drawSpriteCommon(args...);},
            [=](auto && ...args){this->drawChairInfoCommon(args...);}
            );
}

//typedef std::function<QPixmap*(int w, int h, ChairColor &color)> makeChairPixmapFunc;
//typedef std::function<void(QPainter *painter, const QRect &rect, ChairColor &chairColor)> drawChairInfoFunc;

ChairColor* PlaneItemChair::getChairDrawColor()
{
    auto chairTypeColor = PlaneColors().getSeatColor(this->seatType);
    auto chairDrawColor =  (hasPassenger==false)
                              ? &chairTypeColor->empty
                              : isSelected?
                                    &chairTypeColor->main: &chairTypeColor->lighter;
    return chairDrawColor;
}


void PlaneItemChair::drawChairProcess(QPainter *painter, QRect &rect,
                          const drawChairSpriteFunc &drawSpriteFunc,
                          const drawChairInfoFunc &drawInfoFunc
                          )
{
    auto w = rect.width(), h = rect.height();
    
    auto chairDrawColor =  this->getChairDrawColor();
    
    auto key = "seat" + QString(this->seatType) + QString::number(chairDrawColor->type)+ QString::number(this->settings);
    auto sprite = SpriteCache().get(key, w);
    if(sprite==nullptr){
        auto pmChair = makePixmap(w, h, *chairDrawColor, drawSpriteFunc);
        SpriteCache().push(key, pmChair, w);
        sprite = pmChair;
    }
    
    painter->drawPixmap(rect, *sprite);
    
    drawInfoFunc(painter, rect, *chairDrawColor);
}


QPixmap* PlaneItemChair::makePixmap(int width, int height, ChairColor &chairColor,
                                         const drawChairSpriteFunc &drawSpriteFunc)
{    
    QPixmap* pixmap = new QPixmap(width, height);
    pixmap->fill();
 
    QPainter p(pixmap);
    
    p.setRenderHint(QPainter::Antialiasing);
    QPen borderColor(Qt::black, width*0.025);
    p.setPen(borderColor);
 
   // draw rect background
//    p.fillRect(0,0,  width, height, QColor(150,99,99));

    auto parts = chairParts[this->seatType];
    
    drawSpriteFunc(p, width, height, chairColor, parts);
      
    p.end();
    
    return pixmap;
}

//====  DRAW COMMON SEAT ==========================================

void PlaneItemChair::drawSpriteCommon(QPainter &p, int width, int height, ChairColor &chairColor, QList<QRectF> parts)
{
    // место для ног
    if(settings & ChairSettingsEnum::SpacePlus)
        DrawHelper::drawRect(p, parts[1], width,  height, QColorConstants::White, chairColor.border, 7);
    
    // сиденье
    DrawHelper::drawRect(p, parts[0], width,  height, chairColor.color, chairColor.border, 11);
    
    // фиксированная спинка
    if(settings & ChairSettingsEnum::FixedBack)
        DrawHelper::drawRect(p, parts[2], width,  height, chairColor.font);
    
    // подлокотники
    if(settings & ChairSettingsEnum::LeftArmrest)
        DrawHelper::drawRect(p, parts[3], width,  height, chairColor.font);
    if(settings & ChairSettingsEnum::RightArmrest)
        DrawHelper::drawRect(p, parts[4], width,  height, chairColor.font);
    
}

void PlaneItemChair::drawChairInfoCommon(QPainter *painter, const QRect &rect, ChairColor &chairColor)
{   
    auto w = rect.width();
    
    // размер фонта увеличиваем согласно Zoom
    auto fontLetterSize = this->zoom(24, rect);
     
    auto symbolCacheKey = "sl" + QString(this->seatType) + QString::number(chairColor.type);
    QPoint pos(rect.left() + w*0.48, rect.top() + w*0.32);
    
    SymbolRenderStyle style{fontLetterSize, chairColor.font, chairColor.color, QFont::DemiBold,1.0};
    DrawHelper::drawSymbols(painter, pos, this->letter, style, symbolCacheKey, true);
    
    if(this->cardType != CardTypeEnum::Empty && this->cardType>0)
        drawCardTypeInfo(painter, rect, chairColor, this->cardType, 0.68);

}

//====  DRAW BUSSINESS SEAT ==========================================

void PlaneItemChair::drawSpriteBuss(QPainter &p, int width, int height, ChairColor &chairColor, QList<QRectF> parts)
{    
    // сиденье
    DrawHelper::drawRect(p, parts[0], width,  height, chairColor.color, chairColor.border, 11);
        
    // фиксированная спинка
    if(settings & ChairSettingsEnum::FixedBack)
        DrawHelper::drawRect(p, parts[2], width,  height, chairColor.font);
        
    // подлокотники
    if(settings & ChairSettingsEnum::LeftArmrest)
        DrawHelper::drawRect(p, parts[3], width,  height, chairColor.font);
    if(settings & ChairSettingsEnum::RightArmrest)
        DrawHelper::drawRect(p, parts[4], width,  height, chairColor.font);
}

void PlaneItemChair::drawChairInfoBuss(QPainter *painter, const QRect &rect, ChairColor &chairColor)
{
    auto w = rect.width();

    // --- seat letter ----
    // размер фонта 38 увеличиваем согласно Zoom
    auto letterFontSize = zoom(28, rect);
    
    auto letterCacheKey = "sl" + QString(this->seatType) + QString::number(chairColor.type);
    QPoint letterPos(rect.left() + w*0.48, rect.top() + w*0.22);
    SymbolRenderStyle letterStyle{letterFontSize, chairColor.font, chairColor.color, QFont::DemiBold, 1.0};
    DrawHelper::drawSymbols(painter, letterPos, this->letter, letterStyle, letterCacheKey, true);
    
    if(this->cardType != CardTypeEnum::Empty && this->cardType>0)
        drawCardTypeInfo(painter, rect, chairColor, this->cardType, 0.44);
    
    
    //-- draw NAME
    auto nameTextHeight = rect.height() * 0.31;
    QRect nameTextRect(rect.x(),rect.y()+ rect.height() - nameTextHeight, rect.width(), nameTextHeight);
    
    auto cacheKey = "paxName-" + QString::number(this->rowNumber) + QString(this->letter);
    if(this->hasPassenger)
        DrawHelper::drawSprite(painter, nameTextRect, cacheKey, nameTextRect.width(),
            [this](auto p, auto r){drawBusinessPaxName(p, r);},
            false);

//     drawBusinessPaxName(painter,nameTextRect);
   
}

void PlaneItemChair::drawBusinessPaxName(QPainter *painter, const QRect &rect)
{
    if(!this->hasPassenger) return;
    
//    qDebug()<<"paxName"<< rect;
    SymbolRenderStyle style;
    
    auto fontScale = 11 * (location.width()/(qreal)100); // размер фонта зависит от размера ячейки
    style.fontSize = zoom(fontScale , rect); // размер фона зависит от ZOOM
    
//    style.fontSize = zoom(18, rect); // размер фона зависит от ZOOM
    style.color = Qt::black;
    style.backColor = Qt::white;
    
    QTextOption topt;
   
    topt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    topt.setAlignment(Qt::AlignTop|Qt::AlignHCenter);
//    DrawHelper::drawText(painter, rect, "Kozlovqwesdqe Alexander ffsdfdeede", style, topt);
    
     QFont font("Inter", style.fontSize);
     QFontMetrics fm(font);
     auto fH = (fm.height() )*2; // только  2 строки рисуем
     auto textRect = rect;
    
    DrawHelper::drawText(painter, textRect, this->title, style, topt);
}


void PlaneItemChair::drawCardTypeInfo(QPainter *painter, const QRect &rect, ChairColor &chairColor,
                                      CardTypeEnum cardType, qreal topMargin)
{
    auto w = rect.width();
    
    // --- draw star
    auto pmStar = getStarSprite(this->cardType, chairColor);
    auto starW = zoom(0.9*pmStar->width(), rect);
    
    // --- card type ----
    auto cardFontSize=zoom(15, rect);
    
    QString cardText = CardServiceType::getCardAbbr(this->cardType);    
    auto cardCacheKey = "card" + QString(this->seatType) + QString::number(chairColor.type);
    QPoint cardTxtPos(rect.left() + w*0.48 + starW/2, rect.top() + w * topMargin);
    SymbolRenderStyle cardStyle{ cardFontSize, chairColor.font, chairColor.color, QFont::DemiBold, 1.0};
    
    auto cardRect = DrawHelper::drawSymbols(painter, cardTxtPos, cardText, cardStyle, cardCacheKey, true);
    
    //    QPoint starPos = cardTxtRect.topLeft()- QPoint(starW,0);
    QRect starRect(cardRect.x() - starW*1.05, cardRect.y() + starW*0.0, starW, starW);
    painter->drawPixmap(starRect,*pmStar);
}


QPixmap* PlaneItemChair::getStarSprite(CardTypeEnum cardType, ChairColor &chairColor)
{
    auto key = "star" + QString::number((int)cardType) +QString(this->seatType) + QString::number(chairColor.type);
    auto starSprite = SpriteCache().get(key);
    
    if(starSprite!=nullptr) return starSprite;
    
    auto fileName = "star-list-" + CardServiceType::getCardAbbr(cardType) +".png";
    auto fullPath = "C:\\Projects\\QT_Test\\QtTest1\\Components\\PlaneMap\\cardtypes\\" + fileName;
    
    QPixmap starPix(fullPath);
     starSprite = new QPixmap(starPix.size());
//    starSprite = new QPixmap(20,20);
    starSprite->fill(chairColor.color);
    
    QPainter p(starSprite);    
    p.drawPixmap(0,0, starPix);
    p.end();
    SpriteCache().push(key, starSprite);    
    
    return starSprite;
}





