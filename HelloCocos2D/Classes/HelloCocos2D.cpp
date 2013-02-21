#include "HelloCocos2D.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* HelloCocos2D::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelloCocos2D *layer = HelloCocos2D::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloCocos2D::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayer::init());

		CCMenuItemLabel * cocos2dItem = CCMenuItemLabel::create(CCLabelTTF::create("HelloWorld","Arial",20),this,menu_selector(HelloCocos2D::menuCloseCallback));
		cocos2dItem->setPosition(ccp(200,200));
		CCMenu* menu = CCMenu::createWithItem(cocos2dItem);
		menu->setPosition(CCPointZero);
		this->addChild(menu);
		
		bRet = true;
	} while (0);

	return bRet;
}

void HelloCocos2D::menuCloseCallback(CCObject* pSender)
{
	CCScene* helloworld = HelloWorld::scene();
	CCScene* replaceScreen = CCTransitionSlideInL::create(1.2f, helloworld);
	CCDirector::sharedDirector()->replaceScene(replaceScreen);
}
