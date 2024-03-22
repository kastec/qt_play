
#ifndef PLANELAYOUTPARSER_H
#define PLANELAYOUTPARSER_H

#include <QRegularExpression>
#include <QObject>
#include "planeItems/planeItemChair.h"
#include "planeItems/planeItemBase.h"
#include "planeItems/planeItemExit.h"


class PlaneLayoutParser{
    
  public:
   static const int CHAIR_SIZE=100;
   static const int CHAIR_OFFSET=CHAIR_SIZE*0.04;
   static const int CHAIR_ROW_OFFSET=CHAIR_SIZE*0.4;
   static const int CHAIR_ROW_OFFSET_BUSS=CHAIR_SIZE*0.2;
   static const int CHAIR_WALKWAY=CHAIR_SIZE*0.5;
   
   // дополнительно непорпорционально растянуть Nav по вертикали
   constexpr static const qreal NAV_SPREAD_ROWS=1.25;
   
   const QChar NONE_CHAIR='0';
   
   
 public:
   int maxChairsInRow; // максимальное кол-во сидений в ряду
   int rows; // кол-во рядов
   
  public :
    PlaneLayoutParser(): posX(0), posY(0), rows(0), maxChairsInRow(0){
    }
    
    QList<PlaneItemBase*> parse(const QList<QString> &lines)
    {
        parseLayout(lines);
        return planeItems;
    }
    
    QSize getPlaneSize()
    {
        return getPlaneDimensions();
    }

  private:
    QList<PlaneItemBase*> planeItems;
    int airplaneWidth;
    int posX, posY;
      
    QMap<QChar, qreal> chairScales{ {'Y', 1.0}, {'W', 1.0}, {'J', 1.7} };
    
    int exitCount=0;
    
    struct SeatRowMetric{
        int width;
        int height;
        int offset;
        int rowOffset;
        int walkWay;
    };
    
    struct ItemOffset{
        QString refObjId;        
        int yOffset;
        int xOffset;
        ItemOffset():refObjId(""), yOffset(0),xOffset(0){}
    };
    
    struct NamedParams{
        QString name;
        int param1;
        int param2;
        NamedParams():name(""), param1(0),param2(0){}
    };
    
    struct SeatDeclare{
        QChar letter;
        ChairSettingsEnum::Flags settings;
      public:
        SeatDeclare():settings(ChairSettingsEnum::None){}
    };
    
    struct SeatGroup:public ItemOffset{
        QList<SeatDeclare> seats;
    };
    
    struct SeatRowDeclaration{
        QChar seatType;
        QString seatSpec;
        int rowFrom;
        int rowTo;
        int rowSeatCount;
        QList<SeatGroup> groups;      
    };
    
    PlaneItemBase* findItem(QString &id)
    {
        if(id.isEmpty()) return nullptr;
        for(auto &pi: this->planeItems)
            if(pi->id==id) return pi;
        return nullptr;
    }
    
    void parseLayout(const QList<QString> &lines)
    {
        qDebug()<< "parsing airplane layout" << lines.length()<<"lines";
        int lineNum=0;
        for( auto &currLine: lines)
        {            
            lineNum++;
            auto cline = currLine.simplified().trimmed();
            if(cline.isEmpty() || cline.startsWith("#")) continue;
            
            auto lineType = cline.section(' ',0,0).toLower();
            cline = cline.section(' ', 1);            
            auto line = clearLine(cline);          
                       
//            qDebug() << "lineType:" << lineType << line;
            
            if(lineType == "name") qDebug()<< "  layout "<< line;
            if(lineType == "plane_width") setMaxPlaneWidth(line);
            if(lineType == "seat_scale") makeChairSizes(line);
            if(lineType == "row") makeRowSeatsLine(line, lineNum);
            if(lineType == "space") makeSpace(line);
            if(lineType == "exit") makeExit(line);
            if(lineType == "obj") makeObject(line);            
        }
    }
    
    SeatRowMetric calcChairLayoutMetric(const SeatRowDeclaration &row)
    {        
        qreal scale = chairScales.value(row.seatType, 1.0);
        
        int w = CHAIR_SIZE*scale, h=w, offs=CHAIR_OFFSET*scale;
        int rowOffs = (row.seatType=='J'? CHAIR_ROW_OFFSET_BUSS:CHAIR_ROW_OFFSET)* scale;
        int walkWay = (airplaneWidth - (w+offs)*row.rowSeatCount) / (qreal)(row.groups.length()-1);

        return SeatRowMetric {w, h, offs, rowOffs, walkWay };;
    }
    
