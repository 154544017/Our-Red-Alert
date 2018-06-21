#include"cocos2d.h"
#include"Unit.h"
#include "AdvancedUnit.h"
#include "Building.h"
#include "CreateUnitLayer.h"
#include "CombatScene.h"
#include<string>
#include<random>
#include "SimpleAudioEngine.h"
#include "iostream"
#define DEBUG

const int MAX_PLAYER_NUM = 4;
USING_NS_CC;

/*Ѫ������*/
void Bar::updateBarDisplay(float rate) {

	setVisible(true);
	clear();
	auto tg = static_cast<Unit*>(this->getParent());
#ifdef DEBUG
	log("bar father:address %d", this->getParent());
#endif // DEBUG

	auto s = tg->getContentSize();

	drawRect(Point((s.width - length) / 2, s.height + height), Point((s.width + length) / 2, s.height + height + width), color);
	Point endpoint{ s.width / 2 - length / 2 + length * rate,s.height + height + width };
	drawSolidRect(Point((s.width - length) / 2, s.height + height), endpoint, color);
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

void Unit::initBar()
{
	if (current_life) {
		hp_bar = Bar::create();
		float rate = static_cast<float>(current_life) / static_cast<float>(max_life);
		addChild(hp_bar, 10);
		hp_bar->updateBarDisplay(rate);
		hp_bar->stopKeepingVisible();
	}
}

void Unit::setProperties()
{

	type = -1;
	camp = 0;
	selected = 0;

}

void Unit::removeFromMaps()
{
	grid_map->leavePosition(_cur_pos);
	tiled_map->removeChild(this, 1);
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

void Unit::move()
{
	//espΪ��ǰ���ָ��ǰ�յ�ĵ�λ����(�ⲽ�����Ż���
	auto esp = (_cur_dest_point - getPosition()).getNormalized();
	Point next_position = esp * speed + getPosition();
	GridPoint next_gpos = grid_map->getGridPoint(next_position);

	if (_cur_pos == next_gpos)
	{
		setPosition(next_position);
	}
	else if (grid_map->occupyPosition(id, next_gpos))
	{
		setPosition(next_position);
		grid_map->leavePosition(_cur_pos);
		_cur_pos = next_gpos;
	}
	else
	{
#ifdef DEBUG
		std::cout << "failed:try to occupyPosition " << next_gpos._x << "," << next_gpos._y << std::endl;
#endif // DEBUG
		_cur_dest = _cur_pos;
		setCurDestPoint(_cur_dest);
		Point final_dest = grid_map->getPointWithOffset(_final_dest);
		if (camp == unit_manager->player_id && (final_dest - getPosition()).length() > REFIND_PATH_MAX_RANGE) {
			if (!is_delaying)
				tryToSearchForPath();
		}
	}
	if (hasArrivedFinalDest()) {
		if (_grid_path.size()) {
			_cur_dest = _grid_path.back();
			setCurDestPoint(_cur_dest);
			_grid_path.pop_back();
		}
		else
			is_moving = false;
	}
}

void Unit::delay()
{
	if (del_cnt > 0)
		--del_cnt;
	else if (camp == unit_manager->player_id)
		tryToSearchForPath();
}

bool Unit::hasArrivedFinalDest()
{
	return(grid_map->hasApproached(getPosition(), _cur_dest) && getGridPosition() == _cur_dest);
}

GridPath Unit::findPath(const GridPoint & dest) const
{
	auto& gmap = grid_map->getLogicalGridMap();
	GridPoint start = getGridPosition();

	PathFinding path_finding(gmap, start, dest);
	path_finding.searchForPath();
	path_finding.generatePath();
	GridPath grid_path = path_finding.getPath();

	GridPath optimized_path = optimizePath(grid_path);
	return optimized_path;
}

void Unit::setDelayPathFinding(int cnt)
{
	is_delaying = true;
	del_cnt = cnt;
}

GridPath Unit::optimizePath(const GridPath & origin_path) const
{
	auto path_length = origin_path.size();
	if (path_length < 3)
		return origin_path;

	GridPath optimized_path;
	GridPoint pre_point = origin_path[0];
	GridVec pre_dir = { 2,2 };
	for (int i = 1; i < path_length - 1; i++) {
		const auto & point = origin_path[i];
		const auto & dir = (point - pre_point).getDirectionVec();

		if (!(dir == pre_dir)) {
			optimized_path.push_back(pre_point);
			pre_dir = dir;
		}
		pre_point = point;
	}
	optimized_path.push_back(origin_path[path_length - 1]);
	return optimized_path;
}

void Unit::tryToSearchForPath()
{
	if (camp != unit_manager->player_id)
		return;

	if (!grid_map->checkPosition(_final_dest))
		_final_dest = grid_map->findFreePositionNear(_final_dest);

	GridPath grid_path = findPath(_final_dest);
	if (grid_path.size()) {
		del_cnt = -1;
		is_delaying = false;
		auto new_msg = unit_manager->msgs->add_game_message();
		new_msg->set_cmd_code(GameMessage::CmdCode::GameMessage_CmdCode_MOV);
		new_msg->set_unit_type(type);
		new_msg->set_unit_0(id);
		new_msg->set_camp(camp);
		new_msg->genSetGridPath(grid_path);
	}
	else {
		del_cnt = unit_manager->genRandom(16, 32);
		is_delaying = true;
	}
}

void Unit::setGridMap(GridMap * _grid_map)
{
	grid_map = _grid_map;
}

void Unit::setCurPos(const GridPoint _cur)
{
	_cur_pos = _cur;
}

void Unit::set(TMXTiledMap * _tiledMap, GridMap * _gridMap, Layer* _combat_scene, EventListenerTouchOneByOne * _listener)
{
	tiled_map = _tiledMap;
	tiled_map->addChild(this, z_index);
	combat_scene = _combat_scene;
	grid_map = _gridMap;
	spriteTouchListener = _listener;
	setListener();
}

void Unit::addToGmap(Point p)
{
#ifdef DEBUG
	std::cout << "Unit type: " << this->type << std::endl;
#endif // DEBUG
	grid_map->occupyPosition(this->id, p);
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

EventListenerTouchOneByOne * Unit::getListener()
{
	return spriteTouchListener;
}

void Unit::setMobile(bool can)
{
	mobile = can;
}

bool Unit::isMobile()
{
	return mobile;
}

GridPoint Unit::getGridPosition() const
{
	if (grid_map)
		return grid_map->getGridPoint(getPosition());
	else return GridPoint(0, 0);
}

void Unit::setGridPath(const MsgGridPath & msg_grid_path)
{
	int grid_point_size = msg_grid_path.msg_grid_point_size();
	_grid_path = GridPath(grid_point_size);

	for (int i = 0; i < grid_point_size; i++)
		_grid_path[i] = GridPoint{ msg_grid_path.msg_grid_point(i).x(), msg_grid_path.msg_grid_point(i).y() };

	_final_dest = _grid_path[0];
	_cur_dest = _grid_path.back();
	setCurDestPoint(_cur_dest);
	_grid_path.pop_back();
}

void Unit::setDestination(const GridPoint & grid_dest)
{
	_final_dest = grid_dest;
}

void Unit::setCurDestPoint(const GridPoint & grid_dest)
{
	_cur_dest_point = grid_map->getPointWithOffset(_cur_dest);
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
	else if (abs(unitPoint.y - p1.y) + abs(unitPoint.y - p2.y) != abs(p1.y - p2.y)) {
		return false;
	}
	return true;
}

void Unit::update(float f)
{
	timer++;
	if (is_moving)
		move();
	if (is_delaying)
		delay();

	if (camp == unit_manager->player_id && timer % attack_freq == 0) {
		searchEnemy();
		if (is_attack) {
			attack();
		}
	}
}

void Unit::searchEnemy()
{
	const auto & auto_atk_rect = GridRect(_cur_pos - attack_range / 2, attack_range);
	const auto & unit_ids = grid_map->getUnitIDAt(auto_atk_rect);
	for (auto its_id : unit_ids)
	{
		int its_camp = unit_manager->getUnitCamp(its_id);
		if (its_camp != camp)
		{
			log("its_camp:%d   camp:%d", its_camp, camp);
			attack_id = its_id;
			is_attack = true;
			return;
		}
	}
	return;
}

void Unit::attack()
{
	auto new_msg = unit_manager->msgs->add_game_message();
	new_msg->set_cmd_code(GameMessage::CmdCode::GameMessage_CmdCode_ATK);
	new_msg->set_unit_0(id);
	new_msg->set_unit_1(attack_id);
	new_msg->set_damage(5);
	new_msg->set_camp(camp);
	is_attack = false;
}

bool Unit::underAttack(int damage)
{

	current_life -= damage;
	hp_bar->updateBarDisplay(static_cast<float>(current_life) / static_cast<float>(max_life));

	if (current_life < 0) {
		return true;
	}
	else {
		return false;
	}

}

bool UnitManager::init()
{
	std::random_device rd;						//���÷�ȷ����������������������������
	gen = std::default_random_engine(rd());		//����Ĭ�������������������
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
	next_id = _player_id;
}

void UnitManager::setCombatScene(CombatScene * _combat_scene)
{
	combat_scene = _combat_scene;
}

void UnitManager::setSocketClient(chat_client * _socket_client)
{
	socket_client = _socket_client;
}

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

TMXTiledMap * UnitManager::getTiledMap()
{
	return tiled_map;
}

EventListenerTouchOneByOne * UnitManager::getSpriteTouchListener()
{
	return spriteTouchListener;
}

void UnitManager::updateUnitsState()
{
	auto sent_msg_str = msgs->SerializeAsString();
	socket_client->write_data(sent_msg_str);

	auto msg_str = socket_client->read_data();
	msgs->ParseFromString(msg_str);

	for (int i = 0; i < msgs->game_message_size(); i++)
	{
		const GameMessage&  msg = msgs->game_message(i);
		if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_CRT)
		{
			int id = msg.unit_0();
			int camp = msg.camp();
			int unit_type = msg.unit_type();
			auto grid_point = msg.msg_grid_path().msg_grid_point(0);
			Unit* new_unit = createNewUnit(id, camp, unit_type, grid_point.x(), grid_point.y());
			id_map.insert(id, new_unit);
		}
		else if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_ATK)
		{
			int unitid_0 = msg.unit_0();
			int unitid_1 = msg.unit_1();
			int damage = msg.damage();

			Unit * unit_1 = id_map.at(unitid_1);
			Unit * unit_0 = id_map.at(unitid_0);

			if (unit_1) {
				log("attack! unitcamp %d -> unitcamp %d, damage %d\n", unit_0->camp, unit_1->camp, damage);
				genAttackEffect(unitid_0, unitid_1);
				if (unit_1->underAttack(damage))
				{
					if (unit_1->getType() == 0) {
						player_num--;
						checkWinOrLose(unitid_1);
					}
					deleteUnit(unitid_1);
				}
			}
		}
		else if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_MOV)
		{
			Unit * unit_0 = id_map.at(msg.unit_0());
			const MsgGridPath& path = msg.msg_grid_path();
			if (path.msg_grid_point_size())
			{
				unit_0->setGridPath(msg.msg_grid_path());
				unit_0->is_moving = true;
			}
		}
	}
	msgs->clear_game_message();
}

