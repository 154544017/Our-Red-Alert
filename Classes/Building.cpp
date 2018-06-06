#include "Building.h"


USING_NS_CC;


Base* Base::create(const std::string & filename)
{
	Base *ret = new Base();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		if (ret->current_life) {
			ret->setMobile(false);
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

void Base::set(TMXTiledMap * AM, Layer * AL, EventListenerTouchOneByOne * ALis)
{
	tiled_map = AM;
	combat_scene = AL;
	spriteTouchListener = ALis;
}

void Base::setProperties()
{
	type = 0;
	z_index = 5;
	mobile = false;
}
void Base::setListener()
{
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Base::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Base::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Base::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


bool Base::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
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
		target->setOpacity(180);
		// �����û������¼��Ѿ�������������onTouchMoved��onTouchEnded��onTouchCancelled�������Ӧ�������¼��������򲻻���ȥ���м������δ����¼���
		return true;
	}
	return false;
}

void Base::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{

}

void Base::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// ����͸����
	target->setOpacity(255);
	std::string name;
	if (target == this)
	{	
		if (layer_is_created == false)
		{
			baselayer = BaseLayer::create();
			baselayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			baselayer->unit_manager = unit_manager;
			combat_scene->addChild(baselayer, 10);
			layer_is_created = true;
		}
		else
		{
			baselayer->setVisible(true);
		}
	}
}

MilitaryCamp* MilitaryCamp::create(const std::string & filename)
{
	MilitaryCamp *ret = new MilitaryCamp();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		if (ret->current_life) {
			ret->setMobile(false);
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

void MilitaryCamp::set(TMXTiledMap * AM, Layer * AL, EventListenerTouchOneByOne * ALis)
{
	tiled_map = AM;
	combat_scene = AL;
	spriteTouchListener = ALis;
}

void MilitaryCamp::setProperties()
{
	type = 11;
	z_index = 5;
	mobile = false;
}
void MilitaryCamp::setListener()
{
	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(MilitaryCamp::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MilitaryCamp::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MilitaryCamp::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


bool MilitaryCamp::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
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
		target->setOpacity(180);
		// �����û������¼��Ѿ�������������onTouchMoved��onTouchEnded��onTouchCancelled�������Ӧ�������¼��������򲻻���ȥ���м������δ����¼���
		return true;
	}
	return false;
}

void MilitaryCamp::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{

}

void MilitaryCamp::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// ����͸����
	target->setOpacity(255);
	std::string name;
	if (target == this)
	{
		if (layer_is_created == false)
		{
			militaryCampLayer = MilitaryCampLayer::create();
			militaryCampLayer->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
			militaryCampLayer->unit_manager = unit_manager;
			combat_scene->addChild(militaryCampLayer, 10);
			layer_is_created = true;
		}
		else
		{
			militaryCampLayer->setVisible(true);
		}
	}
}
