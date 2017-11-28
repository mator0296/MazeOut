#ifndef recursiveBackTracker
#define recursiveBackTracker
#include <GraphDefine.H>

bool isAvailable(Arc  *arc, Graph_Node<int> *source, Graph_Node<int> *target , Graph *graphMap, bool depth){
	auto AuxTarget = graphMap->get_tgt_node(arc);
	auto AuxSorce = graphMap->get_src_node(arc);
	bool condicionBacktracker = ((( (target->get_info() == AuxTarget->get_info()  -1) || target->get_info()  == AuxTarget->get_info()+ 1) && source->get_info() == AuxSorce->get_info()  ) || ((source->get_info()  == AuxSorce->get_info() -1 || source->get_info()  == AuxSorce->get_info()+ 1) &&  target->get_info() == AuxTarget->get_info()) )  && arc->get_info().available;
	bool condicionDepth	 = ((( (target->get_info() == AuxTarget->get_info()  -1  and arc->get_info().up ) || target->get_info()  == AuxTarget->get_info()+ 1 and arc->get_info().down ) && source->get_info() == AuxSorce->get_info()  ) || ((source->get_info()  == AuxSorce->get_info() -1 and arc->get_info().left || source->get_info()  == AuxSorce->get_info()+ 1 and arc->get_info().right ) &&  target->get_info() == AuxTarget->get_info()) )  && arc->get_info().available;

	return (depth ? condicionDepth: condicionBacktracker);
}

DynList<Arc*> SearchAvailable(Arc  *arc , Graph *graphMap, bool depth){
	DynList<Arc*> list;
	auto target = graphMap->get_tgt_node(arc);
	auto source = graphMap->get_src_node(arc);
	for (Graph::Arc_Iterator ait(*graphMap); ait.has_curr(); ait.next()){
		//cout << ait.get_curr()->get_info().info << endl;
		if(isAvailable(ait.get_curr(), source, target, graphMap,depth)){
			list.append(ait.get_curr());
		}
		if(list.size()  == 4)
			return list;
	}
	
	return list;

}




void changeMap(Arc *arcDest ,  Arc *arcOrig, char **map , Graph *graphMap){
	auto targetDest = graphMap->get_tgt_node(arcDest);
	auto sourceDest = graphMap->get_src_node(arcDest);
	auto targetOrig = graphMap->get_tgt_node(arcOrig);
	auto sourceOrig = graphMap->get_src_node(arcOrig);

	if(targetDest->get_info() > targetOrig->get_info()){
		//bajando		
		map[targetDest->get_info()*2][sourceDest->get_info()*2 + 1] = ' ';
		arcOrig->get_info().down = true;
		arcDest->get_info().up = true;
	}else if(targetDest->get_info() < targetOrig->get_info()){
		//Subiendo
			map[targetOrig->get_info()*2][sourceDest->get_info()*2 + 1] = ' ';
			arcOrig->get_info().up = true;
			arcDest->get_info().down = true;
	}else if(sourceDest->get_info() > sourceOrig->get_info()){
		//derecha		
		map[targetDest->get_info()*2 + 1][sourceDest->get_info()*2] = ' ';
		arcOrig->get_info().right = true;
		arcDest->get_info().left= true;
	}else{
		//izquierda
		map[targetDest->get_info()*2 + 1][sourceOrig->get_info()*2] = ' ';
		arcOrig->get_info().left = true;
		arcDest->get_info().right = true;
	}



}

void Recorrido(Arc *Arc, Graph *graphMap , char **map){
	
	
	
	if(!Arc->get_info().available)return;
	Arc->get_info().available = false;
	auto listArcs = SearchAvailable(Arc, graphMap, false);
	
	
	while(!listArcs.is_empty()){
		int i = rand() % listArcs.size();
		auto ArcDest = listArcs.get(i);
		listArcs.remove([&](auto a){return(a->get_info().info == ArcDest->get_info().info);});
		if(ArcDest->get_info().available)
			changeMap(ArcDest, Arc, map, graphMap);
		Recorrido(ArcDest, graphMap, map);
		
	}
	


}


void resetGraphAvalible(Graph *graphMap){
	
	for (Graph::Arc_Iterator ait(*graphMap); ait.has_curr(); ait.next())
		ait.get_curr()->get_info().available =true;


}

void initRecursiveBackTracker(char **map, Graph *graphMap){
	srand(time(NULL));
	Recorrido(graphMap->get_first_arc(), graphMap, map);

	
}


void Depth(void(*visit)(Arc*), Arc *start, Graph *graphMap){

	if(!start->get_info().available)return;
	start->get_info().available = false;
	visit(start);
	auto listArcs = SearchAvailable(start, graphMap, true);
	
	for(DynList<Arc*>::iterator it (listArcs) ; it.has_curr(); it.next()){
		if(it.get_curr()->get_info().available){
			Depth(visit,it.get_curr(), graphMap);
		}
	}

	
}


