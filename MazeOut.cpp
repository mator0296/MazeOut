#include <GameDefine.h>

int main(){

	mazeSize = 10;
	mapsize = 15;
	Game mazeGame;
	
	if(!mazeGame.init())
		return 0;
	mazeGame.maze.PrintMaze();
	mazeGame.maze.makeDepthPath();

	
	al_register_event_source(mazeGame.event_queue, al_get_display_event_source(mazeGame.display));
	al_register_event_source(mazeGame.event_queue, al_get_keyboard_event_source());
	al_register_event_source(mazeGame.event_queue, al_get_timer_event_source(mazeGame.timer));
	bool doexit = false;
	al_start_timer(mazeGame.timer);
	mazeGame.gamenemy.stateX= 10;
	mazeGame.gamenemy.stateY= 10;
	while(!doexit){
		ALLEGRO_EVENT ev;
		al_clear_to_color(al_map_rgb(0,0,0));
		mazeGame.PaintMap();
		al_flip_display();	
		al_wait_for_event(mazeGame.event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			doexit = true;
		}else if(ev.type == ALLEGRO_EVENT_TIMER) {
			
			
			mazeGame.EnemysMove();
			if(mazeGame.gamenemy.stateX == mazeGame.gameplayer.stateX and  mazeGame.gamenemy.stateY == mazeGame.gameplayer.stateY ){
				cout << "fin del juego" << endl;
				mazeGame.Clean();
				return 0;
			}
			al_clear_to_color(al_map_rgb(0,0,0));
			//cout << "("<<mazeGame.gameplayer.stateY << "," << mazeGame.gameplayer.stateX << ")" << endl;
			mazeGame.PaintMap();
			
			al_flip_display();	
			

		}if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			mazeGame.gameplayer.moveAnimation(ev);
			
		
				
			mazeGame.recalculateEnemysMove();
		}
	}
	
   
 
 	
	mazeGame.Clean();
	return 0;
}