#ifndef _HELLOCHIPMUNK_H_
#define _HELLOCHIPMUNK_H_
#include "cocos2d.h"
#include "chipmunk.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class HelloChipmunk : public cocos2d::CCLayer
{
public:
	HelloChipmunk();
	~HelloChipmunk();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloChipmunk);
	void onEnter();
	void initPhysics();

	void addNewSpriteAtPosition(CCPoint p);
	void update(float dt);
	virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);

private:
	CCTexture2D* m_pSpriteTexture; // weak ref
	CCPhysicsDebugNode* m_pDebugLayer; // weak ref
	cpSpace* m_pSpace; // strong ref
	cpShape* m_pWalls[4];
};


#endif