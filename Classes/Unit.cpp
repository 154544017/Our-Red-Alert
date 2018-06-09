#include"cocos2d.h"
#include"Unit.h"
#include "AdvancedUnit.h"
#include "Building.h"
#include "CreateUnitLayer.h"
#include "CombatScene.h"
#include<string>
#include<random>
//#define DEBUG

const int MAX_PLAYER_NUM = 4;
USING_NS_CC;
/*Ѫ������*/
void Bar::updateBarDisplay(float rate) {
#ifdef DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif // DEBUG
	
	setVisible(true);
	clear();
	auto tg = static_cast<Unit*>(this->getParent());
#ifdef DEBUG
	log("bar father:address %d", this->getParent());
#endif // DEBUG

	auto s = tg->getContentSize();
	
	drawRect(Point((s.width - length) / 2, s.height + 5), Point((s.width + length) / 2, s.height + 5 + width), color);
	Point endpoint{ s.width / 2 - length / 2 + length * rate,s.height + 5 + width };
	drawSolidRect(Point((s.width - length) / 2, s.height + 5), endpoint, color);
}

void Bar::stopKeepingVisible() {
	setVisible(false);
	is_visible = false;
}
void Bar::keepVisible() {
	setVisible(true);
	is_visible = true;
}

Unit * Unit::create(const std::string & filename)
{
	Unit *ret = new Unit();
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		if (ret->current_life) {
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

void Unit::setProperties()
{
	type = -1;
	camp = 0;
	selected = 0;

}

void Unit::displayHP() 
{
	if (hp_bar) {
		hp_bar->keepVisible();
	}
}
void Unit::hideHP() 
{
	if (hp_bar) {
		hp_bar->stopKeepingVisible();
	}
}

void Unit::setGridMap(GridMap * _grid_map)
{
	grid_map = _grid_map;
}

void Unit::set(TMXTiledMap * _tiledMap, Layer * _combatScene, EventListenerTouchOneByOne * _listener)
{
	tiled_map = _tiledMap;
	combat_scene = _combatScene;
	spriteTouchListener = _listener;
}

void Unit::setListener()
{
	Director::getInstance()->getEventDispatcher()
		->addEventListenerWithSceneGraphPriority(spriteTouchListener->clone(), this);
}

void Unit::setUnitManager(UnitManager * _unit_manager)
{
	unit_manager = _unit_manager;
}

void Unit::setMobile(bool can)
{
	mobile = can;
}

bool Unit::isMobile()
{
	return mobile;
}

void Unit::setCamp(int _camp)
{
	camp = _camp;
}

int Unit::getCamp() const
{
	return camp;
}

int Unit::getType() const
{
	return type;
}

int Unit::getSpeed() const
{
	return speed;
}

bool Unit::is_in(Point p1, Point p2) {
	Point unitPoint = this->getPosition();
	if (abs(unitPoint.x - p1.x) + abs(unitPoint.x - p2.x) != abs(p1.x - p2.x)) {
		return false;
	}
	else if(abs(unitPoint.y - p1.y) + abs(unitPoint.y - p2.y) != abs(p1.y - p2.y)) {
		return false;
	}
	return true;
}
void Unit::update(float f) {
	timer++;
	if (camp == unit_manager->player_id && timer % attack_freq == 0)
	{
		if (is_attack) {
			attack();
		}
		else {
			searchEnemy();
		}
	}
}
void Unit::searchEnemy() {
	is_attack = 1;
	attack_id = 5;
}
void Unit::attack()
{

	//unit_manager->msgs->add_game_message()->genGameMessage(GameMessage::CmdCode::GameMessage_CmdCode_ATK, id, target_id, atk, camp, 0, {});
	auto new_msg = unit_manager->msgs->add_game_message();
	new_msg->set_cmd_code(GameMessage::CmdCode::GameMessage_CmdCode_ATK);
	new_msg->set_unit_0(id);
	new_msg->set_unit_1(attack_id);
	new_msg->set_damage(5);
	new_msg->set_camp(camp);


}
bool Unit::underAttack(int damage)
{
	if (current_life > 0) {
		current_life -= damage;
		hp_bar->updateBarDisplay(static_cast<float>(current_life) / static_cast<float>(max_life));
		log("life:%d", current_life);
	}
	return true;

}
bool UnitManager::init()
{
	return true;
}

void UnitManager::setMessageSet(GameMessageSet * _msgs)
{
	msgs = _msgs;
}

void UnitManager::setSpriteTouchListener(EventListenerTouchOneByOne * _spriteTouchListener)
{
	spriteTouchListener = _spriteTouchListener;
}

void UnitManager::setTiledMap(cocos2d::TMXTiledMap * _tiledMap)
{
	tiled_map = _tiledMap;;
}

void UnitManager::setGridMap(GridMap * _grid_map)
{
	grid_map = _grid_map;
}

void UnitManager::setPlayerID(int _player_id)
{
	player_id = _player_id;
}

void UnitManager::setCombatScene(CombatScene * _combat_scene)
{
	combat_scene = _combat_scene;
}

void UnitManager::setBuilding(Building * _building)
{
	building = _building;
}
/*
GridPoint UnitManager::getUnitPosition(int _unit_id)
{
	Unit* unit = id_map.at(_unit_id);
	if (unit)
		return(unit->getGridPosition());
	else
		return{ -1, -1 };
}
*/
int UnitManager::getUnitCamp(int unit_id)
{
	Unit* unit = id_map.at(unit_id);
	if (unit)
		return(unit->camp);
	else
		return 0;
}
/*
GridPoint UnitManager::getBasePosition()
{
	return base_pos;
}
*/
CombatScene * UnitManager::getCombatScene()
{
	return combat_scene;
}

EventListenerTouchOneByOne * UnitManager::getSpriteTouchListener()
{
	return spriteTouchListener;
}

void UnitManager::updateUnitsState()
{
	for (int i = 0; i < msgs->game_message_size(); i++)
	{
		const GameMessage&  msg = msgs->game_message(i);
		if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_CRT)
		{
			int id = msg.unit_0();
			int camp = msg.camp();
			int unit_type = msg.unit_type();
			auto grid_point = msg.msg_grid_path().msg_grid_point(0);
			Unit* new_unit = createNewUnit(id, camp, unit_type,grid_point.x(),grid_point.y());
			id_map.insert(id, new_unit);
			log("id:%d", id);
		}
		else
			if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_ATK)
			{
				int unitid_0 = msg.unit_0();
				int unitid_1 = msg.unit_1();
				int damage = msg.damage();
				//log("attack! unit %d -> unit %d, damage %d", unitid_0, unitid_1, damage);
				Unit * unit_1 = id_map.at(unitid_1);
				if (unit_1)
				{
					genAttackEffect(unitid_0, unitid_1);
					unit_1->underAttack(damage);

				}
			}
	}
	msgs->clear_game_message();
}

