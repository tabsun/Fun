#include "Curtain.h"  
  
Curtain::Curtain(int _w, int _h)  
{  
    w = _w;  
    h = _h;  
    deltaT = 4.0f;  
    fistRange = 0.2f;  
    meterPerPixel = 5.0f/7.0f*0.01f;  
    pFirst = new CurtainGrain[w*h];  
    for (int i = 0; i < w*h; i++)  
    {  
        int x = i%w, y = i/w;  
        CurtainGrain* curGrain = pFirst+i;  
        CurtainGrain* ptemp = new CurtainGrain(x,y);
        memcpy(curGrain, ptemp, sizeof(CurtainGrain));  
        delete ptemp;
        if(y==0) curGrain->SetFixed();  
  
        //set position  
        curGrain->SetPos();  
        //set ground  
        float temp = MapPointToPos(cvPoint(0,610)).y;  
        curGrain->SetGround(temp);  
        //setup the link  
        if(x!=0)  
            curGrain->SetNeighbor(GetGrainById(x-1,y), leftNeibor);  
        if(x!=w-1)  
            curGrain->SetNeighbor(GetGrainById(x+1,y), rightNeibor);  
        if(y!=0)  
            curGrain->SetNeighbor(GetGrainById(x,y-1), upNeibor);  
        if(y!=h-1)  
            curGrain->SetNeighbor(GetGrainById(x,y+1), downNeibor);  
    }  
  
    backImg = cvCreateImage(cvSize(800,640),IPL_DEPTH_8U, 3);  
    cvSet(backImg,cvScalarAll(255));  
    cvLine(backImg,cvPoint(5,610),cvPoint(795,610),CV_RGB(0,0,0),2);  
}  
  
Curtain::~Curtain()  
{  
    delete[] pFirst;  
}  
  
CurtainGrain* Curtain::GetGrainById(int x, int y)  
{  
    return (CurtainGrain*)(pFirst + y*w + x);  
}  
CvPoint Curtain::MapPosToPoint(const Pos _pos)  
{  
    return cvPoint(cvRound(_pos.x/meterPerPixel)+136,  
        cvRound(_pos.y/meterPerPixel)+41);  
}  
  
Pos Curtain::MapPointToPos(const CvPoint _pt)  
{  
    Pos temp;  
    temp.z = 0.0f;  
    temp.x = (_pt.x-136)*meterPerPixel;  
    temp.y = (_pt.y-41)*meterPerPixel;  
    return temp;  
}  
  
void Curtain::InitAllPos(const float scale)  
{  
    for (int x = 0; x  < w; x ++)  
    {  
        for (int y = 0; y < h; y++)  
        {  
            CurtainGrain* curGrain = GetGrainById(x,y);  
            curGrain->SetPos(scale);  
        }  
    }  
}  
  
void Curtain::ReceivePower( const Pos _pos, const Power _f )  
{  
    t = (double)cvGetTickCount();  
    int id = 0;  
    while (id<w*h)  
    {  
        (pFirst+id)->ClearOutsidePower();  
        id ++;  
    }  
  
    Pos tmpPos;  
    Power tmpPower;  
    for( int y = 0; y < h; y ++ )  
    {  
        for (int x = 0; x < w; x++)  
        {  
            CurtainGrain* curGrain = GetGrainById(x,y);  
            curGrain->GetPos(tmpPos);  
            if (abs(tmpPos.x-_pos.x) < fistRange &&  
                abs(tmpPos.y-_pos.y) < fistRange)  
            {  
                float diff = sqrt((tmpPos.x-_pos.x)*(tmpPos.x-_pos.x) +   
                    (tmpPos.y-_pos.y)*(tmpPos.y-_pos.y));  
                tmpPower.px = _f.px*exp(-diff);  
                tmpPower.py = _f.py*exp(-diff);  
                tmpPower.pz = _f.pz*exp(-diff);  
                curGrain->SetOutsidePower( tmpPower );  
                curGrain->UpdatePosAfterPower(deltaT);  
            }  
            else  
            {  
                tmpPower.px = tmpPower.py = tmpPower.pz = 0.0f;  
                curGrain->SetOutsidePower( tmpPower );  
                curGrain->UpdatePosAfterPower(deltaT);  
            }  
        }  
    }  
}  
void Curtain::ReceivePower( const CvPoint _pt, const Power _f )  
{  
    Pos temp = MapPointToPos(_pt);  
    ReceivePower(temp,_f);  
}  
void Curtain::ShowThePosImage( const std::string winName )  
{  
    IplImage* showImg = cvCloneImage(backImg);  
    Pos tmpPos;  
    for (int y = 0; y < h; y++)  
    {  
        for (int x = 0; x < w; x++)  
        {  
            CurtainGrain* curGrain = GetGrainById(x,y);  
            curGrain->GetPos(tmpPos);  
            CvPoint curPt = MapPosToPoint(tmpPos);  
            if(curGrain->isVisited)  
            {  
                if(curGrain->NeiborExist(upNeibor) &&   
                    curGrain->GetNeiborVisitStatus(upNeibor))  
                {  
                    curGrain->GetUpPos(tmpPos);  
                    cvLine(showImg,curPt,MapPosToPoint(tmpPos),cvScalarAll(0));  
                }  
                if(curGrain->NeiborExist(downNeibor) &&  
                    curGrain->GetNeiborVisitStatus(downNeibor))  
                {  
                    curGrain->GetDownPos(tmpPos);  
                    cvLine(showImg,curPt,MapPosToPoint(tmpPos),cvScalarAll(0));  
                }  
                if(curGrain->NeiborExist(leftNeibor) &&  
                    curGrain->GetNeiborVisitStatus(leftNeibor))  
                {  
                    curGrain->GetLeftPos(tmpPos);  
                    cvLine(showImg,curPt,MapPosToPoint(tmpPos),cvScalarAll(0));  
                }  
                if(curGrain->NeiborExist(rightNeibor) &&  
                    curGrain->GetNeiborVisitStatus(rightNeibor))  
                {  
                    curGrain->GetRightPos(tmpPos);  
                    cvLine(showImg,curPt,MapPosToPoint(tmpPos),cvScalarAll(0));  
                }  
                curGrain->isVisited = false;  
            }  
        }  
    }  
    t = ((double)cvGetTickCount() - t)/(1000.0*cvGetTickFrequency());  
    cvShowImage( winName.c_str(), showImg );  
    cvWaitKey(/*MAX(cvRound(deltaT-t),*/1);  
    cvReleaseImage( &showImg );  
}  
