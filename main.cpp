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
void evolute_scn(Scenario scn, int ind, Pose p, vector < Robot > robotstomove, bool level_up);

using namespace std;
vector < Scenario > nodos;
vector < Scenario > nodos_activos;
vector < Scenario > nodos_visitados;
int global_scn_id;
int global_prof_id;

Pose pose_final_dest;
Cart cart_final_sel;
Pose new_pose, new_new_pose;

// Driver program to test methods of graph class
int main()
{

	const int maze_size_x = 80;
	const int maze_size_y = 25;
	const int num_robots = 3;
	vector < vector < bool > > maze;

	vector < Pose > new_candidate_poses;



	//Robot r1 = Robot("r1", 0, 2);
	//Robot r2 = Robot("r2", 1, 2);

	vector < Robot > Robots;
	Robots.push_back(Robot("r1", 0, 2));
	Robots.push_back(Robot("r2", 1, 2));
	//Robots.push_back(Robot("r3", 2, 2));
	
	vector < Cart > Carts;
	Carts.push_back(Cart("c1", 0, 0));
	Carts.push_back(Cart("c2", 1, 0));
	Carts.push_back(Cart("c3", 1, 1));

	global_scn_id = 0;
	global_prof_id = 0;

	Scenario scn = Scenario(global_scn_id, -1, 3, 3, Robots, Carts, global_prof_id);

	Scenario scn_sel;
	
	//MISSION
	pose_final_dest = Pose(2, 0);
	cart_final_sel = Carts.at(0);

	/*scn.addRobot(r1);
	scn.addRobot(r2);
	scn.addCart(c1);
	scn.addCart(c2);
	*/
	
	scn.evaluate(cart_final_sel, pose_final_dest);
	
	scn.print();

	cout << "-value-" << scn.value << endl;
	
	//add the first node
	nodos_activos.push_back(scn);
	nodos.push_back(scn);
	
	vector <string> robots_visitados;
	
	
	
	int iteraciones= 50;


	while (!nodos_activos.empty() /*&& iteraciones>0*/) {
		iteraciones--;
		//al estar ordenados cojo siempre el primer elemento
		scn_sel = nodos_activos.at(0);

		//insert into visited list
		nodos_visitados.push_back(scn_sel);
		//remove the node from actived nodes now, otherwise later can be moved
		nodos_activos.erase(nodos_activos.begin());
		
		//if value equals prof, end
		cout << ">>>Derivando el nodo con value:" << scn_sel.value << " id:" << scn_sel.id << " <<<" << endl;
		//if (scn_sel.value == scn_sel.prof) {
		if(scn_sel.IsSolved(cart_final_sel, pose_final_dest)){
			cout << " FIN." << endl;
			nodos_visitados.push_back(scn_sel);
			break;
		}
		else {
			
			//for each robot
			for (int i = 0; i < scn_sel.robots.size(); i++) {
				//meto en una lista todos los robots menos el actual que voy a mover
				vector < Robot > robottomove;
				for (int k = 0; k < scn_sel.robots.size(); k++) {
					if(i!=k){
						robottomove.push_back(scn_sel.robots[k]);						
					}
				}
				//add the new poses he can reach
				new_candidate_poses.push_back(Pose(scn_sel.robots[i].GetPose().x + 1, scn_sel.robots[i].GetPose().y));
				new_candidate_poses.push_back(Pose(scn_sel.robots[i].GetPose().x, scn_sel.robots[i].GetPose().y + 1));
				new_candidate_poses.push_back(Pose(scn_sel.robots[i].GetPose().x - 1, scn_sel.robots[i].GetPose().y));
				new_candidate_poses.push_back(Pose(scn_sel.robots[i].GetPose().x, scn_sel.robots[i].GetPose().y - 1));
				
				//for each candidate pose (4) there is a new scenario
				while(!new_candidate_poses.empty()){
					new_pose = new_candidate_poses.back();
					new_candidate_poses.pop_back();					
					evolute_scn(scn_sel, i, new_pose, robottomove, true);

				}

				//si no puede moverse nada
				cout << "Robot " << scn_sel.robots[i].GetId() << " NO puede mover mas" << endl;
			}
		}
	}

	//Ya tengo la solucion
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
	cout << ">Numero de movimientos:" << nodos_solucion.size()-1 << endl;
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
	//MODIFICO EL ID:
	global_scn_id++;
		
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

void evolute_scn(Scenario scn, int i_robot, Pose dest_pose, vector < Robot > robotstomove, bool level_up){
	

	//derivar cada posición de cada robot
	if (pose_valid(scn, dest_pose)) {
		//es mas facil clonar.
		Scenario newscn;
		if(level_up){			
			 newscn = Scenario(global_scn_id+1, scn.id, scn.height, scn.width, scn.robots, scn.carts, scn.prof+1);
		}else{
			 
			 newscn = Scenario(global_scn_id+1, scn.id_prev, scn.height, scn.width, scn.robots, scn.carts, scn.prof);
		}
		
		//mueve robots sin carros:
		//r1-> X
		//r1-> cX
		
		//si no hay ningún robot en la nueva posición puedo actualizar el escenario		
		if(!thereisarobot(newscn, dest_pose)){
			//modifica la posición (internamente añade un movimiento al robot)
			newscn.MoveRobotTo(scn.robots[i_robot], dest_pose);			
			//evaluar
			newscn.evaluate(cart_final_sel, pose_final_dest);			
			//si no está definido ese escenario, se añade
			if (!isalreadydefined(newscn)) {
				addtonodes(newscn);
			}
			
			
			//y busca a partir de i en adelante, el resto de robots dónde puede moverse siempre que no haya otro robot en ese sitio
			//for (int k = i_robot+1; k < scn.robots.size(); k++) {			
			while(!robotstomove.empty()){
					cout << "check el mov:" << robotstomove.back().GetId() << endl;
					//find the robot moved and remove it from robotstomove
					Robot robotToMove = robotstomove.back();
					//cout<<"el robot a mover es:"<<robotToMove.GetId()<<endl;
					int k=0;
					for(int i; i < scn.robots.size();i++)
						if(robotToMove.GetId()==newscn.robots[i].GetId()){
							k=i;
							break;
						}
					robotstomove.pop_back();
					
					vector < Pose > new_new_poses;
					new_new_poses.push_back(Pose(newscn.robots[k].GetPose().x + 1, newscn.robots[k].GetPose().y)); //derecha
					new_new_poses.push_back(Pose(newscn.robots[k].GetPose().x, newscn.robots[k].GetPose().y + 1)); //abajo
					new_new_poses.push_back(Pose(newscn.robots[k].GetPose().x - 1, newscn.robots[k].GetPose().y)); //izquierda
					new_new_poses.push_back(Pose(newscn.robots[k].GetPose().x, newscn.robots[k].GetPose().y - 1)); //arriba
					while(!new_new_poses.empty()){
						new_new_pose = new_new_poses.back();
						new_new_poses.pop_back();
						//si comento este print el algoritmo no funciona correctamente! ?¿?¿
						//cout<<"intentando mover:"<<newscn.robots[k].GetId();						
						evolute_scn(newscn, k, new_new_pose, robotstomove, false);
					}							
			 }
			 cout << "no hay más que mover" <<endl;
		}
		
		//mueve robots sin carros donde hay otros robots: Mover a la vez en la misma dirección:
		//Hay casos ya contemplados, por ejemplo, si uno se mueve a la izquierda donde hay un robot pero otro 
		//r1-> r2 ->r3
		//si se va a mover a la izquierda y hay otro robot, mira a ver si ese robot puede moverse a la izquierda
		
		
		//mueve robots con carros:
		//c1r1-> X
		
		//si ademas hay un carro en la misma posici n: puedo mover el carro conmigo
		// ademas que si se va a mover con un carro y hay otro carro delante no puede moverse AUNQUE haya un robot tb
		if (thereisacart(scn, scn.robots[i_robot].GetPose()) && !thereisacart(scn, dest_pose) && !thereisarobot(scn, dest_pose)) {
			cout<<"mueve carro y robotstomove:"<< robotstomove.size() <<endl;
			//es mas facil clonar.
			Scenario newscn;
			if(level_up){
				newscn = Scenario(global_scn_id+1, scn.id, scn.height, scn.width, scn.robots, scn.carts, scn.prof+1);
			}else{
				newscn = Scenario(global_scn_id+1, scn.id_prev, scn.height, scn.width, scn.robots, scn.carts, scn.prof);
			}
							
			//modificar robot
			newscn.MoveRobotWithCartTo(scn.robots[i_robot], dest_pose);
			//get the cart
			int cart_indx = whichcartis_indx(newscn, scn.robots[i_robot].GetPose());
			//modify the cart in the new scenario
			//TODO: POR QUE AQU  ES INDICE?????
			newscn.UpdateCartPose(newscn.carts[cart_indx], dest_pose);
			//evaluar
			newscn.evaluate(cart_final_sel, pose_final_dest);
			//anyadir si no existe ya el escenario
			if (!isalreadydefined(newscn)) {
				addtonodes(newscn);
			}
			
			//y busca a partir de i en adelante, el resto de robots dónde puede moverse
			//for (int k = i; k < scn_sel.robots.size(); k++) {			
			//for (int k = i_robot+1; k < scn.robots.size(); k++) {
			while(!robotstomove.empty()){
				cout << "checkConCAR el mov:" << robotstomove.back().GetId() << endl;
				//find the robot moved and remove it from robotstomove
				Robot robotToMove = robotstomove.back();
				//cout<<"el robot a mover es:"<<robotToMove.GetId()<<endl;
				int k=0;
				for(int i; i < scn.robots.size();i++)
					if(robotToMove.GetId()==newscn.robots[i].GetId()){
						k=i;
						break;
					}
				robotstomove.pop_back();
				
				vector < Pose > new_new_poses;
				new_new_poses.push_back(Pose(newscn.robots[k].GetPose().x + 1, newscn.robots[k].GetPose().y));
				new_new_poses.push_back(Pose(newscn.robots[k].GetPose().x, newscn.robots[k].GetPose().y + 1));
				new_new_poses.push_back(Pose(newscn.robots[k].GetPose().x - 1, newscn.robots[k].GetPose().y));
				new_new_poses.push_back(Pose(newscn.robots[k].GetPose().x, newscn.robots[k].GetPose().y - 1));
				while(!new_new_poses.empty()){
					new_new_pose = new_new_poses.back();
					new_new_poses.pop_back();
					//si descomento esto no funciona! ?¿?¿
					//cout<<"intentando mover:"<<newscn.robots[k].GetId();
					evolute_scn(newscn, k, new_new_pose, robotstomove, false);
				}
					
			}
				
			
		}
		
		
	}

}
