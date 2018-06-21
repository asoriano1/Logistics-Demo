#include "robot.h"
#include "scenario.h"
#include <string>
#include <iostream>
#include <math.h>

using namespace std;


void Scenario::addRobot(Robot r) {
	//cout << "addRobot" << endl;
	//si est? la celda vac?a
	if (arena[r.GetPose().x][r.GetPose().y].compare("  ") == 0) {
		//cout << "vacio" << endl;
		arena[r.GetPose().x][r.GetPose().y] = r.GetId();
		this->robots.push_back(r);
		//si hay un carro
	}
	else if ((arena[r.GetPose().x][r.GetPose().y]).at(0) == 'c') {
		//cout << "carro" << endl;
		arena[r.GetPose().x][r.GetPose().y] = arena[r.GetPose().x][r.GetPose().y] + r.GetId();
		this->robots.push_back(r);
	}
	else //hay otro robot
		cout << "addRobot Error: Some robot already in this position: " << arena[r.GetPose().x][r.GetPose().y] << endl;

}

void Scenario::addCart(Cart c) {

	if (!arena[c.GetPose().x][c.GetPose().y].compare("  ")) {
		arena[c.GetPose().x][c.GetPose().y] = c.GetId();
		this->carts.push_back(c);
	}
	else if ((arena[c.GetPose().x][c.GetPose().y]).at(0) == 'r'){
		//cout << there is a robot here << endl;
		arena[c.GetPose().x][c.GetPose().y] = c.GetId() + arena[c.GetPose().x][c.GetPose().y];
		this->carts.push_back(c);
	}
	else
		cout << "addCart Error: There is something already in this position" << endl;
}

void Scenario::print() {
	cout << endl;
	cout << "-id:" << this->id << "-v:" << this->value << "-prof" << this->prof << endl;
	cout << "----------" << endl;
	for (size_t y = 0; y < height; ++y) {
		cout << "|";
		for (size_t x = 0; x < width; ++x) {
			cout << arena[x][y] << "|";
		}
		cout << endl;
	}
	cout << "----------" << endl;

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
	
	//g(n)+h(n)
	//manhattan_distance + closest robot to cart selected + num_movements(explored_nodes)
	
	Pose cart_pose;

	//search where is now the cart c
	for (int i = 0; i < this->carts.size(); ++i) {
		if (c.GetId() == carts.at(i).GetId()) {
			cart_pose = carts.at(i).GetPose();
			break;
		}
	}

	//Manhattan distance from cart to destination
	this->value = fabs(dest.x - cart_pose.x) + fabs(dest.y - cart_pose.y);
	//cout << "menor cart to destx:" << dest.x << " c.GetPose().x" << cart_pose.x << "dest.y" << dest.y << "c.GetPose().y" << cart_pose.y << "total" << this->value << endl;
	//find the robot closer to cart
	float dist = 999;
	float total_movs = 0;
	for (int i = 0; i < this->robots.size(); ++i) {
		//cout << "dist[" << i << "]: " << sqrt(pow(c.GetPose().x - this->robots[i].GetPose().x, 2) + pow(c.GetPose().y - this->robots[i].GetPose().y, 2));
		if (dist > fabs(cart_pose.x - this->robots[i].GetPose().x) + fabs(cart_pose.y - this->robots[i].GetPose().y)) {
			dist = fabs(cart_pose.x - this->robots[i].GetPose().x) + fabs(cart_pose.y - this->robots[i].GetPose().y);			
			//cout << "menor dist:" << dist << endl;
		}
		//count the total number of movements
		total_movs = total_movs + this->robots[i].n_mov;
	}
	//305 nodos, 59 visitados sin movimientos 
	//2655, 809 visitados con movimientos.
	//1177, 308 visitados con 0.25*movimientos
	//994, 197 visitados con 0.1*movimientos
	this->value = this->value + dist + this->prof + 0.1*total_movs;
	
}

/**
 * Set the robot in one pose
 * Sustituido para penalizar si se coge/deja carro
 * 
 * */
void Scenario::UpdateRobotPose(Robot r, Pose p) {
	
	for (int i = 0; i < this->robots.size(); ++i) {
		if (r.GetId() == this->robots[i].GetId()) {
			this->robots[i].SetPose(p);
			this->robots[i].n_mov++;
			//
			//cout << "el robot "<< this->robots[i].GetId() << " se ha movido "<< this->robots[i].n_mov << " veces" << endl;
			//TODO:el hueco que deja solo ser? vac?o si no deja carro
			//cout << " se pone el robot en y:" << p.y << endl;
			//posición donde estaba ese robot
			//si había un carro, dejo el carro solo
			if (arena[r.GetPose().x][r.GetPose().y].at(0) == 'c'){
				arena[r.GetPose().x][r.GetPose().y] = arena[r.GetPose().x][r.GetPose().y].substr(0, 2);
			}else{ //si no, no pongo nada
				arena[r.GetPose().x][r.GetPose().y] = "  ";
			}//si en la nueva posición hay un carro, lo pongo
			if (arena[p.x][p.y].at(0) == 'c'){
				arena[p.x][p.y] = arena[p.x][p.y] + r.GetId();
			}else{ //si no pongo sólo el robot
				arena[p.x][p.y] = r.GetId();
			}
		}
	}
}

