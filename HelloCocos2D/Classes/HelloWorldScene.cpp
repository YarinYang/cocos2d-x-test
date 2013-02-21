#include "HelloWorldScene.h"
#include "HelloCocos2D.h"

//FILE API
#include <iostream> 

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

#include "tinyxml.h"
#include "tinystr.h"

#include "json/json.h"

#include "libjson.h"

#include "crypto/CCCrypto.h"

#define MAPCELLSIZE 33

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());

		fileOpt();
		createXmlFile();
		parseXmlFile();
		saxDelegatorParse();
		createTinyXmlFile();
		parseTinyXmlFile();
		readJsonCpp();
		writeJsonCpp();
		writeLibJson();
		readLibJson();

		testEncodeBase64();
		testMD5();
		testSha1();
		bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::fileOpt()
{
	char filePath[1024]= {'\0'};
	memset(filePath,0,sizeof(filePath));
	strcat(filePath,CCFileUtils::sharedFileUtils()->getResourceDirectory());
	strcat(filePath,"README");
	FILE* fileRead = fopen(filePath,"rb+"); 

	memset(filePath,0,sizeof(filePath));
	strcat(filePath,CCFileUtils::sharedFileUtils()->getWriteablePath().c_str());
	strcat(filePath,"WRITEME");
	FILE* fileWirte = fopen(filePath,"wb+");

	char buff[200] = {'\0'};  
	while(fgets(buff, sizeof(buff), fileRead) != NULL)  
	{  
		fputs(buff, fileWirte);  
	} 

	fclose(fileRead);
	fclose(fileWirte);
}

void HelloWorld::createXmlFile()
{
	//�����ĵ��ͽڵ�ָ��
	xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
	xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");

	//���ø��ڵ�
	xmlDocSetRootElement(doc,root_node);

	//�ڸ��ڵ���ֱ�Ӵ����ڵ�
	xmlNewTextChild(root_node, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");
	xmlNewTextChild(root_node, NULL, BAD_CAST "newNode2", BAD_CAST "newNode2 content");
	xmlNewTextChild(root_node, NULL, BAD_CAST "newNode3", BAD_CAST "newNode3 content");

	//����һ���ڵ㣬���������ݺ����ԣ�Ȼ���������
	xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"node2");
	xmlNodePtr content = xmlNewText(BAD_CAST"NODE CONTENT");
	xmlAddChild(root_node,node);
	xmlAddChild(node,content);
	xmlNewProp(node,BAD_CAST"attribute",BAD_CAST "yes");

	//�����ӽڵ�
	node = xmlNewNode(NULL, BAD_CAST "son");
	xmlAddChild(root_node,node);

	xmlNodePtr grandson = xmlNewNode(NULL, BAD_CAST "grandson");
	xmlAddChild(node,grandson);
	xmlAddChild(grandson, xmlNewText(BAD_CAST "This is a grandson node"));

	char filePath[1024]= {'\0'};
	memset(filePath,0,sizeof(filePath));
	strcat(filePath,CCFileUtils::sharedFileUtils()->getWriteablePath().c_str());
	strcat(filePath,"CreatedXml.xml");

	//�洢xml�ĵ�
	int nRel = xmlSaveFile(filePath,doc);

	if (nRel != -1)
	{
		//nRel����ֽڱ��ɹ�д��
	}

	//�ͷ��ĵ��ڽڵ㶯̬������ڴ�
	xmlFreeDoc(doc);
}

