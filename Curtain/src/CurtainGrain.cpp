#include "CurtainGrain.h"  
  
CurtainGrain::CurtainGrain()  
{  
    isVisited = false;  
    k = 2.0f;    // N/m^2  
    l = 2.0f;     
    rho = 0.25f; // kg/m^3  
    s = 0.05f;   // m  
    m = rho*s*s; // kg  
    wind_fac = 0.005f;   
    linkPower = 0.03f;  
  
    f.py = m*Gravity_g;  
    f.px = f.pz = 0.0f;  
    prevF = f;  
    vt.vx = vt.vy = vt.vz = 0.0f;  
    prevVt = vt;  
    left = right = up = down = BreakLink;  
}  
CurtainGrain::CurtainGrain( int x, int y )  
{  
    colId = x;  
    rowId = y;  
    isVisited = false;  
    isFixed = false;  
    k = 2.0f;    // N/m^2  
    l = 2.0f;     
    rho = 0.25f; // kg/m^3  
    s = 0.05f;   // m  
    m = rho*s*s*0.02f; // kg  
    wind_fac = 0.005f;   
    linkPower = MAX(0.03f,0.06f-y*0.003f);  
  
    f.py = m*Gravity_g;  
    f.px = f.pz = 0.0f;  
    prevF = f;  
    vt.vx = vt.vy = vt.vz = 0.0f;  
    prevVt = vt;  
    left = right = up = down = BreakLink;  
}  
  
CurtainGrain::~CurtainGrain()  
{  
}  
  
