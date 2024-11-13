#ifndef SHAREDQLIST_H
#define SHAREDQLIST_H

#include <QObject>
#include "qdebug.h"

/*
 Example:

1)
 SharedQList<MyClass> sList;
 sList-append( new MyClass() );

2)
 QList<MyClass*> items;
 ... fill items...
 SharedQList<MyClass> sList(items);

 */

template <class T>
class SharedQList : public QSharedPointer<QList<T*>>
{
  private:
    static QString ptrToStr(void *p){
        auto str = QString::number((std::uintptr_t)(p));
        return str;
    }
  public:
    
    SharedQList(QList<T*> list = QList<T*>())
        : QSharedPointer<QList<T*>>(new QList<T*>(list), &SharedQList::myDeleter)
    {
        //        auto s = QString("created slist: %1,  *list: %2").arg(ptrToStr((void *)this)).arg(ptrToStr((void *)this->data()));
        //        qDebug()<<s;
    }
    
  private:
    static void myDeleter(QList<T*> *list)
    {
        //        qDebug()<<"delete slist: " << ptrToStr(list);
        
        qDeleteAll(*list);
        delete list;
    }
};

#endif // SHAREDQLIST_H
