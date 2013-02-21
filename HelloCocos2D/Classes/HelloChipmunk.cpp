#include "HelloChipmunk.h"

using namespace cocos2d;
using namespace cocos2d::extension;

enum {
	kTagParentNode = 1,
};

enum {
	Z_PHYSICS_DEBUG = 100,
};

CCScene* HelloChipmunk::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelloChipmunk *layer = HelloChipmunk::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

HelloChipmunk::HelloChipmunk()
{

}
HelloChipmunk::~HelloChipmunk()
{
	// manually Free rogue shapes
	for( int i=0;i<4;i++) {
		cpShapeFree( m_pWalls[i] );
	}

	cpSpaceFree( m_pSpace );
}

// on "init" you need to initialize your instance
bool HelloChipmunk::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());


		// enable events
		setTouchEnabled(true);
		setAccelerometerEnabled(true);

		// title
		CCLabelTTF *label = CCLabelTTF::create("Multi touch the screen", "Marker Felt", 36);
		label->setPosition(ccp( 240, 320 - 30));
		this->addChild(label, -1);


		// init physics
		initPhysics();

		// Use batch node. Faster
		CCSpriteBatchNode *parent = CCSpriteBatchNode::create("blocks.png", 100);
		m_pSpriteTexture = parent->getTexture();

		addChild(parent, 0, kTagParentNode);

		addNewSpriteAtPosition(ccp(200,200));

		scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}

void HelloChipmunk::menuCloseCallback(CCObject* pSender)
{

}

void HelloChipmunk::initPhysics()
{
	// init chipmunk
	cpInitChipmunk();

	m_pSpace = cpSpaceNew();

	m_pSpace->gravity = cpv(0, -100);

	//
	// rogue shapes
	// We have to free them manually
	//
	// bottom
	m_pWalls[0] = cpSegmentShapeNew( m_pSpace->staticBody,
		cpv(0,0),
		cpv(480, 0), 0.0f);

	// top
	m_pWalls[1] = cpSegmentShapeNew( m_pSpace->staticBody, 
		cpv(0, 320),
		cpv(480, 320), 0.0f);

	// left
	m_pWalls[2] = cpSegmentShapeNew( m_pSpace->staticBody,
		cpv(0,320),
		cpv(0,0), 0.0f);

	// right
	m_pWalls[3] = cpSegmentShapeNew( m_pSpace->staticBody, 
		cpv(480, 320),
		cpv(480, 0), 0.0f);

	for( int i=0;i<4;i++) {
		m_pWalls[i]->e = 1.0f;
		m_pWalls[i]->u = 1.0f;
		cpSpaceAddStaticShape(m_pSpace, m_pWalls[i] );
	}

	// Physics debug layer
	m_pDebugLayer = CCPhysicsDebugNode::create(m_pSpace);
	this->addChild(m_pDebugLayer, Z_PHYSICS_DEBUG);
}

void HelloChipmunk::update(float delta)
{
	// Should use a fixed size step based on the animation interval.
	int steps = 2;
	float dt = CCDirector::sharedDirector()->getAnimationInterval()/(float)steps;

	for(int i=0; i<steps; i++){
		cpSpaceStep(m_pSpace, dt);
	}
}

void HelloChipmunk::addNewSpriteAtPosition(CCPoint pos)
{
	int posx, posy;

	CCNode *parent = getChildByTag(kTagParentNode);

	posx = CCRANDOM_0_1() * 200.0f;
	posy = CCRANDOM_0_1() * 200.0f;

	posx = (posx % 4) * 85;
	posy = (posy % 3) * 121;


	int num = 4;
	cpVect verts[] = {
		cpv(-24,-54),
		cpv(-24, 54),
		cpv( 24, 54),
		cpv( 24,-54),
	};

	cpBody *body = cpBodyNew(1.0f, cpMomentForPoly(1.0f, num, verts, cpvzero));

	body->p = cpv(pos.x, pos.y);
	cpSpaceAddBody(m_pSpace, body);

	cpShape* shape = cpPolyShapeNew(body, num, verts, cpvzero);
	shape->e = 0.5f; shape->u = 0.5f;
	cpSpaceAddShape(m_pSpace, shape);

	CCPhysicsSprite *sprite = CCPhysicsSprite::createWithTexture(m_pSpriteTexture, CCRectMake(posx, posy, 85, 121));
	parent->addChild(sprite);

	sprite->setBody(body);
	sprite->setPosition(pos);
}

void HelloChipmunk::onEnter()
{
	CCLayer::onEnter();
}

void HelloChipmunk::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	//Add a new body/atlas sprite at the touched location
	CCSetIterator it;
	CCTouch* touch;

	for( it = touches->begin(); it != touches->end(); it++) 
	{
		touch = (CCTouch*)(*it);

		if(!touch)
			break;

		CCPoint location = touch->getLocation();

		addNewSpriteAtPosition( location );
	}
}