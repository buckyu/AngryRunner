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
    
    float timer_lived = 0.0f;
    float timer_toActive = 0.0f;
    
    SpriteFrameCache* sprite_cache;
    SpriteFrameCache* cache;
    
    RepeatForever* animation_standart;
    Animate* animation_action;
    
    Vector<SpriteFrame*> spriteFramesStandart;
    Vector<SpriteFrame*> spriteFramesAction;
    
    b2Vec2 position;
    
    b2World* world;
    
    Layer* map_layer;
    
    public :
        int TYPE = TRAP_MANTRAP;
        b2Body* body;
        GameObject(int bonus_id,int owner,b2Vec2 position, b2World* world,Layer* map_layer);
        void destoy();
        void reCalc(float dt);
        bool isToDestroy();
        bool isActive();
        Sprite *object_sprite;
        SpriteBatchNode* spriteSheet;
    void tryStartActionAnimation();
   
    private :
        void initPhysicObject();
        void initSpriteObject();
        void startLiveCycle();
        void reDraw();
        void tryStartStandartAnimation();
        void StartStandartAnimation();
    
        void StartActionAnimation();
        void stopAllLoopingAnimations();
    
};
