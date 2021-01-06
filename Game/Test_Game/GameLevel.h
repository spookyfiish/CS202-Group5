#pragma once
#include "Utils.h"
#include "GameLane.h"
#include "GraphicsController.h"
#include <vector>

class ILevel
{
public:
	virtual void getMap(vector<GameLane*>& lanes, GraphicsController* graphic) = 0;

	virtual int addLanes(vector<GameLane*>& lanes, GraphicsController* graphic, int laneIndex) = 0;
};

class Level_1 : public ILevel
{
public:
	void getMap(vector<GameLane*>& lanes, GraphicsController* graphic)
	{
		if (!lanes.empty()) lanes.clear();
		GameLane* lane1 = new GameLane(1, random(1, 3), 1 - 2 * random(0, 1), random(1, 3), random(1, 2), graphic);
		GameLane* lane2 = new GameLane(2, random(1, 3), 1 - 2 * random(0, 1), random(1, 3), random(1, 2), graphic);
		GameLane* lane3 = new GameLane(3, random(1, 3), 1 - 2 * random(0, 1), random(1, 3), random(1, 2), graphic);
		GameLane* lane4 = new GameLane(4, random(1, 3), 1 - 2 * random(0, 1), random(1, 3), random(1, 2), graphic);
		GameLane* lane5 = new GameLane(5, random(1, 3), 1 - 2 * random(0, 1), random(1, 3), random(1, 2), graphic);
		GameLane* lane6 = new GameLane(6, random(1, 3), 1 - 2 * random(0, 1), random(1, 3), random(1, 2), graphic);

		//nho de lane theo thu tu khong la collision no deo chay dau

		lanes.push_back(lane1);
		lanes.push_back(lane2);
		lanes.push_back(lane3);
		lanes.push_back(lane4);
		lanes.push_back(lane5);
		lanes.push_back(lane6);
		lanes.push_back(new GameLane(7, 0, 1, 0, 0, graphic));
	}

	int addLanes(vector<GameLane*>& lanes, GraphicsController* graphic, int laneIndex)
	{
		int n = random(5, 7);
		for (int i = laneIndex; i < laneIndex + n; i++)
			lanes.push_back(new GameLane(i, random(1, 3), 1 - 2 * random(0, 1), random(1, 3), random(1, 2), graphic));
		lanes.push_back(new GameLane(laneIndex + n, 0, 1, 0, 0, graphic));
		return n + 1;
	}

};