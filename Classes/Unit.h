#pragma once
#ifndef _UNIT_H_
#define _UNIT_H_

#include <iostream>
#include "cocos2d.h"
#include "GameMessage.pb.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class Unit;
class UnitManager;
class Bar;
class Building;
class CombatScene;

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

class UnitManager : public cocos2d::Ref
{
public:
	int player_id = 0;
	GameMessageSet* msgs = nullptr;
	EventListenerTouchOneByOne * spriteTouchListener;

	CREATE_FUNC(UnitManager);
	bool init();
	void setMessageSet(GameMessageSet* _msgs);
	void setSpriteTouchListener(EventListenerTouchOneByOne * _spriteTouchListener);
	void setTiledMap(cocos2d::TMXTiledMap* _tiledMap);
	void setPlayerID(int _player_id);
	void setCombatScene(CombatScene* _combat_scene);
	void setBuilding(Building * _building);

	CombatScene * getCombatScene();
	EventListenerTouchOneByOne * getSpriteTouchListener();
	int  getUnitTeam(int unit_id);
	//�����µ�λ����Ϣ
	void genCreateMessage(int _unit_type, int team, float x, float y);
	void updateUnitsState();

	void initializeUnitGroup();

	void setBasePosition(Point base_pos);
	Point getBasePosition()const;

	void deselectAllUnits();
	//void selectUnits(cocos2d::Point select_point);
	//void selectUnits(cocos2d::Rect select_rect);
private:
	cocos2d::Map<int, Unit*> id_map;
	std::vector<int> selected_ids;
	std::map<int, int> base_map;

	cocos2d::TMXTiledMap* tiled_map = nullptr;
	CombatScene* combat_scene = nullptr;

	int next_id = 1;
	int base_id = 1;

	Point _base_pos{ 0,0 };

	Building * building = nullptr;
	Unit* createNewUnit(int id, int team, int uint_type, float x, float y);
};

class Unit :public cocos2d::Sprite {
protected:
	GameMessageSet * msgs = nullptr;
	TMXTiledMap * tiled_map = nullptr;
	Layer* combat_scene;
	EventListenerTouchOneByOne * spriteTouchListener;
	int type;
	bool mobile;
	bool selected = false;//�Ƿ�ѡ�е�ǰλ�ú͵�ǰĿ��
	int current_life=100;
	int max_life=100;
	int attack_frequency ;
	double attack_range ;
	int speed ;
	Bar* hp_bar = nullptr;//��������λ����Ѫ��
public:
	//���õ�λ���ڵĲ㣬��ͼָ���Sprite������
	void set(TMXTiledMap *, Layer *, EventListenerTouchOneByOne *);
	virtual void setListener();
	void setUnitManager(UnitManager*);
	
	int id;
	int team = 0;
	int z_index;
	UnitManager* unit_manager = nullptr;
	
	static Unit* create(const std::string & filename);

	virtual void setProperties();
	//virtual void addToMaps(cocos2d::TMXTiledMap* _tiled_map);
	//void removeFromMaps();

	virtual void setTeam(int _team);
	void setMobile(bool can);
	int getTeam() const;
	int getType() const;
	bool isMobile();
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

	friend void UnitManager::updateUnitsState();
};


#endif
