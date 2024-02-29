
#include <QObject>
#include <QPainter>


#include "airplaneViewModel.h"
#include "./src/cardServiceType.h"
//#include "qdebug.h"

AirplaneViewModel::AirplaneViewModel(QObject *parent): QObject(parent)
{
//    qDebug() << "AirplaneViewModel ctor";
    this->set_planeMap(new PlaneMap());
//    qDebug() << "AirplaneViewModel ctor - ex";
}

//void AirplaneViewModel::changeRenderSize(QSize size){
//    planeMap->setScreenSize(size);
//}


/*
void PlaneMap::draw(QPainter *painter, int xOff, int yOff, qreal zoom)
{
    QPoint offset(xOff, yOff);
    QRect scrViewPort (-offset / zoom, screenSize / zoom);
    
    bool useBuffer=false;
    if(useBuffer)
        drawBuffer(painter, offset, zoom, scrViewPort);
    else
        drawItems(painter, offset,  zoom, scrViewPort);
}*/


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
    
    QList<int> paxs;
    setPassengers(paxs);
}

void AirplaneViewModel::setPassengers(const QList<int> &passengers)
//void AirplaneViewModel::setPassengers(QList<Passenger> &passengers)
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
   qDebug() <<"painter:" << name << paintArea;
   qDebug() <<"painter:" <<  paintArea->objectName();
   this->airplanePainter = paintArea;
   paintArea->onPaint = [this](QPainter *p){this->drawAirplaneLayout(p);};
   
//   paintArea->update();
}


void AirplaneViewModel::moveBy(qreal xOff, qreal yOff) {
   qreal ox, oy;
  //qDebug() << xOff <<","<<yOff;
   
   ox = position.x() - xOff;
   oy = position.y() - yOff;
   //    if (ox < 0) ox = 0;
   //    if (oy < 0) oy = 0;
   
//   this->set_position(QPoint(ox,oy));
   position = QPoint(ox,oy);
   updatePaintArea();
}

bool AirplaneViewModel::zoomBy(qreal zoomFactor, qreal centerX, qreal centerY)
{
//   auto newZoom = zoom + zoom * zoomFactor;
//   if(newZoom>maxZoom || newZoom<minZoom) return false;
//   zoom = newZoom;
   
//   position -= QPoint( (centerX-position.x())*zoomFactor , (centerY-position.y())*zoomFactor);
   
//   update();
   return true;
}


void AirplaneViewModel::updatePaintArea()
{
   if(airplanePainter != nullptr)
       airplanePainter->update();
}
    
void AirplaneViewModel::drawAirplaneLayout(QPainter *painter)
{
   qDebug() <<"called DrawAirplaneLayout():" << painter;
   // PLANE RENDER
   int xpos = position.x(), ypos=position.y();
   if(planeMap!=nullptr)
       planeMap->draw(painter, xpos, ypos, 1.0);
   
}
AirplaneViewModel::~AirplaneViewModel()
{
    if(planeMap!=nullptr)
    {
        delete planeMap;
        planeMap=nullptr;        
    }
}
