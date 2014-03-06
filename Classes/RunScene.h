#ifndef __HelloCpp__RunScene__
#define __HelloCpp__RunScene__

#include "../../Classes/pugixml/pugixml.hpp"
#include "cocos2d.h"
#import "Box2D/Box2D.h"
#import "../../Classes/PlayerObject.h"
#include "../../Classes/Setting.h"

class RunScene : public cocos2d::Layer
{
    bool IS_LOADED = false;
    
    std::string map_id;
    
    float map_tmx_offset_y;
    float map_tmx_offset_x;
    
    Size screen_visible_size;
    Point screen_origin_size;
    
    b2World* physic_world;
  
    
    Scene* scene;
    Layer* main_layer;
    Layer* map_layer;
    
    PlayerObject *player_object;
    
    TMXTiledMap *map_tile;
    TMXLayer *map_background;
    
    cocos2d::Size creenSize;
	cocos2d::TMXTiledMap *tileMapNode;
 
    void buttonJumpCallback(Object* pSender);
    virtual void update(float dt);
    void makePhysicPoligonGround(pugi::xml_node tool,int mode);
    std::vector<std::string> split(std::string str, std::string delim);
    
    public:
        Scene* createScene(std::string map_id);
       // CREATE_FUNC(RunScene);
    
    private:
        void createAll();
        void createMap();
        void createControls();
        void createPlayer();
};

#endif 
