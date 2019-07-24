#pragma once
#ifndef GRIDMAP_H_
#define GRIDMAP_H_

#include "cocos2d.h"
#include <vector>

typedef std::vector<std::vector<int>> dyadic_array;		//vector��ʽ�Ķ�ά����

struct GridSize;

/**
	* @brief GridPoint��һ����¼ÿ����ͼ���Ľṹ��
	* @��ͼ��ÿ�����ռ�õ����ش�СΪ 32 * 32 ����
	* @���ǽ�������ͼ����Ϊ 128 * 128 �����
	* @�Դ�Ϊ���������ⵥλ��������ײ���ص���ͬʱ���A*Ѱ·�㷨
*/
struct GridPoint {
	int _x, _y;
	GridPoint(int x = 0, int y = 0) :_x(x), _y(y) { }

/**
	* @brief ����������أ��жϸ���Ƿ��ص�
	* @ �������ļ�����Ԫ����ִ�и��ļӼ��������˴�����׸��
	*/
	bool operator==(const GridPoint& gp2) const;
	friend GridPoint operator+(const GridPoint& gp1, const GridPoint& gp2);
	friend GridPoint operator-(const GridPoint& gp1, const GridPoint& gp2);
	friend GridPoint operator-(const GridPoint& gp, const GridSize& gz);

/**
	* @brief �����õ��������� sgn ��������Ϊ ({-1, 0, 1}, {-1, 0, 1})
	* @return ���ش���������
	*/
	GridPoint getDirectionVec();
};

typedef GridPoint GridVec;

typedef std::vector<GridPoint> GridPath;

struct GridSize
{
	int width;
	int height;
	GridSize(int _width = 0, int _height = 0) : width(_width), height(_height) {};
	friend GridSize operator/(const GridSize& gz, int s);
};

/**
	* @brief �ýṹ���¼ѡ��ĸ������(��Χ)
*/
struct GridRect
{
	GridPoint center;			//���������������
	GridPoint lower_left;		//�����������½�����
	GridSize size;				//�������ĳߴ�

	GridRect(GridPoint _center, GridSize _size);
};

