#ifndef __HELLOTEST_SCENE_H__
#define __HELLOTEST_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

class HelloTest : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloTest);

	void menuCallback(CCObject * pSender);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void onExit();

private:
	CCMenu* m_pItmeMenu;
	CCPoint m_tBeginPos;
	int m_nTestCount;
	unsigned int m_nSoundId;
};

#endif  // __HELLOWORLD_SCENE_H__