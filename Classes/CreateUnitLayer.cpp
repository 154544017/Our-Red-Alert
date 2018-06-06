#include "CreateUnitLayer.h"
#include "Building.h"
USING_NS_CC;
using namespace ui;

BaseLayer* BaseLayer::create()
{
	BaseLayer *ret = new BaseLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool BaseLayer::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bc = Sprite::create("bc.png");
	//��ʼ��layout
	Layout* layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width*0.9, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);
	//��Ӵ��������İ�ť
	// ����һ������militaryCamp��Button����������Layout�����Ͻ�
	Button* militaryCamp = Button::create("/Picture/units/base_3.png",
		"/Picture/units/base_3.png");
	layout->addChild(militaryCamp);
	RelativeLayoutParameter* rp_LeftCenter = RelativeLayoutParameter::create();
	rp_LeftCenter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
	militaryCamp->setLayoutParameter(rp_LeftCenter);
	//��militaryCamp��Ӽ�����
	militaryCamp->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			unit_manager->genCreateMessage(11,1,200,100);
		}
	});

	// ����һ���˳���layer��Button����
	Button* exit = Button::create("backNormal.png",
		"backNormal.png");
	layout->addChild(exit);
	RelativeLayoutParameter* rp_exit = RelativeLayoutParameter::create();
	rp_exit->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
	exit->setLayoutParameter(rp_exit);
	//��exit��Ӽ�����
	exit->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			log("exit CreateUnitLayer layer");
			this->setVisible(false);
		}
	});
	return true;
}

MilitaryCampLayer* MilitaryCampLayer::create()
{
	MilitaryCampLayer *ret = new MilitaryCampLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool MilitaryCampLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bc = Sprite::create("bc.png");
	//��ʼ��layout
	Layout* layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width*0.9, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);

	// ����һ������ariplain��Button����������Layout�����Ͻ�
	Button* airplane = Button::create("/Picture/menu/airplane-menu-up.png",
		"/Picture/menu/airplane-menu-down.png");
	layout->addChild(airplane);
	RelativeLayoutParameter* rp_TopLeft = RelativeLayoutParameter::create();
	rp_TopLeft->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
	airplane->setLayoutParameter(rp_TopLeft);
	airplane->setScale(0.65);
	//��airplane��Ӽ�����
	airplane->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			unit_manager->genCreateMessage(1,1,200,100);
		}
	});

	// ����һ������tank��Button����������Layout�����Ͻ�
	Button* tank = Button::create("/Picture/menu/tank-menu-up.png",
		"/Picture/menu/tank-menu-down.png");
	layout->addChild(tank);
	RelativeLayoutParameter* rp_TopRight = RelativeLayoutParameter::create();
	rp_TopRight->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
	tank->setLayoutParameter(rp_TopRight);
	tank->setScale(0.65);
	//��tank��Ӽ�����
	tank->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			unit_manager->genCreateMessage(2,1,300,100);
		}
	});

	// ����һ������soldier��Button����������Layout������
	Button* soldier = Button::create("/Picture/menu/soldier-menu-up.png",
		"/Picture/menu/soldier-menu-down.png");
	layout->addChild(soldier);
	RelativeLayoutParameter* rp_LeftCenter = RelativeLayoutParameter::create();
	rp_LeftCenter->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
	soldier->setLayoutParameter(rp_LeftCenter);
	soldier->setScale(0.65);
	//��soldier��Ӽ�����
	soldier->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			unit_manager->genCreateMessage(3,1,400,100);
		}
	});

	// ����һ���˳���layer��Button����
	Button* exit = Button::create("backNormal.png",
		"backNormal.png");
	layout->addChild(exit);
	RelativeLayoutParameter* rp_exit = RelativeLayoutParameter::create();
	rp_exit->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_BOTTOM);
	exit->setLayoutParameter(rp_exit);
	//��exit��Ӽ�����
	exit->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			log("exit CreateUnitLayer layer");
			this->setVisible(false);
		}
	});
	return true;
}



