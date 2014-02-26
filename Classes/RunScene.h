#ifndef __HelloCpp__RunScene__
#define __HelloCpp__RunScene__

#include "cocos2d.h"
#import "Box2D/Box2D.h"
#import "PlayerObject.h"
class RunScene : public cocos2d::Layer
{
    PlayerObject *player;
    b2World* world;
    b2ContactListener *contactListener;
 
    cocos2d::Size creenSize;
	cocos2d::TMXTiledMap *tileMapNode;
 
    void buttonJumpCallback(Object* pSender);
    void update(float dt);
    void makeBox2dObjAt();
    
    public:
        static cocos2d::Scene* createScene();
        bool init();
        CREATE_FUNC(RunScene);
};

#endif 
