#ifndef __CREATEUNITLAYER_H__
#define __CREATEUNITLAYER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
class CreateUnitLayer :public cocos2d::Layer
{
public:
	
	virtual bool init();
	//���õ�λ���ڵĲ㣬��ͼָ���Sprite������
	void set(TMXTiledMap *, Layer*, EventListenerTouchOneByOne *);
	Layer * getLayer();
	TMXTiledMap * getMap();
	virtual void realInit() = 0;
protected:
	Layer * addLayer;
	TMXTiledMap * addMap;
	EventListenerTouchOneByOne * addTouchListener;
	
};
//��Ӫ�˵���ֻ�������˵�λ���ڵĵ�ͼָ�룬�㣬��������������������Ż�realInit
class MilitaryCampLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *airplane;
	cocos2d::ui::Button *tank;
	cocos2d::ui::Button *soldier;
	cocos2d::ui::Button *exit;
public:
	void set(TMXTiledMap *, Layer *, EventListenerTouchOneByOne *);
	void realInit();
	CREATE_FUNC(MilitaryCampLayer);
};
//���ز˵���ֻ�������˵�λ���ڵĵ�ͼָ�룬�㣬��������������������Ż�realInit
class BaseLayer : public CreateUnitLayer
{
private:
	cocos2d::ui::Button *Military;
	cocos2d::ui::Button *exit;
public:
	void set(TMXTiledMap *, Layer*, EventListenerTouchOneByOne *);
	void realInit();
	CREATE_FUNC(BaseLayer);
};
#endif
