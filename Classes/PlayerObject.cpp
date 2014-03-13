#include "../../Classes/PlayerObject.h"
#include "../../Classes/Setting.h"

void PlayerObject::init(float poz_x, float poz_y)
{
    player_start_position_x = poz_x;
    player_start_position_y = poz_y;
    
    player_layer = Layer::create();
    
    // load spritebacts
    spriteSheet = SpriteBatchNode::create("player_sprite.png");
    spriteSheetShield = SpriteBatchNode::create("bn_shield.png");

    // load plists
    cache = SpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("bn_shield.plist");
    cache->addSpriteFramesWithFile("player_sprite.plist");

    // init sprites
    shield_sprite = Sprite::createWithSpriteFrameName("bn_shield_1.png");
    shield_sprite->setScale(0.6);
    shield_sprite->setVisible(false);
    spriteSheetShield->addChild(shield_sprite);
    
    player_sprite = Sprite::createWithSpriteFrameName("p_stand.png");
    player_sprite->setScale(1.0);
    spriteSheet->addChild(player_sprite);

    // add sprites to layer
    player_layer->addChild(spriteSheet);
    player_layer->addChild(spriteSheetShield);
    
    // load walk animation
    char str[100] = {0};
    for(int i = 1; i < 8; i++)
    {
        sprintf(str, "p_walk_%02d.png", i);
        if( cache->SpriteFrameCache::getSpriteFrameByName( str )  != nullptr )
            spriteFramesRun.pushBack( cache->SpriteFrameCache::getSpriteFrameByName( str ) );
    }
    
    // load shield animation
    for(int i = 0; i < 8; i++)
    {
        sprintf(str, "bn_shield_%d.png", i);
        if( cache->SpriteFrameCache::getSpriteFrameByName( str )  != nullptr )
            spriteFramesShield.pushBack( cache->SpriteFrameCache::getSpriteFrameByName( str ) );
    }
    
    // load fly animation
    spriteFramesFly.pushBack( cache->SpriteFrameCache::getSpriteFrameByName( "p_jump.png" ) );
    
    
    // start shield animation
    shield_sprite->runAction( RepeatForever::create( Animate::create( Animation::createWithSpriteFrames( spriteFramesShield , 0.11f) ) ) ) ;
}

void PlayerObject::initPhysic(b2World* world)
{
    b2BodyDef playerBodyDef;
    playerBodyDef.position.Set(this->player_start_position_x / PTM_RATIO, this->player_start_position_y / PTM_RATIO);
    playerBodyDef.type = b2_dynamicBody;
   
	body = world->CreateBody(&playerBodyDef);
    
    b2CircleShape circleShape;
	circleShape.m_radius = 1.4;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 10.0f;
	fixtureDef.friction = 0.4f;
    fixtureDef.restitution =  0.0f;
  
	body->CreateFixture(&fixtureDef);
    body->SetUserData( (void*)OBJ_TYPE_PLAYER );
    world->SetContactListener(this);
}


void PlayerObject::tryStartRunAnimation()
{
    Action *act = player_sprite->getActionByTag( this->ANIMATION_RUN_TAG );
    if( act == NULL ) this->StartRunAnimation();
    else if(act->isDone()) this->StartRunAnimation();
}

void PlayerObject::StartRunAnimation()
{
    this->stopAllLoopingAnimations();
    animation_running = RepeatForever::create( Animate::create( Animation::createWithSpriteFrames(spriteFramesRun, 0.05f) ) );
    animation_running->setTag( this->ANIMATION_RUN_TAG );
    player_sprite->runAction( animation_running );
}

void PlayerObject::tryStartFlyAnimation()
{
    Action *act = player_sprite->getActionByTag(this->ANIMATION_FLY_TAG);
    if( act == NULL ) this->StartFlyAnimation();
    else if(act->isDone()) this->StartFlyAnimation();
}

void PlayerObject::StartFlyAnimation()
{
    this->stopAllLoopingAnimations();
    animation_flying = RepeatForever::create( Animate::create( Animation::createWithSpriteFrames(spriteFramesFly, 0.05f) ) );
    animation_flying->setTag(this->ANIMATION_FLY_TAG);
    player_sprite->runAction( animation_flying );
}

void PlayerObject::stopAllLoopingAnimations()
{
    player_sprite->stopActionByTag(this->ANIMATION_RUN_TAG);
    player_sprite->stopActionByTag(this->ANIMATION_FLY_TAG);
}

