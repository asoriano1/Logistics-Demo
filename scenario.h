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

	vector<vector<string>> arena;

	vector<Robot> robots;

	vector<Cart> carts;

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

	void addRobot(Robot r);

	void addCart(Cart c);

	void print();

	bool checkup(Robot r);

	bool checkdown(Robot r);

	bool checkleft(Robot r);

	bool checkright(Robot r);
};

#endif