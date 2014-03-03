#include "BonusObject.h"


void BonusObject::init(int bonus_id)
{
    this->bonus_id = bonus_id;
    
    
}


void BonusObject::placeObject(b2Position w)
{
    this->startLiveCycle();
}


void BonusObject::startLiveCycle()
{
    this->lived_time = 0;
}

void BonusObject::reCalc(float dt)
{
    
    // increase live time
    this->lived_time += dt;
    
}

void BonusObject::reDraw()
{
    
}


void BonusObject::destoy()
{

}







