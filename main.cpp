// ConsoleApplication1.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

/*
const int maze_size_x=80;
const int maze_size_y=25;
vector < vector < bool > > maze;

maze.resize(maze_size_y);
for (size_t y=0;y<maze_size_y;y++)
maze[y].resize(maze_size_x);

for (size_t x=0;x<maze_size_x;x++)
for (size_t y=0;y<maze_size_y;y++)
maze[y][x]=false;
*/


// Program to print BFS traversal from a given
// source vertex. BFS(int s) traverses vertices 
// reachable from s.


#include <iostream>
#include <list>
#include <time.h>
#include <vector>

#include "graph.h"
#include "pose.h"
#include "robot.h"
#include "scenario.h"
#include "cart.h"

bool pose_valid(Scenario scn, Pose p);
bool thereisarobot(Scenario scn, Pose p);
bool thereisacart(Scenario scn, Pose p);
Robot whichrobotis(Scenario scn, Pose p);
int whichrobotis_indx(Scenario scn, Pose p);
Cart whichcartis(Scenario scn, Pose p);
int whichcartis_indx(Scenario scn, Pose p);
bool isalreadydefined(Scenario scn);
void addtonodes(Scenario scn);

using namespace std;
vector < Scenario > nodos;
vector < Scenario > nodos_activos;
vector < Scenario > nodos_visitados;

