
#ifndef SPRITECACHE_H
#define SPRITECACHE_H

#include "qpixmap.h"
#include <QObject>
#include <QMap>


class SpriteCache
{
    
    struct SpriteData{
        uint criteria;
        QPixmap* sprite;
//        SpriteData():criteria(0),sprite(nullptr){}
    };
    
    
  public:
    
    SpriteCache(){   }
    
  private:
    
    inline static QMap<QString, SpriteData> sprites;
    
   
    
  public:
    
    QPixmap* get(QString key, int criteria)
    {
        auto &data = sprites[key];
        auto res = data.sprite!=nullptr && data.criteria==criteria? data.sprite: nullptr;
        return res;
    }
    
    QPixmap* get(QString key)
    {
        return sprites[key].sprite;    
    }
    
    void push(QString key, QPixmap* spriteValue, int criteria = 0)
    {
        auto &data = sprites[key];
        if(data.sprite!=nullptr) // delete prev (old) sprite
            delete data.sprite;
        
        data.criteria = criteria;
        data.sprite = spriteValue;
    }
    
    void remove(QString key)
    {
        auto hasKey = sprites.contains(key);
        if(!hasKey) return;
        
        auto &data = sprites[key];
        if(data.sprite!=nullptr)
            delete data.sprite;
        sprites.remove(key);
    }
    
    void clear()
    {
        auto keys = sprites.keys();
        for(auto &k : keys)
        {
            auto data = sprites.take(k);
            delete data.sprite;
        }
    }
    
};


#endif // SPRITECACHE_H
