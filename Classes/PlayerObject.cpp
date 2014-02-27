#define PTM_RATIO 32.0f
#define SCREEN_TO_WORLD(n) ((n) / PTM_RATIO)
#define WORLD_TO_SCREEN(n) ((n) * PTM_RATIO)
#define B2_ANGLE_TO_COCOS_ROTATION(n) (-1 * CC_RADIANS_TO_DEGREES(n))
#define COCOS_ROTATION_TO_B2_ANGLE(n) (CC_DEGREES_TO_RADIANS(-1 * n))
#include "PlayerObject.h"
#include "Setting.h"
void PlayerObject::init(float poz_x, float poz_y)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    sprite_player = SpriteBatchNode::create("sprite_player.png");
    cache = SpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("sprite_player.plist");
    
    player_sprite = Sprite::createWithSpriteFrameName("01.png");
    player_sprite->setPosition(Point(poz_x, poz_y));
    player_sprite->setScale(1.0);
    sprite_player->addChild(player_sprite);

    char str[100] = {0};
    for(int i = 1; i < 8; i++)
    {
        sprintf(str, "%02d.png", i);
        spriteFrames.pushBack( cache->SpriteFrameCache::getSpriteFrameByName( str ) );
    }
}

void PlayerObject::initPhysic(b2World* world)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    b2BodyDef playerBodyDef;
    playerBodyDef.position.Set(this->player_sprite->getPositionX() / PTM_RATIO, this->player_sprite->getPositionY() / PTM_RATIO);
    playerBodyDef.type = b2_dynamicBody;
   
	body = world->CreateBody(&playerBodyDef);
    
    b2CircleShape circleShape;
	circleShape.m_radius = 0.7;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 10.0f;
	fixtureDef.friction = 0.4f;
    fixtureDef.restitution =  0.0f;
	body->CreateFixture(&fixtureDef);
    body->SetUserData( (void*)OBJ_TYPE_PLAYER );
    world->SetContactListener(this);
}

void PlayerObject::startRunAnimation()
{
    animation_run = Animation::createWithSpriteFrames(spriteFrames, 0.1f);
    player_sprite->runAction( CCRepeatForever::create( CCAnimate::create(this->animation_run) ) );
}



void PlayerObject::jump() {
    
    // first jump
    if ( this->jumps_count == 0 )
    {
        auto now_velocity = this->body->GetLinearVelocity();
        auto now_speed = now_velocity.x;
        
        b2Vec2 impulse = b2Vec2(now_speed, 10.0f);
        //body->ApplyLinearImpulse(impulse, body->GetWorldCenter(),false);
        body->SetLinearVelocity(impulse);
        jumps_count++;
        return;
    }
    
    // secind jump
    if ( this->jumps_count == 1 )
    {
        b2Vec2 impulse = b2Vec2(0.0f, 50.0f);
        body->ApplyLinearImpulse(impulse, body->GetWorldCenter(),false);
        
        jumps_count++;
        return;
    }
 
    
}

void PlayerObject::reDraw()
{
    this->reCalc();
    
    this->player_sprite->setPosition(this->body->GetPosition().x * PTM_RATIO, this->body->GetPosition().y * PTM_RATIO);
}


void PlayerObject::reCalc()
{
   
    
    auto now_velocity = this->body->GetLinearVelocity();
    auto now_speed = now_velocity.x;
    
    if (now_speed > PLAYER_MAX_SPEED)
    {
        this->body->SetLinearDamping(0.1);
    }
    else
    {
        b2Vec2 impulse = b2Vec2(3.0f, 0.0f);
        body->ApplyLinearImpulse(impulse, body->GetWorldCenter(),false);
       // this->body->SetLinearVelocity(impulse);
    }
    //CCLOG("SPEED: %f",now_speed);
    

    
}

void PlayerObject::BeginContact(b2Contact* contact)
{
    
}
void PlayerObject::EndContact(b2Contact* contact)
{
    
}


void PlayerObject::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    this->jumps_count = 0;
}

void PlayerObject::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    int obj_a = (int)contact->GetFixtureA()->GetBody()->GetUserData();
    int obj_b = (int)contact->GetFixtureB()->GetBody()->GetUserData();
    
    
    if ( contact->IsTouching() )
        if( ( obj_a == OBJ_TYPE_GROUND && obj_b == OBJ_TYPE_PLAYER ) ||
           ( obj_b == OBJ_TYPE_GROUND && obj_a == OBJ_TYPE_PLAYER ) )
        {
            this->jumps_count = 0;
        }

}




