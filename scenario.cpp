#include "robot.h"
#include "scenario.h"
#include <string>
#include <iostream>
#include <math.h>

using namespace std;


void Scenario::addRobot(Robot r) {
	//cout << "addRobot" << endl;
	//si está la celda vacía
	if (arena[r.GetPose().x][r.GetPose().y].compare("  ") == 0 ) {
		//cout << "vacio" << endl;
		arena[r.GetPose().x][r.GetPose().y] = r.GetId();
		this->robots.push_back(r);
		//si hay un carro
	}else if((arena[r.GetPose().x][r.GetPose().y]).at(0) == 'c'){
		//cout << "carro" << endl;
		arena[r.GetPose().x][r.GetPose().y] = arena[r.GetPose().x][r.GetPose().y]+r.GetId();
		this->robots.push_back(r);
	}else //hay otro robot
		cout << "addRobot Error: Some robot already in this position: " << arena[r.GetPose().x][r.GetPose().y] << endl;
		
}

void Scenario::addCart(Cart c) {

	if (!arena[c.GetPose().x][c.GetPose().y].compare("  ")) {
		arena[c.GetPose().x][c.GetPose().y] = c.GetId();
		this->carts.push_back(c);
	}
	else
		cout << "addCart Error: There is something already in this position" << endl;
}

void Scenario::print() {
	cout << endl;
	cout <<"-id:" << this->id <<"-v:" << this->value << "-" << endl;
	cout <<"----------"<< endl;
	for (size_t y = 0; y < height; ++y) {
		cout << "|";
		for (size_t x = 0; x < width; ++x) {
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

void Scenario::evaluate(Cart c, Pose dest) {
	//Manhattan distance from cart to destination
	this->value = fabs(dest.x - c.GetPose().x) + fabs(dest.y - c.GetPose().y);
	cout << "menor cart to destx:" << dest.x << " c.GetPose().x" << c.GetPose().x << "dest.y" << dest.y << "c.GetPose().y" << c.GetPose().y << "total" << this->value << endl;
	//find the robot closer to cart
	float dist = 999;
	
	for (int i = 0; i < this->robots.size(); ++i) {
		//cout << "dist[" << i << "]: " << sqrt(pow(c.GetPose().x - this->robots[i].GetPose().x, 2) + pow(c.GetPose().y - this->robots[i].GetPose().y, 2));
		if (dist > fabs(c.GetPose().x - this->robots[i].GetPose().x) + fabs(c.GetPose().y - this->robots[i].GetPose().y)) {
			dist = fabs(c.GetPose().x - this->robots[i].GetPose().x) + fabs(c.GetPose().y - this->robots[i].GetPose().y);
			cout << "menor dist:" << dist << endl;
		}
	}
	this->value = this->value + dist;
}

void Scenario::UpdateRobotPose(Robot r, Pose p) {
	for (int i = 0; i < this->robots.size(); ++i) {
		if (r.GetId() == this->robots[i].GetId()) {
			this->robots[i].SetPose(p);
			//TODO:el hueco que deja solo será vacío si no deja carro
			cout << " se pone el robot en y:" << p.y << endl;
			if(arena[r.GetPose().x][r.GetPose().y].at(0) == 'c')
				arena[r.GetPose().x][r.GetPose().y]=arena[r.GetPose().x][r.GetPose().y].substr(0,2);
			else
				arena[r.GetPose().x][r.GetPose().y] = "  ";	
			if(arena[p.x][p.y].at(0)=='c')
				arena[p.x][p.y] = arena[p.x][p.y] + r.GetId();
			else
				arena[p.x][p.y] = r.GetId();
				
		}
	}
}

void Scenario::UpdateCartPose(Cart c, Pose p) {
	for (int i = 0; i < this->carts.size(); ++i) {
		if (c.GetId() == this->carts[i].GetId()) {
			this->carts[i].SetPose(p);
			//TODO:el hueco que deja solo será vacío si no deja carro

			//si se va a mover un carro es porque hay un robot que lo mueve
			//pero el robot puede haber actualizado su posición ya y el carro hacerlo después
			if(arena[p.x][p.y].at(0) == 'r')
				arena[p.x][p.y] = arena[p.x][p.y] + c.GetId();
			else
				arena[p.x][p.y] = arena[c.GetPose().x][c.GetPose().y];
			arena[c.GetPose().x][c.GetPose().y] = "  ";
		}
	}
}
