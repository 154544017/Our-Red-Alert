#define ASIO_STANDALONE
#pragma warning(disable:4996)
#pragma once
#ifndef COMBAT_SCENE_H_
#define COMBAT_SCENE_H_
#define DEBUG
#include "asio.hpp"
#include"Unit.h"
#include <cocos2d.h>
#include<vector>
#include "GameMessage.pb.h"
#include<chat_client.h>
#include<chat_server.h>
#include"Const.h"
USING_NS_CC;

class MouseRect : public cocos2d::DrawNode
{
public:
	CREATE_FUNC(MouseRect);
	cocos2d::Point touch_start, touch_end;
	void update(float f) override;
};

class PowerDisplay : public Label	
{
public:
	bool init() override;
	void updateDisplay(Power * power);
	CREATE_FUNC(PowerDisplay);
};
class Power : public DrawNode
{
private:
	int max_power = INITIAL_POWER;
	int used_power = 0;
	int cur_length = 0;
	int length = 130;
	int width = 35;
	cocos2d::Color4F red{ 1, 0, 0, 1 };
	cocos2d::Color4F yellow{ 1, 1, 0, 1 };
	cocos2d::Color4F green{ 0, 1, 0.5, 1 };
	cocos2d::Color4F blue{ 0.1, 0.5, 1, 1 };
public:
	PowerDisplay * powerDisplay = nullptr;
	void addMax_power(int delta);
	void spendPower(int power);
	void setCur_length();
	bool checkPower(int delta);
	Color4F getColor();
	void updatePowerDisplay();
	friend void PowerDisplay::updateDisplay(Power * power);
	CREATE_FUNC(Power);
};

class Money : public Label
{
public:
	bool init() override;
	void update(float f) override;
	void updateMoneyDisplay();
	bool checkMoney(int price) const;
	void spendMoney(int cost);
	int getIncreasingAmount() const;
	void setIncreasingAmount(int amount);
	CREATE_FUNC(Money);
private:
	int money = 0;
	int timer = 0;
	int update_period = 20;
	int increase_amount = 0;
};

class CombatScene :public Layer{
public:
	EventListenerTouchOneByOne * destListener = nullptr;
	EventListenerKeyboard * letterListener = nullptr;
	Point cdelta = { 0,0 };
	GameMessageSet * msgs;
	Power * power = nullptr;
	Money * money = nullptr;
	/*����ѡ��*/
	void DrawRectArea(Point p1, Point p2);
	/*��selected_box�����뱻ѡ�еĽڵ�*/
	void getLayerUnit(Point p1, Point p2);
	/*������ͼ*/
	void scrollMap();
	/*���ڹ�����ͼ���ˢ��*/
	virtual void update(float f);
	/*���ڷ���selected_box*/
	
	static Scene* createScene(chat_server * server_context_, chat_client * client_context_);
	virtual bool init(chat_server * server_context_, chat_client * client_context_);
	
	/* ��Ұ����Ϊ���� */
	void focusOnBase();
	
	static CombatScene* create(chat_server * server_context_, chat_client * client_context_);
	
private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	chat_client * client_side = nullptr;//�����ָ��
	chat_server * server_side = nullptr;//�����ָ��
	bool is_clicked = false;
	cocos2d::Point last_touch{ 0, 0 };//������һ�������ĵ�
	MouseRect* mouse_rect = nullptr;//ѡ��
	TMXTiledMap* _combat_map = nullptr;//������Ƭ��ͼ���õ�ָ��
	GridMap * _grid_map = nullptr;
	Point _cursor_position{ 0,0 };
	UnitManager * unit_manager;
	int message_update = 0;
};

inline float Tri_Dsitance(Point t1, Point t2) {
#ifdef DEBUG
	log("dt:t1.x:%f t1.y:%f t2.x:%f t2.y:%f", t1.x, t1.y, t2.x, t2.y);
#endif
	return fabs(t1.x - t2.x) + fabs(t1.y - t2.y);
}

#endif