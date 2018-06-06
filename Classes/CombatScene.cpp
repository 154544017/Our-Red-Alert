#include "CombatScene.h"
#include "ui\CocosGUI.h"
#include "Unit.h"
#define DEBUG
USING_NS_CC;
using namespace ui;

#define BOX_EDGE_WITDH_SMALL 10
#define BOX_EDGE_WITDH 40
#define SCROLL_LENGTH 10

void MouseRect::update(float f)
{
	clear();
	Node* parent = getParent();
	drawRect(touch_start, touch_end, Color4F(0, 1, 0, 1));
}
void CombatScene::DrawRectArea(Point p1, Point p2) {
	DrawNode* drawNode = DrawNode::create();
	this->addChild(drawNode);
	drawNode->drawRect(p1, p2, Color4F(0, 1, 0, 1));
}
void CombatScene::getLayerUnit(Point p1, Point p2) {
	const auto&arrayNode = this->_combat_map->getChildren();
	for (auto&child : arrayNode) {
		Unit * pnode = static_cast<Unit *>(child);
		if (pnode && pnode->is_in(p1-delta, p2-delta)) {
			selected_box.push_back(pnode);
			
		}
	}
}
float CombatScene::getPlayerMoveTime(Vec2 start_pos, Vec2 end_pos) {
	float duration = 0.05f;
	duration = duration*sqrtf((end_pos.x - start_pos.x)*(end_pos.x - start_pos.x) +
		(end_pos.y - start_pos.y)*(end_pos.y - start_pos.y));
	return duration;
}
void CombatScene::playMover(Point position, Unit * _sprite) {
	//��þ����ƶ���ʱ��
	float duration = getPlayerMoveTime(_sprite->getPosition(), position);
	auto moveTo = MoveTo::create(duration, position);
	auto sequence = Sequence::create(moveTo, nullptr);
	_sprite->runAction(sequence);
};
/*
std::vector<Unit*> & CombatScene::getSelected_box()
{
	return selected_box;
}
*/
Scene * CombatScene::createScene() {
	auto scene = Scene::create();
	auto layer = CombatScene::create();

	scene->addChild(layer);
	return scene;
}

bool CombatScene::init() {
	if (!Layer::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/* ���ص�ͼ */
	_combat_map = TMXTiledMap::create("map/BasicMap1.tmx");
	_combat_map->setAnchorPoint(Vec2(0, 0));
	addChild(_combat_map, 0);

	/*���ؾ���ѡ�����*/
	mouse_rect = MouseRect::create();
	mouse_rect->setVisible(false);
	addChild(mouse_rect, 15);

	msgs = new GameMessageSet;
	unit_manager = UnitManager::create();
	unit_manager->retain();
	unit_manager->setMessageSet(msgs);
	unit_manager->setTiledMap(_combat_map);
	unit_manager->setCombatScene(this);

#ifdef DEBUG//����
	/*
	auto worker_sprite = Unit::create("MagentaSquare.png");
	worker_sprite->setPosition(Vec2(visibleSize.width / 2 - 100, visibleSize.height / 2));
	*/
	auto farmer_sprite = Unit::create("MagentaSquare.png");
	farmer_sprite->setPosition(Vec2(visibleSize.width / 2 + 100, visibleSize.height / 2));
	//this->_combat_map->addChild(worker_sprite,10);
	this->_combat_map->addChild(farmer_sprite,10);
#endif
	/*ˢ�½��ܹ�����Ӧ*/
	schedule(schedule_selector(CombatScene::update));

	/* �õ����ÿһ֡��λ�� */
	auto mouse_event = EventListenerMouse::create();
	mouse_event->onMouseMove = [&](Event *event) {
		EventMouse* pem = static_cast<EventMouse*>(event);
		_cursor_position = Vec2(pem->getCursorX(), pem->getCursorY());
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(mouse_event, 1);

	//�õ���ʼ����λ
	unit_manager->setPlayerID(1);
	unit_manager->initializeUnitGroup();


	/*���ؾ���������¼�*/
	auto spriteListener = EventListenerTouchOneByOne::create();
	spriteListener->setSwallowTouches(true);
	spriteListener->onTouchBegan = [this](Touch* touch, Event* event) {
		mouse_rect->touch_start = touch->getLocation();
		auto target = static_cast<Unit*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode)) {
			//log("touch %d", target);
			cancellClickedUnit();
			selected_box.push_back(target);
			getClickedUnit();
			return true;
		}
		return false;
	};
	spriteListener->onTouchEnded = [this](Touch* touch, Event* event) {
		mouse_rect->touch_end = touch->getLocation();
	};
	unit_manager->setSpriteTouchListener(spriteListener);
#ifdef DEBUG
	schedule(schedule_selector(CombatScene::update));
	/*
	auto base1 = Building::create("base_0.png");
	base1->set(this->_combat_map, this);
	base1->setType(0);
	base1->setPosition(Vec2(visibleSize.width / 2 - 100, visibleSize.height / 2));
	this->_combat_map->addChild(base1, 10);
	*/
	//Director::getInstance()->getEventDispatcher()
	//	->addEventListenerWithSceneGraphPriority(spriteListener, worker_sprite);
	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(spriteListener->clone(), farmer_sprite);
#endif
	/*���ز������ʱ��*/
	auto destListener = EventListenerTouchOneByOne::create();

	destListener->onTouchBegan = [this](Touch* touch, Event* event) {
		mouse_rect->touch_start = touch->getLocation();
		mouse_rect->schedule(schedule_selector(MouseRect::update));
		return true;
	};

	destListener->onTouchMoved = [this](Touch* touch, Event* event) {
		mouse_rect->touch_end = touch->getLocation();//�������Ӵ��ĵ�
		mouse_rect->setVisible(true);
	};

	destListener->onTouchEnded = [this](Touch* touch, Event* event) {
		if (mouse_rect->isScheduled(schedule_selector(MouseRect::update)))
			mouse_rect->unschedule(schedule_selector(MouseRect::update));
		mouse_rect->touch_end = touch->getLocation();			
#ifdef DEBUG
		//AllocConsole();
		//freopen("CONIN$", "r", stdin);
		//freopen("CONOUT$", "w", stdout);
		//freopen("CONOUT$", "w", stderr);
		//log("get posititon %f %f", _combat_map->getPosition().x, _combat_map->getPosition().y);
		//log("error buffer:%f %f", mouse_rect->touch_start.x, mouse_rect->touch_start.y);
		//log("error buffer:%f %f", mouse_rect->touch_end.x, mouse_rect->touch_end.y);
		//log("%f", Distance_m2(mouse_rect->touch_start, mouse_rect->touch_end));
#endif // DEBUG
		if (Tri_Dsitance(mouse_rect->touch_start, mouse_rect->touch_end) > 8) {
			cancellClickedUnit();
		}
		if (selected_box.size()) {
			for (int i = 0; i != selected_box.size(); i++) {
				if (selected_box[i]->getNumberOfRunningActions() != 0) {
					selected_box[i]->stopAllActions();
				}
				mouse_rect->setVisible(false);
				if (selected_box[i]->isMobile())
					playMover(mouse_rect->touch_end - delta, selected_box[i]);
			}
		}
		else {
			mouse_rect->setVisible(false);
			if (mouse_rect->isScheduled(schedule_selector(MouseRect::update))) {
				mouse_rect->unschedule(schedule_selector(MouseRect::update));
			}
			getLayerUnit(mouse_rect->touch_start, mouse_rect->touch_end);
			getClickedUnit();
		}
	};
	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(destListener, this->_combat_map);
	return true;
}

