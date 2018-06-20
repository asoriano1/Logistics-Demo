#ifndef POSE
#define POSE

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

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
	Pose moveUP() { return Pose(this->x, this->y-1); };
	Pose moveDOWN() { return Pose(this->x, this->y+1); };
	Pose moveLEFT() { return Pose(this->x-1, this->y); };
	Pose moveRIGHT() { return Pose(this->x+1, this->y); };
	Pose move(int dir) {
		switch(dir){
		
			case UP:
				return this->moveUP();
			case DOWN:
				return this->moveDOWN();
			case LEFT:
				return this->moveLEFT();
			case RIGHT:
				return this->moveRIGHT();
		
		}
	
	}
	bool equals(Pose p){
		return (p.x == this->x && p.y == this->y);
	}
};
#endif