    void makeChairSizes(QString &line)
    {
        auto setts = line.split(' ');
        for(auto &s:setts){
            auto prop = s.split(':');
            if(prop.length()!=2) continue;
            QChar seatClassType = prop[0].at(0); // первый символseatClassType
            chairScales[seatClassType] = prop[1].toDouble();
        }
    }
    
    void setMaxPlaneWidth(QString &line)
    {
        // PLANE_WIDTH 3,3,3(+20)       
        static QRegularExpression regPlaneWidth("^(?<groups>(\\d,)*\\d)(\\((?<addwidth>[+-]?\\d+)\\))?");
        
        auto m = regPlaneWidth.match(line);
        auto groups = m.captured("groups").split(',');
        int addWidth =  m.captured("addwidth").toInt();
        
        this->maxChairsInRow=0;
              
        this->airplaneWidth=0;
        for(auto &g:groups)
            this->maxChairsInRow += g.toInt();
        
        this->airplaneWidth = (CHAIR_SIZE+CHAIR_OFFSET)*this->maxChairsInRow + (groups.count()-1)*CHAIR_WALKWAY + addWidth;
    }
    
    void makeSpace(QString &line)
    {
        auto  yOffset = line.toInt();
        this->posY+= yOffset;
        this->posX=0;
    }
    
    void makeExit(QString &line)
    {
        //        auto  yOffset = line.toInt();
        this->exitCount++;
                
        // TODO: вынести это в разметку лайаута ?
        int exitW = 180;
        int exitH = 120;
        int marginTop = exitH*0.2;
        int marginBottom = exitH*0.2;
        
        // margins        
        if(auto offsStr = takeValueStr('(',')',line); offsStr!=""){
            if(auto marg = parseNamedParams2(offsStr); marg.name=="margin")
            {
                marginTop = marg.param1;
                marginBottom = marg.param2;
            }        
        }
           
        auto exitL = new PlaneItemExit();
        exitL->isLeft = true;
        exitL->title = QString("exit %1 left").arg(this->exitCount);
        exitL->id = QString("exit%1L").arg(this->exitCount);                
        exitL->location = QRect(0, posY+marginTop, exitW, exitH);
        
        auto exitR = new PlaneItemExit();
        exitR->isLeft = false;
        exitR->title = QString("exit %1 right").arg(this->exitCount);
        exitR->id=QString("exit%1R").arg(this->exitCount);
        exitR->location = QRect(airplaneWidth-exitW, posY+marginTop, exitW, exitH);
        
        this->planeItems.append(exitL);
        this->planeItems.append(exitR);
        
        this->posY += exitH+ marginTop + marginBottom;
        this->posX = 0;
    }
    
    
    void makeRowSeatsLine(QString &line, int lineNumber)
    {
//        auto groupId = qHash(line);
        
//        qDebug() << "line: " << lineNumber<< "  " << line;
        auto rowInfo = parseRowSeatsLine(line);
        
        auto chMetric = calcChairLayoutMetric(rowInfo);
   
        for(int r=rowInfo.rowFrom; r<=rowInfo.rowTo; r++)
        {            
            posX=0;
            this->rows=r;
//            qDebug()<< r;
            addRowNumber(r, posY);
                        
            for(int g=0; g<rowInfo.groups.length(); g++){
                auto gr = rowInfo.groups[g];
                
                for(int i=0; i<gr.seats.length(); i++){
                    
                    auto seat = gr.seats[i];
//                    qDebug()<<"seats"<< r << seat.letter ;
                    if(seat.letter!=NONE_CHAIR){  // если символ '0' - то это отсутствие кресла
                        auto chair = new PlaneItemChair();
                        chair->groupId = lineNumber;
                        chair->letter = seat.letter;
                        chair->rowNumber = r;
                        chair->settings = seat.settings;                        
                        chair->id = QString::number(r)+chair->letter;
                        chair->title = QString::number(r)+' '+chair->letter;
                        chair->seatType = rowInfo.seatType;
                        chair->location = QRect(posX + gr.xOffset, posY + gr.yOffset, chMetric.width, chMetric.height);
                        
                        this->planeItems.append(chair);
//                        if(r>=49)
//                            qDebug()<< chair->rowNumber<< chair->letter<<chair->location;
                    }
                    posX += chMetric.width + chMetric.offset;
                }                
                posX += chMetric.walkWay;
            }            
            posY += chMetric.height + chMetric.rowOffset;           
        }
    };    
    
