#pragma once
#ifndef _UNIT_H_
#define _UNIT_H_

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class Unit;
class Bar;

class Bar :public DrawNode {
private:
	int timer = 0;//չʾѪ���ļ�ʱ��
	int display_time = 0;//����չʾʱ��
	float length = 36;//Ѫ����
	float width = 4;//Ѫ����
	bool is_visible = false;//�Ƿ���ʾ
	cocos2d::Color4F color{ 0.8, 0, 0, 0.8 };
public:
	//void updateHide(float f);
	void updateBarDisplay(float rate);
	void setLength(float _length) { length = _length; };
	void setColor(const cocos2d::Color4F& _color) { color = _color; };
	void keepVisible();
	void stopKeepingVisible();
	CREATE_FUNC(Bar);

};

class Unit :public cocos2d::Sprite {
protected:
	TMXTiledMap * addMap;
	Layer* addLayer;
	EventListenerTouchOneByOne * addTouchListener;
	bool can_move;
	int current_life=100;
	int max_life=100;
	int attack_frequency ;
	double attack_range ;
	int speed ;
	
public:
	//���õ�λ���ڵĲ㣬��ͼָ���Sprite������
	void set(TMXTiledMap *, Layer*, EventListenerTouchOneByOne *);
	void set_movable(bool);
	bool is_movable();
	Bar* hp_bar = nullptr;//��������λ����Ѫ��
	int id;
	int team;
	int z_index;
	//UnitOperator* unit_operator=nullptr;
	static Unit* create(const std::string & filename) {
		Unit *ret = new Unit();
		if (ret && ret->initWithFile(filename)) {
			ret->autorelease();
			if (ret->current_life) {
				ret->set_movable(true);
				ret->hp_bar = Bar::create();
				float rate = static_cast<float>(ret->current_life) / static_cast<float>(ret->max_life);
				ret->addChild(ret->hp_bar, 10);
				ret->hp_bar->updateBarDisplay(rate);
				ret->hp_bar->stopKeepingVisible();
			}
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	};
	//���캯��
	Unit(int _max_life, int _atk_freq, double _atk_range, int _speed) 
		:max_life(_max_life), attack_frequency(_atk_freq), attack_range(_atk_range), speed(_speed) 
	{ ; };
	Unit() :max_life(100), attack_frequency(1), attack_range(30), speed(10) { ; };
	//�жϵ�λ�ǲ�����ĳ����Χ��
	bool is_in(Point p1, Point p2);
	//��Ѫ������ʾ�����ز���
	void displayHP();
	void hideHP();
protected:
	bool is_selected = false;//�Ƿ�ѡ��
	//��ǰλ�ú͵�ǰĿ��
};
#endif
