#pragma once
#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "cocos2d.h"
#include "Unit.h"
#include "CreateUnitLayer.h"

USING_NS_CC;
class CombatScene;

/**
*�������࣬�̳���Unit
*/
class Building : public Unit{
private:
	void setProperties() = 0;
public:
	bool layer_is_created = false;
	Point createPosition = Point(0, 0);
	/**
	*���ڲ��Ŷ����Ľ�������
	*/
	Sprite * building;
	/**
	*������ǰ״̬:0->����������״̬;1->��������;2->��ǰ��λ��������
	*/
	int state = 0;
	/**
	*��ǰ������λ������
	*/
	int cur_prod;
	/**
	*��ǰ������λ��������ʱ��
	*/
	int prod_period;
	/**
	*������ǰ��λʱ�Ѿ����ĵ�ʱ��
	*/
	int prod_process;
	/**
	*��¼�����͵�λ������������ʱ��
	*/
	std::map<int, int> period_map;
	/**
	*��������
	*/
	std::vector<int> prod_list;
	/**
	*����������
	*/
	Bar* prod_bar = nullptr;
	/**
	*���������˵��Ŀɼ���
	*/
	void setLayerVisible(Layer *);
	/**
	*�������ݸ�����ռ�ø��
	*/
	void addToGmap(Point p) override;
	/**
	*Ϊ��ͬ�Ľ������䲻ͬ�ļ�����
	*/
	virtual void setListener();
	/**
	*����Ѫ��������������
	*/
	virtual void initBar();
	/**
	*���½���״̬
	*/
	virtual void update(float f);
	/**
	*��_unit_type������������(���ڷǽ���������)
	*/
	void startProduce(int _unit_type);
	/**
	*��_unit_type������������,����createPosition��Ϊ_point(�����λ��)(���ڽ���������)
	*/
	void startProduce(int _unit_type, Point _point);
	/**
	*���ظý�����Χδ��ռ�õ�λ��
	*/
	Point findFreePosition();
};

class Base : public Building
{
private:
	void setProperties() override;
public:	
	/**
	*�ɴ�����Χ�뾶
	*/
	float construct_range = 500;
	static Size size;	
	BaseLayer * baselayer = nullptr;

	/**
	*����baselayer�и���ť�Ŀ�����
	*/
	void setMenuEnable(bool able);

	static Base* create(const std::string & filename);	
	void update(float f);

	/**
	*��Base�������Ӽ�����(������¼�+�����˵�)
	*/
	void setListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

class MilitaryCamp : public Building
{
private:
	void setProperties() override;
	
public:
	static Size size;
	MilitaryCampLayer * militaryCampLayer = nullptr;

	static MilitaryCamp* create(const std::string & filename);

	/**
	*��MilitaryCamp�������Ӽ�����(������¼�+�����˵�)
	*/
	void setListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

class TankFactary : public Building
{
private:
	void setProperties() override;
public:
	static Size size;
	TankFactaryLayer * tankFactaryLayer = nullptr;

	static TankFactary* create(const std::string & filename);

	/**
	*��TankFactory�������Ӽ�����(������¼�+�����˵�)
	*/
	void setListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};

class Mine : public Building
{
private:
	void setProperties() override;;
public:
	static Size size;

	static Mine* create(const std::string & filename);

	/**
	*ÿ���Ǯ����������amount
	*/
	void setIncreasingAmount(int amount);
};

class PowerPlant : public Building
{
private:
	void setProperties() override;;
public:
	static Size size;

	static PowerPlant* create(const std::string & filename);

	/**
	*������󸺺�����delta
	*/
	void addMax_Power(int delta);
};

#endif // !__BUILDING_H__

