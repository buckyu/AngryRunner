#ifndef __HelloCpp__RunScene__
#define __HelloCpp__RunScene__

#include "cocos2d.h"

class RunScene : public cocos2d::Layer
{
    public:
        static cocos2d::Scene* createScene();
        virtual bool init();
        CREATE_FUNC(RunScene);
};

#endif 
