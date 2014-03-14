#ifndef __HelloCpp__PlayerObject__
#define __HelloCpp__PlayerObject__

#import "cocos2d.h"
#import "Box2D/Box2D.h"
#import "GameObject.h"

using namespace cocos2d;

class PlayerObject : public b2ContactListener
{
    
    float timer_toAlive = 0;
    float timer_toImune = 0;
    float timer_shield = 0;
    
    int ANIMATION_RUN_TAG = 1;
    int ANIMATION_FLY_TAG = 2;
    int ANIMATION_BLOOD_TAG = 3;

    bool IS_ALIVE   = true;
    bool IS_RUNNING = false;
    
    int jumps_count = 0;
    
    int player_start_position_x = 0;
    int player_start_position_y = 0;
  
    SpriteFrameCache* cache;
    
    Animate*       animation_blood;
    RepeatForever* animation_running;
    RepeatForever* animation_flying;
    
    Vector<SpriteFrame*> spriteFramesBlood;
    Vector<SpriteFrame*> spriteFramesShield;
    Vector<SpriteFrame*> spriteFramesRun;
    Vector<SpriteFrame*> spriteFramesFly;
    
    Sprite *player_sprite;
    Sprite *shield_sprite;
    Sprite *blood_sprite;
    SpriteBatchNode* spriteSheet;
    SpriteBatchNode* spriteSheetShield;
    SpriteBatchNode* spriteSheetBlood;
    b2Body* body;
    
    public:
        bool onTrap( int type );
    
        void setPosition(float x,float y);
    
        void init(float poz_x, float poz_y);
        void reCalc(float dt);
    
        void initPhysic(b2World* world);
   
        void jump();
        int placeMantrap();
        bool applyShiled();
        void moveRight();
        Layer* player_layer;
        b2Vec2 getPosition();
    
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
        bool isDie();
        bool isImune();
    private:
        void reDraw();
        void killPlayer();
        void tryStartRunAnimation();
        void StartRunAnimation();
        void tryStartFlyAnimation();
        void StartFlyAnimation();
        void stopAllLoopingAnimations();
        bool isContactGrondAndPlayer(b2Contact* contact);
    
        void StartBloodAnimation();
        void CheckBloodAnimation();
    
};

#endif
