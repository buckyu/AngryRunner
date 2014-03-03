#include "Setting.h"
#include "cocos2d.h"
#import "Box2D/Box2D.h"
class BonusObject
{

    int bonus_id;
    float lived_time = 0.0f;
    
    public :
        void init(int bonus_id);
        void placeObject(b2Position w);
        void destoy();
    private :
        void initPhysicObject();
        void initSpriteObject();
        void startLiveCycle();
        void reDraw();
        void reCalc(float dt);
    
};
