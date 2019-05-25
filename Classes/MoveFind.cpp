#include "cocos2d.h"
#include "MoveFind.h"

#include "GameScene.h"

#define DEBUG 0
#define GOAL 1000
USING_NS_CC;

#define across 62
#define movemarknums 177



bool ObjHit(Vec2 p0, TMXTiledMap* map) {
	Size mapSize = map->getMapSize();//地图尺寸
	Size tileSize = map->getTileSize();//瓦片尺寸
	int x = p0.x / map->getTileSize().width;
	int y = map->getMapSize().height - p0.y / map->getTileSize().height;
	//int y = mapSize.height - ((2 * (int)p0.y) / (int)tileSize.height);
	//int x = p0.x / tileSize.width - (y % 2) / 2;
	Vec2 toTileMapPosition = Vec2(x, y);//转化为瓦片坐标
	auto tree = map->getLayer("tree");
	int tileGid = tree->getTileGIDAt(toTileMapPosition);
	//log("%f %f", toTileMapPosition.x, toTileMapPosition.y);
	if (2 == tileGid) {
		return 1;		//会碰撞则返回1
	}
	return 0;
}
bool canto(Vec2 p1,Vec2 p2, TMXTiledMap* map){
	// 给两个坐标然后返回可以直接到达与否 
	if(p1.getDistance(p2)<=across){
		return 0;
	}
	int move_times=p1.getDistance(p2)/across;
	float xmove,ymove;
	xmove = p2.x - p1.x;
	xmove/=move_times;
	ymove = p2.y - p1.y;
	ymove/=move_times;

	for(int i=0;i<move_times;i++){
		p1.x+=xmove;
		p1.y+=ymove;
		
		if(ObjHit(p1,map))return 0;
	}
	return 1;
	
}
// 在开始游戏的时候调用这个函数 



bool GameScene::PointInit() {

	// 用来初始化地图的寻路功能，就是填写can_be的参数用的
	static bool one_times_is_enough = 0;
	if (one_times_is_enough)return 0;
	one_times_is_enough = 1;

//	log("use mapint");
	float movemarker[movemarknums][2];
	auto objectGroup = map->getObjectGroup("point");
	for (int i = 1; i <= movemarknums; i++) {
		auto object = objectGroup->getObject(std::to_string(i));
		//别问我下面两行是啥意思，我也不知道，and需要注意的是，虽然x,y是float，但是却舍去了小数位（蜜汁操作）
		float x = object["x"].asFloat();
		float y = object["y"].asFloat();
		movemarker[i - 1][0] = x;
		movemarker[i - 1][1] = y;
	}
	// 这样一个一个输入看起来有点傻，我看看能不能用程序自动找出来 
	for (int i = 0; i < movemarknums; i++) {
		MapPoint a;
		a.position.x = movemarker[i][0];
		a.position.y = movemarker[i][1];

		movemark.push_back(a);
	}
	
	for (int i = 0; i<movemark.size(); i++) {
		for (int j = i + 1; j < movemark.size(); j++) {
			if (canto(movemark[i].position, movemark[j].position,map)) {
				movemark[i].can_be.push_back(j);
				movemark[j].can_be.push_back(i);

//				log("map %d can to %d", i+1, j+1);
			}
		}
	}
	return 1;
}


bool operator < (const route c1, const route c2){
	return c1.has_move > c2.has_move;// 修改的地方 
}// 用于优先队列的使用 



std::vector<Vec2> GameScene::MoveFind(const Vec2 startpoint, const Vec2& pos_target) {
	Vec2 goalpoint;
	goalpoint = pos_target;// 修改的地方 
	clock_t start_time = clock();
	std::priority_queue<route> finds;
	route start;
	start.position.push_back(startpoint);
	start.has_move = 0;
	if (ObjHit(pos_target,map)) {
		
		return start.position;
	}
	if (canto(start.position[0], pos_target, map)) {
		start.position.push_back(pos_target);
		return start.position;// 能到就直接到，没那么费事														
	}
	// 先找起点终点可以去的节点 
	for (int i = 0; i < movemark.size(); i++) {//这里用到了一个movemark,movemark是一个vector<MapPoint> 
		if (canto(start.position[0], movemark[i].position, map)) {//如果说该节点起点可以去的话 
			route in;
			in.position.push_back(movemark[i].position);
			in.has_move = start.position[0].getDistance(movemark[i].position);//这里的函数名不确定是不是getDistance 

			in.movemarknum = i;
			finds.push(in);// 将可能的考虑进去 
			//log("start can to %d, dis is %f", i+1,in.has_move);
		}

		if (canto(pos_target, movemark[i].position, map)) {// 如果说该节点终点可以去的话 
			movemark[i].has_been = 2;//所有能连接终点的节点其实都是“终止点”
	   //	log("endmark is %d", i);
		   //log("end can to %d", i+1);

		}
	}
	clock_t end2_tiem = clock();
	float spendtime2 = end2_tiem - start_time;
	log("spend %f", spendtime2);
	route ans;// 同来储存结果 

	while (!finds.empty()) {
		route now = finds.top();
		finds.pop();
		Vec2 nowpoint = now.position[now.position.size() - 1];
		//log("now point is %d has move %f", now.movemarknum+1,now.has_move);
		if (now.movemarknum == GOAL) {
			ans = now;

			break;
		}
		// 到这里说明这条路线目前的位置还没到终点
		// 改成has_move记录总距离 
		if (movemark[now.movemarknum].has_been == 1)continue;

		if (movemark[now.movemarknum].has_been == 2) {//能差一步到就连一条线过去 

			//log("has find a ans in %d length %f", now.movemarknum + 1, now.has_move);
			//now.has_move+=pos_target.getDistance(now.position[now.position.size()-1]);
			now.movemarknum = GOAL;
			now.position.push_back(pos_target);
			finds.push(now);
			continue;
		}

		movemark[now.movemarknum].has_been = 1;
		// 然后把这个节点能到的节点都遍历一遍 
		for (int i = 0; i < movemark[now.movemarknum].can_be.size(); i++) {
			int u = movemark[now.movemarknum].can_be[i];
			if (movemark[u].has_been == 1)continue;//查重用 
	//		log("check %d has %d has move %f", now.movemarknum,  movemark[now.movemarknum].has_been,now.has_move);
			now.position.push_back(movemark[u].position);
			float al2 = now.has_move;
			float changedis = movemark[now.movemarknum].position.getDistance(movemark[u].position);
			changedis += movemark[u].position.getDistance(goalpoint);
			changedis -= movemark[now.movemarknum].position.getDistance(goalpoint);
			now.has_move += changedis;

			//log("%d can to %d ids %f", now.movemarknum + 1, u + 1, now.has_move);
			int al = now.movemarknum;
			now.movemarknum = u;
			finds.push(now);//将假设走这一步路的想法考虑进去 
			// 接下来还原now 
			now.position.pop_back();
			now.has_move = al2;
			//log("%d can to %d ids %f", now.movemarknum + 1, u + 1, now.has_move);
			now.movemarknum = al;
		}

	}
	// 复原节点数据
	for (int i = 0; i < movemark.size(); i++) {
		movemark[i].has_been = 0;// 生不带来，死不带走 
	}

	clock_t end_tiem = clock();
	float spendtime = end_tiem - start_time;
	log("spend %f", spendtime);
	return ans.position;//接口是一个vector的点集 
}
