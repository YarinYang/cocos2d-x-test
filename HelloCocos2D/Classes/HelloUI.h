#ifndef __HELLOUI_SCENE_H__
#define __HELLOUI_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;

class HelloUI : public cocos2d::CCLayer,public CCTableViewDelegate,public CCTableViewDataSource
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloUI);
};

#endif // __HELLOWORLD_SCENE_H__
