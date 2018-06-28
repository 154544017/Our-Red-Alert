#pragma once
#ifndef _UNIT_H_
#define _UNIT_H_

#include <iostream>
#include "cocos2d.h"
#include "GameMessage.pb.h"
#include "ui/CocosGUI.h"
#include <vector>
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
class MilitaryCamp;
class TankFactary;

class Bar :public DrawNode {
private:
	/**
	*չʾѪ���ļ�ʱ��
	*/
	int timer = 0;
	int display_time = 0;
	float length = 36;
	float width = 4;
	float height = 5;
	bool is_visible = false;
	cocos2d::Color4F color{ 0.8, 0, 0, 0.8 };
public:
	/**
	*������ʾѪ��
	*/
	void updateBarDisplay(float rate);
	void setLength(float _length) { length = _length; };
	void setHeight(float _height) { height = _height; };
	void setColor(const cocos2d::Color4F& _color) { color = _color; };
	void keepVisible();
	void stopKeepingVisible();
	CREATE_FUNC(Bar);

};


class ConstructRange : public DrawNode{
private:
	Color4F color{ 0.5,0.8,0.95,0.5 };
public:
	/**
	*��ʾ�ɽ��췶Χ(Բ)
	*���� center:Բ��,radius:�뾶
	*/
	void drawRange(Point center,float radius);
	CREATE_FUNC(ConstructRange);
};

class UnitManager : public cocos2d::Ref
{
public:
	Vector<Layer*> createLayer;
	int player_id = 0;
	int player_num;
	Base * base = nullptr;
	Money * money = nullptr;
	Power * power = nullptr;
	ConstructRange * constructRange = nullptr;
	chat_client * socket_client = nullptr;
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

	//��ȡ�����
	int genRandom(int start, int end);

	CombatScene * getCombatScene();
	TMXTiledMap * getTiledMap();
	EventListenerTouchOneByOne * getSpriteTouchListener();
	int  getUnitCamp(int unit_id);

	/**
	*���������µ�λ����Ϣ
	*���� _unit_type:�µ�λ������,camp:�µ�λ����Ӫ,x��y: �µ�λ����λ��
	*/
	void genCreateMessage(int _unit_type, int camp, float x, float y);
	/**
	*������ܵ��ĸ�����Ϣ: ����:CRT,����:ATK,�ƶ�:MOV,����:CHT
	*/
	void updateUnitsState();
	/**
	*���ݵ�ͼ��Ϣ������ʼ��λ
	*/
	void initializeUnitGroup();
	/**
	*���Ż��ش����Ķ���
	*���� base:��Ҫ���Ŷ�����Base*����
	*/
	void playBaseCreateAnimation(Base * base);
	/**
	*������������� delta
	*/
	void setMax_power(int delta);
	/**
	*ÿ�����ӽ�Ǯ��� amount
	*/
	void setIncreasingAmount(int amount);
	/**
	*���汾��Ӫ����λ��
	*/
	void setBasePosition(Point base_pos);
	/**
	*���ݵ���λ��ȷ�������з�ʱͣ���Ǹ�λ��
	*���� attacker:���������ĵ�λ,target:�������ĵ�λ
	*����ֵ GridPoint ������Ŀ�ĵظ������
	*/
	GridPoint getAttackPoint(Unit * attacker,Unit * target);
	Point getBasePosition()const;
	/**
	*����idɾ����λ
	*/
	void deleteUnit(int id);
	/**
	*��Size����ת��ΪGridSize����
	*/
	GridSize getGridSize(Size size);
	/**
	*��Point����ת��ΪGridPoint����
	*/
	GridPoint getGridPoint(Point point);
	/**
	*��GridPoint����ת��ΪPoint����
	*/
	Point getPoint(GridPoint grid_point);
	/**
	*������������center�ͷ�Χsize���ض�Ӧ��GridRect
	*/
	GridRect getGridRect(Point center, Size size);
	/**
	*����յ�ʱ���������ʵĵ�λ�ƶ���position��λ��
	*/
	void selectEmpty(Point position);
	/**
	*��������鵥λʱ,���ݵ��ҽ��в�ͬ�Ĵ���
	*/
	void selectPointUnits(Unit * _unit);
	/**
	*��ѡ�еļ�����λ���д���
	*/
	void getClickedUnit();
	/**
	*���selected_ids
	*/
	void cancellClickedUnit();
	/**
	*�ж�ʤ��
	*/
	void checkWinOrLose(int base_id);
	/**
	*Ϊÿ���¼����player������Ӫ��ʶ
	*/
	void setPlayerNum(chat_client* _socket_client);
private:
	cocos2d::Map<int, Unit*> id_map;
	std::map<int, int> base_map;
	std::default_random_engine gen;
	cocos2d::TMXTiledMap* tiled_map = nullptr;
	CombatScene* combat_scene = nullptr;

	int next_id;

