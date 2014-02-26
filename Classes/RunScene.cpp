#include "RunScene.h"
#import "PlayerObject.h"
#import "B2DebugDrawLayer.h"
#define PTM_RATIO 32
USING_NS_CC;

Scene* RunScene::createScene()
{
    auto scene = Scene::create();
    auto layer = RunScene::create();
    scene->addChild(layer);
    return scene;
}

bool RunScene::init()
{
    if ( !Layer::init() )  return false;
   
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto closeItem = MenuItemImage::create(
                                           "button-jump.png",
                                           "button-jump-select.png",
                                           CC_CALLBACK_1(RunScene::buttonJumpCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
   
    // update timer
    this->schedule(schedule_selector(RunScene::update));
    
    
    //auto sprite = Sprite::create("HelloWorld.png");
    
    // position the sprite on the center of the screen
    //sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
 
    //tileMapNode = TMXTiledMap::create("map.tmx");
    //tileMapNode->setPosition(Point(0, 0));
    //this->addChild(tileMapNode, 0);
    
   
    b2Vec2 gravity = b2Vec2(0.0f, -9.8f);
    world = new b2World(gravity);
    
    
    addChild(B2DebugDrawLayer::create(world, PTM_RATIO), 9999);
    
    contactListener = new b2ContactListener();
    world->SetContactListener(contactListener);
    
    
    player = new PlayerObject();
    player->init();
    player->initPhysic(world);
    this->addChild(player->sprite_player);
    
    this->makeBox2dObjAt();
    
    float playfield_width = visibleSize.width * 10.0; // make the x-boundry 2 times the screen width
    float playfield_height = visibleSize.height * 10.0; // make the y-boundry 2 times the screen height
    Point* center = new Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);

  
    
    return true;
}

void RunScene::update(float dt)
{
    int velocityIterations = 8;
	int positionIterations = 1;
    
    world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			Sprite *myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( b->GetPosition().x * PTM_RATIO , b->GetPosition().y * PTM_RATIO);
			myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}	
	}
    player->reDraw();
    
    Director::getInstance()->getProjection();
    

}

void RunScene::buttonJumpCallback(Object* pSender)
{
    CCLOG("button jump pressed");
    player->moveRight();
}





void RunScene::makeBox2dObjAt()
{
	b2BodyDef bodyDef;
    bodyDef.position.Set(600/PTM_RATIO, 100/PTM_RATIO);
    
	b2Body *body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(400/PTM_RATIO, 10/PTM_RATIO);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1;
	fixtureDef.friction = 1;
	fixtureDef.restitution = 1;
	body->CreateFixture(&fixtureDef);
}
