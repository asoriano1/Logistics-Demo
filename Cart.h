#ifndef CART
#define CART

#include "pose.h"
#include <string>

using namespace std;

class Cart {
private:
	string id;
	Pose pose;
public:
	Cart() {};
	Cart(string ide, int x, int y) {
		this->id = ide;
		this->pose = Pose(x, y);
	};
	void SetId(string ide) { this->id = ide; };
	void SetPose(Pose p) { this->pose = p; };
	string GetId() { return this->id; }
	Pose GetPose() { return this->pose; };
};

#endif