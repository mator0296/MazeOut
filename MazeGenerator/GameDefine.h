#ifndef GAMEDEFINE
#define GAMEDEFINE
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <maze.h>
#include <Define.h>


class gameEntity{
	public:
		ALLEGRO_BITMAP *BitMap;
		int stateX, stateY;
		gameEntity( ALLEGRO_BITMAP *bitmap){
			this->stateY = 0;
			this->stateX = 0;
			this->BitMap = bitmap;
		};
		~gameEntity(){
			this->Clean();	
		};
		void Clean(){
			al_destroy_bitmap(this->BitMap);

		}
		void moveRight(){
			if(this->stateX < (2*mazeSize +1) )
				if(map[this->stateY+1][this->stateX+2] != 'X')
					this->stateX++;
			
		}
		void moveLeft(){
			if(this->stateX > 0 )
				if( map[this->stateY+1][this->stateX] != 'X')
					this->stateX--;
			
		}
		void moveDown(){
			if(this->stateY< (2*mazeSize +1) )
				if( map[this->stateY+2][this->stateX+1] != 'X')
					this->stateY++;
			
		}
		void moveUp(){
			if(this->stateY> 0 )
				if( map[this->stateY][this->stateX+1] != 'X')
					this->stateY--;
			
		}

};

class gamePlayer: public gameEntity{
	public:
		gamePlayer():gameEntity(NULL){}
		gamePlayer( ALLEGRO_BITMAP *player):gameEntity( player){};
		void moveAnimation(ALLEGRO_EVENT  &ev){

			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					this->moveUp();
					break;

				case ALLEGRO_KEY_DOWN:
					this->moveDown();
					break;

				case ALLEGRO_KEY_LEFT:
					this->moveLeft();
					break;

				case ALLEGRO_KEY_RIGHT:
					this->moveRight();
					break;
			}
		}

		~gamePlayer(){
			Clean();
		}

};

class gameEnemy: public gameEntity{
	public:
		DynList<pair<int, int> > moveList;
		int i, rad;
		gameEnemy():gameEntity(NULL), i(0){}
		gameEnemy( ALLEGRO_BITMAP *BitMap):gameEntity(BitMap){};
		void moveAnimation(){
			this->i++;
			if(this->i<moveList.size()){
				auto cord = moveList.get(i);
				this->stateX = cord.second;
				this->stateY = cord.first;

			}
		
		}
		void setmoveList(Maze &maze, gamePlayer &gameplayer){

			moveList = maze.CordPath(make_pair(this->stateY, this->stateX), make_pair(gameplayer.stateY, gameplayer.stateX));
			/*moveList.for_each([](pair<int,int> a){

				cout << "(" << a.first << ',' << a.second << ')' << endl;
			});*/
			this->i = 0;



		}
		~gameEnemy(){
			Clean();
			
		}

};


class Game{
	public:
		int FPS = 3;
		Maze maze;
		ALLEGRO_DISPLAY *display;
		ALLEGRO_BITMAP *wall;
		gamePlayer gameplayer;
		gameEnemy gamenemy;
		ALLEGRO_EVENT_QUEUE *event_queue;
		ALLEGRO_TIMER *timer;

		Game(){
			this->display = NULL;
			this->wall = NULL;
			this->event_queue = NULL;
			this->timer = NULL;
		};
		~Game(){
			this->Clean();
		};

		void Clean(){
			al_destroy_bitmap(this->wall);
			al_destroy_display(this->display);
			al_destroy_event_queue(this->event_queue);	
			al_destroy_timer(this->timer);
	
			this->gameplayer.Clean();
			this->gamenemy.Clean();	
			this->maze.Clean();
						
			
		}

		void EnemysMove(){
			if(pow(this->gameplayer.stateX - this->gamenemy.stateX, 2) + pow(this->gameplayer.stateY - this->gamenemy.stateY, 2) < pow(this->gamenemy.rad,2))
				this->gamenemy.moveAnimation();
		}
		void recalculateEnemysMove(){
			this->gamenemy.setmoveList(this->maze,this->gameplayer);
		}
		

