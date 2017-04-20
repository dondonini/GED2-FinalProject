#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>

using namespace std;
struct Point 
{
	int	x, y;
	float g_movementCost = 1;
	float Heuristic;
	float f;
	Point * parent;
	bool operator==(const Point& other) {
		if (x == other.x && y == other.y)
			return true;
		
		return false;
	}
};

char board_array[21][21] = {
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X' },
	{ 'X', ' ', 'X', 'X', 'X', ' ', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', ' ', 'X', 'X', 'X', ' ', 'X' },
	{ 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X' },
	{ 'X', ' ', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', ' ', 'X' },
	{ 'X', ' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' ', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', ' ', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', ' ', 'X', 'X', 'X', 'X', 'X' },
	{ ' ', ' ', ' ', ' ', 'X', ' ', 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X', ' ', 'X', ' ', ' ', ' ', ' ' },
	{ 'X', 'X', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', ' ', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ 'X', 'X', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X' },
	{ ' ', ' ', ' ', ' ', 'X', ' ', 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X', ' ', 'X', ' ', ' ', ' ', ' ' },
	{ 'X', 'X', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X' },
	{ 'X', ' ', 'X', 'X', 'X', ' ', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', ' ', 'X', 'X', 'X', ' ', 'X' },
	{ 'X', ' ', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X' },
	{ 'X', 'X', 'X', ' ', 'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X', ' ', 'X', ' ', 'X', ' ', 'X', 'X', 'X' },
	{ 'X', ' ', ' ', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X', ' ', ' ', ' ', 'X', ' ', ' ', ' ', ' ', ' ', 'X' },
	{ 'X', ' ', 'X', 'X', 'X', 'X', 'X', 'X', 'X', ' ', 'X', ' ', 'X', 'X', 'X', 'X', 'X', 'X', 'X', ' ', 'X' },
	{ 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
};

class gridGraph 
{
	
public:
	int witdh, height;
	vector<Point *> getNeihbors(Point * location) 
	{
		vector<Point*> neihbors;
		if (board_array[location->y + 1][location->x] == ' ')
		{
			Point * neihborsPoint = new Point;
			neihborsPoint->x = location->x;
			neihborsPoint->y = location->y + 1;

			neihbors.push_back(neihborsPoint);
		}
		if (board_array[location->y - 1][location->x] == ' ')
		{
			Point * neihborsPoint = new Point;
			neihborsPoint->x = location->x;
			neihborsPoint->y = location->y - 1;

			neihbors.push_back(neihborsPoint);
		}
		if (board_array[location->y][location->x + 1] == ' ')
		{
			Point * neihborsPoint = new Point;
			neihborsPoint->x = location->x + 1;
			neihborsPoint->y = location->y ;
		

			neihbors.push_back(neihborsPoint);
		}
		if (board_array[location->y][location->x - 1] == ' ')
		{
			Point * neihborsPoint = new Point;
			neihborsPoint->x = location->x - 1;
			neihborsPoint->y = location->y;
		
	
			neihbors.push_back(neihborsPoint);
		}
		return neihbors;
	}

	float calcHeuristic(Point current, Point Goal) {


	return	abs(current.x - Goal.x) + abs(current.y - Goal.y);
	}
};

class astarSearch 
{
public:
	gridGraph grahp;

	Point * astar(Point * start, Point * Goal) {
		//cout << "astar start" << endl;;
		vector<Point *> OpenList;
		vector<Point *> ClosedList;
		start->g_movementCost = 0;
		start->parent = NULL;
		OpenList.push_back(start);
	//	cout << "start is " << start->x << " " << start->y << endl;
		//cout << "goal is " << Goal->x << " " << Goal->y << endl;
		while (!OpenList.empty())
		{
			Point  * current = OpenList.front();
		
			OpenList.erase(OpenList.begin());
	
			ClosedList.push_back(current);
			if (*current == *Goal) {
				Goal->parent = current->parent;
			//	cout << "yay" << endl;
			
				return current;
			}
		//	cout << "looking at " << current->x << " " << current->y << endl;
		//only returns empty tiles
			vector<Point*> neihbors = grahp.getNeihbors(current);
		//	cout << neihbors.size()<<endl;
			for (int i = 0; i < neihbors.size(); i++)
			{

				Point  * next = neihbors[i];
				bool isInClosedList = false;
				for (int k = 0; k < ClosedList.size(); k++) 
				{
					if (*next == *ClosedList[k]) 
					{
					//	cout << "is in closed List" << endl;
						isInClosedList = true;
						break;
					}

				}
				if (isInClosedList == false) 
				{
					//cout << "is NOT in closed List" << endl;
					
					int isInOpenList = -1;
					for (int k = 0; k < OpenList.size(); k++)
					{
						if (*next == *OpenList[k])
						{
							isInOpenList = k;
							break;
						}

					}
					if (isInOpenList == -1) 
					{
						next->g_movementCost = current->g_movementCost + 1;
						next->Heuristic = grahp.calcHeuristic(*next, *Goal);
						next->f = next->g_movementCost + next->Heuristic;
						next->parent = current;
						OpenList.push_back(next);
					//	cout << "added to Open List" << endl;
					}
					else
					{
						if (next->f < OpenList[isInOpenList]->f) {
						
							OpenList[isInOpenList]->f = next->f;
							OpenList[isInOpenList]->parent = current;
						}
					}

				}
			}
		}
		//cout << "open List empty" << endl;
		return Goal;
	}
};