// Driver program to test methods of graph class
int main()
{

	const int maze_size_x = 80;
	const int maze_size_y = 25;
	const int num_robots = 3;
	vector < vector < bool > > maze;

	vector < Pose > new_poses;

	Pose new_pose;

	Robot r1 = Robot("r1", 0, 2);
	Robot r2 = Robot("r2", 1, 2);

	Cart c1 = Cart("c1", 0, 0);
	Cart c2 = Cart("c2", 1, 0);

	int id = 0;

	//mission
	//TODO QUE EL CARRO SEA GLOBAL PERO SE ACTUALICE
	//igual s� que iba (VUELVE A COMO ESTABA A VER)
	//Cart cart_sel = c1;
	Pose pose_dest = Pose(2, 0);

	Scenario scn = Scenario(id, -1, 3, 3, r1, r2, c1, c2, pose_dest, c1);

	Scenario scn_sel;

	scn.addRobot(r1);
	scn.addRobot(r2);

	scn.addCart(c1);
	scn.addCart(c2);

	scn.print();
	scn.evaluate(c1, pose_dest);

	cout << "-value-" << scn.value << endl;

	//a�ade el primer nodo
	nodos_activos.push_back(scn);
	nodos.push_back(scn);
	//si existe alg�n nodo con el valor a 0=>fin

	//si no, selecciona el de menor value y derivalo
	//si al a�adir nodos, se a�aden ordenados siempre hay que coger el primero

	//derivate(nodo_actual)
	//coge el primer robot, puede mover arriba, abajo , izquierda o derecha
	//si puede crea un escenario nuevo con esa pose
	//definir un m�todo para dar una posici�n y decir si hay robot
	vector <string> robots_visitados;

	while (!nodos_activos.empty()) {

		scn_sel = nodos_activos.at(0);
		//si la evaluaci�n de la primera posici�n es 0, he terminado
		cout << ">>>Derivando el nodo con value:" << scn_sel.value << " id:" << scn_sel.id << " " << endl;
		if (scn_sel.value == 0) {
			cout << " FIN." << endl;
			nodos_visitados.push_back(scn_sel);
			break;
		}
		else {
			//para todos los robots
			for (int i = 0; i < scn_sel.robots.size(); i++) {

				new_pose = Pose(scn_sel.robots[i].GetPose().x + 1, scn_sel.robots[i].GetPose().y);

				if (pose_valid(scn_sel, new_pose) &&
					!thereisarobot(scn_sel, new_pose)) {
					cout << "Robot:" << scn_sel.robots[i].GetId() << " puede mover a x + 1 " << endl;
					//es m�s f�cil clonar.
					id++;
					Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots.at(0), scn_sel.robots.at(1), scn_sel.carts.at(0), scn_sel.carts.at(1), pose_dest, scn_sel.carts.at(0));
					//modificar
					newscn.UpdateRobotPose(newscn.robots[i], new_pose);
					//evaluar
					newscn.evaluate(scn_sel.carts.at(0), pose_dest);
					cout << "escenario id:" << newscn.id << endl;
					//a�adir 
					//1� que no est� ya definido ese escenario
					if (!isalreadydefined(newscn)) {
						addtonodes(newscn);
					}
					else {
						id--;
					}

					//si adem�s hay un carro en la misma posici�n: puedo mover el carro conmigo
					// comprobar adem�s que si se va a mover con un carro y hay otro carro delante no puede moverse
					if (thereisacart(scn_sel, scn_sel.robots[i].GetPose()) && !thereisacart(scn_sel, new_pose)) {
						//es m�s f�cil clonar.
						id++;
						Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots.at(0), scn_sel.robots.at(1), scn_sel.carts.at(0), scn_sel.carts.at(1), pose_dest, scn_sel.carts.at(0));
						//modificar robot
						newscn.UpdateRobotPose(newscn.robots[i], new_pose);
						//get the cart
						int cart_indx = whichcartis_indx(newscn, scn_sel.robots[i].GetPose());
						//modify the cart in the new scenario
						//TODO: POR QUE AQU� ES INDICE?????
						newscn.UpdateCartPose(newscn.carts[cart_indx], new_pose);
						//evaluar
						newscn.evaluate(newscn.carts.at(0), pose_dest);
						//a�adir//TODO:si a�ado de manera ordenada, hago que siempre tenga que coger el primero nodos.at(0)
						if (!isalreadydefined(newscn)) {
							cout << "adem�s se lleva el carro" << endl;
							addtonodes(newscn);
						}
					}

				}
				//si uno puede moverse en y + 1
				new_pose = Pose(scn_sel.robots[i].GetPose().x, scn_sel.robots[i].GetPose().y + 1);
				if (pose_valid(scn_sel, new_pose) &&
					!thereisarobot(scn_sel, new_pose)) {
					cout << "Robot:" << scn_sel.robots[i].GetId() << " puede mover a y + 1 " << endl;
					//es m�s f�cil clonar.
					id++;
					Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots.at(0), scn_sel.robots.at(1), scn_sel.carts.at(0), scn_sel.carts.at(1), pose_dest, scn_sel.carts.at(0));
					//modificar
					newscn.UpdateRobotPose(newscn.robots[i], new_pose);
					//evaluar
					newscn.evaluate(scn_sel.carts.at(0), pose_dest);
					cout << "escenario id:" << newscn.id << endl;
					//a�adir 
					//TODO:
					//1� que no est� ya definido ese escenario
					if (!isalreadydefined(newscn)) {
						addtonodes(newscn);
					}
					else {
						id--;
					}

					//si adem�s hay un carro en la misma posici�n: puedo mover el carro conmigo
					// comprobar adem�s que si se va a mover con un carro y hay otro carro delante no puede moverse
					if (thereisacart(scn_sel, scn_sel.robots[i].GetPose()) && !thereisacart(scn_sel, new_pose)) {
						//es m�s f�cil clonar.
						id++;
						Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots.at(0), scn_sel.robots.at(1), scn_sel.carts.at(0), scn_sel.carts.at(1), pose_dest, scn_sel.carts.at(0));
						//modificar robot
						newscn.UpdateRobotPose(newscn.robots[i], new_pose);
						//get the cart
						int cart_indx = whichcartis_indx(newscn, scn_sel.robots[i].GetPose());
						//modify the cart in the new scenario
						//TODO: POR QUE AQU� ES INDICE?????
						newscn.UpdateCartPose(newscn.carts[cart_indx], new_pose);
						//evaluar
						newscn.evaluate(newscn.carts.at(0), pose_dest);
						//a�adir//TODO:si a�ado de manera ordenada, hago que siempre tenga que coger el primero nodos.at(0)
						if (!isalreadydefined(newscn)) {
							cout << "adem�s se lleva el carro" << endl;
							addtonodes(newscn);
						}
					}

				}
				//si uno puede moverse en x - 1
				new_pose = Pose(scn_sel.robots[i].GetPose().x - 1, scn_sel.robots[i].GetPose().y);
				if (pose_valid(scn_sel, new_pose) &&
					!thereisarobot(scn_sel, new_pose)) {
					cout << "Robot:" << scn_sel.robots[i].GetId() << " puede mover a x - 1 [ " << scn_sel.robots[i].GetPose().x - 1 << " ]" << endl;
					//es m�s f�cil clonar.
					id++;
					Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots.at(0), scn_sel.robots.at(1), scn_sel.carts.at(0), scn_sel.carts.at(1), pose_dest, scn_sel.carts.at(0));
					//modificar
					newscn.UpdateRobotPose(newscn.robots[i], new_pose);
					//evaluar
					newscn.evaluate(scn_sel.carts.at(0), pose_dest);
					cout << "escenario id:" << newscn.id << endl;
					//a�adir 
					//TODO:
					//1� que no est� ya definido ese escenario
					if (!isalreadydefined(newscn)) {
						addtonodes(newscn);
					}
					else {
						id--;
					}

					//si adem�s hay un carro en la misma posici�n: puedo mover el carro conmigo
					// comprobar adem�s que si se va a mover con un carro y hay otro carro delante no puede moverse
					if (thereisacart(scn_sel, scn_sel.robots[i].GetPose()) && !thereisacart(scn_sel, new_pose)) {
						//es m�s f�cil clonar.
						id++;
						Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots.at(0), scn_sel.robots.at(1), scn_sel.carts.at(0), scn_sel.carts.at(1), pose_dest, scn_sel.carts.at(0));
						//modificar robot
						newscn.UpdateRobotPose(newscn.robots[i], new_pose);
						//get the cart
						int cart_indx = whichcartis_indx(newscn, scn_sel.robots[i].GetPose());
						//modify the cart in the new scenario
						//TODO: POR QUE AQU� ES INDICE?????
						newscn.UpdateCartPose(newscn.carts[cart_indx], new_pose);
						//evaluar
						newscn.evaluate(newscn.carts.at(0), pose_dest);
						//a�adir//TODO:si a�ado de manera ordenada, hago que siempre tenga que coger el primero nodos.at(0)
						if (!isalreadydefined(newscn)) {
							cout << "adem�s se lleva el carro" << endl;
							addtonodes(newscn);
						}
					}

				}
				//si uno puede moverse en y - 1
				new_pose = Pose(scn_sel.robots[i].GetPose().x, scn_sel.robots[i].GetPose().y - 1);
				if (pose_valid(scn_sel, new_pose) &&
					!thereisarobot(scn_sel, new_pose)) {
					cout << "Robot:" << scn_sel.robots[i].GetId() << " puede mover a y - 1 " << endl;
					//es m�s f�cil clonar.
					id++;
					Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots.at(0), scn_sel.robots.at(1), scn_sel.carts.at(0), scn_sel.carts.at(1), pose_dest, scn_sel.carts.at(0));
					//modificar
					newscn.UpdateRobotPose(newscn.robots[i], new_pose);
					//evaluar
					newscn.evaluate(scn_sel.carts.at(0), pose_dest);
					cout << "escenario id:" << newscn.id << endl;
					//cout << "la pose en y es:" << newscn.robots[i].GetPose().y << endl;
					//newscn.print();
					//a�adir 
					//TODO:
					//1� que no est� ya definido ese escenario
					if (!isalreadydefined(newscn)) {
						addtonodes(newscn);
					}
					else {
						id--;
					}

					//si adem�s hay un carro en la misma posici�n: puedo mover el carro conmigo
					// comprobar adem�s que si se va a mover con un carro y hay otro carro delante no puede moverse
					if (thereisacart(scn_sel, scn_sel.robots[i].GetPose()) && !thereisacart(scn_sel, new_pose)) {
						//es m�s f�cil clonar.
						id++;
						Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots.at(0), scn_sel.robots.at(1), scn_sel.carts.at(0), scn_sel.carts.at(1), pose_dest, scn_sel.carts.at(0));
						//modificar robot
						newscn.UpdateRobotPose(newscn.robots[i], new_pose);
						//get the cart
						int cart_indx = whichcartis_indx(newscn, scn_sel.robots[i].GetPose());
						//modify the cart in the new scenario
						//TODO: POR QUE AQU� ES INDICE?????
						newscn.UpdateCartPose(newscn.carts[cart_indx], new_pose);
						//evaluar
						newscn.evaluate(newscn.carts.at(0), pose_dest);
						//a�adir//TODO:si a�ado de manera ordenada, hago que siempre tenga que coger el primero nodos.at(0)
						if (!isalreadydefined(newscn)) {
							cout << "adem�s se lleva el carro" << endl;
							addtonodes(newscn);
						}
					}

				}
				//si hay otro robot en esa pose, mover� r1 a donde est� r2 si r2 puede moverse a cualquier otro sitio.

				//si no puede moverse nada
				cout << "Robot " << scn_sel.robots[i].GetId() << " NO puede mover mas" << endl;
			}
			//una vez he creado todos los nodos quito el que he derivado que ya no tiene por qu� ser el primero
			int ind = 0;
			for (std::vector<Scenario>::iterator it = nodos_activos.begin(); it != nodos_activos.end(); ++it, ++ind) {
				if (scn_sel.id == it->id) {
					cout << "borro el escenario con id " << (scn_sel.id) << " o no id " << (it->id) << endl;
					nodos_visitados.push_back(scn_sel);
					nodos_activos.erase(it);
				}
			}
			//y vuelvo a empezar

		}
	}

	//Ya tengo la solución
	//Falta buscar en los nodos visitados empezando por el último .back la lista de predecesores por id_prev

	// Create a graph given in the above diagram
	/*Graph g(4);
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(1, 2);
	g.addEdge(2, 0);
	g.addEdge(2, 3);
	g.addEdge(3, 3);

	cout << "Following is Breadth First Traversal "
	<< "(starting from vertex 2) \n";
	g.BFS(2);*/

	return 0;
}

