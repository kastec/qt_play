
#include <QObject>
#include <QPainter>


#include "Utils/JsonHelper/FileHelper.h"
#include "airplaneViewModel.h"
#include "src/planeItems/cardServiceType.h"
#include "qguiapplication.h"

//TODO: remove, нужна только константа
#include "src/airplaneLayoutContants.h"
//#include "qdebug.h"

AirplaneViewModel::AirplaneViewModel(QObject *parent): QObject(parent),
                                                        airplanePainter(nullptr), navigationPainter(nullptr), zoom(1.0)
{
//    this->isFixedZoom = true;
    this->devicePixelRatio = qApp->devicePixelRatio();
    
    this->set_planeMap(new PlaneMap());
}



void AirplaneViewModel::loadLayout()
{
//    auto type = "359"; //A359-28-24-264
//    auto type = "320"; // A320-20-120
//    auto type = "32A"; //A320-8-150
//    auto type = "32N"; //A320-12-144
//    auto type = "321"; //A321-28-142
//    auto type = "32B"; //A321-16-167
 //   auto type = "32Q"; // A321-12-184
 //   auto type = "3KR"; // A333-28-268
 //     auto type = "333"; // A333-36-265
//     auto type = "359-28-288"; // A350-28-288
//   auto type = "73H"; // B737-20-138
 //   auto type = "77R"; // B777-28-24-375
    //    auto type = "77W"; // B777-30-48-324
   //     auto type = "SU9"; // SU95-12-75
       
//       auto type = "A319"; // ROS
//       auto type = "A320"; // ROS
//       auto type = "A320V2"; // ROS
//       auto type = "B737V4"; // ROS
//    auto type = "B737-900"; // ROS
//    auto type = "B777ER"; // ROS
//     auto type = "SU9"; // ROS
     //auto type = "Y100"; // ROS
//    auto type = "B744A"; // ROS
//    auto type = "B744C-Test"; // ROS
    auto type = "B744C"; // ROS
    
    auto filepath = QString("D:\\0\\airplanes\\%1.airplane").arg(type);
    if(FileHelper::existFile(filepath)==false) return;
    qDebug() << "loading airplane layout" << type;
    auto data = FileHelper::readAll(filepath);
    QString planeStr = QString(data);
    QList<QString>  lines = planeStr.split("\n");
  
   /*
    QList<QString> lines {"NAME 777-300ER",
                         "PLANE_WIDTH  3,4,3",
                         "SEAT_SCALE W:1.15 J:1.7   ## scales chairs in percent",
                         "",
//                         "exit",
                         "row J  1-5  AC, DG, HK",
                         
//                        "exit",
                         // "row Y 37 0,DEFG,0",
                         
                         "space 350",
                         
                         "row W  11-16  A_]C[^, D^E^F^G_ (+30), HK",
                         
                         "space 50",
                         "row Y 17 AC, DEFG (+50), HK",
                         "row Y 18-23 ABC, DEFG (+50), HJK",
                         "space 400",
                         
                         "obj WC  \"WC\":WC1 (11K:-250,-100) 200x200",
                         "obj WC  \"WC 1\" (23A:150,0) 250x200",
                         "obj WC  \"WC 2\" (23D:130,0) 200x180",
                         "obj WC  \"WC 3\" (23F:130,0) 200x180",
                         "obj WC  \"WC 4\" (23J:150,-50) 250x200",
                         
                         
                         "row Y 24-36 ABC, DEFG (-50), HJK",
                         "row Y 37 000,DEFG(-50),000",
                         
                         "exit (margin:-70,50)",
//                          "exit",
//                         "space 200",                         
//                         "obj WC  \"WC 5\" (36A:130,0) 250x150",
//                         "obj WC  \"WC 6\":WC6 (36H:130,50) 250x150",
                         
                         "row Y 38 ABC,0, HJK",
                         "row Y 39-46 ABC, DEFG, HJK",
                         
                       
                         
                         "row Y 47 AB(0,15), DEFG, HK(0,-15)",
                         "row Y 48 AB(0,30), DEFG, HK(0,-30)",
                         "row Y 49 AB(0,45), DEFG, HK(0,-45)",
                         "row Y 50 AB(0,60), DEFG, HK(0,-60)",
                         "row Y 51 0, DEFG, 0",
                         "",""};
    */
    planeMap->createLayout(lines);
  
    this->changeZoomLimits();
    
    QList<int> paxs;
    setPassengers(paxs);
    
    if(this->airplanePainter)this->airplanePainter->update();
    if(this->navigationPainter)this->navigationPainter->update();
}


