#include "../../Classes/RunScene.h"
#import "../../Classes/PlayerObject.h"
#import "../../Classes/B2DebugDrawLayer.h"

#define PTM_RATIO 32

USING_NS_CC;

Scene* RunScene::createScene(std::string map_id)
{
    this->map_id = map_id;
    this->scene = Scene::create();
    this->main_layer = Layer::create();
    this->scene->addChild(main_layer,3);
    
    this->createAll();
    Director::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(RunScene::update),this,1.0f / 60.0f,false);
    this->IS_LOADED = true;
    return scene;
}

void RunScene::createAll()
{
    map_layer = Layer::create();
    this->scene->addChild(map_layer,2);
    
    bg_layer = Layer::create();
    this->scene->addChild(bg_layer,1);

    screen_visible_size = Director::getInstance()->getVisibleSize();
    screen_origin_size = Director::getInstance()->getVisibleOrigin();

    this->createControls();
    this->createMap();
    this->createPlayer();
}


void RunScene::createMap()
{
    std::ostringstream stringStream;
    stringStream << this->map_id << ".tmx";
    std::string map_file = stringStream.str();

    stringStream.str(""); stringStream.clear();
    stringStream << this->map_id << "_bg.tmx";
    std::string bg_file = stringStream.str();
    
    physic_world = new b2World( b2Vec2(0.0f, -20.0f) );
    map_layer->addChild(B2DebugDrawLayer::create(physic_world, PTM_RATIO), 9999);
    
    
    // load tiled map
    this->map_tile = TMXTiledMap::create( map_file );
    this->map_layer->addChild(map_tile,10);
    
    // load tiles bg
    this->bg_tile = TMXTiledMap::create( bg_file );
    bg_tile->setScale(3.0F);
    this->bg_layer->addChild(bg_tile,10);
   

    
    
    // get xy-offset
    map_tmx_offset_y = map_tile->getTileSize().height * map_tile->getMapSize().height / PTM_RATIO ;
    map_tmx_offset_x = map_tile->getTileSize().width  * map_tile->getMapSize().width / PTM_RATIO ;
    
    // read tiled map structure to str
    long size = 0;
    std::string str;
    std::string tmx_full_path = CCFileUtils::sharedFileUtils()->fullPathForFilename( map_file );
    unsigned char * posdataLoc = CCFileUtils::sharedFileUtils() -> getFileData( tmx_full_path.c_str(), "r", &size );
    str.append(reinterpret_cast<const char*>(posdataLoc));
    
    // parse readed xml map data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(str.c_str());
    
    CCLOG("XML READ: %s",result.description());

    // get all object groups
    for (pugi::xml_node object_group = doc.child("map").child("objectgroup"); object_group; object_group = object_group.next_sibling("objectgroup"))
    {
        std::string object_type = object_group.attribute("name").value();
        
        if ( object_type == "ground" || object_type == "rock" )
            for (pugi::xml_node tool = object_group.child("object"); tool; tool = tool.next_sibling("object"))
            {
                this->makePhysicPoligonGround(tool,0);
            }
    }
}

void RunScene::createPlayer()
{
    player_object = new PlayerObject();
    player_object->init(600,1000);
    player_object->initPhysic(physic_world);
    this->map_layer->addChild(player_object->spriteSheet);
}

void RunScene::createControls()
{
    auto jump_item = MenuItemImage::create(
                                           "button-jump.png",
                                           "button-jump-select.png",
                                           CC_CALLBACK_1(RunScene::buttonJumpCallback, this));
    
	jump_item->setPosition(Point(screen_origin_size.x + screen_visible_size.width - jump_item->getContentSize().width/2 ,
                                 screen_origin_size.y + jump_item->getContentSize().height/2));

    auto contol_menu = Menu::create(jump_item, NULL);
    contol_menu->setPosition(Point::ZERO);
    this->main_layer->addChild(contol_menu, 999);
}

void RunScene::update(float dt)
{
    if( ! IS_LOADED ) return;
    
    int velocityIterations = 8;
	int positionIterations = 1;
    
    physic_world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body* b = physic_world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
//			//Sprite *myActor = (CCSprite*)b->GetUserData();
//            //myActor->setPosition( b->GetPosition().x * PTM_RATIO , b->GetPosition().y * PTM_RATIO);
//			//myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
    player_object->reDraw();

    float player_x = this->player_object->player_sprite->getPositionX();
    float player_y = this->player_object->player_sprite->getPositionY();
    
    float mapOffset_x = screen_visible_size.width - player_x - screen_visible_size.width / 2;
    float mapOffset_y = screen_visible_size.height - player_y - screen_visible_size.height / 2;
    
    

    this->map_layer->setPositionX( round( mapOffset_x - 0.5f ) );
    this->map_layer->setPositionY( round( mapOffset_y - 0.5f ) );
    
    this->bg_layer->setPositionX( round( mapOffset_x / 5.0f - 0.5f ) );
    this->bg_layer->setPositionY( -1 * round( mapOffset_y / 10.0f - 0.5f )  - this->screen_visible_size.height / 3 );

}

void RunScene::buttonJumpCallback(Object* pSender)
{
    player_object->jump();
}

void RunScene::makePhysicPoligonGround(pugi::xml_node tool,int mode)
{
	b2BodyDef bodyDef;
    bodyDef.position.Set( 0 / PTM_RATIO, 0 / PTM_RATIO );
	b2Body *body = physic_world->CreateBody(&bodyDef);
    
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
	fixtureDef.restitution = 0.0f;
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
