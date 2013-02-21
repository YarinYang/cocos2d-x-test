//
//  GB2ShapeCache-x.cpp
//  
//  Loads physics sprites created with http://www.PhysicsEditor.de
//  To be used with cocos2d-x
//
//  Generic Shape Cache for box2d
//
//  Created by Thomas Broquist
//
//      http://www.PhysicsEditor.de
//      http://texturepacker.com
//      http://www.code-and-web.de
//  
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
 
#include "GB2ShapeCache-x.h"
#include "Box2D/Box2D.h"
#include "cocoa/CCNS.h"
 
using namespace cocos2d;
 
/**
 * Internal class to hold the fixtures
 */
class FixtureDef {
public:
    FixtureDef()
    : next(NULL) {}
   
    ~FixtureDef() {
        delete next;
        delete fixture.shape;
    }
   
    FixtureDef *next;
    b2FixtureDef fixture;
    int callbackData;
};
 
class BodyDef {
public:
        BodyDef()
        : fixtures(NULL) {}
       
        ~BodyDef() {
                if (fixtures)
                        delete fixtures;
        }
       
        FixtureDef *fixtures;
        CCPoint anchorPoint;
};
 
static GB2ShapeCache *_sharedGB2ShapeCache = NULL;
 
GB2ShapeCache* GB2ShapeCache::sharedGB2ShapeCache(void) {
        if (!_sharedGB2ShapeCache) {
                _sharedGB2ShapeCache = new GB2ShapeCache();
        _sharedGB2ShapeCache->init();
        }
       
        return _sharedGB2ShapeCache;
}
 
bool GB2ShapeCache::init() {
        return true;
}
 
void GB2ShapeCache::reset() {
        std::map<std::string, BodyDef *>::iterator iter;
        for (iter = shapeObjects.begin() ; iter != shapeObjects.end() ; ++iter) {
                delete iter->second;
        }
        shapeObjects.clear();
}
 
void GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape) {
        std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
        assert(pos != shapeObjects.end());
       
        BodyDef *so = (*pos).second;
 
        FixtureDef *fix = so->fixtures;
    while (fix) {
        body->CreateFixture(&fix->fixture);
        fix = fix->next;
    }
}
 
cocos2d::CCPoint GB2ShapeCache::anchorPointForShape(const std::string &shape) {
        std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find(shape);
        assert(pos != shapeObjects.end());
       
        BodyDef *bd = (*pos).second;
        return bd->anchorPoint;
}
 
//typedef CCDictionary<std::string, CCObject*> ObjectDict;
 
