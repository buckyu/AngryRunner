#ifndef __HelloCpp__PlayerObject__
#define __HelloCpp__PlayerObject__

#import "cocos2d.h"
#import "Box2D/Box2D.h"

using namespace cocos2d;

class PlayerObject : public b2ContactListener
{
    int ANIMATION_RUN_TAG = 1;
    int ANIMATION_FLY_TAG = 2;
    
    
    bool IS_ALIVE   = true;
    bool IS_RUNNING = false;
    int jumps_count = 0;
  
    SpriteFrameCache* cache;
    
    RepeatForever* animation_running;
    RepeatForever* animation_flying;
    
    Vector<SpriteFrame*> spriteFramesShield;
    Vector<SpriteFrame*> spriteFramesRun;
    Vector<SpriteFrame*> spriteFramesFly;
    
    b2Body* body;

    int timer_toAlive = 0;
    int timer_toImune = 0;
    
    void reCalc();
    
    public:
        void setPosition(float x,float y);
    
        void init(float poz_x, float poz_y);
        void reDraw();
    
        void initPhysic(b2World* world);
   
        void jump();
        int placeMantrap();
        bool applyShiled();
        void moveRight();
        Sprite *player_sprite;
        Sprite *shield_sprite;
        SpriteBatchNode* spriteSheet;
        SpriteBatchNode* spriteSheetShield;
        b2Vec2 getPosition();

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
