#include "RunScene.h"
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

    return true;
}
