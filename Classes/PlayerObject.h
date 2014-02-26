#ifndef __HelloCpp__PlayerObject__
#define __HelloCpp__PlayerObject__

#import "cocos2d.h"
#import "Box2D/Box2D.h"

using namespace cocos2d;

class PlayerObject 
{
    SpriteFrameCache* sprite_cache;
    SpriteFrameCache* cache;
    Animation* animation_run;
    b2Body* body;
    
    public:
        void init();
        void reDraw();
        void startRunAnimation();
        void initPhysic(b2World* world);
        void jump();
        void moveRight();
        Sprite *player_sprite;
        SpriteBatchNode* sprite_player;
        Vector<SpriteFrame*> spriteFrames;
    
};



#endif
