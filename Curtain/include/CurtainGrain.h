#ifndef _CURTAINGRAIN_H_  
#define _CURTAINGRAIN_H_  
#include <math.h>  
#define MAX(x,y) (x)>(y)?(x):(y)  
#define MIN(x,y) (x)<(y)?(x):(y)  
#define BreakLink 0  
#define Gravity_g 9.8f  
enum NeighborType  
    {  
        upNeibor = 0x01,  
        downNeibor,  
        leftNeibor,  
        rightNeibor  
    };  
  
struct Acc{  
    float   ax;  
    float   ay;  
    float   az;  
};  
struct Power  
{  
    float   px;  
    float   py;  
    float   pz;  
    void Add( const Power f ){  
        px += f.px;  
        py += f.py;  
        pz += f.pz;  
    }  
    Acc GetAcc( const float m ){  
        Acc acc;  
        acc.ax = px/m;  
        acc.ay = py/m;  
        acc.az = pz/m;  
    }  
};  
struct Pos  
{  
    float   x;  
    float   y;  
    float   z;  
};  
struct Velocity  
{  
    float   vx;  
    float   vy;  
    float   vz;  
};  
  
class CurtainGrain  
{  
public:  
    CurtainGrain();  
    CurtainGrain(int x, int y);  
    ~CurtainGrain();  
    int     colId, rowId;  
    bool    isVisited;  
  
    void SetFixed();  
    void SetPos();  
    void SetGround(float _ground);  
    void SetPos(const float scale);  
    void GetPos( Pos &_pos );  
    bool GetNeiborVisitStatus(NeighborType type);  
    bool NeiborExist(NeighborType type);  
    void GetUpPos( Pos &_pos );  
    void GetDownPos( Pos &_pos );  
    void GetLeftPos( Pos &_pos );  
    void GetRightPos( Pos &_pos );  
    void SetNeighbor( CurtainGrain* pNeibor, NeighborType type );  
    void RemoveNeighbor( NeighborType type );  
    void ClearOutsidePower();  
    void SetOutsidePower( const Power out_f );  
    void UpdatePosAfterPower(const float deltaT);  
      
private:  
    void CalcSelfOtherPower( Power &selfPower, Power &otherPower, const Pos otherPos );  
    float iCalcSelfPower( const float dis );  
    // These are state properties of Curtain_Grain  
    bool    isFixed;  
    Pos     pos, prevPos;  
    Velocity vt, prevVt;  
    Power   prevF;  
    float   ground;  
    CurtainGrain*   left;  
    CurtainGrain*   right;  
    CurtainGrain*   up;  
    CurtainGrain*   down;  
    // These are inner properties of Curtain_Grain  
    float   k; // the factor in equation f = k*x^2 when it's drawed  
    float   l; // the factor in equation f = l*x^2 when it's clustered  
    float   rho; // the density in equation m = rho*Volume  
    float   s; // the length of Curtain_Grain, so Volume = s^2  
    float   m; // the weight of one Curtain_Grain  
    float   wind_fac; // the factor of wind strength  
    float   linkPower; // max power the link can bear  
    Power   f; // the sum power which is measured in N(Newton)  
};  
  
  
inline float redef_abs(float x)  
{  
    return x>0?x:-x;  
}  
#endif 