void AirplaneViewModel::setPassengers(const QList<int> &passengers)
{
    auto searcher = planeMap->planeSearcher;
    
 
    auto s1 = searcher.findChair("1C");
    if(s1){
        s1->hasPassenger=true;
        s1->title="KOZLOV ALEXANDER SERGEEVICH";
        QString t1 = "Gold";
        s1->cardType = CardServiceType::getCardType(t1);
        s1->isSelected = true;
    }
    
    auto s3d = searcher.findChair("3D");
    if(s3d){
        s3d->hasPassenger=true;
        s3d->title="KOZLOV ALEXANDER SERGEEVICH";
        QString t1 = "silver";
        s3d->cardType = CardServiceType::getCardType(t1);
        s3d->isSelected = true;
    }
    
  auto s2 = searcher.findChair("2A");
    if(s2){
        s2->hasPassenger=true;
        s2->title="Kozlov Alexander";
        QString t2 = "silver";
        s2->cardType = CardServiceType::getCardType(t2);
        s2->isSelected = true;
    }
    
    auto s3 = searcher.findChair("11C");
    if(s3){
        s3->hasPassenger=true;
        s3->title="KOZLOV ALEXANDER SERGEEVICH";
        QString t3 = "Platinum";
        s3->cardType = CardServiceType::getCardType(t3);
        s3->isSelected = true;
    }
    
    auto s4 = searcher.findChair("12A");
    if(s4)    {
        s4->hasPassenger=true;
        s4->title="KOZLOV ALEXANDER SERGEEVICH";
        QString t4 = "silver";
        s4->cardType = CardServiceType::getCardType(t4);
        s4->isSelected = false;
    }
   
//    for(auto &p: passengers){
//        if(p->seatNumber.isEmpty) continue;
//        if(PlaneItemChair* seat = searcher.findChair(p->seatNumber); seat!=nullptr)
//        {
//          if(seat.seatType=='J' || seat.seatType=='F')
//             seat->title = p->lastName + " " + p->firstName + " " + p->middleName;
//          seat->cardType = GetCardTypeStarsCount(p->cardType);
//    seat.seatSelection = SeatSelectionType::Select;
//        }
//    }
}


void AirplaneViewModel::setSelections(const QList<QString> &selectedSeats){
   planeMap->planeSearcher.setSelections(selectedSeats);
}

void AirplaneViewModel::addPainter(QString name, PaintArea *paintArea)
{   
   if(paintArea == nullptr){
       qDebug() <<"ERROR: paint area" << name << "is NULL";
       return;       
   }
       
   if(name == "airplane"){
       this->airplanePainter = paintArea;
       paintArea->onPaint = [this](QPainter *p){this->drawAirplaneLayout(p);};
   }
   
   if(name == "navigation"){
       this->navigationPainter = paintArea;
       paintArea->onPaint = [this](QPainter *p){this->drawNavigation(p);};
   }
}


void AirplaneViewModel::moveBy(qreal xOff, qreal yOff) {
   qreal ox, oy;
  //qDebug() << xOff <<","<<yOff;
   if( this->isFixedZoom) xOff=0;
   
   ox = position.x() - xOff;   
   oy = position.y() - yOff;
   
   //    if (ox < 0) ox = 0;
   if (oy > 0) oy = 0;
//   qDebug()<< oy;
//   
   auto pos = QPoint(ox,oy);
   pos = validateYRestriction(pos);
   
    this->set_position(pos);  
//   updatePaintArea();
}

