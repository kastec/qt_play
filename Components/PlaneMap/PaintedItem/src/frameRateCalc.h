
#ifndef FRAMERATECALC_H
#define FRAMERATECALC_H

#include "qdatetime.h"
#include <QObject>
#include <QElapsedTimer>

class FrameRateCalc
{
  public:
    int frameMsec;
    int avgFrameMsec;
    
  private:
    const int numberFrames=10;
    QDateTime startRenderTime;
    QList<int> timeValues;
    
    QElapsedTimer etimer;
    
  public:
    void start()
    {
        etimer.start();
    }
    
    int stop(){                
       frameMsec =  etimer.elapsed(); 
        
        timeValues.push_back(frameMsec);
        if(timeValues.length()>numberFrames)
            timeValues.pop_front();
        
        avgFrameMsec = getAvgRenderTime();        
        return frameMsec;
    }
    
  private:
    int getAvgRenderTime()
    {
        int sum = 0;
        for(int i = 0; i < timeValues.length(); i++)
            sum += timeValues[i];
        return sum / timeValues.length();    
    }
};

#endif // FRAMERATECALC_H