bool pose_valid(Scenario scn, Pose p) {
	if (p.x < 0 || p.x >= scn.width || p.y < 0 || p.y >= scn.height)
		return false;
	return true;
}

bool thereisarobot(Scenario scn, Pose p) {
	for (int i = 0; i < scn.robots.size(); ++i) {
		if (scn.robots[i].GetPose().x == p.x && scn.robots[i].GetPose().y == p.y)
			return true;
	}
	return false;
}

bool thereisacart(Scenario scn, Pose p) {
	for (int i = 0; i < scn.carts.size(); ++i) {
		if (scn.carts[i].GetPose().x == p.x && scn.carts[i].GetPose().y == p.y)
			return true;
	}
	return false;
}

/*Hay que estar seguro de que hay un robot antes de llamar*/
Robot whichrobotis(Scenario scn, Pose p) {
	for (int i = 0; i < scn.robots.size(); ++i) {
		if (scn.robots[i].GetPose().x == p.x && scn.robots[i].GetPose().y == p.y)
			return scn.robots[i];
	}
}
/*Hay que estar seguro de que hay un robot antes de llamar*/
int whichrobotis_indx(Scenario scn, Pose p) {
	for (int i = 0; i < scn.robots.size(); ++i) {
		if (scn.robots[i].GetPose().x == p.x && scn.robots[i].GetPose().y == p.y)
			return i;
	}
}
/*Hay que estar seguro de que hay un carro antes de llamar*/
Cart whichcartis(Scenario scn, Pose p) {
	for (int i = 0; i < scn.carts.size(); ++i) {
		if (scn.carts[i].GetPose().x == p.x && scn.carts[i].GetPose().y == p.y)
			return scn.carts[i];
	}
}
/*Hay que estar seguro de que hay un carro antes de llamar*/
int whichcartis_indx(Scenario scn, Pose p) {
	for (int i = 0; i < scn.carts.size(); ++i) {
		if (scn.carts[i].GetPose().x == p.x && scn.carts[i].GetPose().y == p.y)
			return i;
	}
}

