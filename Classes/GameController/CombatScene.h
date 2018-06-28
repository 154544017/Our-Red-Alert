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

/**
	* @brief MouseRect��̳���DrawNode��
	* @���ڼ�����Ϸ�е�ѡ��Ļ��ƺͲ���
*/
class MouseRect : public cocos2d::DrawNode
{
public:
	cocos2d::Point touch_start, touch_end;				//�����¼�����ʼ����յ�
	cocos2d::Point touch_start_map, touch_end_map;		//��Ӧ����Ϸ��ͼ����ʼ�����յ�

/**
	* @brief ����ˢ��ѡ�����ʾ
	*/
	void update(float f) override;
};

/**
	* @brief PowerDisplay��̳���Label��
	* @������ʾ��ˢ����Ϸ�еĵ�������
*/
class PowerDisplay : public Label	
{
public:
	bool init() override;
	void updateDisplay(Power * power);
	CREATE_FUNC(PowerDisplay);
};

/**
	* @brief Power��̳���DrawNode��
	* @������ʾ��ˢ����Ϸ�еĵ�����
*/
class Power : public DrawNode
{
private:
	int max_power = INITIAL_POWER;				//��������
	int used_power = 0;							//��ǰʹ�õĵ���

	int cur_length = 0;							//��ǰ�������ĳ���
	int length = 130;							//�������ĳ���
	int width = 35;								//�������Ŀ��

	cocos2d::Color4F red{ 1, 0, 0, 1 };			//��ɫ��ʾ
	cocos2d::Color4F yellow{ 1, 1, 0, 1 };		//��ɫ
	cocos2d::Color4F green{ 0, 1, 0.5, 1 };		//��ɫ
	cocos2d::Color4F blue{ 0.1, 0.5, 1, 1 };	//��ɫ
public:
	PowerDisplay * powerDisplay = nullptr;		//����ͬPowerDisplay��ͨ�ŵ�ָ��

/**
	* @brief �������ӵ�ǰ����������
	* @param delta ����������
	*/
	void addMax_power(int delta);

/**
	* @brief �½��������ĵ�ǰ�������ĵ���
	* @param delta �ý������ĵĵ���
	*/
	void spendPower(int power);

/**
	* @brief ���õ�ǰ��ʾ�������ĳ���
	*/
	void setCur_length();

/**
	* @brief ���ڼ�鵱ǰ�����Ƿ���Ϊѡ��Ľ����ṩ����
	* @param delta ������������򷵻�true����֮����false
	*/
	bool checkPower(int delta);

/**
	* @brief ���ݵ�ǰ�����ĳ�ԣ�̶�������ʾ��ɫ
	* @�������������չʾ��ɫ�����������ݵ�������̶�������ʾ�̡��ơ���ɫ
	*/
	Color4F getColor();

/**
	* @brief ���µ���������ʾ״̬
	*/
	void updatePowerDisplay();

/**
	* @brief ��Ԫ���������µ�������ʾ״̬��ͬʱ���µ������ݵ���ʾ
	* @param power ��������
	*/
	friend void PowerDisplay::updateDisplay(Power * power);

	CREATE_FUNC(Power);
};

/**
	* @brief Money��̳���Label��
	* @������ʾ��ˢ����Ϸ�еĽ�Ǯ����
*/
class Money : public Label
{
public:
/**
	* @brief ��ʼ����Ǯ��ʾ������
	* @return ��ʼ���ɹ��򷵻�true����֮false
	*/
	bool init() override;

/**
	* @brief ���½�Ǯ��Ŀ
	*/
	void update(float f) override;

/**
	* @brief ���½�Ǯ��������ʾ(��update�������е���)
	*/
	void updateMoneyDisplay();

/**
	* @brief ����Ǯ��Ŀ���ж�ʣ���Ǯ�ܷ�֧��Ŀ�꽨��
	* @param price Ŀ�꽨���ļ۸�
	* @return ���㹻֧���򷵻�ture����֮false
	*/
	bool checkMoney(int price) const;

/**
	* @brief ���콨����۳���Ǯ
	* @param cost Ŀ�꽨���Ľ������
	*/
	void spendMoney(int cost);

/**
	* @brief �õ����½�Ǯ�������ٶȵ�ֵ
	* @return �����������
	*/
	int getIncreasingAmount() const;

/**
	* @brief ���õ��½�Ǯ�������ٶȵ�ֵ
	* @param amount ��������(�󳡵�����)
	*/
	void setIncreasingAmount(int amount);
	CREATE_FUNC(Money);
private:
	int money = 0;				//��Ǯ����
	int timer = 0;				//��Ϸ��������֡��
	int update_period = 20;		//ˢ������
	int increase_amount = 0;	//��Ǯ��֡�����ӵķ���(�����Ŀ������)
};

