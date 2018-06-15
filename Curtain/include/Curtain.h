#ifndef _CURTAIN_H_  
#define _CURTAIN_H_  
#include "CurtainGrain.h"  
#include "cv.h"  
#include "highgui.h"  
#include <string.h>  
  
class Curtain  
{  
public:  
    Curtain(int _w, int _h);  
    ~Curtain();  
    void InitAllPos(const float scale);  
    void ReceivePower( const Pos _pos, const Power _f );  
    void ReceivePower( const CvPoint _pt, const Power _f );  
    void ShowThePosImage( const std::string winName );  
  
    float           fistRange;  // the range affected by the fist measured in m  
private:  
    // These are the inner properties  
    int             w, h;       // the curtain has w*h grains  
    CurtainGrain*   pFirst;     // the first curtain grain in the curtain  
  
    // These are the operate params  
    float           deltaT;     // the time interval for update, it's measured in ms  
    IplImage*       backImg;    // a white image to plaint on  
    float           meterPerPixel; // how long(in m) one pixel distance denotes   
    CurtainGrain*   GetGrainById(int x, int y);  
    CvPoint         MapPosToPoint(const Pos _pos);  
    Pos             MapPointToPos(const CvPoint _pt);  
    double          t;  
};  
  
#endif  
