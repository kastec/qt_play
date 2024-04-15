
#include "appMessageBus.h"


SINGLETON_PATTERN_IMPLIMENT(AppMessageBus)


AppMessageBus::AppMessageBus(QObject *parent) : QObject{parent}
{
    
}

