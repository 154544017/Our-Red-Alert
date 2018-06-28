#ifndef __CREATEUNITLAYER_H__
#define __CREATEUNITLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Unit.h"
USING_NS_CC;

class Building;

/**
*������λ�˵��Ļ���,�̳���Layer
*/
class CreateUnitLayer :public cocos2d::Layer
{
private:
	Point center_position;
public:	
	/**
	*��������ʱ����ʾ����
	*/
	Sprite * rec = nullptr;
	/**
	*��ʾ���εĴ�С,����Ҫ���������Ĵ�С�仯
	*/
	Size rec_size;
	/**
	*��ʾ���ε�����λ��(�������Ļ,�����λ�ö�Ӧ)
	*/
	Point rec_center;
	/**
	*��ʾ���ε�����λ��(�������Ƭ��ͼ,�봴��λ�ö�Ӧ)
	*/
	Point rec_abs_center;
	/**
	*�˵�����Ӧ�Ľ���
	*/
	Building* center = nullptr;
	/**
	*��ʾ�õ�λ�����Ƿ񴴽�(��������Ǯ��������)
	*/
	bool building = false;
	/**
	*��ʾ�õ�λ�����Ƿ񴴽�(λ����������)
	*/
	bool can_build = true;
	/**
	*��ʾ����ʾ�����Ƿ񱻴���
	*/
	bool is_rec_created = false;
	/**
	*�˵�������,������Ӹ��ְ�ť
	*/
	Layout * layout;
	/**
	*����ʾ������Ӽ����¼�,�����������ʹ�����Ϣ
	*/
	void addListenerToRect(int type);
	/**
	*����Ǯ�������Ƿ��㹻����
	*/
	bool checkBuilding(int money, int power);
	/**
	*�����Ǯ�����죬��ִ�иú���������rec���ж��Ƿ���Դ���������򴴽�type�Ľ���
	*/
	void onBuilding(int type,Size size);
	/**
	*�ж���ʾ�����Ƿ��ڽ��췶Χ��
	*/
	bool checkInRange(Point p);

	UnitManager * unit_manager;
	virtual bool init() = 0;
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

	/**
	*���ô����˵�����ť�Ŀ�����
	*/
	void setEnable(bool able);
};

class MilitaryCampLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *dog;
	cocos2d::ui::Button *soldier;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static MilitaryCampLayer * create();

};

class TankFactaryLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *airplane;
	cocos2d::ui::Button *tank;
	cocos2d::ui::Button *exit;
public:
	virtual bool init();
	static TankFactaryLayer * create();
};
#endif