		bool init(){

			if(!al_init()) {
				fprintf(stderr, "failed to initialize allegro!\n");
				return false;
			}
			if(!al_install_keyboard()) {
		      fprintf(stderr, "failed to initialize the keyboard!\n");
		      return false;
		   	}
			if(!al_init_image_addon()) {
				return false;
			}
			this->maze.initMaze(mazeSize);
			this->maze.MakeMaze();

			map = this->maze.getMaze();

			this->display = al_create_display(mapsize*30, mapsize*30);
			if(!this->display) {
				fprintf(stderr, "failed to create display!\n");
				return false;
			}


			this->wall = al_load_bitmap("./Resources/roca.bmp");
		 	if(!this->wall) {
				fprintf(stderr, "failed to create wall bitmap!\n");
				al_destroy_display(this->display);

				return false;
			}


			this->gameplayer.BitMap = al_load_bitmap("./Resources/pacman.bmp");
		 	if(!this->gameplayer.BitMap) {
				fprintf(stderr, "failed to create player bitmap!\n");
				al_destroy_display(this->display);
				al_destroy_bitmap(this->wall);
				return false;
			}

			this->gamenemy.BitMap = al_load_bitmap("./Resources/enemigo.bmp");
			this->gamenemy.stateY = maze.enemysCord.first;
			this->gamenemy.stateX = maze.enemysCord.second;
			this->gamenemy.rad = 9;
		 	if(!this->gamenemy.BitMap) {
		 		
				fprintf(stderr, "failed to create enemy bitmap!\n");
				al_destroy_display(this->display);
				al_destroy_bitmap(this->wall);
				this->gameplayer.Clean();
				return false;
			}
			this->event_queue = al_create_event_queue();
			if(!this->event_queue) {
				fprintf(stderr, "failed to create event_queue!\n");
				al_destroy_bitmap(this->wall);
				al_destroy_display(this->display);
				this->gameplayer.Clean();
				this->gamenemy.Clean();
				return false;
			}
		 	this->timer = al_create_timer(1.0/this->FPS);
			if(!this->timer) {
				fprintf(stderr, "failed to create timer!\n");
				return false;
				al_destroy_bitmap(this->wall);
				al_destroy_display(this->display);
				al_destroy_event_queue(this->event_queue);		
				this->gameplayer.Clean();
				this->gamenemy.Clean();
			}
			
		   
			return true;
		}


		void PaintMap(){

			int xStart =0, xEnd = mapsize, yStart=0, yEnd = mapsize;
			pair<int, int> enemyPaint = make_pair((this->gamenemy.stateY+1)*30,(this->gamenemy.stateX+1)*30); 
			pair<int, int> playerPaint = make_pair((this->gameplayer.stateY+1)*30,(this->gameplayer.stateX+1)*30); 


			if(gameplayer.stateY > mapsize/2 ){

				yStart = gameplayer.stateY - mapsize/2;
				yEnd = mapsize + gameplayer.stateY - mapsize/2;
				enemyPaint.first = (this->gamenemy.stateY+1 - yStart)*30;
				playerPaint.first = 30*(mapsize/2+ 1);

			}

			if(gameplayer.stateX > mapsize/2 ){

				xStart = gameplayer.stateX - mapsize/2;
				xEnd = mapsize + gameplayer.stateX - mapsize/2;
				enemyPaint.second = (this->gamenemy.stateX+1 - xStart)*30;
				playerPaint.second = 30*(mapsize/2+ 1);
			}

			if(gameplayer.stateY > 2*mazeSize - mapsize/2  ){
				yStart =  2*mazeSize-mapsize +1 ;
				yEnd = 2*mazeSize+1;
				enemyPaint.first = (this->gamenemy.stateY - yStart+1)*30;
				playerPaint.first = (this->gameplayer.stateY-yStart+1)*30;
			}
			if (gameplayer.stateX > 2*mazeSize-mapsize/2){
				xStart =  2*mazeSize-mapsize +1;
				xEnd = 2*mazeSize+1;
				enemyPaint.second = (this->gamenemy.stateX+1 -xStart )*30;
				playerPaint.second = (this->gameplayer.stateX-xStart + 1)*30;
			}


			for(int i = yStart ; i < yEnd ; i++){
				for (int j = xStart ; j< xEnd  ; ++j){
					
					 if(map[i][j] == 'X' ){
		
						al_draw_bitmap(wall, (j -xStart)*30, (i- yStart)*30, 0);
					}		

				}
			}

			al_draw_bitmap_region(gameplayer.BitMap,0,0, 30,  30, playerPaint.second,  playerPaint.first, 0);

			if(this->gamenemy.stateX + 1 >= xStart and this->gamenemy.stateX + 1 < xEnd and this->gamenemy.stateY + 1 >= yStart  and this->gamenemy.stateY + 1 <yEnd)
				al_draw_bitmap_region(gamenemy.BitMap,0,0, 30,  30,enemyPaint.second ,  enemyPaint.first, 0);

			
		}


};

#endif