    SeatRowDeclaration parseRowSeatsLine(QString &line)
    {
        SeatRowDeclaration row;
                
        auto parts = line.split(" ");
        
        auto seatTypeParts = parts.takeFirst().split(':');
        row.seatType = seatTypeParts.takeFirst().at(0);
        row.seatSpec = !seatTypeParts.isEmpty()? seatTypeParts.takeFirst():"";
        
        auto fromToList = parts.takeFirst().split('-');
        row.rowFrom = fromToList.takeFirst().toInt();
        row.rowTo = (!fromToList.isEmpty()) ? fromToList.takeFirst().toInt():row.rowFrom;
        
        static QRegularExpression regSeatGroups("(?<letters>[0A-Z\\[\\]_^]+)(?<props>((\\(.*?\\))|{.*?})*)");        
        auto seatGroups = parts.takeFirst();
        auto sg = regSeatGroups.globalMatch(seatGroups);
        
        row.rowSeatCount=0;
        
        while( sg.hasNext() ) {
            auto match = sg.next();
            
            
            auto letters = match.captured("letters").toUpper();
            SeatGroup seatGroup = GetSeatGroup(letters);
                       
            row.rowSeatCount += seatGroup.seats.length();
            
            auto props = match.captured("props");
            if(!props.isEmpty()){
                auto offsets = getValueStr('(',')',props);
                parseOffsets(offsets, seatGroup);
                //0auto style = getValuesFromString('{','}',props);
                //parseStyle(offsets, seatGroup);
            }
           
            row.groups.append(seatGroup);
        }
        return row;
    }
    
    SeatGroup GetSeatGroup(QString letters)
    {
        SeatGroup seatGroup;
        SeatDeclare seat;
        for(auto &c:letters)
        {
            if(c.isSpace()) continue;
            if(c.isLetterOrNumber())
            {
                if(seat.letter.isLetterOrNumber())
                    seatGroup.seats.append(seat);
                
                seat.letter=c;
                seat.settings= ChairSettingsEnum::None;                    
            }
            if(c=='[') seat.settings |= ChairSettingsEnum::LeftArmrest;
            if(c==']') seat.settings |= ChairSettingsEnum::RightArmrest;
            if(c=='_') seat.settings |= ChairSettingsEnum::FixedBack;
            if(c=='^') seat.settings |= ChairSettingsEnum::SpacePlus;
        }
        if(seat.letter.isLetterOrNumber())
            seatGroup.seats.append(seat);
        return seatGroup;
    }
    
    void addRowNumber(int rowNumber, int posY)
    {
        auto rowNumItem = new PlaneItemText();
        auto strNum = (rowNumber<10?"0":"") + QString::number(rowNumber);
        rowNumItem->type="rownum";
        rowNumItem->id = "rownum"+strNum;
        rowNumItem->title = strNum;
        
        // для RowNumber указываем только позицию,
        // размер rect делаем во всю ширину, чтобы при зуме всегда был виден RowNum
        rowNumItem->location = QRect(-80, posY+PlaneLayoutParser::CHAIR_SIZE*0.3, 2000,PlaneLayoutParser::CHAIR_SIZE);
        
        this->planeItems.append(rowNumItem);
    }
    