/**
	* @brief GridMap�൱��һ����TMX��ͼ����ϢͶӰ��һ��͸���ĵ�ͼ�����
	* @���ڽ���ͼ����Ҫ�������Ϣ���л��ܡ�����
*/
class GridMap:public cocos2d::Ref{
public:
/**
	* @brief ��TMXTiledMap�е�ͼ����Ϣ���ص�����ͼ��
	* @return ָ��GridMap��ָ��
	*/
	static GridMap* create(const cocos2d::TMXTiledMap * tiled_map);

/**
	* @brief ��TMXTiledMap�Ը���ͼִ�г�ʼ������
	* @����ͼ�ϵĿ��õ���_gmap�ϱ�עΪ0����ռ�ݵ��עΪ1
	* @return ����ʼ���ɹ��򷵻�true,��֮false
	*/
	bool initWithTiledMap(const cocos2d::TMXTiledMap * tiled_map);

/**
	* @brief ��GridPoint������ת����Point����
	* @param grid_point Ҫת����GridPoint����
	* @return ��Ӧ��Point����
	*/
	cocos2d::Point getPoint(const GridPoint& grid_point);

/**
	* @brief ��Point������ת����GridPoint����
	* @param point Ҫת����Point����
	* @return ��Ӧ��GridPoint����
	*/
	GridPoint getGridPoint(const cocos2d::Point& point);

/**
	* @brief ����������ƫ����Offset��Point������ת����GridPoint����
	* @param point Ҫת���Ĵ�ƫ������Point����
	* @return ��Ӧ��GridPoint����
	*/
	GridPoint getGridPointWithOffset(const cocos2d::Point& p);

/**
	* @brief ��GridPoint������ת���ɷ�Ӧ����ƫ����Point����
	* @param point Ҫת����GridPoint����
	* @return ��Ӧ�Ĵ�ƫ������Point����
	*/
	cocos2d::Point getPointWithOffset(const GridPoint& gp);

/**
	* @brief �жϵ�λ�Ƿ񵽴���Ŀ����
	* @param cur_fp ��λĿǰ��ռ�ݵ�Point����
	* @param dest_gp ��λ��ǰ��Ҫǰ����Ŀ�����GridPoint����
	* @return ����ִ��򷵻�true,��֮false
	*/
	bool hasApproached(const cocos2d::Point& cur_fp, const GridPoint& dest_gp);

/**
	* @brief ��λ����ĳ������øú���ռ�ݸø�㣬���ı�_gmap��_umap��ֵ
	* @param id ��ע��λ��id
	* @param pos ��λ��Ҫռ�õĸ��λ��
	* @param occupy_grid ��λ�Ƿ����н���ռ�øø��(���˷ɻ���������е�λ��ռ�ݸ�㣬�������ص�)
	* @return ���ռ�óɹ�(˵����㵱ǰ����)�򷵻�true����֮����false
	*/
	bool occupyPosition(int id, const GridPoint& pos, bool occupy_grid = true);

/**
	* @brief ��λ����ĳ����������øú���ռ�ݸø�����򣬼��ı�_gmap��_umap��ֵ
	* @param id ��ע��λ��id
	* @param grec ��λ��Ҫռ�õĸ��λ��
	* @param occupy_grid ��λ�Ƿ����н���ռ�øø��(���˷ɻ���������е�λ��ռ�ݸ�㣬�������ص�)
	* @return ���ռ�óɹ�(˵����㵱ǰ����)�򷵻�true����֮����false
	*/
	void occupyPosition(int id, const GridRect& grec, bool occupy_grid = true);

/**
	* @brief ��λ����ĳ������øú���ռ�ݸø�㣬���ı�_gmap��_umap��ֵ
	* @param id ��ע��λ��id
	* @param pos ��λ��Ҫռ�õĸ��λ��
	* @param occupy_grid ��λ�Ƿ����н���ռ�øø��(���˷ɻ���������е�λ��ռ�ݸ�㣬�������ص�)
	* @return ���ռ�óɹ�(˵����㵱ǰ����)�򷵻�true����֮����false
	*/
	bool occupyPosition(int id, const cocos2d::Point& pos, bool occupy_grid = true);

/**
	* @brief �жϸ���Ƿ��ڵ�ͼ������
	* @param gp ��������λ����Ϣ
	* @return �����ǰ����ڵ�ͼ��Χ�ڷ���true����֮����false
	*/
	bool checkPointInMap(const GridPoint& gp) const;

/**
	* @brief �жϸ���Ƿ��ڵ�ͼ������
	* @param x ��������ˮƽλ������
	* @param y ����������ֱλ������
	* @return �����ǰ����ڵ�ͼ��Χ�ڷ���true����֮����false
	*/
	bool checkPointInMap(int x, int y) const;

/**
	* @brief �ڵ���occupyPosition��ʱ����ø���Ƿ����
	* @param gp ��������λ����Ϣ
	* @return �����㵱ǰ���÷���true����֮����false
	*/
	bool checkPosition(const GridPoint & gp);

/**
	* @brief �ڵ���occupyPosition��ʱ����ø�������Ƿ����
	* @param gp ������������λ����Ϣ
	* @return ����������ǰ���÷���true����֮����false
	*/
	bool checkPosition(const GridRect & grec);

/**
	* @brief ��λ�뿪ĳ������øú����뿪�ø�㣬�ı�_gmap��_umap��ֵ
	* @param pos ��λ��Ҫ�뿪�ĸ��λ��
	* @param occupy_grid ��λ�Ƿ����н���ռ�øø��
	*/
	void leavePosition(const GridPoint & pos, bool occupy_grid = true);

/**
	* @brief ��λ�뿪ĳ����������øú����뿪�ø�����򣬸ı�_gmap��_umap��ֵ
	* @param grec ��λ��Ҫ�뿪�ĸ������
	* @param occupy_grid ��λ�Ƿ����н���ռ�øø��
	*/
	void leavePosition(const GridRect & grec, bool occupy_grid = true);

/**
	* @brief Ѱ����Ŀ���㸽���Ŀ��ø��(����λ�н��յ㱻ռ��ʱ���ô˺���)
	* @param origin_gp Ŀ����
	* @return ���������Ŀ��ø��
	*/
	GridPoint findFreePositionNear(const GridPoint& origin_gp);

/**
	* @brief ��ȡ_gmap�������ø��ͱ�ռ�ݸ���λ����Ϣ
	* @return _gmap
	*/
	const dyadic_array& getLogicalGridMap();

/**
	* @brief ��ȡ_umap����ռ�ݵ�ͼÿ�����ĵ�λid��Ϣ
	* @return _umap
	*/
	const dyadic_array& getUnitMap();

/**
	* @brief ��ȡ_umap��ĳ����㷶Χ�ڵ���ֵ�����ڸø�㷶Χ��ÿ�����ĵ�λid��Ϣ
	* @return �洢id��Ϣ��vector
	*/
	std::vector<int> getUnitIDAt(const GridRect& range) const;

private:
	dyadic_array _gmap;					//�洢����Ƿ����ռ����Ϣ�Ķ�ά����
	dyadic_array _umap;					//�洢ռ�ø��ĵ�λID��Ϣ�Ķ�ά����
	int _map_width, _map_height;		//����ͼ�Ŀ�Ⱥ͸߶�
	int _tile_width, _tile_height;		//ÿ����Ŀ�Ⱥ͸߶�

	cocos2d::Vec2 _offset_vec;			//ƫ�����������СΪ(_tile_width / 2, _tile_height / 2)
};

#endif