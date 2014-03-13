#include "GameObject.h"


GameObject::GameObject(int bonus_id,int owner,b2Vec2 position,b2World* world,Layer* map_layer)
{
    this->bonus_id = bonus_id;
    this->owner    = owner;
    this->position = position;
    this->world    = world;
    this->map_layer = map_layer;
    
    this->initSpriteObject();
    this->initPhysicObject();
    
    CCLOG("Game object placed");
}

void GameObject::initSpriteObject()
{
    spriteSheet = SpriteBatchNode::create("tr_mantrap.png");
    cache = SpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("tr_mantrap.plist");
    
    object_sprite = Sprite::createWithSpriteFrameName("mantrap_0.png");
    object_sprite->setPosition(Point( this->position.x , this->position.y ));
    object_sprite->setScale(0.8f);
    spriteSheet->addChild(object_sprite);
    char str[100] = {0};
    
    // standtart live animation
    for(int i = 0; i < 1; i++)
    {
        sprintf(str, "mantrap_%d.png", i);
        if ( cache->SpriteFrameCache::getSpriteFrameByName( str ) != nullptr )
            spriteFramesStandart.pushBack( cache->SpriteFrameCache::getSpriteFrameByName( str ) );
    }
    
    // animation on action
    for(int i = 0; i < 20; i++)
    {
        sprintf(str, "mantrap_%d.png", i);
        if ( cache->SpriteFrameCache::getSpriteFrameByName( str ) != nullptr )
            spriteFramesAction.pushBack( cache->SpriteFrameCache::getSpriteFrameByName( str ) );
    }
    
    this->map_layer->addChild(spriteSheet, 999);
    this->StartStandartAnimation();
    CCLOG("Object: sprite init");
    
}

void GameObject::initPhysicObject()
{
    log("s %f %f",this->position.x,this->position.y);
    
    b2BodyDef playerBodyDef;
    playerBodyDef.position.Set(this->position.x / PTM_RATIO, this->position.y / PTM_RATIO);
    playerBodyDef.type = b2_dynamicBody;
    
	body = world->CreateBody(&playerBodyDef);
    
    b2PolygonShape groundBox;
    groundBox.SetAsBox(2.0f, 0.3f);
   // groundBox.SetAsBox(2, 0.3, b2Vec2(0, 0), 0);
    b2CircleShape circleShape;
	circleShape.m_radius = 2.4;
    
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &groundBox;
	fixtureDef.density = 1.1;
	fixtureDef.friction = 999999.4f;
    fixtureDef.restitution =  0.0f;
	body->CreateFixture(&fixtureDef);
    body->SetLinearVelocity( b2Vec2(0, -100) );
   
    body->SetUserData( (void*)OBJ_TYPE_TRAP );
    
    CCLOG("Object: physic body init");
}

void GameObject::reCalc(float dt)
{
    // increase live time
    this->lived_time += dt;
    this->reDraw();
    
   for (b2ContactEdge* edge = this->body->GetContactList(); edge; edge = edge->next)
   {
       if ( edge->contact->IsTouching() )
       {
           b2Body* obj_a = edge->contact->GetFixtureA()->GetBody();
           b2Body* obj_b = edge->contact->GetFixtureB()->GetBody();
           
           if( ( obj_a == this->body && (int)obj_b->GetUserData() == OBJ_TYPE_PLAYER ) ||
              ( obj_b == this->body && (int)obj_a->GetUserData() == OBJ_TYPE_PLAYER ) )
           {
               this->tryStartActionAnimation();
           }
           else
           {
               this->tryStartStandartAnimation();
           }
       }
   }
    


}

bool GameObject::isToDestroy()
{
    if( lived_time > 10.0f )
        return true;
    else
        return false;
}

void GameObject::reDraw()
{
    this->object_sprite->setPosition(this->body->GetPosition().x * PTM_RATIO , this->body->GetPosition().y * PTM_RATIO );
    double angle = (int)( this->body->GetAngle() * 180.0f / 3.14f ) % 360 * -1 ;
    this->object_sprite->setRotation(angle);
}

void GameObject::destoy()
{
    this->map_layer->removeChild(spriteSheet);
    this->world->DestroyBody(this->body);


}

void GameObject::tryStartActionAnimation()
{
    Action *act = object_sprite->getActionByTag(this->ANIMATION_ACTION_TAG);
    if( act == NULL ) this->StartActionAnimation();
    else if(act->isDone()) this->StartActionAnimation();
}

void GameObject::StartActionAnimation()
{
    this->stopAllLoopingAnimations();
    animation_action = Animate::create( Animation::createWithSpriteFrames(spriteFramesAction, 0.02f) );
    animation_action->setTag(this->ANIMATION_ACTION_TAG);
    object_sprite->runAction( animation_action );
}

void GameObject::tryStartStandartAnimation()
{
    Action *act_actrion = object_sprite->getActionByTag(this->ANIMATION_ACTION_TAG);
    if( act_actrion != NULL )
        if(! act_actrion->isDone() ) return;

    
    
    Action *act = object_sprite->getActionByTag(this->ANIMATION_STANDART_TAG);
    if( act == NULL ) this->StartStandartAnimation();
    else if(act->isDone()) this->StartStandartAnimation();
}

void GameObject::StartStandartAnimation()
{
    this->stopAllLoopingAnimations();
    animation_standart = RepeatForever::create( Animate::create( Animation::createWithSpriteFrames(spriteFramesStandart, 0.3f) ) );
    animation_standart->setTag(this->ANIMATION_STANDART_TAG);
    object_sprite->runAction( animation_standart );
}

void GameObject::stopAllLoopingAnimations()
{
    object_sprite->stopActionByTag(this->ANIMATION_STANDART_TAG);
    object_sprite->stopActionByTag(this->ANIMATION_ACTION_TAG);
}