void UnitManager::initializeUnitGroup(){
	/* ���س�ʼ������ */
	auto* obj_group = tiled_map->getObjectGroup("InitialUnits");
	auto& objs = obj_group->getObjects();
	log(objs.size());
	for (auto& obj : objs) {
		auto& dict = obj.asValueMap();
		float x = dict["x"].asFloat();
		float y = dict["y"].asFloat();
		int camp = dict["camp"].asInt();
		int type = dict["type"].asInt();

		if (camp == player_id) {
			genCreateMessage(type,camp, x, y);
		}
	}
}

void UnitManager::setBasePosition(Point base_pos)
{
	_base_pos = base_pos;
	combat_scene->focusOnBase();
}

Point UnitManager::getBasePosition() const
{
	return _base_pos;
}

Unit* UnitManager::createNewUnit(int id, int camp, int unit_type,float x,float y)
{
	Unit* nu;
	Base* tmp_base;

	switch (unit_type)
	{
	case 1:
		nu = Fighter::create("Picture/units/fighter.png");
		break;
	case 2:
		nu = Tank::create("Picture/units/tank.png");
		break;
	case 3:
		nu = Soldier::create("Picture/units/soldier.png");
		break;
	case 0:
		tmp_base = Base::create("Picture/units/base_0.png");
		base_map[id] = camp;
		nu = tmp_base;
		if (camp == player_id)
			setBasePosition(Point(x, y));
		break;
	case 11:
		nu = MilitaryCamp::create("Picture/units/base_2.png");
		break;
	default:
		break;
	}

	nu->unit_manager = this;
	nu->setProperties();
	nu->id = id;
	nu->camp = camp;
	nu->set(tiled_map, (Layer *)combat_scene, spriteTouchListener);
	nu->setGridMap(grid_map);
	nu->setListener();
	nu->setAnchorPoint(Vec2(0.5, 0.5));
	nu->setPosition(x, y);
	tiled_map->addChild(nu);
	nu->schedule(schedule_selector(Unit::update));

	return(nu);
}

//�����µ�λ���Գ���
void UnitManager::genCreateMessage(int _unit_type,int camp, float x,float y)
{
	auto new_msg = msgs->add_game_message();
	new_msg->set_cmd_code(GameMessage::CmdCode::GameMessage_CmdCode_CRT);
	new_msg->set_unit_type(_unit_type);
	new_msg->set_camp(camp);
	new_msg->set_unit_0(next_id);
	MsgGridPath *gridpath = new MsgGridPath;
	auto newgridpoint = gridpath->add_msg_grid_point();
	newgridpoint->set_x(x);
	newgridpoint->set_y(y);
	new_msg->set_allocated_msg_grid_path(gridpath);
	next_id += MAX_PLAYER_NUM;
}

