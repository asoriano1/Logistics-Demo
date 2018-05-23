// ConsoleApplication1.cpp: define el punto de entrada de la aplicaciÃ³n de consola.
//

//#include "stdafx.h"

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

	//Robot r1 = Robot("r1", 0, 2);
	//Robot r2 = Robot("r2", 1, 2);

	vector < Robot > Robots;
	Robots.push_back(Robot("r1", 0, 2));
	Robots.push_back(Robot("r2", 1, 2));
	Robots.push_back(Robot("r3", 2, 2));

	vector < Cart > Carts;
	Carts.push_back(Cart("c1", 0, 0));
	Carts.push_back(Cart("c2", 1, 0));
	Carts.push_back(Cart("c3", 1, 1));

	int id = 0;

	//mission
	//TODO QUE EL CARRO SEA GLOBAL PERO SE ACTUALICE
	//igual s  que iba (VUELVE A COMO ESTABA A VER)
	//Cart cart_sel = c1;
	Pose pose_dest = Pose(2, 0);
	Cart cart_selected = Carts.at(0);

	Scenario scn = Scenario(id, -1, 3, 3, Robots, Carts, 0);

	Scenario scn_sel;

	/*scn.addRobot(r1);
	scn.addRobot(r2);
	scn.addCart(c1);
	scn.addCart(c2);
	*/
	scn.print();
	
	scn.evaluate(cart_selected, pose_dest);

	cout << "-value-" << scn.value << endl;
	
	//a ade el primer nodo
	nodos_activos.push_back(scn);
	nodos.push_back(scn);
	//si existe alg n nodo con el valor a 0=>fin

	//si no, selecciona el de menor value y derivalo
	//si al a adir nodos, se a aden ordenados siempre hay que coger el primero

	//derivate(nodo_actual)
	//coge el primer robot, puede mover arriba, abajo , izquierda o derecha
	//si puede crea un escenario nuevo con esa pose
	//definir un m todo para dar una posici n y decir si hay robot
	vector <string> robots_visitados;
	
	//int iteraciones= 100;


	while (!nodos_activos.empty() /*&& iteraciones>0*/) {
		//iteraciones--;
		//al estar ordenados cojo siempre el primer elemento
		scn_sel = nodos_activos.at(0);

		//lo quito de los activos ahora porque luego no se dónde estará
		nodos_visitados.push_back(scn_sel);
		nodos_activos.erase(nodos_activos.begin());
		
		//si la evaluacion de la primera posicion es 0, he terminado
		cout << ">>>Derivando el nodo con value:" << scn_sel.value << " id:" << scn_sel.id << " " << endl;
		if (scn_sel.value == scn_sel.prof) {
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
					//es m s facil clonar.
					id++;
				
					Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots, scn_sel.carts, scn_sel.prof+1);
					//modificar
					newscn.UpdateRobotPose(newscn.robots[i], new_pose);
					//evaluar
					newscn.evaluate(cart_selected, pose_dest);
					cout << "escenario id:" << newscn.id << endl;
					//a adir 
					//1  que no est  ya definido ese escenario
					if (!isalreadydefined(newscn)) {
						addtonodes(newscn);
					}
					else {
						id--;
					}

					//si adem s hay un carro en la misma posici n: puedo mover el carro conmigo
					// comprobar adem s que si se va a mover con un carro y hay otro carro delante no puede moverse
					if (thereisacart(scn_sel, scn_sel.robots[i].GetPose()) && !thereisacart(scn_sel, new_pose)) {
						//es m s f cil clonar.
						id++;
						Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots, scn_sel.carts, scn_sel.prof+1);
						//modificar robot
						newscn.UpdateRobotPose(newscn.robots[i], new_pose);
						//get the cart
						int cart_indx = whichcartis_indx(newscn, scn_sel.robots[i].GetPose());
						//modify the cart in the new scenario
						//TODO: POR QUE AQU  ES INDICE?????
						newscn.UpdateCartPose(newscn.carts[cart_indx], new_pose);
						//evaluar
						newscn.evaluate(cart_selected, pose_dest);
						//a adir//TODO:si a ado de manera ordenada, hago que siempre tenga que coger el primero nodos.at(0)
						if (!isalreadydefined(newscn)) {
							cout << "ademas se lleva el carro" << endl;
							addtonodes(newscn);
						}
					}

				}
				//si uno puede moverse en y + 1
				new_pose = Pose(scn_sel.robots[i].GetPose().x, scn_sel.robots[i].GetPose().y + 1);
				if (pose_valid(scn_sel, new_pose) &&
					!thereisarobot(scn_sel, new_pose)) {
					cout << "Robot:" << scn_sel.robots[i].GetId() << " puede mover a y + 1 " << endl;
					//es m s f cil clonar.
					id++;
					Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots, scn_sel.carts, scn_sel.prof+1);
					//modificar
					newscn.UpdateRobotPose(newscn.robots[i], new_pose);
					//evaluar
					newscn.evaluate(cart_selected, pose_dest);
					cout << "escenario id:" << newscn.id << endl;
					//a adir 
					//TODO:
					//1  que no est  ya definido ese escenario
					if (!isalreadydefined(newscn)) {
						addtonodes(newscn);
					}
					else {
						id--;
					}

					//si adem s hay un carro en la misma posici n: puedo mover el carro conmigo
					// comprobar adem s que si se va a mover con un carro y hay otro carro delante no puede moverse
					if (thereisacart(scn_sel, scn_sel.robots[i].GetPose()) && !thereisacart(scn_sel, new_pose)) {
						//es m s f cil clonar.
						id++;
						Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots, scn_sel.carts, scn_sel.prof+1);
						//modificar robot
						newscn.UpdateRobotPose(newscn.robots[i], new_pose);
						//get the cart
						int cart_indx = whichcartis_indx(newscn, scn_sel.robots[i].GetPose());
						//modify the cart in the new scenario
						//TODO: POR QUE AQU  ES INDICE?????
						newscn.UpdateCartPose(newscn.carts[cart_indx], new_pose);
						//evaluar
						newscn.evaluate(cart_selected, pose_dest);
						//a adir//TODO:si a ado de manera ordenada, hago que siempre tenga que coger el primero nodos.at(0)
						if (!isalreadydefined(newscn)) {
							cout << "ademas se lleva el carro" << endl;
							addtonodes(newscn);
						}
					}

				}
				//si uno puede moverse en x - 1
				new_pose = Pose(scn_sel.robots[i].GetPose().x - 1, scn_sel.robots[i].GetPose().y);
				if (pose_valid(scn_sel, new_pose) &&
					!thereisarobot(scn_sel, new_pose)) {
					cout << "Robot:" << scn_sel.robots[i].GetId() << " puede mover a x - 1 [ " << scn_sel.robots[i].GetPose().x - 1 << " ]" << endl;
					//es m s f cil clonar.
					id++;
					Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots, scn_sel.carts, scn_sel.prof+1);
					//modificar
					newscn.UpdateRobotPose(newscn.robots[i], new_pose);
					//evaluar
					newscn.evaluate(cart_selected, pose_dest);
					cout << "escenario id:" << newscn.id << endl;
					//a adir 
					//TODO:
					//1  que no est  ya definido ese escenario
					if (!isalreadydefined(newscn)) {
						addtonodes(newscn);
					}
					else {
						id--;
					}

					//si adem s hay un carro en la misma posici n: puedo mover el carro conmigo
					// comprobar adem s que si se va a mover con un carro y hay otro carro delante no puede moverse
					if (thereisacart(scn_sel, scn_sel.robots[i].GetPose()) && !thereisacart(scn_sel, new_pose)) {
						//es m s f cil clonar.
						id++;
						Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots, scn_sel.carts, scn_sel.prof+1);
						//modificar robot
						newscn.UpdateRobotPose(newscn.robots[i], new_pose);
						//get the cart
						int cart_indx = whichcartis_indx(newscn, scn_sel.robots[i].GetPose());
						//modify the cart in the new scenario
						//TODO: POR QUE AQU  ES INDICE?????
						newscn.UpdateCartPose(newscn.carts[cart_indx], new_pose);
						//evaluar
						newscn.evaluate(cart_selected, pose_dest);
						//a adir//TODO:si a ado de manera ordenada, hago que siempre tenga que coger el primero nodos.at(0)
						if (!isalreadydefined(newscn)) {
							cout << "ademas se lleva el carro" << endl;
							addtonodes(newscn);
						}
					}

				}
				//si uno puede moverse en y - 1
				new_pose = Pose(scn_sel.robots[i].GetPose().x, scn_sel.robots[i].GetPose().y - 1);
				if (pose_valid(scn_sel, new_pose) &&
					!thereisarobot(scn_sel, new_pose)) {
					cout << "Robot:" << scn_sel.robots[i].GetId() << " puede mover a y - 1 " << endl;
					//es m s f cil clonar.
					id++;
					Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots, scn_sel.carts, scn_sel.prof+1);
					//modificar
					newscn.UpdateRobotPose(newscn.robots[i], new_pose);
					//evaluar
					newscn.evaluate(cart_selected, pose_dest);
					cout << "escenario id:" << newscn.id << endl;
					//cout << "la pose en y es:" << newscn.robots[i].GetPose().y << endl;
					//newscn.print();
					//a adir 
					//TODO:
					//1  que no est  ya definido ese escenario
					if (!isalreadydefined(newscn)) {
						addtonodes(newscn);
					}
					else {
						id--;
					}

					//si adem s hay un carro en la misma posici n: puedo mover el carro conmigo
					// comprobar adem s que si se va a mover con un carro y hay otro carro delante no puede moverse
					if (thereisacart(scn_sel, scn_sel.robots[i].GetPose()) && !thereisacart(scn_sel, new_pose)) {
						//es m s f cil clonar.
						id++;
						Scenario newscn = Scenario(id, scn_sel.id, scn_sel.height, scn_sel.width, scn_sel.robots, scn_sel.carts, scn_sel.prof+1);
						//modificar robot
						newscn.UpdateRobotPose(newscn.robots[i], new_pose);
						//get the cart
						int cart_indx = whichcartis_indx(newscn, scn_sel.robots[i].GetPose());
						//modify the cart in the new scenario
						//TODO: POR QUE AQU  ES INDICE?????
						newscn.UpdateCartPose(newscn.carts[cart_indx], new_pose);
						//evaluar
						newscn.evaluate(cart_selected, pose_dest);
						//a adir//TODO:si a ado de manera ordenada, hago que siempre tenga que coger el primero nodos.at(0)
						if (!isalreadydefined(newscn)) {
							cout << "ademas se lleva el carro" << endl;
							addtonodes(newscn);
						}
					}

				}
				//si hay otro robot en esa pose, mover  r1 a donde est  r2 si r2 puede moverse a cualquier otro sitio.

				//si no puede moverse nada
				cout << "Robot " << scn_sel.robots[i].GetId() << " NO puede mover mas" << endl;
			}
			//una vez he creado todos los nodos quito el que he derivado que ya no tiene por que  ser el primero
			/*int ind = 0;
			for (std::vector<Scenario>::iterator it = nodos_activos.begin(); it != nodos_activos.end(); ++it, ++ind) {
				if (scn_sel.id == it->id) {
					cout << "borro el escenario con id " << (scn_sel.id) << " o no id " << (it->id) << endl;
					nodos_visitados.push_back(scn_sel);
					nodos_activos.erase(it);
				}
			}*/
			//y vuelvo a empezar

		}
	}

	//Ya tengo la soluciÃ³n
	//Falta buscar en los nodos visitados empezando por el Ãºltimo .back la lista de predecesores por id_prev
	vector < Scenario > nodos_solucion;
	//primer nodo es el último añadido
	nodos_solucion.push_back(nodos_visitados.back());
	int ind = 0;
	while(nodos_solucion.back().id!=0){
		for(int i=0; i < nodos_visitados.size();++i){
			if (nodos_solucion.back().id_prev == nodos_visitados[i].id) {
				nodos_solucion.push_back(nodos_visitados[i]);			
			}
		}		
	}
	//están ordenados al revés para evitar insertar al principio (más coste)
	//si se imprimen al revés deberían mostrar la solución
	vector<Scenario>::iterator it = nodos_solucion.end(); 
	cout << ">Numero de movimientos:" << nodos_solucion.size() << endl;
	cout << ">Nodos creados:" << nodos.size() << endl;
	cout << ">Nodos visitados:" << nodos_visitados.size() << endl;
	cout << ">Secuencia de soluciones:" << endl;
	while (!nodos_solucion.empty()){
	
		nodos_solucion.back().print();
		nodos_solucion.pop_back();
	
	} 
	

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
	//cout << "Buscando entre los " << nodos.size() << " escenarios ya definidos, " << objects_at_scn << " objs iguales" << endl;

	for (vector<Scenario>::iterator it = nodos.begin(); it != nodos.end(); ++it) {
		objetcs_equals = 0;
		//Entre todos los robots de ese escenario
		for (int i = 0; i < it->robots.size(); ++i) {
			//para cada robot que se encuentra dentro del escenario seleccionado
			for (int j = 0; j < scn.robots.size(); ++j) {
				if (it->robots[i].GetId() == scn.robots[j].GetId()) {
					//cout << "checking robot " << it->robots[i].GetId() << " at some node [" << it->robots[i].GetPose().x << ","
					//	<< it->robots[i].GetPose().y << "] vs at newscn [" << scn.robots[j].GetPose().x << "," <<
					//	scn.robots[j].GetPose().y << "]" << endl;
					if (it->robots[i].GetPose().x == scn.robots[j].GetPose().x &&
						it->robots[i].GetPose().y == scn.robots[j].GetPose().y) {
					//	cout << "se ha encontrado un escenario igual de robots " << objetcs_equals << endl;
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
					//cout << "se ha encontrado un escenario igual de carts " << objetcs_equals << endl;
					objetcs_equals++;
				}
			}
		}
		if (objetcs_equals == objects_at_scn) {
			//cout << "escenario ya definido" << endl;
			return true;
		}
	}

	return false;
}