void GB2ShapeCache::addShapesWithFile(const std::string &plist) {
        //const char *fullName = CCFileUtils::fullPathFromRelativePath(plist.c_str());
        //ObjectDict *dict = CCFileUtils::dictionaryWithContentsOfFile(fullName);
        const char *fullName = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(plist.c_str());
        CCDictionary *dict = CCDictionary::createWithContentsOfFile(fullName);
        CCAssert(dict != NULL, "Shape-file not found"); // not triggered - cocos2dx delivers empty dict if non was found
    CCAssert(dict->count() != 0, "plist file empty or not existing");
       
        //ObjectDict *metadataDict = (ObjectDict *)dict->objectForKey("metadata");
        //int format = static_cast<CCString *>(metadataDict->objectForKey("format"))->toInt();
        //ptmRatio = static_cast<CCString *>(metadataDict->objectForKey("ptm_ratio"))->toFloat();
        CCDictionary *metadataDict = (CCDictionary *)dict->objectForKey("metadata");
    int format = static_cast<CCString *>(metadataDict->objectForKey("format"))->intValue();
        ptmRatio = static_cast<CCString *>(metadataDict->objectForKey("ptm_ratio"))->floatValue();
        CCAssert(format == 1, "Format not supported");
 
        //ObjectDict *bodyDict = (ObjectDict *)dict->objectForKey("bodies");
        CCDictionary *bodyDict = (CCDictionary *)dict->objectForKey("bodies");
 
    b2Vec2 vertices[b2_maxPolygonVertices];
 
        //ObjectDict::CCObjectMapIter iter;     //ËÆºõÃ»ÓÃ
       
        //bodyDict->begin();
        std::string bodyName;
        //ObjectDict *bodyData;
        //CCDictionary *bodyData;
        CCDictElement* elem=NULL;
        CCDICT_FOREACH(bodyDict, elem){
                bodyName=elem->getStrKey();
                CCDictionary* bodyData=(CCDictionary*)elem->getObject();
                BodyDef *bodyDef=new BodyDef();
                bodyDef->anchorPoint = CCPointFromString(static_cast<CCString *>(bodyData->objectForKey("anchorpoint"))->getCString());
                //CCDictionary* fixtureList=(CCDictionary*)bodyData->objectForKey("fixtures");
                CCArray* fixtureList=(CCArray*)bodyData->objectForKey("fixtures");
                FixtureDef** nextFixtureDef=&(bodyDef->fixtures);
                CCObject* fixture=NULL;
                CCARRAY_FOREACH(fixtureList, fixture){
                        CCDictionary* fixtureDict=(CCDictionary*)fixture;
                        b2FixtureDef basicData;
                        basicData.filter.categoryBits=static_cast<CCString*>(fixtureDict->objectForKey("filter_categoryBits"))->intValue();
                        basicData.filter.maskBits=static_cast<CCString*>(fixtureDict->objectForKey("filter_maskBits"))->intValue();
                        basicData.filter.groupIndex=static_cast<CCString*>(fixtureDict->objectForKey("filter_groupIndex"))->intValue();
                        basicData.friction=static_cast<CCString*>(fixtureDict->objectForKey("friction"))->floatValue();
                        basicData.density=static_cast<CCString*>(fixtureDict->objectForKey("density"))->floatValue();
                        basicData.restitution=static_cast<CCString*>(fixtureDict->objectForKey("restitution"))->floatValue();
                        basicData.isSensor=(bool)static_cast<CCString*>(fixtureDict->objectForKey("isSensor"))->intValue();
 
                        CCString* cb=static_cast<CCString*>(fixtureDict->objectForKey("userdataCbValue"));
                        int callbackData=0;
                        if(cb)
                                callbackData=cb->intValue();
 
                        std::string fixtureType=static_cast<CCString*>(fixtureDict->objectForKey("fixture_type"))->getCString();
                        if(fixtureType=="POLYGON"){
                                CCArray* polygonsArray=(CCArray*)fixtureDict->objectForKey("polygons");
                                CCObject* polygon;
                                CCARRAY_FOREACH(polygonsArray, polygon){
                                        CCArray* points=(CCArray*)polygon;
                                        FixtureDef* fix=new FixtureDef();
                                        fix->fixture=basicData;
                                        fix->callbackData=callbackData;
 
                                        b2PolygonShape* polyshape=new b2PolygonShape();
                                        int vindex=0;
                                        assert(points->count()<=b2_maxPolygonVertices); //vs. CCAssert?
                                        CCObject* pt=NULL;
                                        CCARRAY_FOREACH(points, pt){
                                                CCPoint offset=CCPointFromString(static_cast<CCString*>(pt)->getCString() );
                                                vertices[vindex].x=(offset.x/ptmRatio);
                                                vertices[vindex].y=(offset.y/ptmRatio);
                                                vindex++;
                                        }
                                        polyshape->Set(vertices, vindex);
                                        fix->fixture.shape=polyshape;
 
                                        // create a list
                                        *nextFixtureDef = fix;
                                        nextFixtureDef = &(fix->next);
                                }
                        }
                        else if(fixtureType=="CIRCLE"){
                                FixtureDef* fix=new FixtureDef();
                                fix->fixture=basicData;
                                fix->callbackData=callbackData;
 
                                CCDictionary* circleData=(CCDictionary*)fixtureDict->objectForKey("circle");
                                b2CircleShape* circleShape=new b2CircleShape();
                                circleShape->m_radius=static_cast<CCString*>(circleData->objectForKey("radius"))->floatValue()/ptmRatio;
                                CCPoint p=CCPointFromString(static_cast<CCString*>(circleData->objectForKey("position"))->getCString());
                                circleShape->m_p=b2Vec2(p.x/ptmRatio, p.y/ptmRatio);
                                fix->fixture.shape=circleShape;
 
                                // create a list
                                *nextFixtureDef = fix;
                                nextFixtureDef = &(fix->next);
                        }
                        else{
                                CCAssert(0, "Unknown fixtureType");
                        }
 
                        // add the body element to the hash
                        //shapeObjects[bodyName] = bodyDef;
                }
                shapeObjects[bodyName] = bodyDef;
        }
} //addShapesWithFile