void HelloWorld::parseXmlFile()
{
	xmlDocPtr doc;           //��������ĵ�ָ��
	xmlNodePtr curNode;      //������ָ��(����Ҫ��Ϊ���ڸ��������ƶ�)
	xmlChar *szKey;          //��ʱ�ַ�������

	char filePath[1024]= {'\0'};
	memset(filePath,0,sizeof(filePath));
	strcat(filePath,CCFileUtils::sharedFileUtils()->getWriteablePath().c_str());
	strcat(filePath,"CreatedXml.xml");

	doc = xmlReadFile(filePath,"GB2312",XML_PARSE_RECOVER); //�����ļ�
	if (NULL == doc)
	{  
		CCLOG("Document not parsed successfully.");  
		return ;
	}

	curNode = xmlDocGetRootElement(doc); //ȷ���ĵ���Ԫ��
	/*���ȷ�ϵ�ǰ�ĵ��а�������*/
	if (NULL == curNode)
	{
		CCLOG("empty document.");  
		xmlFreeDoc(doc);
		return ;
	}

	if (xmlStrcmp(curNode->name, BAD_CAST "root"))
	{
		CCLOG("document of the wrong type, root node != root");
		xmlFreeDoc(doc);
		return ;
	}

	curNode = curNode->xmlChildrenNode;
	xmlNodePtr propNodePtr = curNode;
	while(curNode != NULL)
	{
		if (!xmlStrcmp(curNode->name, BAD_CAST "newNode1"))
		{
			xmlNodePtr tempNode;
			tempNode = curNode->next;
			xmlUnlinkNode(curNode);
			xmlFreeNode(curNode);
			curNode = tempNode;
			continue;
		}

		//ȡ���ڵ��е�����
		if ((!xmlStrcmp(curNode->name, (const xmlChar *)"newNode1")))
		{
			szKey = xmlNodeGetContent(curNode);
			CCLOG("newNode1: %s",szKey);
			xmlFree(szKey);
		}
		//���Ҵ�������attribute�Ľڵ�
		if (xmlHasProp(curNode,BAD_CAST "attribute"))
		{
			propNodePtr = curNode;
		}

		curNode = curNode->next;
	}

	//��������
	xmlAttrPtr attrPtr = propNodePtr->properties;
	while (attrPtr != NULL)
	{
		if (!xmlStrcmp(attrPtr->name, BAD_CAST "attribute"))
		{
			xmlChar* szAttr = xmlGetProp(propNodePtr,BAD_CAST "attribute");
			CCLOG("get attribute = %s",szAttr);
			xmlFree(szAttr);
		}
		attrPtr = attrPtr->next;
	}

	xmlFreeDoc(doc);

}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCScene* helloCocos2d = CCTransitionJumpZoom::create(1.3f, HelloCocos2D::scene());
	CCDirector::sharedDirector()->replaceScene(helloCocos2d);
}
void HelloWorld::saxDelegatorParse()
{
	char filePath[1024]= {'\0'};
	memset(filePath,0,sizeof(filePath));
	strcat(filePath,CCFileUtils::sharedFileUtils()->getWriteablePath().c_str());
	strcat(filePath,"CreatedXml.xml");

	CCSAXParser *parser = new CCSAXParser();
	parser->setDelegator(this);
	parser->init("UTF-8");
	parser->parse(filePath);

	delete parser;
}


void HelloWorld::startElement(void *ctx, const char *name, const char **atts)
{
	if(atts){
		int i=0;
		while(atts[i])
		{
			std::string keystrN(atts[i]);
			std::string keystrV(atts[i+1]);
			i=i+2;  
		}
	}
}
void HelloWorld::endElement(void *ctx, const char *name)
{

}
void HelloWorld::textHandler(void *ctx, const char *s, int len)
{
	CC_UNUSED_PARAM(ctx);  
	std::string currString((char*)s,0,len);  
	CCLOG("s_len=%d",strlen(s));  
	CCLOG(currString.c_str());
}


void HelloWorld::createTinyXmlFile()
{
	try
	{
		//����һ��XML���ĵ�����
		TiXmlDocument *myDocument = new TiXmlDocument();
		//����һ����Ԫ�ز����ӡ�
		TiXmlElement *RootElement = new TiXmlElement("Persons");
		myDocument->LinkEndChild(RootElement);
		//����һ��PersonԪ�ز����ӡ�
		TiXmlElement *PersonElement = new TiXmlElement("Person");
		RootElement->LinkEndChild(PersonElement);
		//����PersonԪ�ص����ԡ�
		PersonElement->SetAttribute("ID", "1");
		//����nameԪ�ء�ageԪ�ز����ӡ�
		TiXmlElement *NameElement = new TiXmlElement("name");
		TiXmlElement *AgeElement = new TiXmlElement("age");
		PersonElement->LinkEndChild(NameElement);
		PersonElement->LinkEndChild(AgeElement);
		//����nameԪ�غ�ageԪ�ص����ݲ����ӡ�
		TiXmlText *NameContent = new TiXmlText("Yarin");
		TiXmlText *AgeContent = new TiXmlText("24");
		NameElement->LinkEndChild(NameContent);
		AgeElement->LinkEndChild(AgeContent);
		
		char filePath[1024]= {'\0'};
		memset(filePath,0,sizeof(filePath));
		strcat(filePath,CCFileUtils::sharedFileUtils()->getWriteablePath().c_str());
		strcat(filePath,"CreatedTinyXml.xml");

		myDocument->SaveFile(filePath);//���浽�ļ�
	}
	catch (string& e)
	{
		return ;
	}
	return ;
}