void CurtainGrain::SetFixed()  
{  
    isFixed = true;  
}  
void CurtainGrain::SetPos()  
{  
    prevPos.x = pos.x = colId*s;  
    prevPos.y = pos.y = rowId*s;  
    prevPos.z = pos.z = 0.0f;  
}  
void CurtainGrain::SetGround(float _ground)  
{  
    ground = _ground;  
}  
void CurtainGrain::SetPos(const float scale)  
{  
    prevPos.y *= scale;  
    pos.y *= scale;  
}  
void CurtainGrain::GetPos( Pos &_pos )  
{  
    if( rowId==2 && colId== 0 && pos.y > 100 )  
        int tempInt = 0;  
    _pos = pos;  
}  
bool CurtainGrain::GetNeiborVisitStatus(NeighborType type)  
{  
    switch (type)  
    {  
    case upNeibor:  
        return up->isVisited;  
    case downNeibor:  
        return down->isVisited;  
    case leftNeibor:  
        return left->isVisited;  
    case rightNeibor:  
        return right->isVisited;  
    default:  
        break;  
    }  
    return false;  
}  
bool CurtainGrain::NeiborExist(NeighborType type)  
{  
    switch(type)  
    {  
    case upNeibor:  
        return up != BreakLink;  
    case downNeibor:  
        return down != BreakLink;  
    case leftNeibor:  
        return left != BreakLink;  
    case rightNeibor:  
        return right != BreakLink;  
    default:  
        return false;  
    }  
}  
void CurtainGrain::GetUpPos( Pos &_pos )  
{  
    if(up) up->GetPos(_pos);  
}  
void CurtainGrain::GetDownPos( Pos &_pos )  
{  
    if(down) down->GetPos(_pos);  
}  
void CurtainGrain::GetLeftPos( Pos &_pos )  
{  
    if(left) left->GetPos(_pos);  
}  
void CurtainGrain::GetRightPos( Pos &_pos )  
{  
    if(right) right->GetPos(_pos);  
}  
void CurtainGrain::SetNeighbor( CurtainGrain* pNeibor, NeighborType type )  
{  
    if ( pNeibor )  
    {  
        switch (type)  
        {  
        case upNeibor:  
            {  
                up = pNeibor;  
                break;  
            }  
        case downNeibor:  
            {  
                down = pNeibor;  
                break;  
            }  
        case leftNeibor:  
            {  
                left = pNeibor;  
                break;  
            }  
        case rightNeibor:  
            {  
                right = pNeibor;  
                break;  
            }  
  
        default:  
            break;  
        }  
    }  
}  
void CurtainGrain::RemoveNeighbor( NeighborType type )  
{  
    switch (type)  
        {  
        case upNeibor:  
            {  
                up = BreakLink;  
                break;  
            }  
        case downNeibor:  
            {  
                down = BreakLink;  
                break;  
            }  
        case leftNeibor:  
            {  
                left = BreakLink;  
                break;  
            }  
        case rightNeibor:  
            {  
                right = BreakLink;  
                break;  
            }  
  
        default:  
            break;  
        }  
}  
void CurtainGrain::ClearOutsidePower()  
{  
    prevF = f;  
    f.px = f.pz = 0.0f;  
    f.py = m*Gravity_g;  
}  
void CurtainGrain::SetOutsidePower( const Power out_f )  
{  
    f.Add(out_f);  
    Power selfPower,otherPower;  
    Pos   tmpPos;  
  
    if (up)  
    {  
        if(!up->isVisited)  
        {  
            up->GetPos( tmpPos );  
            CalcSelfOtherPower( selfPower, otherPower, tmpPos );  
            float p = selfPower.px + selfPower.py + selfPower.pz;  
            if (p > linkPower)  
            {  
                up->down = BreakLink;  
                up = BreakLink;  
            }  
            else  
            {  
                f.Add( selfPower );  
                up->f.Add( otherPower );  
            }  
        }  
    }  
    if (down)  
    {  
        if(!down->isVisited)  
        {  
            down->GetPos( tmpPos );  
            CalcSelfOtherPower( selfPower, otherPower, tmpPos );  
            float p = selfPower.px + selfPower.py + selfPower.pz;  
            if (p > linkPower)  
            {  
                down->up = BreakLink;  
                down = BreakLink;  
            }  
            else  
            {  
                f.Add( selfPower );  
                down->f.Add( otherPower );  
            }  
        }  
    }  
    if (left)  
    {  
        if(!left->isVisited)  
        {  
            left->GetPos( tmpPos );  
            CalcSelfOtherPower( selfPower, otherPower, tmpPos );  
            float p = selfPower.px + selfPower.py + selfPower.pz;  
            if (p > linkPower)  
            {  
                left->right = BreakLink;  
                left = BreakLink;  
            }  
            else  
            {  
                f.Add( selfPower );  
                left->f.Add( otherPower );  
            }  
        }  
    }  
    if (right)  
    {  
        if(!right->isVisited)  
        {  
            right->GetPos( tmpPos );  
            CalcSelfOtherPower( selfPower, otherPower, tmpPos );  
            float p = selfPower.px + selfPower.py + selfPower.pz;  
            if (p > linkPower)  
            {  
                right->left = BreakLink;  
                right = BreakLink;  
            }  
            else  
            {  
                f.Add( selfPower );  
                right->f.Add( otherPower );  
            }  
        }  
    }  
    // Add the wind power  
    Power wind;  
    wind.px = -wind_fac*(pos.x - prevPos.x);  
    wind.py = -wind_fac*(pos.y - prevPos.y);  
    wind.pz = -wind_fac*(pos.z - prevPos.z);  
    f.Add(wind);  
  
    isVisited = true;  
}  
void CurtainGrain::UpdatePosAfterPower( const float deltaT )  
{  
    if(isFixed) return ;  
      
    float deltaTInS = deltaT*0.001f;  
    Pos temp = pos;  
    Velocity tmpVt = vt;  
    Power tmpF = f;  
    tmpF.Add(prevF);  
    //vt.vx = vt.vx + 0.5*tmpF.px/m*deltaTInS;  
    //vt.vy = vt.vy + 0.5*tmpF.py/m*deltaTInS;  
    //vt.vz = vt.vz + 0.5*tmpF.pz/m*deltaTInS;  
  
    //pos.x = pos.x + (redef_abs(vt.vx*deltaTInS)<0.05?vt.vx*deltaTInS:(vt.vx<0?-0.05:0.05));  
    //pos.y = pos.y + (redef_abs(vt.vy*deltaTInS)<0.05?vt.vy*deltaTInS:(vt.vy<0?-0.05:0.05));  
    //pos.z = pos.z + (redef_abs(vt.vz*deltaTInS)<0.05?vt.vz*deltaTInS:(vt.vz<0?-0.05:0.05));  
    pos.x = 2*pos.x - prevPos.x + f.px/m*deltaTInS*deltaTInS;  
    pos.y = 2*pos.y - prevPos.y + f.py/m*deltaTInS*deltaTInS;  
    pos.z = 2*pos.z - prevPos.z + f.pz/m*deltaTInS*deltaTInS;  
    // the grain must be above the ground  
    pos.y = MIN(ground,pos.y);  
  
    prevVt = tmpVt;  
    prevPos = temp;  
}  
  
void CurtainGrain::CalcSelfOtherPower(   
    Power &selfPower, Power &otherPower,  
    const Pos otherPos )  
{  
    float disx = otherPos.x - pos.x;  
    float disy = otherPos.y - pos.y;  
    float disz = otherPos.z - pos.z;  
    float dis = sqrt(disx*disx + disy*disy + disz*disz);  
      
    float strength = iCalcSelfPower( dis );  
    float selfPx = disx/dis*strength;  
    float selfPy = disy/dis*strength;  
    float selfPz = disz/dis*strength;  
  
    selfPower.px = selfPx;  
    selfPower.py = selfPy;  
    selfPower.pz = selfPz;  
    otherPower.px = -selfPx;  
    otherPower.py = -selfPy;  
    otherPower.pz = -selfPz;  
}  
float CurtainGrain::iCalcSelfPower( const float dis )  
{  
    if( dis > 0 )  
    {  
        float tmp = redef_abs(dis)-s;  
        if(tmp > 0)  
        {  
            return k*tmp*tmp; // positive means "pull" power  
        }  
        else  
        {  
            return -l*tmp*tmp; // negative means "push" power  
        }  
    }  
    else  
    {  
        return 0.0f;  
    }  
}  
