
#include <QObject>
#include <QPainter>


#include "airplaneViewModel.h"
#include "./src/cardServiceType.h"
#include "qguiapplication.h"
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
       
    QList<QString> lines {"NAME 777-300ER",
                         "PLANE_WIDTH  3,4,3",
                         "SEAT_SCALE W:1.15 J:1.7   ## scales chairs in percent",
                         "",
//                         "exit",
                         "row J  1-5  AC, DG, HK",
                         
//                        "exit",
                         // "row Y 37 0,DEFG,0",
                         
                         "space 350",
                         
                         "row W  11-16  AC, DEFG (+30), HK",
                         
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
                         
                         "exit (margin:-70,30)",
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
    
    planeMap->createLayout(lines);
    this->changeZoomLimits();
    
    QList<int> paxs;
    setPassengers(paxs);    
}

void AirplaneViewModel::setPassengers(const QList<int> &passengers)
{
    auto searcher = planeMap->planeSearcher;
    
    
    auto s1 = searcher.findChair("1C");
    s1->hasPassenger=true;
    s1->title="KOZLOV ALEXANDER SERGEEVICH";
    QString t1 = "silver";
    s1->cardType = CardServiceType::getCardType(t1);
    s1->isSelected = true;
    
    auto s2 = searcher.findChair("2A");
    s2->hasPassenger=true;
    s2->title="Kozlov Alexander";
    QString t2 = "Gold";
    s2->cardType = CardServiceType::getCardType(t2);
    s2->isSelected = false;
    
  
    auto s3 = searcher.findChair("11C");
    s3->hasPassenger=true;
    s3->title="KOZLOV ALEXANDER SERGEEVICH";
    QString t3 = "Platinum";
    s3->cardType = CardServiceType::getCardType(t3);
    s3->isSelected = true;
    
    
    auto s4 = searcher.findChair("12A");
    s4->hasPassenger=true;
    s4->title="KOZLOV ALEXANDER SERGEEVICH";
    QString t4 = "silver";
    s3->cardType = CardServiceType::getCardType(t4);
    s4->isSelected = false;
    
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
   //    if (oy < 0) oy = 0;
 
    this->set_position(QPoint(ox,oy));  
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
   auto navMapScale = __max(planeMap->layoutSize.width()/(qreal)srcRenderSize.width(),
                            planeMap->layoutSize.height()/(qreal)srcRenderSize.height());
   return navMapScale;
}

void AirplaneViewModel::setNavPos(qreal x, qreal y) {

   if(planeMap->navViewRect.contains(x,y)) return;

   auto scale = getNavMapScale();  
   auto scrSize = airplanePainter->size().toSize();

   
   QPoint centerOffs = QPoint(scrSize.width()/2, scrSize.height()/2);
   
   QPoint mapPoint = +centerOffs  -QPoint(x,y) * scale *zoom;
   
   if(this->isFixedZoom)
       mapPoint.setX(position.x());

   this->set_position(mapPoint);
}

void AirplaneViewModel::moveNavBy(qreal xOff, qreal yOff)
{
   if(this->isFixedZoom) xOff=0;
   
   auto scale = getNavMapScale();
   QPointF move = QPointF(xOff,yOff)* scale* zoom;
//   qDebug()<< "   move:" << move;
   QPointF newPos = position + move;
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