void addtonodes(Scenario scn) {
	//cout << "intentando anyadir escenario" << endl;
	//si no hay ningun nodo activo lo añado el último
	if(nodos_activos.size()<=0){
		nodos_activos.push_back(scn);
		nodos.push_back(scn);
		scn.print();
		return;
	}
	//si el valor del escenario es menor que el del primer nodo de activos lo meto el primero
	if (scn.value <= nodos_activos.at(0).value) {
		nodos_activos.insert(nodos_activos.begin(), scn);
		nodos.push_back(scn);
		//cout << "escenario nuevo con id " << scn.id << ", se anyade el primero con value:" << scn.value << endl;
		scn.print();
		return;
	}
	//busco donde meterlo de manera ordenada
	int i = 0;
	for (std::vector<Scenario>::iterator it = nodos_activos.begin(); it != nodos_activos.end(); ++it, ++i) {
		if (scn.value < it->value) {
			nodos_activos.insert(it, scn);
			nodos.push_back(scn);
			//cout << "escenario nuevo con id " << scn.id << ", se anyade en " << i << " con value:" << scn.value << endl;
			scn.print();
			return;
		}
	}
	//es el ultimo
	nodos_activos.push_back(scn);
	nodos.push_back(scn);
	//cout << "escenario nuevo con id " << scn.id << ", se anyade el ultimo con value:" << scn.value << endl;
	scn.print();
	return;
}