void Scenario::UpdateCartPose(Cart c, Pose p) {
	for (int i = 0; i < this->carts.size(); ++i) {
		if (c.GetId() == this->carts[i].GetId()) {
			this->carts[i].SetPose(p);			
			//TODO:el hueco que deja solo ser? vac?o si no deja carro

			//si se va a mover un carro es porque hay un robot que lo mueve
			//pero el robot puede haber actualizado su posici?n ya y el carro hacerlo despu?s
			//si hay un robot en la nueva posición
			if (arena[p.x][p.y].at(0) == 'r'){
				arena[p.x][p.y] = c.GetId() + arena[p.x][p.y];
			}else{ //si no pongo sólo el carro
				arena[p.x][p.y] = arena[c.GetPose().x][c.GetPose().y];
			}//en cualquier otro caso la posición anterior se quedará vacía, ya que un carro no se puede mover sólo
			arena[c.GetPose().x][c.GetPose().y] = "  ";
		}
	}
}

void Scenario::MoveRobotTo(Robot r, int direction) {
	
	Pose p = r.GetPose().move(direction);
	
	for (int i = 0; i < this->robots.size(); ++i) {
		if (r.GetId() == this->robots[i].GetId()) {
			this->robots[i].SetPose(p);
			this->robots[i].n_mov++;		
			//para minimizar el coger/dejar carros se cuenta coger y dejar como un movimiento más
			//debería tener en cuenta que si varios robots lo hacen en el mismo paso no tiene sentido duplicarlo	
			if(this->robots[i].hasACart()){
				this->robots[i].placeCart();
				this->robots[i].n_mov++;			
			}
			
			//Si el robot no está orientado hacia la posición que desea moverse, penalizo la corrección de orientación
			//DEBERIA CALCULAR AQUÍ SI LA POSICIÓN NUEVA COINCIDE CON LA ORIENTACIÓN
			if(direction!=r.GetPose().theta)
				this->robots[i].n_mov=this->robots[i].n_mov+5;			
			//¿PENALIZACIÓN DOBLE SI EL GIRO ES DE 180º?
			
			
			
			//cout << "el robot "<< this->robots[i].GetId() << " se ha movido "<< this->robots[i].n_mov << " veces" << endl;
			//TODO:el hueco que deja solo ser? vac?o si no deja carro
			//cout << " se pone el robot en y:" << p.y << endl;
			//posición donde estaba ese robot
			//si había un carro, dejo el carro solo
			if (arena[r.GetPose().x][r.GetPose().y].at(0) == 'c'){
				arena[r.GetPose().x][r.GetPose().y] = arena[r.GetPose().x][r.GetPose().y].substr(0, 2);
			}else{ //si no, no pongo nada
				arena[r.GetPose().x][r.GetPose().y] = "  ";
			}//si en la nueva posición hay un carro, lo pongo
			if (arena[p.x][p.y].at(0) == 'c'){
				arena[p.x][p.y] = arena[p.x][p.y] + r.GetId();
			}else{ //si no pongo sólo el robot
				arena[p.x][p.y] = r.GetId();
			}
		}
	}
}

void Scenario::MoveRobotWithCartTo(Robot r, int direction) {

	Pose p = r.GetPose().move(direction);
	
	for (int i = 0; i < this->robots.size(); ++i) {
		if (r.GetId() == this->robots[i].GetId()) {
			this->robots[i].SetPose(p);
			this->robots[i].n_mov++;
			this->robots[i].n_mov++; //penalización doble por moverse con carro
			//para minimizar el coger/dejar carros se cuenta coger y dejar como un movimiento más
			if(!this->robots[i].hasACart()){
				this->robots[i].pickCart();
				this->robots[i].n_mov++;			
			}
			
			//Si el robot no está orientado hacia la posición que desea moverse, penalizo la corrección de orientación
			if(direction!=r.GetPose().theta)
				this->robots[i].n_mov=this->robots[i].n_mov+5;			
				
			//
			//cout << "el robot "<< this->robots[i].GetId() << " se ha movido "<< this->robots[i].n_mov << " veces" << endl;
			//TODO:el hueco que deja solo ser? vac?o si no deja carro
			//cout << " se pone el robot en y:" << p.y << endl;
			//posición donde estaba ese robot
			//si había un carro, dejo el carro solo
			if (arena[r.GetPose().x][r.GetPose().y].at(0) == 'c'){
				arena[r.GetPose().x][r.GetPose().y] = arena[r.GetPose().x][r.GetPose().y].substr(0, 2);
			}else{ //si no, no pongo nada
				arena[r.GetPose().x][r.GetPose().y] = "  ";
			}//si en la nueva posición hay un carro, lo pongo
			if (arena[p.x][p.y].at(0) == 'c'){
				arena[p.x][p.y] = arena[p.x][p.y] + r.GetId();
			}else{ //si no pongo sólo el robot
				arena[p.x][p.y] = r.GetId();
			}
		}
	}
}

bool Scenario::IsSolved(Cart c, Pose p){

	for (int i = 0; i < this->carts.size(); ++i) {
		if (c.GetId() == this->carts[i].GetId() && (this->carts[i].GetPose().x == p.x && this->carts[i].GetPose().y == p.y)) {
			return true;
		}
	}
	return false;
}
