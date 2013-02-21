#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "HelloMap.h"
#include "HelloMusic.h"
#ifdef CC_ENABLE_BOX2D_INTEGRATION
#include "HelloBox2D.h"
#endif
#if CC_ENABLE_CHIPMUNK_INTEGRATION
#include "HelloChipmunk.h"
#endif
#include "HelloPhysicsEditor.h"
#include "HelloUI.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	CCScene *pScene = NULL;
    // create a scene. it's an autorelease object
    //pScene = HelloWorld::scene();
	//pScene = HelloMap::scene();
	//pScene = HelloMusic::scene();
#ifdef CC_ENABLE_BOX2D_INTEGRATION
	//pScene = HelloBox2D::scene();
#endif
#ifdef CC_ENABLE_CHIPMUNK_INTEGRATION
	//pScene = HelloChipmunk::scene();
#endif	
	//pScene = HelloPhysicsEditor::scene();
	pScene = HelloUI::scene();
    // run
    pDirector->runWithScene(pScene);
    return true;
} 

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