void PlayerObject::jump() {
    this->tryStartRunAnimation();
    // first jump
    if ( this->jumps_count == 0 )
    {
        this->tryStartFlyAnimation();
        
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
        this->tryStartFlyAnimation();
        
        b2Vec2 impulse = b2Vec2(0.0f, 50.0f);
        body->ApplyLinearImpulse(impulse, body->GetWorldCenter(),false);
        
        jumps_count++;
        return;
    }
}

void PlayerObject::reDraw()
{
    this->player_layer->setPosition(this->body->GetPosition().x * PTM_RATIO, this->body->GetPosition().y * PTM_RATIO);
}


void PlayerObject::reCalc(float dt)
{
    auto now_velocity = this->body->GetLinearVelocity();
    auto now_speed = now_velocity.x;
    // PLAYER_MAX_SPEED
    if (now_speed > PLAYER_MAX_SPEED)
    {
        this->body->SetLinearDamping(0.5);
    }
    else
    {
        //50
        b2Vec2 impulse = b2Vec2(PLAYER_FORW_IMPLS, 0.0f);
        body->ApplyLinearImpulse(impulse, body->GetWorldCenter(),true);
    }
    
    
    // update timers
    if( this->timer_shield > 0 ) this->timer_shield -= dt;
    
    
    // check shield
    if ( this->timer_shield > 0 && ! shield_sprite->isVisible() )
        shield_sprite->setVisible(true);
    if ( this->timer_shield <= 0 && shield_sprite->isVisible() )
         shield_sprite->setVisible(false);
    
    this->reDraw();
}

void PlayerObject::BeginContact(b2Contact* contact)
{
    if( this->isContactGrondAndPlayer(contact) )
        this->tryStartRunAnimation();
}

void PlayerObject::EndContact(b2Contact* contact)
{
    
}

void PlayerObject::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    this->jumps_count = 0;
    
    int obj_a = (int)contact->GetFixtureA()->GetBody()->GetUserData();
    int obj_b = (int)contact->GetFixtureB()->GetBody()->GetUserData();
    
    
    // frezze traps
    if ( obj_a == OBJ_TYPE_TRAP && obj_b == OBJ_TYPE_GROUND )
    {
        contact->GetFixtureA()->GetBody()->SetLinearDamping(100);
        contact->GetFixtureA()->GetBody()->SetAngularDamping(100);
    }
    
    if ( obj_b == OBJ_TYPE_TRAP && obj_a == OBJ_TYPE_GROUND )
    {
        contact->GetFixtureA()->GetBody()->SetLinearDamping(100);
        contact->GetFixtureB()->GetBody()->SetAngularDamping(100);
    }
}

void PlayerObject::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    if( this->isContactGrondAndPlayer(contact) )
        this->jumps_count = 0;
    
    int obj_a = (int)contact->GetFixtureA()->GetBody()->GetUserData();
    int obj_b = (int)contact->GetFixtureB()->GetBody()->GetUserData();
    
    if( ( obj_a == OBJ_TYPE_TRAP && obj_b == OBJ_TYPE_PLAYER ) ||
       ( obj_b == OBJ_TYPE_TRAP && obj_a == OBJ_TYPE_PLAYER ) )
            contact->SetEnabled(false);
}

bool PlayerObject::isContactGrondAndPlayer(b2Contact* contact)
{
    int obj_a = (int)contact->GetFixtureA()->GetBody()->GetUserData();
    int obj_b = (int)contact->GetFixtureB()->GetBody()->GetUserData();
    if ( contact->IsTouching() )
        if( ( obj_a == OBJ_TYPE_GROUND && obj_b == OBJ_TYPE_PLAYER ) ||
           ( obj_b == OBJ_TYPE_GROUND && obj_a == OBJ_TYPE_PLAYER ) )
            return true;
    return false;
}

b2Vec2 PlayerObject::getPosition()
{
    b2Vec2 player_real_position = b2Vec2(this->body->GetPosition().x * PTM_RATIO, this->body->GetPosition().y * PTM_RATIO );
    return player_real_position;
}

bool PlayerObject::applyShiled()
{
    if ( this->timer_shield > 0 ) return false;
    
    this->timer_shield = PLAYER_SHIELD_TIME;
    return true;
}

int PlayerObject::placeMantrap()
{
    return TRAP_MANTRAP;
}

void PlayerObject::setPosition(float x , float y)
{
    this->body->SetTransform( b2Vec2( x / PTM_RATIO , y / PTM_RATIO ), 0);
}

