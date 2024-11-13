
#ifndef PLANELAYOUTPARSER_H
#define PLANELAYOUTPARSER_H

#include <QRegularExpression>
#include <QObject>
#include "planeItems/planeItemChair.h"
#include "planeItems/planeItemBase.h"
#include "planeItems/planeItemExit.h"
#include "planeItems/planeItemText.h"
#include "planeItems/planeItemLine.h"
#include "planeItems/planeItemRowNumber.h"
#include "airplaneLayoutContants.h"

using namespace AirplaneLayoutConstants;

class PlaneLayoutParser{
    
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
        
        // для красоты сделаем отступ сверху на 50 пиклелей
        this->posY = 50;
        
        for( auto &currLine: lines)
        {            
            lineNum++;
            auto cline = currLine.simplified().trimmed();
            if(cline.isEmpty() || cline.startsWith("#")) continue;
            
            auto lineType = cline.section(' ',0,0).toLower();
            cline = cline.section(' ', 1);            
            auto line = clearLine(cline);          
                       
            // qDebug() << "lineType:" << lineType << line;
            
            if(lineType == "name") qDebug()<< "  layout "<< line;
            if(lineType == "plane_width") setMaxPlaneWidth(line);
            if(lineType == "seat_scale") makeChairSizes(line);
            if(lineType == "row") makeRowSeatsLine(line, lineNum);
            if(lineType == "space") makeSpace(line);
            if(lineType == "exit") makeExit(line);
            if(lineType == "obj") makeObject(line);
            if(lineType == "text") makeText(line);
            if(lineType == "lineh") makeLineH(line);  
        }
        // для красоты сделаем отступ снизу на 50 пиклелей
        this->posY += 50;
    }
    
    SeatRowMetric calcChairLayoutMetric(const SeatRowDeclaration &row)
    {        
        qreal scale = chairScales.value(row.seatType, 1.0);
        
        int w = CHAIR_SIZE*scale, h=w, offs = CHAIR_OFFSET*scale;
        int rowOffs = (row.seatType=='J' ? CHAIR_ROW_OFFSET_BUSS : CHAIR_ROW_OFFSET)* scale;
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
        
        this->airplaneWidth = (AirplaneLayoutConstants::CHAIR_SIZE+AirplaneLayoutConstants::CHAIR_OFFSET)*this->maxChairsInRow + (groups.count()-1)*CHAIR_WALKWAY + addWidth;
    }
    
    void makeSpace(QString &line)
    {
        auto yOffset = line.trimmed().toInt();
        this->posY+= yOffset;
        this->posX=0;
    }
    
    void makeExit(QString &line)
    {
        //        auto  yOffset = line.toInt();
        this->exitCount++;
        bool isShort = line.contains("short");
        
        int exitW = (!isShort)? 180 : 80;
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
        exitL->isShort = isShort;
        exitL->title = QString("exit %1 left").arg(this->exitCount);
        exitL->id = QString("exit%1L").arg(this->exitCount);                
        exitL->location = QRect(0, posY+marginTop, exitW, exitH);
        
        auto exitR = new PlaneItemExit();
        exitR->isLeft = false;
        exitR->isShort = isShort;
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
                    if(seat.letter!=AirplaneLayoutConstants::NONE_CHAIR){  // если символ '0' - то это отсутствие кресла
                        auto chair = new PlaneItemChair();
                        chair->groupId = lineNumber;
                        chair->rowGroupId = g;
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
        auto rowNumItem = new PlaneItemRowNumber();
        auto strNum = (rowNumber<10?"0":"") + QString::number(rowNumber);
        rowNumItem->id = "rownum" + strNum;
        rowNumItem->title = strNum;
        
        // для RowNumber указываем только позицию,
        // размер rect делаем во всю ширину, чтобы при зуме всегда был виден RowNum
        rowNumItem->location = QRect(-80, posY+CHAIR_SIZE*0.3, 2000, CHAIR_SIZE);
        
        this->planeItems.append(rowNumItem);
    }
    
    std::pair<QString,QString> parseObjName(QString &line)
    {
        //Ex: obj WC "Watercloset 1":WC1 (11KA:-250,0) 200x200
        //Ex: "Watercloset 1":WC1
        //Ex: "Watercloset 1
        static QRegularExpression regPattern("\\\"(?<title>[^\"]+)\\\"(:(?<id>\\w+))?");
        
        // get object id and name
        auto regMmatch = regPattern.match(line);
        if(!regMmatch.hasMatch()) return std::make_pair("","");
        
        auto id = regMmatch.captured("id");
        auto title = regMmatch.captured("title");
        line.remove(regMmatch.capturedStart(), regMmatch.capturedEnd() - regMmatch.capturedStart());
//        qDebug() << "parse obj-name: "<< line;
//        qDebug() << "   id: "<< id;
//        qDebug() << "   title: "<< title;
        return std::make_pair(id,title);
    }
    
    std::pair<int,int> parseSize(QString &line)
    {
        //Ex: 200x200
        static QRegularExpression regPattern("(?<width>\\d+)x(?<height>\\d+)");
        
        // get object id and name
        auto regMatch = regPattern.match(line);
        if(!regMatch.hasMatch()) return std::make_pair(0,0);
        
        int width = regMatch.captured("width").toInt();
        int height = regMatch.captured("height").toInt();
        line.remove(regMatch.capturedStart(), regMatch.capturedEnd() - regMatch.capturedStart());
        return std::make_pair(width,height);
    }
    
    QColor parseColor(QString &line)
    {
        //Ex: #AA00FF
        static QRegularExpression regPattern("(?<color>#[0-9A-F]{6})");
        
//        qDebug()<< "parse color:" <<  line;
        // get object id and name
        auto regMatch = regPattern.match(line);
        if(!regMatch.hasMatch()) return Qt::black; // QColor::black();
        
        
        QString color = regMatch.captured("color");
//        qDebug()<< "   s-color:" <<  color;
        bool ok;
        int r = color.mid(1,2).toUInt(&ok, 16);
        int g = color.mid(3,2).toUInt(&ok, 16);
        int b = color.mid(5,2).toUInt(&ok, 16);
        line.remove(regMatch.capturedStart(), regMatch.capturedEnd() - regMatch.capturedStart());
        
        return QColor(r,g,b);
    }
    
    int parsePointSize(QString &line)
    {
        //Ex: 15pt
        static QRegularExpression regPattern("(?<pointsize>[0-9]{1,2})pt");
        
        //        qDebug()<< "parse color:" <<  line;
        // get object id and name
        auto regMatch = regPattern.match(line);
        if(!regMatch.hasMatch()) return 0;        
        
        QString sPSize = regMatch.captured("pointsize");
        //qDebug()<< "   point-size:" <<  sPSize;
        bool ok;
        int pointSize = sPSize.toUInt();
//        qDebug()<< "   point-size:" <<  pointSize;

        line.remove(regMatch.capturedStart(), regMatch.capturedEnd() - regMatch.capturedStart());
        
        return pointSize;
    }
    std::pair<int,int> parseLocation(QString &line)
    {
        // Ex: (11KA:-250,0)
        // Ex: (-250,0)
        // Ex: (250)
        //Ex: ([reference object?:] offY, offX)
        
//        qDebug() << "parce location" << line;
        
        auto offsStr = takeValueStr('(',')', line);
        if(offsStr.isEmpty()) return  std::pair(0, this->posY);
            
        ItemOffset offset;
        parseOffsets(offsStr, offset);
        
        auto x = 0, y = posY;
        
        if(!offset.refObjId.isEmpty()){
            if(offset.refObjId[0]=='0') offset.refObjId = offset.refObjId.mid(1);
                
            auto refItem = findItem(offset.refObjId);
            if(refItem!=nullptr)
            {
                x = refItem->location.left();
                y = refItem->location.top();
            }
            else {qDebug() << "unknown object " <<offset.refObjId<< "in"<< line;}
        }
       
        return  std::pair(x + offset.xOffset, y + offset.yOffset);
    }
    
    void makeObject(const QString &line)
    {
        //Ex: obj WC "Watercloset 1":WC1 (11KA:-250,0) 200x200
       
        auto objType = line.section(' ',0,0).toLower();
        auto params = line.section(' ', 1);
        
        PlaneItemBase *item=nullptr;
        if(objType=="wc") item = new PlaneItemWc();
        
        if(item==nullptr){
            qDebug() << "unknown object '" << objType << "' in:" << line;   
            return;
        }
        
        // get object id and name
        auto [id, name] = parseObjName(params);
        item->id = id;
        item->title = name;
        
        // get object location        
        auto [x, y] = parseLocation(params);
        auto [width, height] = parseSize(params);
        item->location = QRect(x, y, width, height);
        
        this->planeItems.append(item);
    }
    
    void makeText(const QString &line)
    {
        //Ex: text "hello world1":txtHW (11KA:-250,0) 20 #FF00AA
        //Ex: text "hello world1" (-40) 10
        //Ex: text [text]:[id] (OffY, OffX) [FontSize] [color]
        
        auto params = line;        
        auto item = new PlaneItemText();
        
        auto [id, title] = parseObjName(params);
        item->id = id;
        item->title = title;
        
        // get object location  
        auto [x,y] = parseLocation(params);
 
        // get text color
        item->color = parseColor(params);
        
        // размер фонта
        int fontsize = parsePointSize(params);
        item->fontSize = fontsize > 0 ? fontsize : item->fontSize;
        
        // расчитаем место под текст
        QFont font("Inter", item->fontSize);
        QFontMetrics fm(font);
        int width = fm.horizontalAdvance(item->title);
        int height = fm.height();
        
        item->location = QRect(x, y, width * 1.03, height);
        
        this->planeItems.append(item);        
    }
    
    void makeLineH(const QString &line)
    {
        qDebug() << "parse-line:" << line;
        //Ex: lineH (04D: 150,50) 300 #555555 15pt
        //Ex: lineH (margin) length [color] [width]
       
        auto params = line;        
        auto item = new PlaneItemLine();
        
        // get object location  
        auto [x,y] = parseLocation(params);
                
        item->color = parseColor(params);
        // размер ширины
        int width = parsePointSize(params);
        item->width = width > 0 ? width : item->width;
        
        // оставшийся параметр - длинна линии        
        auto length = params.trimmed().toInt();
        
        item->location = QRect(QPoint(x, y), QSize(length , width)); // ширина линии - это высота у Rect
        
//        this->posY+= width + 1;  ? надо ли ?
//        this->posX=0;
       
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
        // В качестве размера по вертикали возьмем последний this->posY
        // а вот max(posX) надо найти
        int maxW=0, maxH=this->posY;
        for(int i=0; i< planeItems.length();i++)
        {
            auto item = planeItems[i];
            
            // не учитываем элемент с номером ряда, т.к. он сильно растянут, чтобы работал ZOOM
            if(item->type=="rownum") continue;
//            if(item->location.right()> maxW)
//                qDebug()<<"max "<< item->type<< item->id<< item->title<< item->location;
            maxW = __max(item->location.right(), maxW);
//            maxH = __max(item->location.bottom(), maxH);
        }
        
       return QSize(maxW,maxH);
    }
};

#endif // PLANELAYOUTPARSER_H
