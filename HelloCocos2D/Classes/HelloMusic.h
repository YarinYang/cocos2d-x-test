#ifndef _HELLOMUSIC_H_
#define _HELLOMUSIC_H_

#include "cocos2d.h"

#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

class HelloMusic : public cocos2d::CCLayer
{
private:
	CCMenu* m_pItmeMenu;
	CCPoint m_tBeginPos;
	int m_nTestCount;
	unsigned int m_nSoundId;
public:
	HelloMusic();
	~HelloMusic();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCallback(CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloMusic);

	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void onExit();
};

#endif