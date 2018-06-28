#pragma once
#ifndef A_STAR_PATH_FINDING_H_
#define A_STAR_PATH_FINDING_H_

#include <GridMap.h>
#include "cocos2d.h"
#include <vector>

class Grid;

typedef std::vector<std::vector<Grid>> grid_matrix;
typedef std::vector<Grid *> grid_vec;
enum { VACANT, OCCUPIED, START, DESTINATION, OPEN, CLOSE };

/**
	* @brief Grid��̻�����¼Ѱ·�������������ĸ�����ֵ
*/
class Grid {
public:
	Grid();
	int getFlag()const { return _flag; }
	void setFlag(int flag) { _flag = flag; }
	int getX()const { return _x; }
	int getY()const { return _y; }
	void setPosition(int x, int y) { _x = x; _y = y; }
	int getG()const { return _g; }
	void setG(int g) { _g = g; }
	int getH()const { return _h; }
	void setH(int h) { _h = h; }
	int getF()const { return _f; }
	void setF(int f) { _f = f; }
	Grid * getParent()const { return _parent; }
	void setParent(Grid* parent) { _parent = parent; }
private:
	int _x;
	int _y;

	int _flag;		//����ı�־(��¼��ռ�á�δ��ռ�á���㡢�յ㡢���š��������״̬)
	int _g;			//Gֵ��ŷ����þ���(����㵽ָ��������ƶ�����)
	int _h;			//Hֵ�������پ���(�Ӹõ㵽�յ�Ĺ�����ۣ���������)
	int _f;			//���ߴ��� f = g + h

	Grid* _parent;	//���ڵ�
};


/**
	* @brief PathFinding��ΪѰ·�㷨�ĺ�������
	* @�ṩ�˳�ʼ��·��������·��������·��������·���ĸ���Ҫ�ӿ�
*/
class PathFinding {
public:
/**
	* @brief ���캯������Ҫ���ڳ�ʼ�������Ϣ����ֹ��
	* @param map ��_mapֵΪ1ʱ��ʾ����ͨ������֮����
	* @param start ��¼��ʼ���λ��
	* @param destination ��¼��ֹ���λ��
	*/
	PathFinding(const dyadic_array& map, GridPoint start, GridPoint destination);

/**
	* @brief Ѱ�ҿ��ܵ�·��
	*/
	void searchForPath();

/**
	* @brief ����һ��·��
	*/
	void generatePath();

/**
	* @brief ������·������
	* @return ����·��
	*/
	GridPath getPath()const { return _path; }
private:
	int _width;									//��ͼ���
	int _height;								//��ͼ�߶�
	std::vector<std::vector<Grid>>  _map;		//��ͼ����
	Grid * _starting_point;						//��ʼ��
	Grid * _terminal_point;						//�յ�

	grid_vec _open_list;						//�����б�
	grid_vec _close_list;						//����б�
	GridPath _path;								//����·��

/**
	* @brief �ӵ�ǰ�����б���ѡȡ���ߴ���F��С�ĵĸ����Ϊ��һ������ĸ��
	* @return Fֵ��С�ĸ��
	*/
	Grid * getNextGrid();

/**
	* @brief ��鵱ǰ��㸽���˸�����״̬
	* @param grid ��ǰ����������ĸ��
	*/
	void checkSurroundedGrid(Grid & grid);

/**
	* @brief �жϵ�ǰ����Ƿ��ڵ�ͼ��Χ��
	* @param grid ��ǰ������ĸ��
	* @return ���ڵ�ͼ��Χ���򷵻�true����֮false
	*/
	bool isInMapRange(cocos2d::Point & grid);

/**
	* @brief �ж��Ƿ�Ϊת�ǣ�����б�Ŵ����ϰ���(��������������ƶ�����������)
	* @param g1 ��ǰ���ڼ���ĸ��
	* @param g2 ���ܵ���һ���ĸ��
	* @return ���ø��Ϊת���򷵻�true����֮false
	*/
	bool isCorner(Grid & g1, Grid & g2);

/**
	* @brief �жϸø���Ƿ����(�����ϰ���)
	* @param grid ��ǰ���ڼ���ĸ��
	* @return ���ø������򷵻�true����֮false
	*/
	bool isAvailable(Grid & grid);

/**
	* @brief ����ŷ����þ���(Gֵ)
	* @param g1 ��ǰ���ڼ���ĸ��
	* @param grid �ڸ��
	* @return EuclideanDistance
	*/
	int getEuclideanDistance(Grid & g1, Grid & g2);

/**
	* @brief ���������پ���(Hֵ)
	* @param g1 ��ǰ���ڼ���ĸ��
	* @param g2 �յ���
	* @return ManhattanDistance
	*/	
	int getManhattanDistance(Grid & g1, Grid & g2);

/**
	* @brief ���Ѽ��뵽����б�ĸ��ӿ����б����Ƴ�
	* @param grid ��Ҫ�ӿ����б����Ƴ��ĸ��
	*/		
	void removeFromOpenList(Grid * grid);
};


#endif