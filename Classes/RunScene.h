#ifndef __HelloCpp__RunScene__
#define __HelloCpp__RunScene__

#include "pugixml/pugixml.hpp"
#include "cocos2d.h"
#import "Box2D/Box2D.h"
#import "PlayerObject.h"
#include "Setting.h"

class RunScene : public cocos2d::Layer
{
    float map_tmx_offset_y;
    TMXTiledMap *map_tile;
    TMXLayer *map_background;
    
    Layer* map_layer;
    
    PlayerObject *player;
    b2World* world;
    b2ContactListener *contactListener;
 
    Size visibleSize;
    Point origin;
    
    cocos2d::Size creenSize;
	cocos2d::TMXTiledMap *tileMapNode;
 
    void buttonJumpCallback(Object* pSender);
    void update(float dt);
    void makePhysicPoligonGround(pugi::xml_node tool,int mode);
    std::vector<std::string> split(std::string str, std::string delim);
    
    public:
        static cocos2d::Scene* createScene();
        bool init();
        CREATE_FUNC(RunScene);
};

#endif 
