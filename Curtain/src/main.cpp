#include "Curtain.h"  
#include "cv.h"  
#include "highgui.h"  
#define EPS_FLOAT 0.000000001f  
  
CvPoint g_cur,g_prev;  
Power g_power;  
  
float g_powerBase = 0.0001f;  
float g_powerScale = 0.0004f;  
  
void OnMouse(int Event,int x,int y,int flags,void* param)  
{  
    switch (Event)  
    {  
    case CV_EVENT_LBUTTONDOWN:  
        {  
            g_power.pz = -g_powerBase;  
        }  
        break;  
    case CV_EVENT_MOUSEMOVE:  
        {  
            if (flags&CV_EVENT_FLAG_LBUTTON)  
            {  
                g_power.px = (abs(x-g_prev.x)*g_powerScale + g_powerBase)*  
                    (x>g_prev.x?1.0f:-1.0f);  
                g_power.py = (abs(y-g_prev.y)*g_powerScale + g_powerBase)*  
                    (y>g_prev.y?1.0f:-1.0f);  
                g_power.pz = -g_powerBase;  
            }  
            else  
            {  
                g_power.px = g_power.py = g_power.pz = 0.0f;  
            }  
        }  
        break;  
    default:  
        g_power.px = g_power.py = g_power.pz = 0.0f;  
    }  
      
    g_prev.x = g_cur.x = x;  
    g_prev.y = g_cur.y = y;  
}  
int main(int argc, char** argv)  
{  
    g_power.px = g_power.py = g_power.pz = 0.0f;  
    g_prev.x = g_prev.y = g_cur.x = g_cur.y = 0;  
    std::string winName = "display";  
    cvNamedWindow(winName.c_str());  
    cvSetMouseCallback(winName.c_str(),OnMouse,NULL);  
  
    Curtain curtain(70,30);  
    curtain.InitAllPos(1.0);  
  
    int tdk = 0;  
    while (true)  
    {  
        //printf("tdk=%d\n",tdk++);  
        curtain.ReceivePower(g_cur,g_power);  
        curtain.ShowThePosImage("display");  
        char c = cvWaitKey(1);  
        if(c==27) break;  
    }  
    return 0;  
}  
