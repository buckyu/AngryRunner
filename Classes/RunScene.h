#ifndef __HelloCpp__RunScene__
#define __HelloCpp__RunScene__

#include "../../Classes/pugixml/pugixml.hpp"
#include "cocos2d.h"
#import "Box2D/Box2D.h"
#import "../../Classes/PlayerObject.h"
#import "../../Classes/GameObject.h"
#include "../../Classes/Setting.h"

class RunScene : public cocos2d::Layer , public b2ContactListener
{
    bool IS_LOADED = false;
    
    
    std::string map_id;
    
    std::vector<GameObject> game_objects;
    
    float map_tmx_offset_y;
    float map_tmx_offset_x;
    
    Size screen_visible_size;
    Point screen_origin_size;
    
    b2World* physic_world;
  
    
    Scene* scene;
    Layer* main_layer;
    Layer* map_layer;
    Layer* bg_layer;
    
    PlayerObject *player_object;
    
    TMXTiledMap *map_tile;
    TMXTiledMap *bg_tile;
    
    cocos2d::Size creenSize;
	cocos2d::TMXTiledMap *tileMapNode;
 
    void buttonJumpCallback(Object* pSender);
    void buttonMantrapCallback(Object* pSender);
    void buttonShieldCallback(Object* pSender);
    void buttonBackCallback(Object* pSender);
    
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
        
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
        bool isContactTrapAndPlayer(b2Contact* contact);
};

#endif
