#include "../../Classes/RunScene.h"
#import "../../Classes/PlayerObject.h"
#import "../../Classes/B2DebugDrawLayer.h"

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
    
    map_layer = Layer::create();
    addChild(map_layer);
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto closeItem = MenuItemImage::create(
                                           "button-jump.png",
                                           "button-jump-select.png",
                                           CC_CALLBACK_1(RunScene::buttonJumpCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                 origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
   

    
    b2Vec2 gravity = b2Vec2(0.0f, -20.0f);
    world = new b2World(gravity);

    map_layer->addChild(B2DebugDrawLayer::create(world, PTM_RATIO), 9999);
    
    contactListener = new b2ContactListener();
    world->SetContactListener(contactListener);

    
    
    player = new PlayerObject();
    player->init(600,1000);
    player->initPhysic(world);
    this->map_layer->addChild(player->spriteSheet);
    
    
    
   
    
    // load map
    this->map_tile = TMXTiledMap::create("map_01.tmx");
    this->map_layer->addChild(map_tile,10);
    
    
    
    
    
    // get y-offset
    map_tmx_offset_y = map_tile->getTileSize().height * map_tile->getMapSize().height / PTM_RATIO ;
    
    // parse map for physics object
    std::string tmx_full_path = CCFileUtils::sharedFileUtils()->fullPathForFilename( "map_01.tmx" );
    //std::string tmx_full_path = "/assets/map.tmx";

    long size = 0;
    unsigned char * posdataLoc = CCFileUtils::sharedFileUtils() -> getFileData( tmx_full_path.c_str(), "r",&size );
    
    std::string str;
    str.append(reinterpret_cast<const char*>(posdataLoc));
    
    //std::string tmx_full_path = "map.tmx";
    pugi::xml_document doc;
    
    pugi::xml_parse_result result = doc.load(str.c_str());
   // pugi::xml_parse_result result = doc.load_file( tmx_full_path.c_str() );
    
    CCLOG("XML READ: %s",result.description());
    for (pugi::xml_node tool = doc.child("map").child("objectgroup").child("object"); tool; tool = tool.next_sibling("object"))
    {
        this->makePhysicPoligonGround(tool,0);
    }

    // set updater
    this->schedule(schedule_selector(RunScene::update));

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
			//Sprite *myActor = (CCSprite*)b->GetUserData();
            //myActor->setPosition( b->GetPosition().x * PTM_RATIO , b->GetPosition().y * PTM_RATIO);
			//myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}	
	}
    player->reDraw();

    float player_x = this->player->player_sprite->getPositionX();
    float player_y = this->player->player_sprite->getPositionY();
    
    float mapOffset_x = visibleSize.width - player_x - visibleSize.width / 2;
    float mapOffset_y = visibleSize.height - player_y - visibleSize.height / 2;

    this->map_layer->setPositionX(mapOffset_x);
    this->map_layer->setPositionY(mapOffset_y);
}

void RunScene::buttonJumpCallback(Object* pSender)
{
    player->jump();
}

void RunScene::makePhysicPoligonGround(pugi::xml_node tool,int mode)
{
	b2BodyDef bodyDef;
    bodyDef.position.Set( 0 / PTM_RATIO, 0 / PTM_RATIO );
	b2Body *body = world->CreateBody(&bodyDef);
    
    b2PolygonShape shape;
    std::vector<b2Vec2> shape_points;

    pugi::xml_node polyobject_node = tool.child("polygon");
        
    float x_offset = ::atof( tool.attribute("x").value() );
    float y_offset = ::atof( tool.attribute("y").value() );

    std::string poins_string = polyobject_node.attribute("points").value();
    std::vector<std::string> points_array = this->split(poins_string, " ");

    for (auto &attack : points_array )
    {
        std::vector<std::string> p_array = this->split(attack, ",");
        if(p_array.size() == 2)
        {
            b2Vec2 p = b2Vec2( ( ::atof(p_array[0].c_str()) + x_offset ) / PTM_RATIO ,
                               ( ::atof(p_array[1].c_str()) + y_offset ) / PTM_RATIO * -1 + this->map_tmx_offset_y  );
            
            shape_points.push_back(p);
        }
    }
    
    if( shape_points.size() < 3 ) return;
    
    shape.Set( &shape_points[0], shape_points.size());
    
	b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
	fixtureDef.density = 1;
	fixtureDef.friction = 0.4;
	fixtureDef.restitution = 0.1f;
    body->SetUserData( (void*)OBJ_TYPE_GROUND );
	body->CreateFixture(&fixtureDef);
}

std::vector<std::string> RunScene::split(std::string str, std::string delim)
{
    unsigned start = 0;
    unsigned end;
    std::vector<std::string> v;
    
    while( (end = str.find(delim, start)) != std::string::npos )
    {
        v.push_back(str.substr(start, end-start));
        start = end + delim.length();
    }
    v.push_back(str.substr(start));
    return v;
}
