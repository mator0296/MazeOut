#ifndef GRAPHDEFINE
#define GRAPHDEFINE
#include <tpl_graph.H>
#include <htlist.H>
#include <tpl_dynListQueue.H>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <cmath>

using namespace std;
class arcData{
	public:
		string info;
		bool available;
		bool up, down, right, left;
		arcData(){ this->info = "None";this->available = true;this->up = false; this->down = false; this->right = false; this->left = false;}
		arcData(string data,  bool available = true){this->info = data; this->available = available; this->up = false; this->down = false; this->right = false; this->left = false;};
		~arcData(){};
		
};

using Arc = Graph_Arc<arcData>;
using Node = Graph_Node<int>;
using Graph =  List_Digraph<Node,Arc >;

#endif