bool AirplaneViewModel::zoomBy(qreal zoomFactor, qreal centerX, qreal centerY)
{    
    auto newZoom = zoom + zoom * zoomFactor;
    if(newZoom>maxZoom || newZoom<minZoom)
       return false;
    
    zoom = newZoom;
    
    position -= QPoint( (centerX-position.x())*zoomFactor , (centerY-position.y())*zoomFactor);
    
    updatePaintArea();
    return true;
}

void AirplaneViewModel::wheelBy(qreal wheelFactor, qreal centerX, qreal centerY)
{
    if(!this->isFixedZoom)
       zoomBy(wheelFactor / 120.0 / 10.0, centerX, centerY);
    else
       moveBy(0,-wheelFactor/1.5);
}

// --- Props
bool AirplaneViewModel::onPositionChanging(QPoint newPos){
   position = newPos;   
   updatePaintArea();
   return true;
}

bool AirplaneViewModel::onZoomChanging(qreal newZoom){        
   auto zoomFactor = (newZoom-zoom)/zoom;
   bool res = zoomBy(zoomFactor,screenSize.width()/2, screenSize.height()/2);
   return res; 
}

//---------

void AirplaneViewModel::changeVisibleSize(QSize size)
{
   this->screenSize = size;
   this->changeZoomLimits();  
}


void AirplaneViewModel::changeZoomLimits()
{
   auto planeWidth = planeMap->layoutSize.width();
   auto screenWidth = this->screenSize.width();
   
   if(!isFixedZoom){
       maxZoom = screenWidth / (qreal)(planeWidth/4);
       minZoom = screenWidth / (qreal)(4*planeWidth);       
   }
   
   else{
       auto oldZoom = zoom;
       maxZoom = screenWidth / (qreal)(planeWidth*1.1); 
       minZoom = maxZoom;  
       zoom = minZoom;
       // center
       
       auto p = QPoint(planeWidth*0.1*zoom, position.y()* (zoom/oldZoom));
       this->set_position(p);
   }
}


QPoint AirplaneViewModel::getMoveToCenterAt(QString id, qreal viewZoom)
{
   if(planeMap==nullptr) return QPoint(0,0);
   
   //    auto item = planeMap->findItem(id);
   auto item = planeMap->findChair(id);
   if(item == nullptr)
       return position;
   
   if(viewZoom==0.0) viewZoom=this->zoom;
   
   auto itemCenter = item->location.center();

   auto itemOnScr = (itemCenter * viewZoom);
   
   
   QPoint center(screenSize.width()/2,screenSize.height()/2);
   auto tlWin = itemOnScr - center;
   auto newPosition = -tlWin;
   
   if(this->isFixedZoom)
       newPosition.setX(this->position.x());
   
   return newPosition;
} 

QString AirplaneViewModel::getIdAt(int x, int y){
   QPoint p( (x - position.x())/ zoom, (y-position.y())/ zoom );
   auto item = planeMap->findItem(p);
   return (item!=nullptr)? item->id : "";
}



qreal AirplaneViewModel::getNavMapScale(){
   QSize srcRenderSize = navigationPainter->size().toSize();
   auto navMapScale = std::max(
       planeMap->layoutSize.width() /(qreal)srcRenderSize.width(),
       (planeMap->layoutSize.height() *AirplaneLayoutConstants::NAV_SPREAD_ROWS)/(qreal)srcRenderSize.height());
   return navMapScale;
}

QPoint AirplaneViewModel::validateYRestriction(QPoint pos)
{
   if(pos.y()>0)
       pos.setY(0);
  
   auto lowPoint =   (-pos.y() + airplanePainter->height()) / zoom;
  
   if(lowPoint >= planeMap->layoutSize.height())
   {
       auto lowPointY = airplanePainter->height() - planeMap->layoutSize.height() * zoom;
       pos.setY(lowPointY); 
   }
   return pos;
}

