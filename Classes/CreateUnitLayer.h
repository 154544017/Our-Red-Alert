#ifndef __CREATEUNITLAYER_H__
#define __CREATEUNITLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Unit.h"
USING_NS_CC;

class CreateUnitLayer :public cocos2d::Layer
{
private:
	Point center_position;
public:	
	Sprite * rec = nullptr;
	Size rec_size;
	Point rec_center;
	Point rec_abs_center;
	bool building = false;
	bool can_build = true;
	bool is_rec_created = false;
	Layout * layout;
	UnitManager * unit_manager;
	virtual bool init() = 0;	
	Point findFreePosition();
	void setCenterPosition(Point p);
	void addListenerToRect(int type);
	//����Ǯ�������Ƿ��㹻����
	bool checkBuilding(int money, int power);
	//�����Ǯ�����죬��ִ�иú���������rec���ж��Ƿ���Դ���������򴴽�type�Ľ���
	void onBuilding(int type,Size size);
	bool checkInRange(Point p);
};

class BaseLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *militaryCamp;
	cocos2d::ui::Button *powerPlant;
	cocos2d::ui::Button *mine;
	cocos2d::ui::Button *tankFactary;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static BaseLayer * create();
};

class MilitaryCampLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *airplane;
	cocos2d::ui::Button *soldier;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static MilitaryCampLayer * create();

};

class TankFactaryLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *tank;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static TankFactaryLayer * create();
};
#endif