void HelloWorld::parseTinyXmlFile()
{
	try
	{
		char filePath[1024]= {'\0'};
		memset(filePath,0,sizeof(filePath));
		strcat(filePath,CCFileUtils::sharedFileUtils()->getWriteablePath().c_str());
		strcat(filePath,"CreatedTinyXml.xml");

		//����һ��XML���ĵ�����
		TiXmlDocument *myDocument = new TiXmlDocument(filePath);
		myDocument->LoadFile();
		//��ø�Ԫ�أ���Persons��
		TiXmlElement *RootElement = myDocument->RootElement();
		//�����Ԫ�����ƣ������Persons��
		CCLOG(RootElement->Value());
		//��õ�һ��Person�ڵ㡣
		TiXmlElement *FirstPerson = RootElement->FirstChildElement();
		//��õ�һ��Person��name�ڵ��age�ڵ��ID���ԡ�
		TiXmlElement *NameElement = FirstPerson->FirstChildElement();
		TiXmlElement *AgeElement = NameElement->NextSiblingElement();
		TiXmlAttribute *IDAttribute = FirstPerson->FirstAttribute();
		//�����һ��Person��name���ݣ��������ǣ�age���ݣ�����ID���ԣ�����

		CCLOG(NameElement->FirstChild()->Value());
		CCLOG(AgeElement->FirstChild()->Value());
		CCLOG(IDAttribute->Value());
	}
	catch (string& e)
	{
		return ;
	}
	return ;
}

void HelloWorld::readJsonCpp()
{
	unsigned long size; 
	char* file = (char*)CCFileUtils::sharedFileUtils()->getFileData("testjson.json","r", &size);
	
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(std::string(file),root, false))
	{
		return ;
	}

	std::string name = root["name"].asString();
	int age = root["age"].asInt();

	CCLOG("name=%s",name.c_str());
	CCLOG("age=%d",age);

	////////////////////////////////////
	file = (char*)CCFileUtils::sharedFileUtils()->getFileData("testjson2.json","r", &size);
	if (!reader.parse(std::string(file),root, false))
	{
		return ;
	}

	size = root.size();
	for (int i=0; i<size; ++i)
	{
		name = root[i]["name"].asString();
		age = root[i]["age"].asInt();
		CCLOG("name=%s,age=%d",name.c_str(),age);
	}
}
void HelloWorld::writeJsonCpp()
{
	Json::Value root;
	Json::FastWriter writer;
	Json::Value person;

	person["name"] = "hello world";
	person["age"] = 100;
	root.append(person);

	std::string json_file = writer.write(root);


	char filePath[1024]= {'\0'};
	memset(filePath,0,sizeof(filePath));
	strcat(filePath,CCFileUtils::sharedFileUtils()->getWriteablePath().c_str());
	strcat(filePath,"writeJsonCpp.json");

	FILE* file = fopen(filePath,"w+");

	fwrite(json_file.c_str(),json_file.size(),1,file);

	fclose(file);
}

void HelloWorld::readLibJson()
{
	//read json
	unsigned long size; 
	char* str = (char*)CCFileUtils::sharedFileUtils()->getFileData("testlibjson.json","r", &size);

	if(libjson::is_valid(str)==false)  {   
		delete str;   
		str=NULL;   
		printf("Parse faild!\n");   
		system("pause");   
		exit(0); 
	}  
	JSONNode rn=libjson::parse(str);
	delete str;  
	str=NULL;  
	int tmp = rn.size();
	CCLOG("%d",tmp);
	for (int i=0;i<rn[1].size();i++)
	{
		JSONNode temp=rn[1][i];
		for(int j=0;j<temp.size();j++)
		{
			CCLOG("%s:%s",temp[j].name().c_str(),temp[j].as_string().c_str());
		}
	}
}

