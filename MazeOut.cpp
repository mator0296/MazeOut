#include <GameDefine.h>
#include <stdexcept>  
int main(){

	mazeSize = 10;
	mapsize = 10;
	resourceSize = 70;
	Game mazeGame;
	
	if(!mazeGame.init(1))
		return 0;


	mazeGame.maze.PrintMaze();
	mazeGame.maze.makeDepthPath();

	
	al_register_event_source(mazeGame.event_queue, al_get_display_event_source(mazeGame.display));
	al_register_event_source(mazeGame.event_queue, al_get_keyboard_event_source());
	al_register_event_source(mazeGame.event_queue, al_get_timer_event_source(mazeGame.timer));
	al_register_event_source(mazeGame.event_queue, al_get_mouse_event_source());



	try{ 
		mazeGame.controleMenu();
	}catch(overflow_error &e){

		cout << e.what() << endl;
		
	}

	
   
 
 	
	return 0;
}