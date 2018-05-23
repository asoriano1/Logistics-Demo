#ifndef SCENARIO
#define SCENARIO

#include <vector>
#include<iostream>
#include <list>
#include <time.h>
#include "robot.h"
#include "cart.h"

using namespace std;

class Scenario {

public:

	int height;

	int width;

	vector<vector <string> > arena;

	vector<Robot> robots;

	vector<Cart> carts;

	float value;

	int id;

	int id_prev;
	
	int prof;

	Scenario() {}
	Scenario(int h, int w) {
		this->height = h;
		this->width = w;

		arena.resize(height);
		for (size_t x = 0; x < height; x++)
			arena[x].resize(width);


		for (size_t x = 0; x < height; x++)
			for (size_t y = 0; y < width; y++)
				arena[x][y] = "  ";
	};
/*	Scenario(int id, int prev, int h, int w, Robot r1, Robot r2, Cart c1, Cart c2, Pose destination, Cart cartselected) {
		this->id = id;
		this->id_prev = prev;
		this->height = h;
		this->width = w;

		arena.resize(width);
		for (size_t x = 0; x < width; x++)
			arena[x].resize(height);


		for (size_t x = 0; x < width; x++)
			for (size_t y = 0; y < height; y++)
				arena[x][y] = "  ";

		addCart(c1);
		addCart(c2);

		addRobot(r1);
		addRobot(r2);

		evaluate(cartselected, destination);

	}*/
	
	Scenario(int id, int prev, int h, int w, vector<Robot> robots, vector<Cart> carts, int prof) {
		this->id = id;
		this->id_prev = prev;
		this->height = h;
		this->width = w;
		this->prof = prof;

		arena.resize(width);
		for (size_t x = 0; x < width; x++)
			arena[x].resize(height);


		for (size_t x = 0; x < width; x++)
			for (size_t y = 0; y < height; y++)
				arena[x][y] = "  ";

		for (int i = 0; i < robots.size(); ++i) {
			addRobot(robots.at(i));
		}
		for (int i = 0; i < carts.size(); ++i) {
			addCart(carts.at(i));
		}
		
		//evaluate(cartselected, destination);

	}

	void addRobot(Robot r);

	void addCart(Cart c);

	void print();

	bool checkup(Robot r);

	bool checkdown(Robot r);

	bool checkleft(Robot r);

	bool checkright(Robot r);

	void evaluate(Cart c, Pose dest);

	void UpdateRobotPose(Robot r, Pose p);

	void UpdateCartPose(Cart r, Pose p);
};

#endif
