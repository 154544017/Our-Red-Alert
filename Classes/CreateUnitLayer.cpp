#include "CreateUnitLayer.h"
#include "Building.h"
#include "CombatScene.h"
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
			building = true;
		}
		if (building)
		{
			//���ò������
			unit_manager->getCombatScene()->destListener->setEnabled(false);
			//���ز˵�
			this->setVisible(false);
			onBuilding(11);
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
			auto position = findFreePosition();
			unit_manager->genCreateMessage(1,1,position.x,position.y);
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
			auto position = findFreePosition();
			unit_manager->genCreateMessage(2,1,position.x,position.y);
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
			auto position = findFreePosition();
			unit_manager->genCreateMessage(3, 1, position.x, position.y);
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

Point MilitaryCampLayer::findFreePosition()
{
	Point military_point = unit_manager->getMilitaryPosition();
	GridPoint military = unit_manager->getGridPoint(military_point);
	GridPoint a = unit_manager->grid_map->findFreePositionNear(military);
	log("%d %d", a._x, a._y);
	Point p = unit_manager->getPoint(a);
	return p;
}


void CreateUnitLayer::addListenerToRect(int type)
{
	auto spriteListener = EventListenerTouchOneByOne::create();
	spriteListener->setSwallowTouches(false);
	spriteListener->onTouchBegan = [=](Touch* touch, Event* event) {
		auto target = static_cast<Unit*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode)) {
			if (can_build)
			{
				rec->setVisible(false);
				unit_manager->genCreateMessage(type,1,rec_abs_center.x,rec_abs_center.y);
				building = false;
				//��ʾ�˵�
				this->setVisible(true);
				//�������ò������
				unit_manager->getCombatScene()->destListener->setEnabled(true);
				unit_manager->getCombatScene()->removeChild(rec);
				return true;
			}			
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(spriteListener, rec);
}

void CreateUnitLayer::onBuilding(int type)
{
	rec = Sprite::create("MagentaSquare.png");
	rec->setAnchorPoint(Vec2(0.5, 0.5));
	rec_size = rec->getContentSize();
	unit_manager->getCombatScene()->addChild(rec);
	rec->setVisible(false);
	addListenerToRect(type);
	auto mouse_event = EventListenerMouse::create();
	mouse_event->onMouseMove = [&](Event *event) {
		if (building)
		{
			EventMouse* pem = static_cast<EventMouse*>(event);
			Point delta = unit_manager->getCombatScene()->delta;
			rec_center = Vec2(pem->getCursorX(), pem->getCursorY());
			rec->setPosition(rec_center);
			rec_abs_center = rec_center - delta;
			if (unit_manager->grid_map->checkPosition(unit_manager->getGridRect(rec_abs_center, rec_size)))
			{
				can_build = true;
				rec->setOpacity(255);
			}
			else
			{
				can_build = false;
				rec->setOpacity(120);
			}
			rec->setVisible(true);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mouse_event, 1);

}
