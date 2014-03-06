#include "../../Classes/AppDelegate.h"
#include "../../Classes/RunScene.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto eglView = EGLView::getInstance();
    director->setOpenGLView(eglView);
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);
    
CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
    
    auto run_scene = new RunScene();
    director->runWithScene(run_scene->createScene("map_01"));
    
    return true;
}


// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

}
