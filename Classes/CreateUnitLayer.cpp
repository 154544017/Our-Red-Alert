#include "CreateUnitLayer.h"
#include "Building.h"
#include "CombatScene.h"
#include "SimpleAudioEngine.h"
#include "Const.h"
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
	layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width*0.9, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);
	//��Ӵ��������İ�ť
	// ����һ������militaryCamp��Button����������Layout�����Ͻ�
	militaryCamp = Button::create("/Picture/units/base_3.png",
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
			if (checkBuilding(MILITARY_CAMP_MONEY, BUILDING_CONSUME_POWER))
			{				
				//���ò������
				unit_manager->getCombatScene()->destListener->setEnabled(false);
				//���ز˵�
				this->setVisible(false);
				onBuilding(11,MilitaryCamp::size);
			}				
		}				
	});
	//���һ������mine��Button����������layout�����Ͻ�
	Button* mine = Button::create("/Picture/ui/gold.png",
		"/Picture/ui/gold.png");
	mine->setScale(0.1);
	layout->addChild(mine);
	RelativeLayoutParameter* rp_top_right = RelativeLayoutParameter::create();
	rp_top_right->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
	mine->setLayoutParameter(rp_top_right);
	//��mine��Ӽ�����
	mine->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{			
			if (checkBuilding(MINE_MONEY, BUILDING_CONSUME_POWER))
			{
				//���ò������
				unit_manager->getCombatScene()->destListener->setEnabled(false);
				//���ز˵�
				this->setVisible(false);
				onBuilding(12,Mine::size);
			}
		}
	});

	//���һ������powerPlant��Button����������layout�����в�
	powerPlant = Button::create("power.png",
		"power.png");
	powerPlant->setScale(0.15);
	layout->addChild(powerPlant);
	RelativeLayoutParameter* rp_left_center = RelativeLayoutParameter::create();
	rp_left_center->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
	powerPlant->setLayoutParameter(rp_left_center);
	//��powerPlant��Ӽ�����
	powerPlant->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (checkBuilding(POWER_PLANT_MONEY, 0))
			{
				//���ò������
				unit_manager->getCombatScene()->destListener->setEnabled(false);
				//���ز˵�
				this->setVisible(false);
				onBuilding(13, PowerPlant::size);
			}
		}
	});

	//���һ������TankFactary��Button����������layout���Ҳ�
	tankFactary = Button::create("basebutton.png",
		"basebutton.png");
	tankFactary->setScale(0.8);
	layout->addChild(tankFactary);
	RelativeLayoutParameter* rp_right_center = RelativeLayoutParameter::create();
	rp_right_center->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
	tankFactary->setLayoutParameter(rp_right_center);
	//��tankFacktary��Ӽ�����
	tankFactary->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (checkBuilding(TANK_FACTARY_MONEY, BUILDING_CONSUME_POWER))
			{
				//���ò������
				unit_manager->getCombatScene()->destListener->setEnabled(false);
				//���ز˵�
				this->setVisible(false);
				onBuilding(14,TankFactary::size);
			}
		}
	});

	// ����һ���˳���layer��Button����
	exit = Button::create("backNormal.png",
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
	layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width*0.9, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);

	// ����һ������ariplain��Button����������Layout�����Ͻ�
	airplane = Button::create("/Picture/menu/airplane-menu-up.png",
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
			if (unit_manager->money->checkMoney(2000))
			{
				auto position = findFreePosition();
				unit_manager->genCreateMessage(1, 1, position.x, position.y);
				unit_manager->money->spendMoney(2000);
			}
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/insufficientfound.wav");	
		}
	});

	// ����һ������soldier��Button����������Layout������
	soldier = Button::create("/Picture/menu/soldier-menu-up.png",
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
			if (unit_manager->money->checkMoney(2000))
			{
				auto position = findFreePosition();
				unit_manager->genCreateMessage(3, 1, position.x, position.y);
				unit_manager->money->spendMoney(2000);
			}
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/insufficientfound.wav");
		}
	});

	// ����һ���˳���layer��Button����
	exit = Button::create("backNormal.png",
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


bool TankFactaryLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bc = Sprite::create("bc.png");
	//��ʼ��layout
	layout = Layout::create();
	layout->setLayoutType(LayoutType::RELATIVE);
	layout->setContentSize(Size(bc->getContentSize().width*0.9, bc->getContentSize().height*0.8));
	layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	layout->setBackGroundImage("bc.png");
	this->addChild(layout);

	// ����һ����������̹�˵�Button����������Layout�����Ͻ�
	tank = Button::create("/Picture/menu/tianqi.png",
		"/Picture/menu/tianqi.png");
	layout->addChild(tank);
	RelativeLayoutParameter* rp_TopLeft = RelativeLayoutParameter::create();
	rp_TopLeft->setAlign(RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
	tank->setLayoutParameter(rp_TopLeft);
	tank->setScale(0.33);
	//������̹����Ӽ�����
	tank->addTouchEventListener([=](Ref * pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (unit_manager->money->checkMoney(2000))
			{
				auto position = findFreePosition();
				unit_manager->genCreateMessage(2, 1, position.x, position.y);
				unit_manager->money->spendMoney(2000);
			}
			else
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/insufficientfound.wav");
		}
	});
	// ����һ���˳���layer��Button����
	exit = Button::create("backNormal.png",
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

TankFactaryLayer * TankFactaryLayer::create()
{
	TankFactaryLayer *ret = new TankFactaryLayer();
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

Point CreateUnitLayer::findFreePosition()
{
	GridPoint create_center = unit_manager->getGridPoint(center_position);
	GridPoint a = unit_manager->grid_map->findFreePositionNear(create_center);
	unit_manager->setUnitCreateCenter(center_position);
	Point p = unit_manager->getPoint(a);
	return p;
}

void CreateUnitLayer::setCenterPosition(Point p)
{
	center_position = p;
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
				//�رջ��ؽ��췶Χ
				unit_manager->constructRange->setVisible(false);
				return true;
			}			
		}
		return false;
	};
	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(spriteListener, rec);
}

bool CreateUnitLayer::checkBuilding(int money, int power)
{
	if (unit_manager->money->checkMoney(money))
	{
		if (unit_manager->power->checkPower(power))
		{
			building = true;		
			unit_manager->money->spendMoney(money);
			unit_manager->power->spendPower(power);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/insufficientfound.wav");
		return false;
	}
}

void CreateUnitLayer::onBuilding(int type,Size size)
{
	unit_manager->constructRange->setVisible(true);
	rec = Sprite::create("MagentaSquare.png");
	rec->setAnchorPoint(Vec2(0.5, 0.5));
	rec->setContentSize(size);
	rec_size = size;
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
			if (unit_manager->grid_map->checkPosition(unit_manager->getGridRect(rec_abs_center, rec_size)) && checkInRange(rec_abs_center))
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

bool CreateUnitLayer::checkInRange(Point p)
{
	Point baseP = unit_manager->getBasePosition();
	float range = unit_manager->base->construct_range;
	float distance = (p.x - baseP.x)*(p.x - baseP.x) + (p.y - baseP.y)*(p.y - baseP.y);
	return distance <= range * range;
}

