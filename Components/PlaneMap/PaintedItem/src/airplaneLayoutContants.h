
#ifndef AIRPLANELAYOUTCONTANTS_H
#define AIRPLANELAYOUTCONTANTS_H

#include <QObject>

namespace AirplaneLayoutConstants {

static const int CHAIR_SIZE=100;
static const int CHAIR_OFFSET=CHAIR_SIZE*0.04;
static const int CHAIR_ROW_OFFSET=CHAIR_SIZE*0.4;
static const int CHAIR_ROW_OFFSET_BUSS=CHAIR_SIZE*0.2;
static const int CHAIR_WALKWAY=CHAIR_SIZE*0.5;

       // дополнительно непорпорционально растянуть Nav по вертикали
constexpr static const qreal NAV_SPREAD_ROWS=1.25;

static const QChar NONE_CHAIR='0';
}

#endif // AIRPLANELAYOUTCONTANTS_H