void CombatScene::focusOnBase(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto base_point = unit_manager->getBasePosition();
	Vec2 base_vec = base_point - visibleSize / 2;

	/* ����Ի���Ϊ���ĵ���Ұ������GridMap�Ĵ�С */
	if(_combat_map->getBoundingBox().size.height<base_vec.y+visibleSize.height)
		base_vec.y = _combat_map->getBoundingBox().size.height - visibleSize.height;
	if (_combat_map->getBoundingBox().size.width < base_vec.x + visibleSize.width)
		base_vec.x = _combat_map->getBoundingBox().size.width - visibleSize.width;
	if (base_vec.x < 0)
		base_vec.x = 0;
	if (base_vec.y < 0)
		base_vec.y = 0;

	_combat_map->setPosition(Point(0, 0) - base_vec);
}

void CombatScene::update(float f){
	unit_manager->updateUnitsState();
	scrollMap();
	delta = _combat_map->getPosition();
}
void CombatScene::scrollMap(){
	auto map_center = _combat_map->getPosition();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int horizontal_state, vertical_state;
	horizontal_state = (origin.x + visibleSize.width - BOX_EDGE_WITDH_SMALL < _cursor_position.x)
		+ (origin.x + visibleSize.width - BOX_EDGE_WITDH < _cursor_position.x)
		- (origin.x + BOX_EDGE_WITDH_SMALL > _cursor_position.x)
		- (origin.x + BOX_EDGE_WITDH > _cursor_position.x);

	vertical_state = (origin.y + visibleSize.height - BOX_EDGE_WITDH_SMALL < _cursor_position.y)
		+ (origin.y + visibleSize.height - BOX_EDGE_WITDH < _cursor_position.y)
		- (origin.y + BOX_EDGE_WITDH_SMALL > _cursor_position.y)
		- (origin.y + BOX_EDGE_WITDH > _cursor_position.y);

	Vec2 scroll(0, 0);
	scroll += Vec2(-SCROLL_LENGTH, 0)*horizontal_state;
	scroll += Vec2(0, -SCROLL_LENGTH)*vertical_state;
	map_center += scroll;
	//move_amount -= scroll;
	if (_combat_map->getBoundingBox().containsPoint((-scroll) + Director::getInstance()->getVisibleSize())
		&& _combat_map->getBoundingBox().containsPoint(-scroll)) {
		_combat_map->setPosition(map_center);
	}
	

}
