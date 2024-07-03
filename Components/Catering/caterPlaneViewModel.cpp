
#include "caterPlaneViewModel.h"
#include "Utils/JsonHelper/FileHelper.h"
#include "Utils/boolinq.h"
#include "Components/PlaneMap/PaintedItem/src/planeLayoutParser.h"
#include "Components/PlaneMap/PaintedItem/src/planeItems/planeItemBase.h"

CaterPlaneViewModel::CaterPlaneViewModel(QObject *parent): QObject{parent}
{
    auto q1 = new CaterSeatViewModel(this);
    q1->x=0;
    q1->y=0;
    q1->name="q1";
    
    auto q2 = new CaterSeatViewModel(this);
    q2->x=320;
    q2->y=120;
    q2->name="q2";
    
    items.append(q1);
    items.append(q2);
}



void CaterPlaneViewModel::loadLayout()
{
    auto type = "73H";
    auto filepath = QString("D:\\0\\airplanes\\%1.airplane").arg(type);
    if(FileHelper::existFile(filepath)==false) return;
    auto data = FileHelper::readAll(filepath);
    QString planeStr = QString(data);
    QList<QString>  lines = planeStr.split("\n");
    
    PlaneLayoutParser layoutParser;
    auto items = layoutParser.parse(lines);
    auto layoutSize = layoutParser.getPlaneSize();
    
    auto bussSeats =  boolinq::from(items).where( [](PlaneItemBase *i){return i->type=="seat";})
                         .cast<PlaneItemChair *>()
                         .where([](PlaneItemChair * i){return i->seatType=='J' || i->seatType=='F';}) // бизнес или первый класс
                         .toQList();
    
    auto maxW = boolinq::from(bussSeats).select([](PlaneItemChair * i){return i->location.right();}).max();
    auto maxH = boolinq::from(bussSeats).select([](PlaneItemChair * i){return i->location.bottom();}).max();
    
    this->layoutSize = QSize(maxW, maxH);
           // TODO:
    // map bussSeats -> internl onjecta
    // qDeleteAll(items);
}

