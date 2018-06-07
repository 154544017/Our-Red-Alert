#pragma once
#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "cocos2d.h"
#include "Unit.h"

USING_NS_CC;
//��������,�̳���Unit
class Building : public Unit
{
private:
	int id;
	int type;
public:
	Layer * getLayer();
	TMXTiledMap* getMap();
	static Building* create(const std::string & filename);
	//type��ʾ����������ͣ�0Ϊ���أ�1Ϊ��Ӫ����ͬ��ļ��������ò�ͬ�Ĳ˵�
	void setType(int);
	int getType();
	void  addListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};





#endif // !__BUILDING_H__