void UnitManager::initializeUnitGroup() {
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
			genCreateMessage(type, camp, x, y);
#ifdef DEBUG
			std::cout << "type: " << type << "camp: " << camp << "pos: " << x << "," << y << std::endl;
#endif // DEBUG
		}
	}
}

void UnitManager::setMax_power(int delta)
{
	power->addMax_power(delta);
}

void UnitManager::setIncreasingAmount(int amount)
{
	money->setIncreasingAmount(amount);
}

void UnitManager::setUnitCreateCenter(Point center)
{
	unit_create_center = center;
}

void UnitManager::deleteUnit(int id)
{
	Unit* unit = id_map.at(id);
	if (unit)
	{
		auto itor = std::find(selected_ids.begin(), selected_ids.end(), id);
		if (itor != selected_ids.end())
			selected_ids.erase(itor);

		unit->removeFromMaps();

		id_map.erase(id);
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

Point UnitManager::getUnitCreateCenter()
{
	return unit_create_center;
}

GridSize UnitManager::getGridSize(Size _size)
{
	auto size = getGridPoint(_size);
	return GridSize(size._x, size._y);
}

GridRect UnitManager::getGridRect(Point _p, Size _size)
{
	return GridRect(getGridPoint(_p), getGridSize(_size));
}

GridPoint UnitManager::getGridPoint(Point p)
{
	return grid_map->getGridPoint(p);
}

Point UnitManager::getPoint(GridPoint gp)
{
	return grid_map->getPoint(gp);
}


Unit* UnitManager::createNewUnit(int id, int camp, int unit_type, float x, float y)
{
	Unit* nu;
	Base* tmp_base;
	std::vector<std::string > pic_paths = {
		 "Picture/units/base_", "Picture/units/airplane_", "Picture/units/footman_front_",
		"Picture/units/robot_front_0_"
	};

	int pic_num = camp % 4;
	std::string pic_file;
	if (unit_type<4)
		 pic_file = pic_paths[unit_type] + std::to_string(pic_num) + ".png";

	switch (unit_type)
	{
	case 1:
		nu = Fighter::create(pic_file);
		break;
	case 2:
		nu = Tank::create(pic_file);
		break;
	case 3:
		nu = Soldier::create(pic_file);
		break;
	case 0:
		tmp_base = Base::create(pic_file);
		base = tmp_base;
		base_map[id] = camp;
		nu = tmp_base;
		if (camp == player_id)
			setBasePosition(Point(x, y));
		constructRange = ConstructRange::create();
		constructRange->drawRange(Point(x, y), tmp_base->construct_range);
		constructRange->setVisible(false);
		getTiledMap()->addChild(constructRange, 4);
		break;
	case 11:
		nu = MilitaryCamp::create("Picture/units/military.png");
		break;
	case 12:
		nu = Mine::create("Picture/units/mine_0.png");
		break;
	case 13:
		nu = PowerPlant::create("Picture/units/power_0.png");
		break;
	case 14:
		nu = TankFactary::create("Picture/units/base_3.png");
	default:
		break;
	}

	nu->unit_manager = this;
	nu->setProperties();
	nu->id = id;
	nu->camp = camp;
	nu->set(tiled_map, grid_map, (Layer*)combat_scene, spriteTouchListener);
	nu->initBar();
	nu->setAnchorPoint(Vec2(0.5, 0.5));
	if (nu->isMobile())
	{
		if (getUnitCreateCenter() != Point(0, 0))
		{
			auto centerGP = getGridPoint(Point(getUnitCreateCenter().x, getUnitCreateCenter().y));
			for (int i = 1; i < 10; i++)
			{
				auto freeGP = GridPoint(centerGP._x, centerGP._y - i);
				if (grid_map->checkPosition(freeGP))
				{
					nu->setPosition(grid_map->getPointWithOffset(freeGP));
					grid_map->occupyPosition(nu->id, freeGP);
					nu->setCurPos(freeGP);
					break;
				}
			}
			nu->setDestination(getGridPoint(Point(x, y)));
			nu->tryToSearchForPath();
		}
		else
		{
			nu->setPosition(x, y);
			nu->addToGmap(Point(x, y));
			nu->setCurPos(getGridPoint(Point(x, y)));
		}
	}
	else
	{
		nu->setPosition(x, y);
		nu->addToGmap(Point(x, y));
		nu->setCurPos(getGridPoint(Point(x, y)));
	}


	//ˢ�µ�����
	power->updatePowerDisplay();
	nu->schedule(schedule_selector(Unit::update));
	return(nu);
}

//�����µ�λ���Գ���
void UnitManager::genCreateMessage(int _unit_type, int camp, float x, float y)
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
	float duration = sqrtf((end_pos.x - start_pos.x)*(end_pos.x - start_pos.x) +
		(end_pos.y - start_pos.y)*(end_pos.y - start_pos.y)) / _speed;
	return duration;
}

void UnitManager::playMover(Point position, Unit * _sprite) {
	//��þ����ƶ���ʱ��
	float duration = getPlayerMoveTime(_sprite->getPosition(), position, _sprite->getSpeed());
	log("%f,%f", _sprite->getPositionX(), _sprite->getPositionY());
	auto moveTo = MoveTo::create(duration, position);
	auto sequence = Sequence::create(moveTo, nullptr);
	_sprite->runAction(sequence);
}
int UnitManager::genRandom(int start, int end)
{
	std::uniform_int_distribution<> rd(start, end);
	return(rd(gen));
}
;

void UnitManager::selectEmpty(Point position)
{
	float order = 1.f;
	for (auto& id : selected_ids){
		Unit * unit = id_map.at(id);

		if (!unit || !unit->isMobile())
			continue;

		GridPoint grid_dest = grid_map->getGridPoint(position);
		//log("Unit ID: %d, plan to move to:(%d, %d)", id, grid_dest.x, grid_dest.y);
		unit->setDestination(grid_dest);
		unit->setDelayPathFinding(PATH_FINDING_TERMINAL * static_cast<int>(order/5));
		order += 1.f;
		//unit->tryToSearchForPath();
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

void UnitManager::checkWinOrLose(int base_id)
{
	Unit* base = id_map.at(base_id);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (base->getCamp() == player_id) {
		auto lose_label = Label::createWithTTF("You Lose!", "/fonts/arial.ttf", 88);
		lose_label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2));
		combat_scene->addChild(lose_label, 40);
		Director::getInstance()->getEventDispatcher()
			->removeAllEventListeners();

	}
	if (base->getCamp() != player_id && player_num == 1) {
		auto win_label = Label::createWithTTF("You Win!", "/fonts/arial.ttf", 88);
		win_label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2));
		combat_scene->addChild(win_label, 40);
		Director::getInstance()->getEventDispatcher()
			->removeAllEventListeners();
	}

}

void UnitManager::setPlayerNum(chat_client * _socket_client)
{
	player_num = _socket_client->total();
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
	if (((abs(getPosition().x - to_.x) < speed_) && (abs(getPosition().y - to_.y) < speed_)))
		removeFromParent();
	else
		setPosition(getPosition() + move_);
}

void ConstructRange::drawRange(Point p, float r)
{

	drawDot(p, r, color);
}
