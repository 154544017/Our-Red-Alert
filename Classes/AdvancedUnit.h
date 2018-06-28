#pragma once
#ifndef __ADVANCEDUNIT_H__
#define __ADVANCEDUNIT_H__


#include "Unit.h"

class Dog : public Unit
{
public:
	static Dog* create(const std::string& filename);
	/**
	*����_direction�����Ŷ�Ӧ�Ķ���
	*/
	void playMoveAnimation();
	/**
	*�жϷ����Ƿ����仯
	*/
	bool checkDirectionChange(Vec2 direction);
private:
	/**
	*����move�ķ���
	*/
	int _direction = 0;
	/**
	*��һ��move�ķ���
	*/
	int _preDirection = 0;
	/**
	*��дmove() ����momve���򲥷Ŷ���
	*/
	void move() override;

	void setProperties() override;
};
class Fighter : public Unit
{
public:
	static Fighter* create(const std::string& filename);
	/**
	*����move�ķ�������Ҫ��ת�ĽǶ�
	*/
	float getRotation(Vec2 direction);
	/**
	*��дfindPath Ѱ�Һ��Ը��ռ�õ�·��
	*/
	GridPath findPath(const GridPoint& dest)const override;

private:
	void setProperties() override;
	//��дmove ʵ�֡�����(�ƶ���ռ�ø��)������,�����ݷ�����ת
	void move() override;
};


class Tank : public Unit
{
public:
	static Tank* create(const std::string& filename);
	/**
	*����move�ķ�������Ҫ��ת�ĽǶ�
	*/
	float getRotation(Vec2 direction);

private:

	void setProperties() override;
	//��дmove ����move������ת
	void move() override;
};

class Soldier : public Unit
{
public:
	static Soldier* create(const std::string& filename);
private:

	void setProperties() override;
};

#endif // !__ADVANCEDUNIT_H__