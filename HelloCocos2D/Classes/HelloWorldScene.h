#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

#include "CCSAXParser.h"


class HelloWorld : public cocos2d::CCLayer,cocos2d::CCSAXDelegator
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	void fileOpt();

	void createXmlFile();
	void parseXmlFile();

	virtual void startElement(void *ctx, const char *name, const char **atts);
	virtual void endElement(void *ctx, const char *name);
	virtual void textHandler(void *ctx, const char *s, int len);

	void saxDelegatorParse();

	void createTinyXmlFile();
	void parseTinyXmlFile();

	void readJsonCpp();
	void writeJsonCpp();

	void readLibJson();
	void writeLibJson();

	void testEncodeBase64();
	void testMD5();
	void testSha1();
};

#endif  // __HELLOWORLD_SCENE_H__