#pragma once
#ifndef _UNIT_H_
#define _UNIT_H_

#include <iostream>
#include "cocos2d.h"
#include "GameMessage.pb.h"
#include "ui/CocosGUI.h"
#include "GridMap.h"
#include "AStarPathFinding.h"
#include"chat_client.h"

USING_NS_CC;
using namespace ui;

class Unit;
class UnitManager;
class Bar;
class Building;
class Money;
class Power;
class Base;
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

class ConstructRange : public DrawNode
{
private:
	Color4F color{ 0.5,0.8,0.95,0.5 };
public:
	void drawRange(Point p,float r);
	CREATE_FUNC(ConstructRange);
};

class UnitManager : public cocos2d::Ref
{
public:
	int player_id = 0;
	Base * base = nullptr;
	Money * money = nullptr;
	Power * power = nullptr;
	ConstructRange * constructRange = nullptr;
	GameMessageSet* msgs = nullptr;
	GridMap* grid_map = nullptr;
	EventListenerTouchOneByOne * spriteTouchListener;
	std::vector<int> selected_ids;

	CREATE_FUNC(UnitManager);
	bool init();
	void setMessageSet(GameMessageSet* _msgs);
	void setSpriteTouchListener(EventListenerTouchOneByOne * _spriteTouchListener);
	void setTiledMap(cocos2d::TMXTiledMap* _tiledMap);
	void setGridMap(GridMap *);
	void setPlayerID(int _player_id);
	void setCombatScene(CombatScene* _combat_scene);
	void setSocketClient(chat_client* _socket_client);
	//��ȡ�˶���ʱ��
	float getPlayerMoveTime(Vec2 start_pos, Vec2 end_pos, int _speed);
	//ִ���˶��Ĳ���
	void playMover(Point position, Unit * _sprite);
	//��ȡ�����
	int genRandom(int start, int end);

	CombatScene * getCombatScene();
	TMXTiledMap * getTiledMap();
	EventListenerTouchOneByOne * getSpriteTouchListener();
	int  getUnitCamp(int unit_id);

	//�����µ�λ����Ϣ
	void genCreateMessage(int _unit_type, int camp, float x, float y);
	void updateUnitsState();

	void initializeUnitGroup();

	void setMax_power(int delta);
	void setIncreasingAmount(int amount);
	void setUnitCreateCenter(Point center);
	void setBasePosition(Point base_pos);
	Point getBasePosition()const;
	Point getUnitCreateCenter();

	GridSize getGridSize(Size);
	GridRect getGridRect(Point, Size);
	GridPoint getGridPoint(Point);
	Point getPoint(GridPoint);
	//������յ�ʱ�Ĳ���
	void selectEmpty(Point position);
	//�������λʱ�Ĳ������ҷ�������)
	void selectPointUnits(Unit * _unit);
	//��selected_ids�ﵥλ�Ĳ���
	void getClickedUnit();
	//���selected_ids
	void cancellClickedUnit();
private:
	cocos2d::Map<int, Unit*> id_map;
	std::map<int, int> base_map;

	cocos2d::TMXTiledMap* tiled_map = nullptr;
	CombatScene* combat_scene = nullptr;
	chat_client * socket_client = nullptr;

	int next_id = 1;
	int base_id = 1;

	Point _base_pos{ 0,0 };
	Point unit_create_center{ 0,0 };
	Unit* createNewUnit(int id, int camp, int uint_type, float x, float y);

	void genAttackEffect(int unit_id0, int unit_id1);
	std::default_random_engine gen;
};

class Unit :public cocos2d::Sprite {
protected:
	GameMessageSet * msgs = nullptr;
	TMXTiledMap * tiled_map = nullptr;
	GridMap* grid_map = nullptr;
	Layer* combat_scene = nullptr;
	EventListenerTouchOneByOne * spriteTouchListener = nullptr;

	GridPoint _cur_pos;
	GridPoint _cur_dest;
	GridPoint _final_dest;
	GridPath _grid_path;
	Point _final_position;
	
	//count of refinding path ����Ѱ·������
	int rfp_cnt = 0;
	//count of delay �ӳ�Ѱ·������
	int del_cnt = -1;

	int type;
	bool mobile;

	bool is_attack;
	bool is_moving = false;
	bool is_delaying = false; //�ӳ�Ѱ·��־

	bool selected = false;//�Ƿ�ѡ�е�ǰλ�ú͵�ǰĿ��
	int current_life=100;
	int max_life=100;
	int ATK = 0;
	int attack_frequency ;
	double attack_range ;
	int speed ;
	Bar* hp_bar = nullptr;//��������λ����Ѫ��

	//�ƶ���λ
	virtual void move();
	bool hasArrivedFinalDest();
	//Ѱ·�㷨
	virtual GridPath findPath(const GridPoint& dest)const;

	//�Ż�Ѱ·(���������������ͬһ��·�������ԭ�ȵļ���GridPath�ϳ�Ϊһ��)
	GridPath optimizePath(const GridPath & origin_path) const;

	
public:
	void setGridMap(GridMap *);
	//����λ�󶨼����������ӵ���ͼ��
	void setCurPos(const GridPoint _cur);
	void set(TMXTiledMap *, GridMap * _gridMap,Layer * _combat_scene, EventListenerTouchOneByOne *);
	virtual void addToGmap(Point p);
	virtual void setListener();
	void setUnitManager(UnitManager*);
	EventListenerTouchOneByOne * getListener();
	int id;
	int camp = 0;
	int z_index;
	int attack_id;
	int attack_freq = 50;
	int timer = 0;
	UnitManager* unit_manager = nullptr;
	
	static Unit* create(const std::string & filename);

	void initBar();
	virtual void setProperties();
	//void removeFromMaps();

	void tryToSearchForPath();
	GridPoint getGridPosition() const;
	void setGridPath(const MsgGridPath & msg_grid_path);
	void setDestination(const GridPoint& grid_dest,const Point& point);
	void goToFinalPosition();

	virtual void setCamp(int _camp);
	void setMobile(bool can);
	int getCamp() const;
	int getType() const;
	int getSpeed() const;
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
	void attack();
	void searchEnemy();
	bool underAttack(int damage);
	friend void UnitManager::updateUnitsState();
	virtual void update(float f);
};

class TrajectoryEffect : public cocos2d::ParticleFire
{
public:
	virtual bool init() override;
	void setPath(cocos2d::Vec2, cocos2d::Vec2);

	CREATE_FUNC(TrajectoryEffect);
private:
	void updatefire(float);
	cocos2d::Vec2 from_, to_, move_;
	int speed_ = 3;

};

#endif