    void makeObject(const QString &line)
    {
        //Ex: obj WC "Watercloset 1":WC1 (11KA:-250,0) 200x200
        static QRegularExpression regName("\\\"(?<title>[^\"]+)\\\"(:(?<id>\\w+))?");
        static QRegularExpression regSize("(?<width>\\d+)x(?<height>\\d+)");
        
        auto objType = line.section(' ',0,0).toLower();
        auto params = line.section(' ', 1);
        
        PlaneItemBase *item=nullptr;
        if(objType=="wc") item = new PlaneItemWc();
        
        if(item==nullptr){
            qDebug() << "unknown object '" << objType << "' in:" << line;   
            return;
        }
        
        // get object id and name
        auto matchName = regName.match(params);
        if(matchName.hasMatch()){
            item->id = matchName.captured("id");
            item->title = matchName.captured("title");
            params.remove(matchName.capturedStart(), matchName.capturedEnd() - matchName.capturedStart());
        }
        
        // get object location        
        auto offsStr = takeValueStr('(',')',params);
        ItemOffset offset;
        parseOffsets(offsStr, offset);
        
        auto x = 0, y = posY;
        if(offset.refObjId.isEmpty()==false){
            auto refItem = findItem(offset.refObjId);
            if(refItem!=nullptr)
            {
                x = refItem->location.left();
                y = refItem->location.top();
            }
            else {qDebug() << "unknown object " <<offset.refObjId<< "in"<< line;}
        }
        
        // get object size
        int width=10,height=10;
        auto matchSize = regSize.match(params);
        if(matchSize.hasMatch()){
            width = matchSize.captured("width").toInt();
            height = matchSize.captured("height").toInt();
            params.remove(matchName.capturedStart(), matchName.capturedEnd() - matchName.capturedStart());
        }
        
        item->location = QRect(x + offset.xOffset, y + offset.yOffset, width, height);
        
        this->planeItems.append(item);
    }
    
    
    QString getValueStr(QChar symbol1, QChar symbol2, QString &value, bool remove=false){
        int st = value.indexOf(symbol1);
        if(st<0) return "";
        int end = value.indexOf(symbol2, st+1);
        auto prop = value.mid(st+1, end-(st+1));
        if(remove)value.remove(st, end-st+1);
        return prop;
    }
    
    QString takeValueStr(QChar symbol1,QChar symbol2, QString &value){
        return getValueStr(symbol1, symbol2, value, true);
    }
    
  /*  void parseOffsets(QString val, ItemOffset &offset){
        // ex: 12S:+30,-10
        //12S:+30,-10 # +10 # -22,+56 # 22,-56 # 12S # WC1:-12 # WC1
        
        if(val.isEmpty()) return;
        
        static QRegularExpression regOffsets("^((?<refobj>\\w+?)(:|$))?((?<yoff>([+-]?\\d+\\b))(,(?<xoff>([+-]?\\d+\\b)))?)?");
 
        auto m = regOffsets.match(val);
        offset.refObjId= m.captured("refobj");
        offset.yOffset= m.captured("yoff").toInt();
        offset.xOffset= m.captured("xoff").toInt();
        
    }*/
    
    
   
    void parseOffsets(QString val, ItemOffset &offset){
        // ex: 12S:+30,-10
        //12S:+30,-10 # +10 # -22,+56 # 22,-56 # 12S # WC1:-12 # WC1
        
        if(val.isEmpty()) return;
        auto np = parseNamedParams2(val);
         
        offset.refObjId = np.name;
        offset.yOffset = np.param1;
        offset.xOffset = np.param2;
    }
    
    NamedParams parseNamedParams2(QString val){
        //ex: 12S:+30,-10
        //12S:+30,-10 # +10 # -22,+56 # 22,-56 # 12S # WC1:-12 # WC1
        
        if(val.isEmpty()) return NamedParams();
        
        NamedParams np;
        static QRegularExpression regOffsets("^((?<name>\\w+?)(:|$))?((?<p1>([+-]?\\d+\\b))(,(?<p2>([+-]?\\d+\\b)))?)?");
        auto m = regOffsets.match(val);
        np.name = m.captured("name");        
        np.param1 = m.captured("p1").toInt();
        np.param2 = m.captured("p2").toInt();
        return np;
    }
   
    QString clearLine(QString &line){        
        static QRegularExpression regClear("\\s*(?<d>[:\\[(,+-])\\s*");
        
        auto commentPos = line.indexOf("##");
        if(commentPos>=0) line.truncate(commentPos);
        auto res  = line.replace(regClear, "\\1").trimmed();        
        return res;
    }
    
    QSize getPlaneDimensions()
    {
        int maxW=0, maxH=0;
        for(int i=0; i< planeItems.length();i++)
        {
            auto item = planeItems[i];
            
            // не учитываем элемент с номером ряда, т.к. он сильно растянут, чтобы работал ZOOM
            if(item->type=="rownum") continue;
//            if(item->location.right()> maxW)
//                qDebug()<<"max "<< item->type<< item->id<< item->title<< item->location;
            maxW = __max(item->location.right(), maxW);
            maxH = __max(item->location.bottom(), maxH);
        }
        return QSize(maxW,maxH);
    }
};

#endif // PLANELAYOUTPARSER_H
