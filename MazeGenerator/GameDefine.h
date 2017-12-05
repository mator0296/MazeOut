#ifndef GAMEDEFINE
#define GAMEDEFINE
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <maze.h>
#include <Define.h>


class gameEntity{
	public:
		ALLEGRO_BITMAP *BitMap, *right, *left, *walkLeft, *walkRight, *up;
		int state;

		int stateX, stateY;
		gameEntity( ALLEGRO_BITMAP *bitmap){
			this->stateY = 0;
			this->stateX = 0;
			this->state = 0;
			this->BitMap = bitmap;
		};
		~gameEntity(){
			this->Clean();	
		};
		void Clean(){
			al_destroy_bitmap(this->BitMap);
			al_destroy_bitmap(this->right);
			al_destroy_bitmap(this->left);
			al_destroy_bitmap(this->walkLeft);
			al_destroy_bitmap(this->walkRight);
			al_destroy_bitmap(this->up);

		}
		void setCord(pair<int,int> xy){

			this->stateX = xy.second;
			this->stateY = xy.first;
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
		bool type;
		gamePlayer():gameEntity(NULL),type(true){}
		gamePlayer( ALLEGRO_BITMAP *player):gameEntity( player){};
		void moveAnimation(ALLEGRO_EVENT  &ev){

			if(type)	
				switch(ev.keyboard.keycode) {

					case ALLEGRO_KEY_UP:
						this->moveUp();
						this->state = 1;
						break;

					case ALLEGRO_KEY_DOWN:
						this->moveDown();
						this->state = 0;
						break;

					case ALLEGRO_KEY_LEFT:
						this->moveLeft();
						this->state = 2;
						break;

					case ALLEGRO_KEY_RIGHT:
						this->moveRight();
						this->state = 3;
						break;
				}
			else
				switch(ev.keyboard.keycode) {
					
					case ALLEGRO_KEY_W:
						this->moveUp();
						this->state = 1;
						break;

					case ALLEGRO_KEY_S:
						this->moveDown();
						this->state = 0;
						break;

					case ALLEGRO_KEY_A:
						this->moveLeft();
						this->state = 2;
						break;

					case ALLEGRO_KEY_D:
						this->moveRight();
						this->state = 3;
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
				if(stateX > cord.second)
					this->state = 2;
				else if(stateX < cord.second)
					this->state = 3;

				if(stateY > cord.first)
					this->state = 1;
				else if(stateY < cord.first)
					this->state = 0;
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
		int numEnemys;
		int numPeople;
		Maze maze;
		ALLEGRO_DISPLAY *display;
		ALLEGRO_BITMAP *wall, *graund, *Menu, *MenuJugar,  *MenuArcade, *MenuMultiplayer, *MenuSalir;
		gamePlayer gameplayer, gameMulti;
		gameEnemy *gamenemy;
		ALLEGRO_EVENT_QUEUE *event_queue;
		ALLEGRO_TIMER *timer;
		 ALLEGRO_SAMPLE *gotera;

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
			al_destroy_bitmap(this->graund);
			al_destroy_bitmap(this->MenuArcade);
			al_destroy_bitmap(this->MenuSalir);
			al_destroy_bitmap(this->MenuJugar);
			al_destroy_bitmap(this->Menu);
			
			this->gameplayer.Clean();
			for (int i = 0; i < this->numEnemys; ++i)
				this->gamenemy[i].Clean();	
			delete this->gamenemy;
			this->maze.Clean();	
		}

		void EnemysMove(){
			for (int i = 0; i < this->numEnemys; ++i)
				if(pow(this->gameplayer.stateX - this->gamenemy[i].stateX, 2) + pow(this->gameplayer.stateY - this->gamenemy[i].stateY, 2) < pow(this->gamenemy[i].rad,2))
					this->gamenemy[i].moveAnimation();
		}
		void recalculateEnemysMove(){
			for (int i = 0; i < this->numEnemys; ++i)
				this->gamenemy[i].setmoveList(this->maze,this->gameplayer);
		}
		

		bool init(int NumEnemys, int NumPeople){
			this->numEnemys = NumEnemys;
			this->numPeople = NumPeople;
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
			if(!al_install_mouse()) {
		      fprintf(stderr, "failed to initialize the mouse!\n");
		      return false;
		   }

		    if(!al_install_audio()){
				fprintf(stderr, "failed to initialize audio!\n");
				return false;
			}

			if(!al_init_acodec_addon()){
				fprintf(stderr, "failed to initialize audio codecs!\n");
				return false;
			}

			if (!al_reserve_samples(1)){
				fprintf(stderr, "failed to reserve samples!\n");
				return false;
			}
			
			this->maze.initMaze(mazeSize, numEnemys);
			this->maze.MakeMaze();


			map = this->maze.getMaze();

			this->display = al_create_display(mapsize*resourceSize, mapsize*resourceSize);
			if(!this->display) {
				fprintf(stderr, "failed to create display!\n");
				return false;
			}


			this->wall = al_load_bitmap("./Resources/PrimerNivel/Arbusto.png");
		 	if(!this->wall) {
				fprintf(stderr, "failed to create wall bitmap!\n");
				al_destroy_display(this->display);

				return false;
			}
			this->graund = al_load_bitmap("./Resources/PrimerNivel/piso.png");
		 	if(!this->graund) {
				fprintf(stderr, "failed to create graund bitmap!\n");
				al_destroy_display(this->display);
				al_destroy_bitmap(this->wall);
				return false;
			}


			this->gameplayer.BitMap = al_load_bitmap("./Resources/PrimerNivel/Niño_Frente.png");
			this->gameplayer.left = al_load_bitmap("./Resources/PrimerNivel/Niño_Izquierda.png");
			this->gameplayer.right = al_load_bitmap("./Resources/PrimerNivel/Niño_Derecha.png");
			this->gameplayer.walkRight = al_load_bitmap("./Resources/PrimerNivel/Niño_CaminandoDer.png");
			this->gameplayer.walkLeft = al_load_bitmap("./Resources/PrimerNivel/Niño_CaminandoIzq.png");
			this->gameplayer.up = al_load_bitmap("./Resources/PrimerNivel/Niño_Espalda.png");

			this->gameplayer.setCord(this->maze.playerCord);
		 	if(!this->gameplayer.BitMap) {
				fprintf(stderr, "failed to create player bitmap!\n");
				al_destroy_display(this->display);
				al_destroy_bitmap(this->wall);
				al_destroy_bitmap(this->graund);
				return false;
			}

			this->gameMulti.BitMap = al_load_bitmap("./Resources/PrimerNivel/Niño_Frente.png");
			this->gameMulti.left = al_load_bitmap("./Resources/PrimerNivel/Niño_Izquierda.png");
			this->gameMulti.right = al_load_bitmap("./Resources/PrimerNivel/Niño_Derecha.png");
			this->gameMulti.walkRight = al_load_bitmap("./Resources/PrimerNivel/Niño_CaminandoDer.png");
			this->gameMulti.walkLeft = al_load_bitmap("./Resources/PrimerNivel/Niño_CaminandoIzq.png");
			this->gameMulti.up = al_load_bitmap("./Resources/PrimerNivel/Niño_Espalda.png");
			this->gameMulti.type = false;

			this->gameMulti.setCord(this->maze.playerCord);
		 	if(!this->gameMulti.BitMap) {
				fprintf(stderr, "failed to create player bitmap!\n");
				al_destroy_display(this->display);
				al_destroy_bitmap(this->wall);
				al_destroy_bitmap(this->graund);
				return false;
			}

			this->gamenemy = new gameEnemy[NumEnemys];
			for(int i =0;i<NumEnemys; i++){
				this->gamenemy[i].BitMap = al_load_bitmap("./Resources/PrimerNivel/Malo1_Frente.png");
				this->gamenemy[i].left = al_load_bitmap("./Resources/PrimerNivel/Malo1_Izquierda.png");
				this->gamenemy[i].right = al_load_bitmap("./Resources/PrimerNivel/Malo1_Derecha.png");
				this->gamenemy[i].walkRight = al_load_bitmap("./Resources/PrimerNivel/Malo1_CaminandoDer.png");
				this->gamenemy[i].walkLeft = al_load_bitmap("./Resources/PrimerNivel/Malo1_CaminandoIzq.png");
				this->gamenemy[i].up = al_load_bitmap("./Resources/PrimerNivel/Malo1_Espalda.png");

				this->gamenemy[i].setCord(maze.enemysCord[i]);
				this->gamenemy[i].rad = 9;
			 	if(!this->gamenemy[i].BitMap) {
			 		
					fprintf(stderr, "failed to create enemy bitmap!\n");
					al_destroy_display(this->display);
					al_destroy_bitmap(this->wall);
					al_destroy_bitmap(this->graund);
					this->gameplayer.Clean();
					return false;
				}
			}
			this->event_queue = al_create_event_queue();
			if(!this->event_queue) {
				fprintf(stderr, "failed to create event_queue!\n");
				al_destroy_bitmap(this->wall);
				al_destroy_bitmap(this->graund);
				al_destroy_display(this->display);
				this->gameplayer.Clean();
				for (int i = 0; i < this->numEnemys; ++i)
				{
					this->gamenemy[i].Clean();
				}
				
				return false;
			}
		 	this->timer = al_create_timer(1.0/this->FPS);
			if(!this->timer) {
				fprintf(stderr, "failed to create timer!\n");
								al_destroy_bitmap(this->wall);
				al_destroy_display(this->display);
				al_destroy_bitmap(this->graund);
				al_destroy_event_queue(this->event_queue);		
				this->gameplayer.Clean();
				for (int i = 0; i < this->numEnemys; ++i)
				{
					this->gamenemy[i].Clean();
				}
				return false;

			}

			this->Menu = al_load_bitmap("./Resources/Fondos/FondoMenu-01.png");
			if(!this->Menu) {
				fprintf(stderr, "failed to create Menu!\n");
				
				al_destroy_bitmap(this->wall);
				al_destroy_display(this->display);
				al_destroy_bitmap(this->graund);
				al_destroy_event_queue(this->event_queue);		
				this->gameplayer.Clean();
				for (int i = 0; i < this->numEnemys; ++i)
				{
					this->gamenemy[i].Clean();
				}
				al_destroy_timer(timer);
				return false;
			}

			this->MenuSalir = al_load_bitmap("./Resources/Fondos/FondoMenu-05.png");
			if(!this->MenuSalir) {
				fprintf(stderr, "failed to create MenuSalir!\n");
				
				al_destroy_bitmap(this->wall);
				al_destroy_display(this->display);
				al_destroy_bitmap(this->graund);
				al_destroy_bitmap(this->Menu);
				al_destroy_event_queue(this->event_queue);		
				this->gameplayer.Clean();
				for (int i = 0; i < this->numEnemys; ++i)
				{
					this->gamenemy[i].Clean();
				}
				al_destroy_timer(timer);
				return false;
			}
			this->MenuArcade = al_load_bitmap("./Resources/Fondos/FondoMenu-04.png");
			if(!this->MenuArcade) {
				fprintf(stderr, "failed to create MenuArcade!\n");
				
				al_destroy_bitmap(this->wall);
				al_destroy_display(this->display);
				al_destroy_bitmap(this->graund);
				al_destroy_bitmap(this->Menu);
				al_destroy_bitmap(this->MenuSalir);
				al_destroy_event_queue(this->event_queue);		
				this->gameplayer.Clean();
				for (int i = 0; i < this->numEnemys; ++i)
				{
					this->gamenemy[i].Clean();
				}
				al_destroy_timer(timer);
				return false;
			}
			this->MenuJugar = al_load_bitmap("./Resources/Fondos/FondoMenu-02.png");
			if(!this->MenuJugar) {
				fprintf(stderr, "failed to create MenuJugar!\n");
				
				al_destroy_bitmap(this->wall);
				al_destroy_display(this->display);
				al_destroy_bitmap(this->Menu);
				al_destroy_bitmap(this->MenuSalir);
				al_destroy_bitmap(this->MenuArcade);
				al_destroy_bitmap(this->graund);
				al_destroy_event_queue(this->event_queue);		
				this->gameplayer.Clean();
				for (int i = 0; i < this->numEnemys; ++i)
				{
					this->gamenemy[i].Clean();
				}
				al_destroy_timer(timer);
				return false;
			}
			this->MenuMultiplayer = al_load_bitmap("./Resources/Fondos/FondoMenu-03.png");
			if(!this->MenuMultiplayer) {
				fprintf(stderr, "failed to create MenuMJ!\n");
				
				al_destroy_bitmap(this->wall);
				al_destroy_display(this->display);
				al_destroy_bitmap(this->graund);
				al_destroy_bitmap(this->MenuArcade);
				al_destroy_bitmap(this->MenuSalir);
				al_destroy_bitmap(this->MenuJugar);
				al_destroy_bitmap(this->Menu);
				al_destroy_event_queue(this->event_queue);		
				this->gameplayer.Clean();
				for (int i = 0; i < this->numEnemys; ++i)
				{
					this->gamenemy[i].Clean();
				}
				al_destroy_timer(timer);
				return false;
			}

			 gotera = al_load_sample( "./Resources/Gotera.wav" );
			 if(!this->gotera) {
					fprintf(stderr, "failed to create Gotera!\n");
					
					al_destroy_bitmap(this->wall);
					al_destroy_display(this->display);
					al_destroy_bitmap(this->graund);
					al_destroy_bitmap(this->MenuArcade);
					al_destroy_bitmap(this->MenuSalir);
					al_destroy_bitmap(this->MenuJugar);
					al_destroy_bitmap(this->MenuMultiplayer);
					al_destroy_bitmap(this->Menu);
					al_destroy_event_queue(this->event_queue);		
					this->gameplayer.Clean();
					for (int i = 0; i < this->numEnemys; ++i)
					{
						this->gamenemy[i].Clean();
					}
					al_destroy_timer(timer);
					return false;
				}
		   
			return true;
		}


		void PaintMap(int Move, gamePlayer &ActualPlayerPint ){

			int xStart =0, xEnd = mapsize, yStart=0, yEnd = mapsize;
			pair<int, int> playerPaint = make_pair((ActualPlayerPint.stateY+1)*resourceSize,(ActualPlayerPint.stateX+1)*resourceSize); 
			pair<int, int> enemyPaint[6];

			for (int i = 0; i < this->numEnemys; ++i)
				enemyPaint[i] = make_pair((this->gamenemy[i].stateY+1)*resourceSize,(this->gamenemy[i].stateX+1)*resourceSize); 

			if(ActualPlayerPint.stateY > mapsize/2 ){

				yStart = ActualPlayerPint.stateY - mapsize/2;
				yEnd = mapsize + ActualPlayerPint.stateY - mapsize/2;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].first = (this->gamenemy[i].stateY+1 - yStart)*resourceSize;
				playerPaint.first = resourceSize*(mapsize/2+ 1);

			}

			if(ActualPlayerPint.stateX > mapsize/2 ){

				xStart = ActualPlayerPint.stateX - mapsize/2;
				xEnd = mapsize + ActualPlayerPint.stateX - mapsize/2;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].second = (this->gamenemy[i].stateX+1 - xStart)*resourceSize;
				playerPaint.second = resourceSize*(mapsize/2+ 1);
			}

			if(ActualPlayerPint.stateY > 2*mazeSize - mapsize/2  ){
				yStart =  2*mazeSize-mapsize +1 ;
				yEnd = 2*mazeSize+1;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].first = (this->gamenemy[i].stateY - yStart+1)*resourceSize;
				playerPaint.first = (ActualPlayerPint.stateY-yStart+1)*resourceSize;
			}
			if (ActualPlayerPint.stateX > 2*mazeSize-mapsize/2){
				xStart =  2*mazeSize-mapsize +1;
				xEnd = 2*mazeSize+1;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].second = (this->gamenemy[i].stateX+1 -xStart )*resourceSize;
				playerPaint.second = (ActualPlayerPint.stateX-xStart + 1)*resourceSize;
			}


			


			for(int i = yStart ; i < yEnd ; i++){
				for (int j = xStart ; j< xEnd  ; ++j){
					
					 if(map[i][j] == 'X' ){
						al_draw_bitmap(this->graund, (j -xStart)*resourceSize + Move, (i- yStart)*resourceSize , 0);
						al_draw_bitmap(wall, (j -xStart)*resourceSize+Move, (i- yStart)*resourceSize, 0);
					}else{
						al_draw_bitmap(this->graund, (j -xStart)*resourceSize+Move, (i- yStart)*resourceSize, 0);
					}

				}
			}

			if(ActualPlayerPint.state == 0)
				al_draw_bitmap_region(ActualPlayerPint.BitMap,0,0, resourceSize,  resourceSize, playerPaint.second+ Move,  playerPaint.first , 0);
			if(ActualPlayerPint.state == 1)
				al_draw_bitmap_region(ActualPlayerPint.up,0,0, resourceSize,  resourceSize, playerPaint.second+ Move,  playerPaint.first , 0);
			if(ActualPlayerPint.state == 2)
				al_draw_bitmap_region(ActualPlayerPint.left,0,0, resourceSize,  resourceSize, playerPaint.second+ Move,  playerPaint.first , 0);
			if(ActualPlayerPint.state == 3)
				al_draw_bitmap_region(ActualPlayerPint.right,0,0, resourceSize,  resourceSize, playerPaint.second+ Move,  playerPaint.first , 0);

			for (int i = 0; i < this->numEnemys; ++i){
				if(this->gamenemy[i].stateX + 1 >= xStart and this->gamenemy[i].stateX + 1 < xEnd and this->gamenemy[i].stateY + 1 >= yStart  and this->gamenemy[i].stateY + 1 <yEnd){
					if(gamenemy[i].state == 0)
						al_draw_bitmap_region(gamenemy[i].BitMap,0,0, resourceSize,  resourceSize, enemyPaint[i].second+ Move,  enemyPaint[i].first , 0);
					if(gamenemy[i].state == 1)
						al_draw_bitmap_region(gamenemy[i].up,0,0, resourceSize,  resourceSize, enemyPaint[i].second+ Move,  enemyPaint[i].first , 0);
					if(gamenemy[i].state == 2)
						al_draw_bitmap_region(gamenemy[i].left,0,0, resourceSize,  resourceSize, enemyPaint[i].second+ Move,  enemyPaint[i].first , 0);
					if(gamenemy[i].state == 3)
						al_draw_bitmap_region(gamenemy[i].right,0,0, resourceSize,  resourceSize, enemyPaint[i].second+ Move,  enemyPaint[i].first , 0);
				}
			}	
		}

		void enemyMovePaint(){
			int xStart =0, xEnd = mapsize, yStart=0, yEnd = mapsize;
			pair<int, int> playerPaint = make_pair((this->gameplayer.stateY+1)*resourceSize,(this->gameplayer.stateX+1)*resourceSize); 
			pair<int, int> enemyPaint[6];

			for (int i = 0; i < this->numEnemys; ++i)
				enemyPaint[i] = make_pair((this->gamenemy[i].stateY+1)*resourceSize,(this->gamenemy[i].stateX+1)*resourceSize); 

			if(gameplayer.stateY > mapsize/2 ){

				yStart = gameplayer.stateY - mapsize/2;
				yEnd = mapsize + gameplayer.stateY - mapsize/2;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].first = (this->gamenemy[i].stateY+1 - yStart)*resourceSize;
				playerPaint.first = resourceSize*(mapsize/2+ 1);

			}

			if(gameplayer.stateX > mapsize/2 ){

				xStart = gameplayer.stateX - mapsize/2;
				xEnd = mapsize + gameplayer.stateX - mapsize/2;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].second = (this->gamenemy[i].stateX+1 - xStart)*resourceSize;
				playerPaint.second = resourceSize*(mapsize/2+ 1);
			}

			if(gameplayer.stateY > 2*mazeSize - mapsize/2  ){
				yStart =  2*mazeSize-mapsize +1 ;
				yEnd = 2*mazeSize+1;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].first = (this->gamenemy[i].stateY - yStart+1)*resourceSize;
				playerPaint.first = (this->gameplayer.stateY-yStart+1)*resourceSize;
			}
			if (gameplayer.stateX > 2*mazeSize-mapsize/2){
				xStart =  2*mazeSize-mapsize +1;
				xEnd = 2*mazeSize+1;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].second = (this->gamenemy[i].stateX+1 -xStart )*resourceSize;
				playerPaint.second = (this->gameplayer.stateX-xStart + 1)*resourceSize;
			}


			for(int i = yStart ; i < yEnd ; i++){
				for (int j = xStart ; j< xEnd  ; ++j){
					
					 if(map[i][j] == 'X' ){
						al_draw_bitmap(this->graund, (j -xStart)*resourceSize , (i- yStart)*resourceSize, 0);
						al_draw_bitmap(wall, (j -xStart)*resourceSize, (i- yStart)*resourceSize, 0);
					}else{
						al_draw_bitmap(this->graund, (j -xStart)*resourceSize, (i- yStart)*resourceSize, 0);
					}

				}
			}
			if(gameplayer.state == 0)
				al_draw_bitmap_region(gameplayer.BitMap,0,0, resourceSize,  resourceSize, playerPaint.second,  playerPaint.first, 0);
			if(gameplayer.state == 1)
				al_draw_bitmap_region(gameplayer.up,0,0, resourceSize,  resourceSize, playerPaint.second,  playerPaint.first, 0);
			if(gameplayer.state == 2)
				al_draw_bitmap_region(gameplayer.left,0,0, resourceSize,  resourceSize, playerPaint.second,  playerPaint.first, 0);
			if(gameplayer.state == 3)
				al_draw_bitmap_region(gameplayer.right,0,0, resourceSize,  resourceSize, playerPaint.second,  playerPaint.first, 0);

		
			for (int i = 0; i < this->numEnemys; ++i){
				if(this->gamenemy[i].stateX + 1 >= xStart and this->gamenemy[i].stateX + 1 < xEnd and this->gamenemy[i].stateY + 1 >= yStart  and this->gamenemy[i].stateY + 1 <yEnd){
					if(gamenemy[i].state == 0)
						al_draw_bitmap_region(gamenemy[i].BitMap,0,0, resourceSize,  resourceSize, enemyPaint[i].second,  enemyPaint[i].first-resourceSize/2, 0);
					if(gamenemy[i].state == 1)
						al_draw_bitmap_region(gamenemy[i].up,0,0, resourceSize,  resourceSize, enemyPaint[i].second,  enemyPaint[i].first+resourceSize/2, 0);
					if(gamenemy[i].state == 2)
						al_draw_bitmap_region(gamenemy[i].walkLeft,0,0, resourceSize,  resourceSize, enemyPaint[i].second+resourceSize/2,  enemyPaint[i].first, 0);
					if(gamenemy[i].state == 3)
						al_draw_bitmap_region(gamenemy[i].walkRight,0,0, resourceSize,  resourceSize, enemyPaint[i].second-resourceSize/2,  enemyPaint[i].first, 0);
				}
			}	
	
		}

		void playerMovePaint(){
			int xStart =0, xEnd = mapsize, yStart=0, yEnd = mapsize;
			pair<int, int> playerPaint = make_pair((this->gameplayer.stateY+1)*resourceSize,(this->gameplayer.stateX+1)*resourceSize); 
			pair<int, int> enemyPaint[6];

			for (int i = 0; i < this->numEnemys; ++i)
				enemyPaint[i] = make_pair((this->gamenemy[i].stateY+1)*resourceSize,(this->gamenemy[i].stateX+1)*resourceSize); 

			if(gameplayer.stateY > mapsize/2 ){

				yStart = gameplayer.stateY - mapsize/2;
				yEnd = mapsize + gameplayer.stateY - mapsize/2;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].first = (this->gamenemy[i].stateY+1 - yStart)*resourceSize;
				playerPaint.first = resourceSize*(mapsize/2+ 1);

			}

			if(gameplayer.stateX > mapsize/2 ){

				xStart = gameplayer.stateX - mapsize/2;
				xEnd = mapsize + gameplayer.stateX - mapsize/2;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].second = (this->gamenemy[i].stateX+1 - xStart)*resourceSize;
				playerPaint.second = resourceSize*(mapsize/2+ 1);
			}

			if(gameplayer.stateY > 2*mazeSize - mapsize/2  ){
				yStart =  2*mazeSize-mapsize +1 ;
				yEnd = 2*mazeSize+1;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].first = (this->gamenemy[i].stateY - yStart+1)*resourceSize;
				playerPaint.first = (this->gameplayer.stateY-yStart+1)*resourceSize;
			}
			if (gameplayer.stateX > 2*mazeSize-mapsize/2){
				xStart =  2*mazeSize-mapsize +1;
				xEnd = 2*mazeSize+1;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].second = (this->gamenemy[i].stateX+1 -xStart )*resourceSize;
				playerPaint.second = (this->gameplayer.stateX-xStart + 1)*resourceSize;
			}

			for(int i = yStart ; i < yEnd ; i++){
				for (int j = xStart ; j< xEnd  ; ++j){
					
					 if(map[i][j] == 'X' ){
						al_draw_bitmap(this->graund, (j -xStart)*resourceSize , (i- yStart)*resourceSize, 0);
						al_draw_bitmap(wall, (j -xStart)*resourceSize, (i- yStart)*resourceSize, 0);
					}else{
						al_draw_bitmap(this->graund, (j -xStart)*resourceSize, (i- yStart)*resourceSize, 0);
					}

				}
			}
			if(gameplayer.state == 0)
				al_draw_bitmap_region(gameplayer.BitMap,0,0, resourceSize,  resourceSize, playerPaint.second,  playerPaint.first-resourceSize/2, 0);
			if(gameplayer.state == 1)
				al_draw_bitmap_region(gameplayer.up,0,0, resourceSize,  resourceSize, playerPaint.second,  playerPaint.first+resourceSize/2, 0);
			if(gameplayer.state == 2)
				al_draw_bitmap_region(gameplayer.walkLeft,0,0, resourceSize,  resourceSize, playerPaint.second+resourceSize/2,  playerPaint.first, 0);
			if(gameplayer.state == 3)
				al_draw_bitmap_region(gameplayer.walkRight,0,0, resourceSize,  resourceSize, playerPaint.second-resourceSize/2,  playerPaint.first, 0);


			for (int i = 0; i < this->numEnemys; ++i){
				if(this->gamenemy[i].stateX + 1 >= xStart and this->gamenemy[i].stateX + 1 < xEnd and this->gamenemy[i].stateY + 1 >= yStart  and this->gamenemy[i].stateY + 1 <yEnd){
					if(gamenemy[i].state == 0)
						al_draw_bitmap_region(gamenemy[i].BitMap,0,0, resourceSize,  resourceSize, enemyPaint[i].second,  enemyPaint[i].first, 0);
					if(gamenemy[i].state == 1)
						al_draw_bitmap_region(gamenemy[i].up,0,0, resourceSize,  resourceSize, enemyPaint[i].second,  enemyPaint[i].first, 0);
					if(gamenemy[i].state == 2)
						al_draw_bitmap_region(gamenemy[i].left,0,0, resourceSize,  resourceSize, enemyPaint[i].second,  enemyPaint[i].first, 0);
					if(gamenemy[i].state == 3)
						al_draw_bitmap_region(gamenemy[i].right,0,0, resourceSize,  resourceSize, enemyPaint[i].second,  enemyPaint[i].first, 0);
				}
			}	
		}

		bool isEndGame(){

			for (int i = 0; i < this->numEnemys; ++i)
				if(gamenemy[i].stateX == gameplayer.stateX and  gamenemy[i].stateY == gameplayer.stateY ){
					return true;
				}

			return false;
		}

		

		bool winnigGame(){
			return(this->gameplayer.stateY == this->maze.endCord.first and this->gameplayer.stateX == this->maze.endCord.second );
		}

		void controleMenu(){
			bool doexit = false;
			al_play_sample(this->gotera, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
			al_draw_bitmap(this->Menu, 0, 0, 0);
			al_flip_display();	
			while(!doexit){
				ALLEGRO_EVENT ev;
				
				
				al_wait_for_event(this->event_queue, &ev);




				if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					return;
				}
				else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
					if(ev.mouse.x > 451 and ev.mouse.x < 681  and ev.mouse.y >455 and ev.mouse.y < 495){
						int result = HistoryMode();
						if(result == 0){
							cout << "Gano felicitaciones" << endl;
						}else if(result== 1){
							cout << "Perdio Arcade" << endl;
						}else{
							return;
						}
					}else if(ev.mouse.x > 460 and ev.mouse.x < 683  and ev.mouse.y >518 and ev.mouse.y < 560){
						int result = multiPlayer();
						if(result == 0){
							cout << "Gano felicitaciones" << endl;
						}else if(result== 1){
							cout << "Perdio Arcade" << endl;
						}else{
							return;
						}

					}else if(ev.mouse.x > 460 and ev.mouse.x < 683  and ev.mouse.y >585 and ev.mouse.y < 626){
						int result = arcade();
						if(result == 0){
							cout << "Gano felicitaciones" << endl;
						}else if(result== 1){
							cout << "Perdio Arcade" << endl;
						}else{
							return;
						}
					}else if(ev.mouse.x > 456 and ev.mouse.x < 680  and ev.mouse.y >649 and ev.mouse.y < 681){
						return;
					}
					
					

				}
				else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {

				

					if(ev.mouse.x > 451 and ev.mouse.x < 681  and ev.mouse.y >455 and ev.mouse.y < 495)
						al_draw_bitmap(this->MenuJugar, 0, 0, 0);

					else if(ev.mouse.x > 460 and ev.mouse.x < 683  and ev.mouse.y >518 and ev.mouse.y < 560)
						al_draw_bitmap(this->MenuMultiplayer, 0, 0, 0);

					else if(ev.mouse.x > 460 and ev.mouse.x < 683  and ev.mouse.y >585 and ev.mouse.y < 626)
						al_draw_bitmap(this->MenuArcade, 0, 0, 0);

					else if(ev.mouse.x > 456 and ev.mouse.x < 680  and ev.mouse.y >649 and ev.mouse.y < 681)
						al_draw_bitmap(this->MenuSalir, 0, 0, 0);

					else
						al_draw_bitmap(this->Menu, 0, 0, 0);
						
					al_flip_display();	
				}
				

		      

			}

			al_stop_samples();
		}
		int  HistoryMode(){


		}

		int  multiPlayer(){
			al_resize_display(this->display,2*al_get_display_width(this->display), al_get_display_height(this->display));
			al_clear_to_color(al_map_rgb(0,0,0));
			this->PaintMap(0, this->gameplayer);
			this->PaintMap(resourceSize * mapsize, this->gameMulti);
			al_flip_display();	

			//
			while(true){
				ALLEGRO_EVENT ev;
				
				al_wait_for_event(this->event_queue, &ev);
				if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					return 2;
				}else if(ev.type == ALLEGRO_EVENT_TIMER) {
					
					if(this->winnigGame()){
						restarGame();
					}

					if(this->isEndGame()){
						return 1;
					}
					this->EnemysMove();

				
					//this->enemyMovePaint();
					
					//al_flip_display();
					//al_rest(0.05);	
					this->PaintMap(0, this->gameplayer);
					this->PaintMap(resourceSize * mapsize, this->gameMulti);
					al_flip_display();	
					

				}if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {

					this->gameplayer.moveAnimation(ev);
					this->gameMulti.moveAnimation(ev);
					
					//this->playerMovePaint();
					//al_flip_display();
					//al_rest(0.05);	
					this->PaintMap(0, this->gameplayer);
					this->PaintMap(resourceSize * mapsize, this->gameMulti);
					al_flip_display();	
						
					this->recalculateEnemysMove();
				}
			}

		}

		int  arcade(){

			al_start_timer(this->timer);
			al_clear_to_color(al_map_rgb(0,0,0));
			this->PaintMap(0, this->gameplayer);
			al_flip_display();	

			while(true){
				ALLEGRO_EVENT ev;
				
				al_wait_for_event(this->event_queue, &ev);
				if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					return 2;
				}else if(ev.type == ALLEGRO_EVENT_TIMER) {
					
					if(this->winnigGame()){
						restarGame();
					}

					if(this->isEndGame()){
						return 1;
					}
					this->EnemysMove();

				
					//this->enemyMovePaint();
					
					al_flip_display();
					//al_rest(0.05);	
					this->PaintMap(0, this->gameplayer);
					al_flip_display();	
					

				}if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {

					this->gameplayer.moveAnimation(ev);
					
					//this->playerMovePaint();
					al_flip_display();
					//al_rest(0.05);	
					this->PaintMap(0, this->gameplayer);
					al_flip_display();	
						
					this->recalculateEnemysMove();
				}
			}
		}

		void restarGame(){

			this->maze.restarMaze();
			this->gameplayer.setCord(this->maze.playerCord);

			for(int i =0;i<numEnemys; i++)
				this->gamenemy[i].setCord(maze.enemysCord[i]);
		}


};

#endif