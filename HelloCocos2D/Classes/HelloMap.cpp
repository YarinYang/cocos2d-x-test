#include "HelloMap.h"

#define MAPTILESIZE 33
typedef enum _CONTROLTAG{
	TAG_UP = 100,
	TAG_DOWN,TAG_LEFT,TAG_RIGHT,TAG_FIRE
}CONTROLTAG;


using namespace cocos2d;

CCScene* HelloMap::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelloMap *layer = HelloMap::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloMap::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		CC_BREAK_IF(! CCLayer::init());
		_mapTild = CCTMXTiledMap::create("tmw_desert_spacing.tmx");
		this->addChild(_mapTild);
		_mapTild->setPosition(ccp(0,0));

		CCMenuItemFont* upMenuItem = CCMenuItemFont::create("UP",this,menu_selector(HelloMap::menuCallback));
		CCMenuItemFont* downMenuItem = CCMenuItemFont::create("DOWN",this,menu_selector(HelloMap::menuCallback));
		CCMenuItemFont* leftMenuItem = CCMenuItemFont::create("LEFT",this,menu_selector(HelloMap::menuCallback));
		CCMenuItemFont* rightMenuItem = CCMenuItemFont::create("RIGHT",this,menu_selector(HelloMap::menuCallback));
		CCMenuItemFont* fireMenuItem = CCMenuItemFont::create("FIRE",this,menu_selector(HelloMap::menuCallback));
		upMenuItem->setTag(TAG_UP);
		downMenuItem->setTag(TAG_DOWN);
		leftMenuItem->setTag(TAG_LEFT);
		rightMenuItem->setTag(TAG_RIGHT);
		fireMenuItem->setTag(TAG_FIRE);

		upMenuItem->setPosition(ccp(screenSize.width/2,screenSize.height-30));
		downMenuItem->setPosition(ccp(screenSize.width/2,30));
		leftMenuItem->setPosition(ccp(50,screenSize.height/2));
		rightMenuItem->setPosition(ccp(screenSize.width-50,screenSize.height/2));
		fireMenuItem->setPosition(ccp(50,screenSize.height-30));

		CCMenu *menu = CCMenu::create(upMenuItem,downMenuItem,leftMenuItem,rightMenuItem,fireMenuItem,NULL);
		menu->setPosition(CCPointZero);

		this->addChild(menu);

		_spriteNpc = CCSprite::create("SpinningPeas.png");
		_mapTild->addChild(_spriteNpc);
		_spriteNpc->setPosition(ccp(MAPTILESIZE/2,MAPTILESIZE/2));
		

		bRet = true;
	} while (0);

	return bRet;
}

void HelloMap::menuCallback(CCObject* pSender)
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int tagItem = pMenuItem->getTag();
	CCPoint spriteCurPos = ccp((int)(_spriteNpc->getPositionX()/MAPTILESIZE),_mapTild->getMapSize().height-1-(int)(_spriteNpc->getPositionY()/MAPTILESIZE));
	CCPoint spriteNextPos = spriteCurPos;
	CCTMXLayer *ly = _mapTild->layerNamed("layer2");
	unsigned int gid = 0;
	switch ( tagItem )
	{
	case TAG_UP:
		spriteNextPos = ccp(spriteNextPos.x,spriteNextPos.y-1);
		if ( spriteNextPos.y < 0 )return;
		gid = ly->tileGIDAt(spriteNextPos);
		if ( gid ) return;
		if ( _mapTild->getContentSize().height+_mapTild->getPositionY()>320+MAPTILESIZE )
		{
			if ( _spriteNpc->getPositionY()+_mapTild->getPositionY()>160 )
			{
				_mapTild->setPositionY(_mapTild->getPositionY()-MAPTILESIZE);
				_mapOffset.y -= MAPTILESIZE;
			}
			_spriteNpc->setPositionY(_spriteNpc->getPositionY()+MAPTILESIZE);
		}
		else if ( _spriteNpc->getPositionY()<_mapTild->getContentSize().height-MAPTILESIZE )
		{
			_spriteNpc->setPositionY(_spriteNpc->getPositionY()+MAPTILESIZE);
		}
		break;
	case TAG_DOWN:
		spriteNextPos = ccp(spriteNextPos.x,spriteNextPos.y+1);
		if ( spriteNextPos.y > _mapTild->getMapSize().height-1 )return;
		gid = ly->tileGIDAt(spriteNextPos);
		if ( gid ) return;
		if ( _mapTild->getPositionY()<0 )
		{
			if ( _spriteNpc->getPositionY()+_mapTild->getPositionY()<160 )
			{
				_mapTild->setPositionY(_mapTild->getPositionY()+MAPTILESIZE);
				_mapOffset.y += MAPTILESIZE;
			}
			_spriteNpc->setPositionY(_spriteNpc->getPositionY()-MAPTILESIZE);
		}
		else if ( _spriteNpc->getPositionY()>MAPTILESIZE )
		{
			_spriteNpc->setPositionY(_spriteNpc->getPositionY()-MAPTILESIZE);
		}
		break;
	case TAG_LEFT:
		spriteNextPos = ccp(spriteNextPos.x-1,spriteNextPos.y);
		if ( spriteNextPos.x < 0 )return;
		gid = ly->tileGIDAt(spriteNextPos);
		if ( gid ) return;
		if ( _mapTild->getPositionX()<0 )
		{
			if ( _spriteNpc->getPositionX()+_mapTild->getPositionX()<240 )
			{
				_mapTild->setPositionX(_mapTild->getPositionX()+MAPTILESIZE);
				_mapOffset.x += MAPTILESIZE;
			}
			_spriteNpc->setPositionX(_spriteNpc->getPositionX()-MAPTILESIZE);
		}
		else if(_spriteNpc->getPositionX()>MAPTILESIZE)
		{
			_spriteNpc->setPositionX(_spriteNpc->getPositionX()-MAPTILESIZE);
		}
		break;
	case TAG_RIGHT:
		spriteNextPos = ccp(spriteNextPos.x+1,spriteNextPos.y);
		if ( spriteNextPos.x > _mapTild->getMapSize().width-1 )return;
		gid = ly->tileGIDAt(spriteNextPos);
		if ( gid ) return;
		if ( _mapTild->getContentSize().width+_mapTild->getPositionX()>480+MAPTILESIZE )
		{
			if ( _spriteNpc->getPositionX()+_mapTild->getPositionX()>240 )
			{
				_mapTild->setPositionX(_mapTild->getPositionX()-MAPTILESIZE);
				_mapOffset.x -= MAPTILESIZE;
			}
			_spriteNpc->setPositionX(_spriteNpc->getPositionX()+MAPTILESIZE);
		}
		else if ( _spriteNpc->getPositionX()<_mapTild->getContentSize().width-MAPTILESIZE )
		{
			_spriteNpc->setPositionX(_spriteNpc->getPositionX()+MAPTILESIZE);
		}
		break;
	case TAG_FIRE:
		ly->setTileGID(46,spriteCurPos);
		break;
	}
	

}