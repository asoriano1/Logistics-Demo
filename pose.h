#ifndef POSE
#define POSE

using namespace std;

class Pose {
	
public:
	int x;
	int y;
	Pose() {};
	Pose(int xx, int yy) {
		this->x = xx;
		this->y = yy;
	};
};
#endif