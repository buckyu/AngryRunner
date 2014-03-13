#include "Setting.h"
#include "cocos2d.h"
#import "Box2D/Box2D.h"

using namespace cocos2d;

class GameObject
{

    int ANIMATION_STANDART_TAG = 1;
    int ANIMATION_ACTION_TAG = 2;
    
    int bonus_id;
    int owner;
    
    float lived_time = 0.0f;
    
    SpriteFrameCache* sprite_cache;
    SpriteFrameCache* cache;
    
    RepeatForever* animation_standart;
    Animate* animation_action;
    
    Vector<SpriteFrame*> spriteFramesStandart;
    Vector<SpriteFrame*> spriteFramesAction;
    
    b2Body* body;
    
    b2Vec2 position;
    
    b2World* world;
    
    Layer* map_layer;
    
    public :
        GameObject(int bonus_id,int owner,b2Vec2 position, b2World* world,Layer* map_layer);
        void destoy();
        void reCalc(float dt);
        bool isToDestroy();
        Sprite *object_sprite;
        SpriteBatchNode* spriteSheet;
    
    private :
        void initPhysicObject();
        void initSpriteObject();
        void startLiveCycle();
        void reDraw();
    
        void tryStartStandartAnimation();
        void StartStandartAnimation();
        void tryStartActionAnimation();
        void StartActionAnimation();
        void stopAllLoopingAnimations();
    
    
    
    
};
