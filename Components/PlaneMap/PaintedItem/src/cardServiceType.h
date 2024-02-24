
#ifndef CARDSERVICETYPE_H
#define CARDSERVICETYPE_H

#include <QObject>

enum CardTypeEnum : int {
    Empty = 0,
    Basic = 1,
    Silver = 2,
    Gold = 3,
    Platinum = 4 
};

class CardServiceType
{
 
  public:
    static CardTypeEnum getCardType(QString &cardType)
    {
        if(cardType.isEmpty()) return CardTypeEnum::Empty;
        
        auto type = cardType.toLower();
        
        if(type=="base" || type=="basic") return CardTypeEnum::Basic;
        if(type=="silver") return CardTypeEnum::Silver;
        if(type=="gold") return CardTypeEnum::Gold;
        if(type=="platinum") return CardTypeEnum::Platinum;
        return CardTypeEnum::Empty;
    }
    
    static const  QString getCardAbbr(CardTypeEnum cardType)  
    {       
        if(cardType<=CardTypeEnum::Basic) return "";
        if(cardType==CardTypeEnum::Silver) return "SLV";
        if(cardType<=CardTypeEnum::Gold) return "GLD";
        if(cardType<=CardTypeEnum::Platinum) return "PLT";
        return "";
    }
    
};

#endif // CARDSERVICETYPE_H