void brief(void(*visit)(Arc*), Arc *start, Graph *graphMap){

	if(!start->get_info().available)return;
	start->get_info().available = false;
	DynListQueue<Arc*> arcQueue;
	arcQueue.put(start);
	while(!arcQueue.is_empty()){
		auto a = arcQueue.get();
		a->get_info().available = false;
		visit(a);
		auto listArcs = SearchAvailable(a, graphMap, true);
		for(DynList<Arc*>::iterator it (listArcs) ; it.has_curr(); it.next()){
			if(it.get_curr()->get_info().available){
				arcQueue.put(it.get_curr());
			}
		}

	}
	

	
}

bool DepthPath(Arc *start, Arc *Dest, Graph *graphMap, DynList<Arc*> &path){
	

	if(start->get_info().info ==  Dest->get_info().info) return true;
	if(!start->get_info().available)return false;
	start->get_info().available = false;

	auto listArcs = SearchAvailable(start, graphMap, true);
	
	for(DynList<Arc*>::iterator it (listArcs) ; it.has_curr(); it.next()){
		if(it.get_curr()->get_info().available){
			if(DepthPath(it.get_curr(), Dest, graphMap, path)){
				path.insert(it.get_curr());
				return true;
			}
		}
	}

	return false;

	
}

DynList<Arc*>* briefPath( Arc *start, Arc *ArcDest, Graph *graphMap){

	if(!start->get_info().available)return NULL;
	start->get_info().available = false;
	DynListQueue<DynList<Arc*>*> arcQueue;
	auto listStar = new DynList<Arc*>();
	listStar->append(start);
	arcQueue.put(listStar);
	while(!arcQueue.is_empty()){
		auto list = arcQueue.get();
		auto arc = list->get_last();
		auto listArcs = SearchAvailable(arc, graphMap, true);
		for(DynList<Arc*>::iterator it (listArcs) ; it.has_curr(); it.next()){
			if(it.get_curr()->get_info().available){
				if(it.get_curr() == ArcDest){
					auto newList = new DynList<Arc*>(*list);
					newList->append(it.get_curr());
					while(!arcQueue.is_empty()){

						auto a = arcQueue.get();
						delete a;
					}
					return newList;

				}
				it.get_curr()->get_info().available=false;
				auto newList = new DynList<Arc*>(*list);
				newList->append(it.get_curr());
				arcQueue.put(newList);
			}
		}
		delete list;

	}
	
	return NULL;
	
}
pair<int,int> arcToCord(Arc *arc, Graph *graph){
	auto target = graph->get_tgt_node(arc);
	auto source = graph->get_src_node(arc);	
	return make_pair(2*target->get_info(), 2*source->get_info());
}

DynList<pair<int,int> > arcToCord(DynList<Arc*>* MoveArc, Graph *graph){

	DynList<pair<int, int> > moveCord;
	
	for(DynList<Arc*>::iterator it(*MoveArc); it.has_curr();it.next() ){
		auto target = graph->get_tgt_node(it.get_curr());
		auto source = graph->get_src_node(it.get_curr());
		
		if(moveCord.size()!=0){
			auto lastCord = moveCord.get_last();
			if(2*target->get_info() - lastCord.first  == 0 and 2*source->get_info() - lastCord.second >0){

				moveCord.append(make_pair(2*target->get_info(), 2*source->get_info() -1));
			}else if(2*target->get_info() - lastCord.first  > 0 and 2*source->get_info() - lastCord.second ==0){
				moveCord.append(make_pair(2*target->get_info()-1, 2*source->get_info()));
			}else if(2*target->get_info() - lastCord.first  == 0 and 2*source->get_info() - lastCord.second <0){

				moveCord.append(make_pair(2*target->get_info(), 2*source->get_info() +1));
			}else if(2*target->get_info() - lastCord.first  < 0 and 2*source->get_info() - lastCord.second ==0){
				moveCord.append(make_pair(2*target->get_info()+1, 2*source->get_info()));
			}
		}
		
		moveCord.append(arcToCord(it.get_curr(),graph));

	}
	return moveCord;
}



pair<Arc*, int> cordToArc(pair<int,int> cord, Graph *graph){

	if(cord.first%2 == 0 and cord.second%2 == 0){
		for (Graph::Arc_Iterator ait(*graph); ait.has_curr(); ait.next()){
			auto target = graph->get_tgt_node(ait.get_curr());
			auto source = graph->get_src_node(ait.get_curr());
			if(target->get_info()== (cord.first/2) and source->get_info() == (cord.second/2))
				return make_pair(ait.get_curr(), 0);	

		}

	}else if(cord.first%2 == 0){

		for (Graph::Arc_Iterator ait(*graph); ait.has_curr(); ait.next()){
			auto target = graph->get_tgt_node(ait.get_curr());
			auto source = graph->get_src_node(ait.get_curr());
			if(target->get_info()== (cord.first/2)  and source->get_info() == (cord.second/2))
				return make_pair(ait.get_curr(), 1);	// up

		}
	}else if(cord.second%2 == 0){
		for (Graph::Arc_Iterator ait(*graph); ait.has_curr(); ait.next()){
			auto target = graph->get_tgt_node(ait.get_curr());
			auto source = graph->get_src_node(ait.get_curr());
			if(target->get_info()== (cord.first/2) and source->get_info() == (cord.second/2))
				return make_pair(ait.get_curr(), 2);	// left

		}
	}
}

#endif