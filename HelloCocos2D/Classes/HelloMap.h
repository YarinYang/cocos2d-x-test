#ifndef _HELLOMAP_H_
#define _HELLOMAP_H_

#include "cocos2d.h"

using namespace cocos2d;

class HelloMap : public cocos2d::CCLayer
{
public:
	CCTMXTiledMap* _mapTild;
	CCSprite* _spriteNpc;
	CCPoint _mapOffset;
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCallback(CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloMap);
};

#endif