#ifndef __HelloCpp__PlayerObject__
#define __HelloCpp__PlayerObject__

#import "cocos2d.h"
#import "Box2D/Box2D.h"

using namespace cocos2d;

class PlayerObject : public b2ContactListener
{
    int ANIMATION_RUN_TAG = 1;
    int ANIMATION_FLY_TAG = 2;
    
    bool IS_RUNNING = false;
    int jumps_count = 0;
  
    SpriteFrameCache* sprite_cache;
    SpriteFrameCache* cache;
    
    RepeatForever* animation_running;
    RepeatForever* animation_flying;
    
    Vector<SpriteFrame*> spriteFramesRun;
    Vector<SpriteFrame*> spriteFramesFly;
    
    b2Body* body;
    
    
    void reCalc();
    
    public:
        void init(float poz_x, float poz_y);
        void reDraw();
    
        void initPhysic(b2World* world);
        void jump();
        void moveRight();
        Sprite *player_sprite;
        SpriteBatchNode* spriteSheet;
    
    
    
    private:
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
        void tryStartRunAnimation();
        void StartRunAnimation();
    
        void tryStartFlyAnimation();
        void StartFlyAnimation();
    
        void stopAllLoopingAnimations();
    
        bool isContactGrondAndPlayer(b2Contact* contact);
};



#endif