/**
	* @brief mini��̳���Sprite��
	* @������ʾ��Ϸ�е�С��ͼ
*/
class Minimap :public Sprite {
public:
/**
	* @brief ����С��ͼ
	* @param filename С��ͼͼƬ��·��
	* @return ָ��ǰС��ͼ��ָ��
	*/
	static Minimap *create(const std::string &filename);

/**
	* @brief ��־С��ͼ�ϵĸ�����λ
	* @param index ��λ��id
	* @param target ��λ������
	*/
	void addpoint(int index, Point & target);

	DrawNode* drawNode = nullptr;	//С��ͼ��λ�ı�ע
private:
	float mini_width;		//С��ͼ�Ŀ��
	float mini_height;		//С��ͼ�ĸ߶�
};

/**
	* @CombatScene��̳���Layer��
	* @��������Ϸ��������
	* @��Ϸ�������ǽ���Ҫ�Ĺ���ȫ��������������У�ͬʱҲ�ǽ����¹��ܲ��Եĵط�
	* @���ſ����Ľ��н������ࡢ����ࡢ��λ��ȫ��ת���˳�ȥ�����������ڵ�Unit�ࡢGridMap�ࡢPathFinding��
	* @ֻ��������Ϸ�ĳ�ʼ�������ص�ͼ������ѡ���ƶ���ͼ�Ȼ�����Ϸ����
*/
class CombatScene :public Layer{
public:
	EventListenerTouchOneByOne * destListener = nullptr;			//��������
	EventListenerKeyboard * letterListener = nullptr;				//���̼�����

	Point cdelta = { 0,0 };											//��ͼƫ����
	GameMessageSet * msgs;											//ָ����Ϣģ���ָ��
	Power * power = nullptr;										//ָ�������Ϣ��ָ��
	Money * money = nullptr;										//ָ���Ǯ��Ϣ��ָ��

/**
	* @brief ����ѡ��
	* @param p1 ����¼�����ʼ��
	* @param target ����¼����յ�
	*/
	void DrawRectArea(Point p1, Point p2);

/**
	* @brief ��selected_box�����뱻ѡ�еĽڵ�
	* @param p1 ѡ��ĳ�ʼ��
	* @param p2 ѡ����յ�
	*/
	void getLayerUnit(Point p1, Point p2);

/**
	* @brief �϶���ͼ 
	* @������Ƶ���ͼ�߽�10~40����ʱ�ƶ���ͼ��0~10����ʱ�ٶȷ���
	* @�����¼����������Ҽ�ʱҲ���ƶ���ͼ��ʾλ��
	*/
	void dragMap();

/**
	* @brief ˢ��С��ͼ����ʾ
	*/	
	void updateMircoLocation();

/**
	* @brief �����϶���ͼ����ӵ�λ�õ�ˢ��
	*/
	virtual void update(float f);

/**
	* @brief ���ڷ���selected_box
	*/
	static Scene* createScene(chat_server * server_context_, chat_client * client_context_);
	virtual bool init(chat_server * server_context_, chat_client * client_context_);
	
/**
	* @brief ����Ұ��������Ϊ���� 
	*/
	void focusOnBase();
	
	static CombatScene* create(chat_server * server_context_, chat_client * client_context_);
	
private:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;	//��¼�����¼�������

	chat_client * client_side = nullptr;					//�����ָ��
	chat_server * server_side = nullptr;					//�����ָ��
	TMXTiledMap* _combat_map = nullptr;						//������Ƭ��ͼ���õ�ָ��
	UnitManager * unit_manager;								//��Ϸ��������ָ��
	GridMap * _grid_map = nullptr;							//����ͼ
	MouseRect* mouse_rect = nullptr;						//ѡ��
	Minimap * mini_map = nullptr;							//С��ͼ

	bool is_clicked = false;								//flag����¼�Ƿ���
	cocos2d::Point last_touch{ 0, 0 };						//������һ�������ĵ�
	Point _cursor_position{ 0,0 };							//���λ��
	int message_update = 0;									//��Ϣ�ĸ���
};

inline float Tri_Dsitance(Point t1, Point t2) {
#ifdef DEBUG
	log("dt:t1.x:%f t1.y:%f t2.x:%f t2.y:%f", t1.x, t1.y, t2.x, t2.y);
#endif
	return fabs(t1.x - t2.x) + fabs(t1.y - t2.y);
}

#endif