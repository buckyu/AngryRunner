#define PTM_RATIO 32.0f
#define SCREEN_TO_WORLD(n) ((n) / PTM_RATIO)
#define WORLD_TO_SCREEN(n) ((n) * PTM_RATIO)
#define B2_ANGLE_TO_COCOS_ROTATION(n) (-1 * CC_RADIANS_TO_DEGREES(n))
#define COCOS_ROTATION_TO_B2_ANGLE(n) (CC_DEGREES_TO_RADIANS(-1 * n))

#include "PlayerObject.h"

void PlayerObject::init()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    sprite_player = SpriteBatchNode::create("sprite_player.png");
    cache = SpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("sprite_player.plist");
    
    player_sprite = Sprite::createWithSpriteFrameName("01.png");
    player_sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
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
   
    CCLOG("%f , %f",this->player_sprite->getPositionX(), this->player_sprite->getPositionY());
    
    b2BodyDef playerBodyDef;
    playerBodyDef.position.Set(this->player_sprite->getPositionX() / PTM_RATIO, this->player_sprite->getPositionY() / PTM_RATIO);
    playerBodyDef.type = b2_dynamicBody;
    
    
	body = world->CreateBody(&playerBodyDef);
    
    b2CircleShape circleShape;
	circleShape.m_radius = 0.7;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution =  1.0f;
	body->CreateFixture(&fixtureDef);
}

void PlayerObject::startRunAnimation()
{
    animation_run = Animation::createWithSpriteFrames(spriteFrames, 0.1f);
    player_sprite->runAction( CCRepeatForever::create( CCAnimate::create(this->animation_run) ) );
}

void PlayerObject::moveRight() {
    b2Vec2 impulse = b2Vec2(7.0f, 20.0f);
    body->ApplyLinearImpulse(impulse, body->GetWorldCenter(),false);
    
    this->startRunAnimation();
}

void PlayerObject::jump() {
    b2Vec2 impulse = b2Vec2(24.0f, 15.0f);
    body->ApplyLinearImpulse(impulse, body->GetWorldCenter(),false);
}

void PlayerObject::reDraw()
{
    this->player_sprite->setPosition(this->body->GetPosition().x * PTM_RATIO, this->body->GetPosition().y * PTM_RATIO);
    
    
}