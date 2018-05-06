#include "robot.h"
#include "scenario.h"
#include <string>
#include <iostream>

using namespace std;


void Scenario::addRobot(Robot r) {

	if (!arena[r.GetPose().x][r.GetPose().y].compare("  "))
		arena[r.GetPose().x][r.GetPose().y] = r.GetId();
	else
		cout << "addRobot Error: Some robot already in this position" << endl;
}

void Scenario::addCart(Cart c) {

	if (!arena[c.GetPose().x][c.GetPose().y].compare("  "))
		arena[c.GetPose().x][c.GetPose().y] = c.GetId();
	else
		cout << "addRobot Error: There is something already in this position" << endl;
}

void Scenario::print() {
	cout << endl;
	cout <<"----------"<< endl;
	for (size_t x = 0; x < height; x++) {
		cout << "|";
		for (size_t y = 0; y < width; y++) {
			cout << arena[x][y] << "|";
		}
		cout << endl;
	}
	cout <<"----------"<< endl;
}

bool Scenario::checkup(Robot r) {
	//if its inside area (top) and there is no robot
	if (r.GetPose().x - 1 >= 0 && arena[r.GetPose().x - 1][r.GetPose().y].at(0) != 'r')
		return true;
	return false;
}

bool Scenario::checkdown(Robot r) {
	//if its inside area (top) and there is no robot
	if (r.GetPose().x + 1 < this->height && arena[r.GetPose().x + 1][r.GetPose().y].at(0) != 'r')
		return true;
	return false;
}

bool Scenario::checkleft(Robot r) {
	//if its inside area (top) and there is no robot
	if (r.GetPose().y - 1 >= 0 && arena[r.GetPose().x - 1][r.GetPose().y].at(0) != 'r')
		return true;
	return false;
}

bool Scenario::checkright(Robot r) {
	//if its inside area (top) and there is no robot
	if (r.GetPose().y + 1 < this->width && arena[r.GetPose().x - 1][r.GetPose().y].at(0) != 'r')
		return true;
	return false;
}