bool isalreadydefined(Scenario scn) {
	//para cada escenario ya definido
	int objetcs_equals = 0;
	int objects_at_scn = scn.robots.size() + scn.carts.size();
	cout << "Buscando entre los " << nodos.size() << " escenarios ya definidos, " << objects_at_scn << " objs iguales" << endl;

	for (vector<Scenario>::iterator it = nodos.begin(); it != nodos.end(); ++it) {
		objetcs_equals = 0;
		//Entre todos los robots de ese escenario
		for (int i = 0; i < it->robots.size(); ++i) {
			//para cada robot que se encuentra dentro del escenario seleccionado
			for (int j = 0; j < scn.robots.size(); ++j) {
				if (it->robots[i].GetId() == scn.robots[j].GetId()) {
					cout << "checking robot " << it->robots[i].GetId() << " at some node [" << it->robots[i].GetPose().x << ","
						<< it->robots[i].GetPose().y << "] vs at newscn [" << scn.robots[j].GetPose().x << "," <<
						scn.robots[j].GetPose().y << "]" << endl;
					if (it->robots[i].GetPose().x == scn.robots[j].GetPose().x &&
						it->robots[i].GetPose().y == scn.robots[j].GetPose().y) {
						cout << "se ha encontrado un escenario igual de robots " << objetcs_equals << endl;
						objetcs_equals++;
					}
				}
			}
		}
		//Entre todos los carros de ese escenario
		for (int i = 0; i < it->carts.size(); ++i) {
			for (int j = 0; j < scn.carts.size(); ++j) {
				if (it->carts[i].GetId() == scn.carts[j].GetId() &&
					(it->carts[i].GetPose().x == scn.carts[j].GetPose().x &&
						it->carts[i].GetPose().y == scn.carts[j].GetPose().y)) {
					cout << "se ha encontrado un escenario igual de carts " << objetcs_equals << endl;
					objetcs_equals++;
				}
			}
		}
		if (objetcs_equals == objects_at_scn) {
			cout << "escenario ya definido" << endl;
			return true;
		}
	}

	return false;
}

void addtonodes(Scenario scn) {
	cout << "intentando a�adir escenario" << endl;
	if (scn.value <= nodos_activos.at(0).value) {
		nodos_activos.insert(nodos_activos.begin(), scn);
		nodos.push_back(scn);
		cout << "escenario nuevo con id " << scn.id << ", se añade el primero con value:" << scn.value << endl;
		scn.print();
		return;
	}
	int i = 0;
	for (std::vector<Scenario>::iterator it = nodos_activos.begin(); it != nodos_activos.end(); ++it, ++i) {
		if (scn.value < it->value) {
			nodos_activos.insert(it, scn);
			nodos.push_back(scn);
			cout << "escenario nuevo con id " << scn.id << ", se añade en " << i << " con value:" << scn.value << endl;
			scn.print();
			return;
		}
	}
	//es el �ltimo
	nodos_activos.push_back(scn);
	nodos.push_back(scn);
	cout << "escenario nuevo con id " << scn.id << ", se añade el ultimo con value:" << scn.value << endl;
	scn.print();
	return;
}