#include "Building.h"
#include "CreateUnitLayer.h"

USING_NS_CC;


Building * Building::create(const std::string & filename)
{
	Building *ret = new Building();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		if (ret->current_life) {
			ret->set_movable(false);
			ret->hp_bar = Bar::create();
			float rate = static_cast<float>(ret->current_life) / static_cast<float>(ret->max_life);
			ret->addChild(ret->hp_bar, 10);
			ret->hp_bar->updateBarDisplay(rate);
			ret->hp_bar->stopKeepingVisible();
		}
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

Layer * Building::getLayer()
{
	return addLayer;
}


TMXTiledMap* Building::getMap()
{
	return addMap;
}

void Building::setType(int ty = 1)
{
	type = ty;
	this->addListener();
}

int Building::getType()
{
	return type;
}

void Building::addListener()
{
	
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Building::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Building::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Building::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


bool Building::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	// ��õ�ǰ�¼�������Ŀ�����
	auto target = static_cast<Unit*>(event->getCurrentTarget());
	// ��õ�ǰ�Ĵ�����
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	// ��ô��������ContentSize
	Size s = target->getContentSize();
	// ���λ�þ���
	Rect rect = Rect(0, 0, s.width, s.height);
	// ���λ�þ��ΰ���������
	if (rect.containsPoint(locationInNode))
	{
		log("onTouchBegan... x = %f, y = %f", locationInNode.x, locationInNode.y);
		// ����͸����
		/*
		getLayer()->cancellClickedUnit();
		getLayer()->getSelected_box().push_back(target);	
		getLayer()->getClickedUnit();
		*/
		target->setOpacity(180);
		// �����û������¼��Ѿ�������������onTouchMoved��onTouchEnded��onTouchCancelled�������Ӧ�������¼��������򲻻���ȥ���м������δ����¼���
		return true;
	}
	return false;
}

void Building::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{

}

void Building::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// ����͸����
	target->setOpacity(255);
	std::string name;
	if (target == this)
	{
		if (type == 1)
		{
			auto createUnitLayer = MilitaryCampLayer::create();
			createUnitLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			createUnitLayer->set(addMap, addLayer, addTouchListener);
			getLayer()->addChild(createUnitLayer, 10);
		}
		else if (type == 0)
		{
			auto createUnitLayer = BaseLayer::create();
			createUnitLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			createUnitLayer->set(addMap, addLayer, addTouchListener);
			getLayer()->addChild(createUnitLayer, 10);
		}
	}
}