float UnitManager::getPlayerMoveTime(Vec2 start_pos, Vec2 end_pos, int _speed) 
{
	float duration =sqrtf((end_pos.x - start_pos.x)*(end_pos.x - start_pos.x) +
		(end_pos.y - start_pos.y)*(end_pos.y - start_pos.y))/ _speed;
	return duration;
}

void UnitManager::playMover(Point position, Unit * _sprite) {
	//��þ����ƶ���ʱ��
	float duration = getPlayerMoveTime(_sprite->getPosition(), position,_sprite->getSpeed());
	auto moveTo = MoveTo::create(duration, position);
	auto sequence = Sequence::create(moveTo, nullptr);
	_sprite->runAction(sequence);
};

void UnitManager::selectEmpty(Point position)
{
	for (auto& id : selected_ids)
	{
		if (id_map.at(id)->getNumberOfRunningActions() != 0)
		{
			id_map.at(id)->stopAllActions();
		}
		if (id_map.at(id)->isMobile())
		{
			playMover(position, id_map.at(id));
		}
	}
}

void UnitManager::selectPointUnits(Unit * _unit)
{
	if (_unit->camp == player_id)
	{
		cancellClickedUnit();
		selected_ids.push_back(_unit->id);
		getClickedUnit();
	}
}

void UnitManager::getClickedUnit() 
{
	for (auto& id : selected_ids)
	{
		id_map.at(id)->displayHP();
		id_map.at(id)->setOpacity(180);
	}
}

void UnitManager::cancellClickedUnit() 
{
	for (auto& id : selected_ids)
	{
		id_map.at(id)->hideHP();
		id_map.at(id)->setOpacity(255);
	}
	selected_ids.clear();
	selected_ids.shrink_to_fit();
}
void UnitManager::genAttackEffect(int unit_id0, int unit_id1)
{
	//log("position %f,%f,%f,%f", cur_fp.x, cur_fp.y, target_fp.x, target_fp.y);
	Unit* unit_0 = id_map.at(unit_id0);
	Unit* unit_1 = id_map.at(unit_id1);
	if (unit_0 && unit_1)
	{
		auto trajectory_effect = TrajectoryEffect::create();
		trajectory_effect->setPath(unit_0->getPosition(), unit_1->getPosition());
		tiled_map->addChild(trajectory_effect, 20);
	}
}
bool TrajectoryEffect::init()
{
	if (!ParticleFire::init())
		return false;
	setScale(0.1);
	setPositionType(PositionType::RELATIVE);
	return true;
}

void TrajectoryEffect::setPath(cocos2d::Vec2 from, cocos2d::Vec2 to)
{
	from_ = from;
	to_ = to;
	setPosition(from_);
	move_ = (to_ - from_).getNormalized()*speed_;
	schedule(schedule_selector(TrajectoryEffect::updatefire));
}

void TrajectoryEffect::updatefire(float)
{
	if (((abs(getPosition().x - to_.x)<speed_) && (abs(getPosition().y - to_.y)<speed_)))
		removeFromParent();
	else
		setPosition(getPosition() + move_);
}
/*
void UnitManager::selectUnits(Point select_point)
{
	
	if (selected_ids.size())
	{
		
		for (auto & id_unit : id_map)
			if (id_unit.second->camp != player_id && id_unit.second->getBoundingBox().containsPoint(select_point))
			{

				for (auto & id : selected_ids)
				{
					//log("Unit ID: %d, tracing enemy id: %d", id, id_unit.second->id);
					Unit* unit = id_map.at(id);
					if (!unit || !unit->isMobile())
						continue;
					GridPoint target_pos = getUnitPosition(id_unit.first);
					unit->setTarget(id_unit.second->id);
					unit->setDestination(target_pos);
					log("Unit %d, start tracing FP", id);
					unit->tryToFindPath();
				}
				return;
			}
		for (auto & id_unit : id_map)
			if (id_unit.second->camp == player_id && id_unit.second->getBoundingBox().containsPoint(select_point))
			{
				deselectAllUnits();
				selected_ids.push_back(id_unit.first);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/selecttarget.wav");
				id_unit.second->displayHPBar();
				return;
			}
		for (auto & id : selected_ids)
		{
			Unit* unit = id_map.at(id);

			if (!unit || !unit->isMobile())
				continue;

			GridPoint grid_dest = grid_map->getGridPoint(select_point);
			log("Unit ID: %d, plan to move to:(%d, %d)", id, grid_dest.x, grid_dest.y);

			unit->setDestination(grid_dest);
			log("Unit %d, start moving FP", id);
			unit->abandonTracing();
			unit->tryToFindPath();

		}
		return;
	}
	else
		for (auto & id_unit : id_map)
			if (id_unit.second->camp == player_id && id_unit.second->getBoundingBox().containsPoint(select_point))
			{
				deselectAllUnits();
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/selecttarget.wav");
				selected_ids.push_back(id_unit.first);
				id_unit.second->displayHPBar();
				return;
			}
	
	return;
}
*/