void HelloWorld::writeLibJson()
{
	//write json
	JSONNode n(JSON_NODE);
	n.push_back(JSONNode("RootA", "Value in parent node"));
	JSONNode c(JSON_ARRAY);
	c.set_name("ChildNode");

	JSONNode c1(JSON_NODE),c2(JSON_NODE);
	c1.push_back(JSONNode("ChildA","String Value c1"));
	c1.push_back(JSONNode("ChildB","dsf c1"));
	c2.push_back(JSONNode("ChildA","String Value c2"));
	c2.push_back(JSONNode("ChildB","dsf c2"));

	c.push_back(c1);
	c.push_back(c2);
	n.push_back(c);
	CCLOG("==%s",n.write_formatted().c_str()); 

	unsigned long size; 
	char filePath[1024]= {'\0'};
	memset(filePath,0,sizeof(filePath));
	strcat(filePath,CCFileUtils::sharedFileUtils()->getWriteablePath().c_str());
	strcat(filePath,"testlibjson.json");


	FILE* file = fopen(filePath,"w+");

	fwrite(n.write_formatted().c_str(),n.write_formatted().size(),1,file);

	fclose(file);
}


void HelloWorld::testEncodeBase64()
{
	const char* plaintext = "Show me the money";
	int plaintextLength = strlen(plaintext);
	int bufferLength = plaintextLength * 2;
	char* buffer = static_cast<char*>(malloc(bufferLength));
	int encodedTextLength = CCCrypto::encodeBase64(plaintext, plaintextLength, buffer, bufferLength);

	CCLOG("\n");
	CCLOG("encodeBase64():\n");
	CCLOG("  plaintext: %s\n", plaintext);
	CCLOG("  plaintextLength: %d\n", plaintextLength);
	CCLOG("  encodedTextLength: %d\n", encodedTextLength);
	CCLOG("  encodedText: %s\n", buffer);

	CCLOG("decodeBase64():\n");
	char* plaintext2 = static_cast<char*>(malloc(encodedTextLength + 1));
	int plaintextLength2 = CCCrypto::decodeBase64(buffer, plaintext2, encodedTextLength + 1);
	CCLOG("  plaintext: %s\n", plaintext2);
	CCLOG("  plaintextLength: %d\n", plaintextLength2);
	CCLOG("  string compare: %s\n", strcmp(plaintext, plaintext2) == 0 ? "OK" : "ERROR");
	CCLOG("\n\n");
}

void HelloWorld::testMD5()
{
	const char* plaintext = "Show me the money";
	unsigned char buffer[CCCrypto::MD5_BUFFER_LENGTH];
	CCCrypto::MD5((void*)plaintext, strlen(plaintext), buffer);

	char md5str[33];
	md5str[32] = 0;
	int ci = 0;
	for (int i = 0; i < CCCrypto::MD5_BUFFER_LENGTH; ++i)
	{
		sprintf (&(md5str[i * 2]), "%02x", buffer[i]);
	}

	CCLOG("\n");
	CCLOG("MD5():\n");
	CCLOG("  input: %s\n", plaintext);
	CCLOG("  md5: %s\n", md5str);
	CCLOG("\n\n");
}
void HelloWorld::testSha1()
{
	const char* plaintext = "300000";
	int plaintextLength = strlen(plaintext);
	const char* keytext = "cocos2d-x";
	int keytextLength = strlen(plaintext);

	int bufferLength = 20;
	unsigned char* buffer = static_cast<unsigned char*>(malloc(bufferLength));
	CCCrypto::sha1((unsigned char*)(static_cast<const char*>(plaintext)),plaintextLength,(unsigned char*)(static_cast<const char*>(keytext)),keytextLength,buffer,bufferLength);
	
	while( bufferLength > 0 )
	{
		CCLOG( " %02x", *buffer );
		bufferLength--;
		buffer++;
	}
}
