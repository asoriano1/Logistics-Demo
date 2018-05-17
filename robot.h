#ifndef ROBOT
#define ROBOT

#include "pose.h"
#include <string>

using namespace std;

class Robot {
private:
	string id;
	Pose pose;
	bool hasCart;
public:
	Robot() {};
	Robot(string ide, int x, int y){
		this->id = ide;
		this->pose = Pose(x, y);
		this->hasCart = false;
	};
	void SetId(string ide) { this->id = ide; };
	void SetPose(Pose p) { this->pose = p; };
	string GetId() { return this->id; };
	Pose GetPose() { return this->pose; };
	void pickCart() { this->hasCart = true; };
	void placeCart() { this->hasCart = false; };
	bool hasACart() { return this->hasCart; };
}; 

#endif