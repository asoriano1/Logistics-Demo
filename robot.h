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
	/*Robot(string ide, int x, int y){
		this->id = ide;
		this->pose = Pose(x, y);
		this->hasCart = false;
		n_mov = 0;
	};*/
	Robot(string ide, int x, int y, int dir){
		this->id = ide;
		this->pose = Pose(x, y, dir);
		this->hasCart = false;
		n_mov = 0;
	};
	void SetId(string ide) { this->id = ide; };
	void SetPose(Pose p) { this->pose = p; };
	string GetId() { return this->id; };
	Pose GetPose() { return this->pose; };
	void pickCart() { this->hasCart = true; };
	void placeCart() { this->hasCart = false; };
	bool hasACart() { return this->hasCart; };
	int n_mov;
}; 

#endif