bool AirplaneViewModel::setNavPos(qreal x, qreal y)
{
   auto [navViewPortRect,navAirplaneRect] = planeMap->getNavPaintSizes();

   // 1 если не попали в облать отрисовки NavMap - ничего не реагируем
   if(!navAirplaneRect.contains(x,y)) return false;
       
   // 2 если попали в облать отрисовки ViewPort - ничего смещам, ловим только перетаскивание
   if(navViewPortRect.contains(x,y)) return true;
   
   // 3 если попали в область отрисовки, но не в ViewPort - центруем ViewPort на место клика
   auto scale = getNavMapScale();  
   auto scrSize = QSize(airplanePainter->width(), airplanePainter->height());
   
   QPoint centerOffs = QPoint(scrSize.width()/2, scrSize.height()/2);
   auto xn =  x-navAirplaneRect.x();
   auto yn = (y-navAirplaneRect.y()) / AirplaneLayoutConstants::NAV_SPREAD_ROWS;
   QPoint airplamePoint = +centerOffs  -QPoint(xn, yn) * scale *zoom;
   
   if(this->isFixedZoom)
       airplamePoint.setX(position.x());
   
   airplamePoint = validateYRestriction(airplamePoint);

   this->set_position(airplamePoint);
   return true;
}


void AirplaneViewModel::moveNavBy(qreal xOff, qreal yOff)
{
   if(this->isFixedZoom) xOff=0;
   
   auto scale = getNavMapScale();
   
   auto spreadRows = AirplaneLayoutConstants::NAV_SPREAD_ROWS;
   QPointF move = QPointF(xOff,yOff) / spreadRows * scale* zoom;
   QPointF newPos = position + move;
   
   newPos = validateYRestriction(newPos.toPoint());

   this->set_position(newPos.toPoint());   
}


/// ============================================
///  DRAWING AIRPLANE AND NAVIGATION
/// ============================================

void AirplaneViewModel::updatePaintArea()
{
   if(airplanePainter != nullptr)
       airplanePainter->update();
   
   if(navigationPainter != nullptr)
       navigationPainter->update();
}

QRect AirplaneViewModel::getSrcViewPort()
{
   if(airplanePainter==nullptr) return QRect();
   QSize srcRenderSize = airplanePainter->size().toSize();// / this->devicePixelRatio;
   QRect scrViewPort (-position / zoom, srcRenderSize / zoom);
   return scrViewPort;
}

void AirplaneViewModel::drawAirplaneLayout(QPainter *painter)
{
   auto srcViewPort = getSrcViewPort();
   
   framerate.start();
   
//   this->screenSize = painter->viewport().size() / this->devicePixelRatio;
//   QRect srcViewPort (-position / zoom, screenSize / zoom);
   
//   qDebug()<<"2. drawAirplaneLayout()------";
//   qDebug()<<"airRect:"<<painter->viewport().size();
//   qDebug()<<"srcRenderSize:"<<this->screenSize;
//   qDebug()<<"scrViewPort:"<<srcViewPort;
   
   // PLANE RENDER
//   qDebug() <<"called MAP-Layout():" << scrViewPort << "pos:"<< position;
   
   planeMap->drawLayout(painter, zoom, srcViewPort);
   
   framerate.stop();
   
   
   this->set_avgRenderTime(framerate.avgFrameMsec);
   this->set_renderTime(framerate.frameMsec);
}

void AirplaneViewModel::drawNavigation(QPainter *painter)
{
   auto srcViewPort = getSrcViewPort();
//   this->screenSize = painter->viewport().size() / this->devicePixelRatio;
//   QRect scrViewPort (-position / zoom, screenSize / zoom);
   
//   qDebug() <<"called NAV-Layout():" << position;
   planeMap->drawNavMap(painter, srcViewPort);   
}



AirplaneViewModel::~AirplaneViewModel()
{
    if(planeMap!=nullptr)
    {
        delete planeMap;
        planeMap=nullptr;        
    }
}