	Point _base_pos{ 0,0 };
	/**
	*�����µ�λ
	*���� _unit_type:�µ�λ������,camp:�µ�λ����Ӫ,x��y: �µ�λ����λ��
	*/
	Unit* createNewUnit(int id, int camp, int uint_type, float x, float y);
	/**
	*������unit_id0��unit_id1�Ĺ�����Ч
	*���� unit_id0:������id, unit_id1:��������id
	*/
	void genAttackEffect(int unit_id0, int unit_id1);

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
	Point _cur_dest_point;
	GridPoint _final_dest;
	GridPath _grid_path;
	/**
	*count of delay �ӳ�Ѱ·������
	*/
	int del_cnt = -1;

	int type;
	bool mobile;


	bool is_moving = false;
	/**
	*count of delay �ӳ�Ѱ·������
	*/
	bool is_delaying = false; 
	/**
	*�Ƿ�ѡ�е�ǰĿ��
	*/
	bool selected = false;
	int current_life=100;
	int max_life=100;
	int ATK = 0;
	int attack_frequency ;

	int speed ;
	Bar* hp_bar = nullptr;

	/**
	*�����趨·�������ƶ�
	*/
	virtual void move();
	/**
	*��λ�ӳ�Ѱ·
	*/
	void delay();
	bool hasArrivedFinalDest();	
	/**
	*Ѱ·�㷨
	*/
	virtual GridPath findPath(const GridPoint& dest)const;
	/**
	*�Ż�Ѱ·(���������������ͬһ��·�������ԭ�ȵļ���GridPath�ϳ�Ϊһ��)
	*/
	GridPath optimizePath(const GridPath & origin_path) const;

	
public:
	void setGridMap(GridMap *);
	void setCurPos(const GridPoint _cur);
	/**
	*����λ�ӵ���Ƭ��ͼ���󶨼�����,��grid_map��Ϊ��ǰ����ͼ
	*/
	void set(TMXTiledMap *, GridMap * _gridMap,Layer * _combat_scene, EventListenerTouchOneByOne *);
	/**
	*����λ�ӵ�����ͼ��ռ�ö�Ӧ���
	*/
	virtual void addToGmap(Point p);
	/**
	*����λ�󶨼�����
	*/
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
	bool is_attack = false;
	GridSize attack_range;
	/**
	*����ռ�õĸ�����
	*/
	GridRect rec;
	bool is_in_attack = false;
	static Unit* create(const std::string & filename);
	/**
	*�����ӳ�Ѱ·
	*/
	void setDelayPathFinding(int cnt);
	/**
	*����Ѫ��
	*/
	virtual void initBar();
	/**
	*���õ�λ����
	*/
	virtual void setProperties();
	/**
	*����λ����Ƭ��ͼȥ�����ͷ�ռ�ø�㣬���ű�ը��Ч
	*/
	void removeFromMaps();
	/**
	*����find_path()Ѱ��һ��·�����ҷ��͵�λ�ƶ�����Ϣ
	*/
	void tryToSearchForPath();

	GridPoint getGridPosition() const;
	void setGridPath(const MsgGridPath & msg_grid_path);
	void setDestination(const GridPoint& grid_dest);
	void setCurDestPoint(const GridPoint& grid_dest);
	void setListenerEnable(bool enable);
	virtual void setCamp(int _camp);
	void setMobile(bool can);
	int getCamp() const;
	int getType() const;
	int getSpeed() const;
	bool isMobile();
	/**
	*���캯��
	*/
	Unit(int _max_life, int _atk_freq, double _atk_range, int _speed) 
		:max_life(_max_life), attack_frequency(_atk_freq), attack_range(_atk_range), speed(_speed), rec(GridRect{ 0,0 })
	{ ; };
	Unit() :max_life(100), attack_frequency(1), speed(10), rec(GridRect{ 0,0 }) { ; };
	/**
	*�жϵ�λ�ǲ�����ĳ����Χ��
	*/
	bool is_in(Point p1, Point p2);
	/**
	*��Ѫ������ʾ�����ز���
	*/
	void displayHP();
	void hideHP();
	/**
	*���͵�λ��������Ϣ
	*/
	void attack();
	/**
	*Ѱ�ҹ�����Χ�ĵ���
	*/
	void searchEnemy();
	/**
	*������ʱ��Ѫ
	*/
	bool underAttack(int damage);
	friend void UnitManager::updateUnitsState();
	/**
	*���µ�λ״̬�������ݲ�ͬ״ִ̬�в�ͬ����
	*/
	virtual void update(float f);
};

class TrajectoryEffect : public cocos2d::ParticleFire
{
public:
	virtual bool init() override;
	/**
	*�趨������Ч·��
	*/
	void setPath(cocos2d::Vec2, cocos2d::Vec2);

	CREATE_FUNC(TrajectoryEffect);
private:
	/**
	*���¹�����Чλ��
	*/
	void updatefire(float);
	cocos2d::Vec2 from_, to_, move_;
	int speed_ = 7;
};

class ExplosionEffect :public cocos2d::ParticleFire
{
public:
	virtual bool init() override;

	CREATE_FUNC(ExplosionEffect);
private:
	void remove(float f);
};

#endif
