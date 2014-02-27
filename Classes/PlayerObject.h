#ifndef __HelloCpp__PlayerObject__
#define __HelloCpp__PlayerObject__

#import "cocos2d.h"
#import "Box2D/Box2D.h"

using namespace cocos2d;

class PlayerObject : public b2ContactListener
{
    int jumps_count = 0;
  
    SpriteFrameCache* sprite_cache;
    SpriteFrameCache* cache;
    Animation* animation_run;
    b2Body* body;

    void reCalc();
    
    public:
        void init(float poz_x, float poz_y);
        void reDraw();
        void startRunAnimation();
        void initPhysic(b2World* world);
        void jump();
        void moveRight();
        Sprite *player_sprite;
        SpriteBatchNode* sprite_player;
        Vector<SpriteFrame*> spriteFrames;
    
    
    private:
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};



#endif
