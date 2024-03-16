
#ifndef PLANELAYOUTSEARCHER_H
#define PLANELAYOUTSEARCHER_H

#include <QObject>
#include <QRect>
#include <QMap>
#include "planeItemChair.h"
#include "qdebug.h"

#include "planeItemBase.h"

struct PlaneTreeItem
{
  public :
    QRect rect;
    QList<PlaneTreeItem> treeItems;
    QList<PlaneItemBase*> planeItems;
    
  public:
    void add(PlaneItemBase* item){
        planeItems.append(item);
        rect = rect.united(item->location);
    }
    void add(PlaneTreeItem treeitem){
        treeItems.append(treeitem);
        rect = rect.united(treeitem.rect);
    }
};

class PlaneLayoutSearcher{
  private:
    PlaneTreeItem root;
    
    QList<PlaneItemChair*> seats;
    int rows, chairsInRow;
 //   QMap<QString, PlaneItemBase*> allItemsMap;
    
  public:
    
    PlaneItemChair* findChair(const QString &seatNumber){
        
        auto index = PlaneItemChair::getSeatIndex(chairsInRow, seatNumber);
//        qDebug()<<"find-ch"<<seatNumber<< index;
//        qDebug()<< "seats-len"<<this->seats.length() << "chairsInRow:"<< chairsInRow;
        if(index <= 0 || index> this->seats.length())
        {
            // TODO: можно поискать по всем Items через find();
            return nullptr;
        }
        if(seats[index]==nullptr){
            auto mess = QString("Error find seat: seat '%1' not found in layout!").arg(seatNumber);
            qDebug()<< mess; // TODO: write to log
            return nullptr;
        }
//        qDebug()<< "   found: "<< seats[index]->rowNumber<< seats[index]->letter<< "ind:"<<seats[index]->index;
        return seats[index];
    }
    
    PlaneItemBase* find(const QPoint &point){
        auto item = findItem(root, point);
        return item;
    }
    
    QList<PlaneItemBase*> findItems(const QRect &viewPort){
        QList<PlaneItemBase*> vpItems;
        findItemsInRect(viewPort, vpItems, this->root);
        return vpItems;
    }
    
    auto getMaxIndexInRow(const QList<PlaneItemBase*> &planeItems)
    {
        int maxLetterNum=0;
        int maxRow=0;
        for(auto &i:planeItems)
        {
            if(i->type!="seat") continue;
            auto seat = (PlaneItemChair*)i;
            auto letterNum = seat->letter.toLatin1()-'A';
            maxLetterNum = __max(letterNum, maxLetterNum);
            maxRow = __max(maxRow, seat->rowNumber);
        }
//        qDebug()<<"getMaxIndexInRow:"<< maxRow<< maxLetterNum;
        return std::make_tuple(maxRow, maxLetterNum);
    }
    
    void setItems(const QList<PlaneItemBase*> &planeItems)
    {
        auto [rows, maxChairIndexInRow] = getMaxIndexInRow(planeItems);
        this->rows = rows;
        this->chairsInRow = maxChairIndexInRow+1;
        
        root = makeItemGroupsTree(planeItems);
                
        seats.clear();
  
        seats.resize((rows+1) * chairsInRow);
        qDebug()<< "seats allow:" << seats.length() << "rows:"<< rows<<"chairsInRow:"<<chairsInRow;
        for(auto &i:planeItems)
        {
            if(i->type!="seat") continue;
            auto seat = (PlaneItemChair*)i;
            
            seat->index = this->chairsInRow * seat->rowNumber + (seat->letter.toLatin1() - 'A');
            
//            if(seat->rowNumber<3)
//                qDebug()<< "seat-ind:" << seat->index << seat->rowNumber<<seat->letter;
            this->seats[seat->index] = seat;
        }
    }
    
    void dump(){ dump(root,0); }
    
    
    void setSelections(const QList<QString> &selectedSeats){
        for(auto &s : seats)
            s->isSelected = false;        
        
        for(auto &seatNum:selectedSeats)
        {
            auto index = PlaneItemChair::getSeatIndex(chairsInRow, seatNum);
            auto s = seats[index];
            s->isSelected = true;
        }
    }
    
    void clearSelections(){
        for(auto &s : seats)
            s->isSelected = true;
    }
    
    
  private:
    PlaneItemBase* findItem(PlaneTreeItem &item, const QPoint &point)
    {
        for(auto &i: item.planeItems)
            if(i->location.contains(point))
                return i;
        
        for(auto &treeitem: item.treeItems){
            if(treeitem.rect.contains(point)){
                auto PlaneItemBase = findItem(treeitem, point);
                if(PlaneItemBase!=nullptr) return PlaneItemBase;
            }
        }
        return nullptr;
    };
    
    void findItemsInRect(const QRect &viewPort, QList<PlaneItemBase*> &planeItems, const PlaneTreeItem &treeItem)
    {
        if(treeItem.rect.intersects(viewPort)==false)
            return;
        
        for(auto &pi : treeItem.planeItems)
            if(pi->location.intersects(viewPort))
                planeItems.append(pi);
        
        for(auto &ti : treeItem.treeItems)
            findItemsInRect(viewPort, planeItems, ti);
    };
    
    PlaneTreeItem makeItemGroupsTree(const QList<PlaneItemBase*> &planeItems)
    {
        PlaneTreeItem root;
        PlaneTreeItem otherItemsGroup;
        
        PlaneTreeItem currGroup;
        PlaneTreeItem currRow;
        int currGroupId=-1;
        int currRowNum=-1;
                
        
        for(auto &i:planeItems)
        {
            if(i->type!="seat"){
                otherItemsGroup.add(i);
                continue;
            }
            
            //-- seat --
            auto chair = (PlaneItemChair*) i;
//            if(chair->rowNumber>=49)
//                qDebug()<<"searcher-gr:"<< chair->rowNumber<< chair->letter;
            if(chair->rowNumber!=currRowNum && currRowNum!=-1)
            {
                currGroup.add(currRow);
                currRow = PlaneTreeItem();
            }
            
            if(chair->groupId!=currGroupId && currGroupId!=-1)
            {
                // если в группе тоьлко один Row, то можно его вынести на уровень выше
                if(currGroup.treeItems.length()==1)
                    root.add(currGroup.treeItems[0]);
                else
                    root.add(currGroup);
                
                currGroup= PlaneTreeItem();
            }
            
            currRow.add(chair);
            currRowNum = chair->rowNumber;
            currGroupId = chair->groupId;
        }
        currGroup.add(currRow);
        root.add(currGroup);
        currGroup = PlaneTreeItem();
        root.add(otherItemsGroup);
               
        return root;
    }
    
    void dump(const PlaneTreeItem &treeItem, int level){        
        auto offset = QString("[l:%1]").arg(level)+QString("    ").repeated(level);
        auto info = QString("%1 (%3,%4: %5x%6) subGroups: %7  items: %8 ")
                        .arg(offset)
                        .arg(treeItem.rect.x()).arg(treeItem.rect.y()).arg(treeItem.rect.width()).arg(treeItem.rect.height())
                        .arg(treeItem.treeItems.length())
                        .arg(treeItem.planeItems.length());
        
        qDebug() << info;
        
        for(auto &ti:treeItem.treeItems)
            dump(ti, level+1);
        
        if(treeItem.planeItems.length()==0) return;
        
        auto infoItem = offset+"    ";
        for(auto &pi:treeItem.planeItems)
            infoItem.append(pi->title+",  ");
        
        qDebug() << infoItem;
    }
    
}; // class PlaneLayoutSearcher



#endif // PLANELAYOUTSEARCHER_H


















