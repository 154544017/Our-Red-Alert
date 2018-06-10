#ifndef __CREATEUNITLAYER_H__
#define __CREATEUNITLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Unit.h"
USING_NS_CC;

class CreateUnitLayer :public cocos2d::Layer
{
private:
	
public:	
	Sprite * rec = nullptr;
	Size rec_size;
	Point rec_center;
	Point rec_abs_center;
	bool building = false;
	bool can_build = true;
	Layout * layout;
	UnitManager * unit_manager;
	virtual bool init() = 0;	
	void addListenerToRect(int type);
	//�����Ǯ�����죬��ִ�иú���������rec���ж��Ƿ���Դ���������򴴽�type�Ľ���
	void onBuilding(int type);
};

class BaseLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *Military;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static BaseLayer * create();
};

class MilitaryCampLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *airplane;
	cocos2d::ui::Button *tank;
	cocos2d::ui::Button *soldier;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	Point findFreePosition();
	static MilitaryCampLayer * create();

};
#endif
