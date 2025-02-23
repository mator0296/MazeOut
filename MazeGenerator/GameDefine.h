#ifndef GAMEDEFINE
#define GAMEDEFINE
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_video.h>
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
			if(this->BitMap)
				al_destroy_bitmap(this->BitMap);
			if(this->right)
				al_destroy_bitmap(this->right);
			if(this->left)
				al_destroy_bitmap(this->left);
			if(this->walkLeft)
				al_destroy_bitmap(this->walkLeft);
			if(this->walkRight)
				al_destroy_bitmap(this->walkRight);
			if(this->up)
				al_destroy_bitmap(this->up);

		}
		void setCord(pair<int,int> xy){

			this->stateX = xy.second;
			this->stateY = xy.first;
		}
		bool moveRight(){
			if(this->stateX < (2*mazeSize +1) )
				if(map[this->stateY+1][this->stateX+2] != 'X'){
					this->stateX++;
                    return true;
                }
            return false;
			
		}
		bool moveLeft(){
			if(this->stateX > 0 )
				if( map[this->stateY+1][this->stateX] != 'X'){
					this->stateX--;
                    return true;
                }
            return false;
			
		}
		bool moveDown(){
			if(this->stateY< (2*mazeSize +1) )
				if( map[this->stateY+2][this->stateX+1] != 'X'){
					this->stateY++;
                    return true;
                }
            return false;
			
		}
		bool moveUp(){
			if(this->stateY> 0 )
				if( map[this->stateY][this->stateX+1] != 'X'){
					this->stateY--;
                    return true;
                }
			return false;
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

       
        void changeState(int i){

            if(i == 0){
                this->BitMap = al_load_bitmap("./Resources/PrimerNivel/Niño_Frente.png");
                this->left = al_load_bitmap("./Resources/PrimerNivel/Niño_Izquierda.png");
                this->right = al_load_bitmap("./Resources/PrimerNivel/Niño_Derecha.png");
                this->walkRight = al_load_bitmap("./Resources/PrimerNivel/Niño_CaminandoDer.png");
                this->walkLeft = al_load_bitmap("./Resources/PrimerNivel/Niño_CaminandoIzq.png");
                this->up = al_load_bitmap("./Resources/PrimerNivel/Niño_Espalda.png");
            }else if(i == 1){
                this->BitMap = al_load_bitmap("./Resources/SegundoNivel/Psycho_lvl2_F.png");
                this->left = al_load_bitmap("./Resources/SegundoNivel/Psycho_lvl2_L3.png");
                this->right = al_load_bitmap("./Resources/SegundoNivel/Psycho_lvl2_R3.png");
                this->walkRight = al_load_bitmap("./Resources/SegundoNivel/Psycho_lvl2_R1.png");
                this->walkLeft = al_load_bitmap("./Resources/SegundoNivel/Psycho_lvl2_L1.png.png");
                this->up = al_load_bitmap("./Resources/SegundoNivel/Psycho_lvl2_B.png");

            }else{
                this->BitMap = al_load_bitmap("./Resources/PrimerNivel/Niño_Frente.png");
                this->left = al_load_bitmap("./Resources/PrimerNivel/Niño_Izquierda.png");
                this->right = al_load_bitmap("./Resources/PrimerNivel/Niño_Derecha.png");
                this->walkRight = al_load_bitmap("./Resources/PrimerNivel/Niño_CaminandoDer.png");
                this->walkLeft = al_load_bitmap("./Resources/PrimerNivel/Niño_CaminandoIzq.png");
                this->up = al_load_bitmap("./Resources/PrimerNivel/Niño_Espalda.png");

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
        int Available;
		gameEnemy():gameEntity(NULL), i(0), Available(false), rad(3){}
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
			this->i = 0;
		}
		~gameEnemy(){
			Clean();
			
		}
        void changeState(int i){

            if(i == 0){
                this->BitMap = al_load_bitmap("./Resources/PrimerNivel/Malo1_Frente.png");
                this->left = al_load_bitmap("./Resources/PrimerNivel/Malo1_Izquierda.png");
                this->right = al_load_bitmap("./Resources/PrimerNivel/Malo1_Derecha.png");
                this->walkRight = al_load_bitmap("./Resources/PrimerNivel/Malo1_CaminandoDer.png");
                this->walkLeft = al_load_bitmap("./Resources/PrimerNivel/Malo1_CaminandoIzq.png");
                this->up = al_load_bitmap("./Resources/PrimerNivel/Malo1_Espalda.png");
            }else if(i == 1){
                this->BitMap = al_load_bitmap("./Resources/PrimerNivel/Malo1_Frente.png");
                this->left = al_load_bitmap("./Resources/PrimerNivel/Malo1_Izquierda.png");
                this->right = al_load_bitmap("./Resources/PrimerNivel/Malo1_Derecha.png");
                this->walkRight = al_load_bitmap("./Resources/PrimerNivel/Malo1_CaminandoDer.png");
                this->walkLeft = al_load_bitmap("./Resources/PrimerNivel/Malo1_CaminandoIzq.png");
                this->up = al_load_bitmap("./Resources/PrimerNivel/Malo1_Espalda.png");

            }else{
                this->BitMap = al_load_bitmap("./Resources/PrimerNivel/Malo1_Frente.png");
                this->left = al_load_bitmap("./Resources/PrimerNivel/Malo1_Izquierda.png");
                this->right = al_load_bitmap("./Resources/PrimerNivel/Malo1_Derecha.png");
                this->walkRight = al_load_bitmap("./Resources/PrimerNivel/Malo1_CaminandoDer.png");
                this->walkLeft = al_load_bitmap("./Resources/PrimerNivel/Malo1_CaminandoIzq.png");
                this->up = al_load_bitmap("./Resources/PrimerNivel/Malo1_Espalda.png");

            }
        }

};

class gamePeople: public gameEntity{
    public:
        string dent;
        bool Efect;
        gamePeople():gameEntity(NULL), Efect(false){}
        gamePeople(ALLEGRO_BITMAP *BitMap):gameEntity(BitMap){};
        void moveAnimation(){
            
            this->state  = rand()%4;
            if(this->state== 0)
                if(this->moveUp())
                    return;
            if(this->state== 1)
               if(this->moveDown())
                    return ;
            if(this->state== 2)
                if(this->moveLeft())
                    return;
            if(this->state== 3)
                if(this->moveRight())
                    return;
                     
        }
        void changeState(){

            if(!Efect){
                this->BitMap = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"_F.png").c_str());
                this->left = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"_I.png").c_str());
                this->right = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"_D.png").c_str());
                this->walkRight = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"_DC.png").c_str());
                this->walkLeft = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"_IC.png").c_str());
                this->up = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"_E.png").c_str());
                Efect = true;
            }else{
                this->BitMap = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"O_F.png").c_str());
                this->left = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"O_I.png").c_str());
                this->right = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"O_D.png").c_str());
                this->walkRight = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"O_DC.png").c_str());
                this->walkLeft = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"O_IC.png").c_str());
                this->up = al_load_bitmap(("./Resources/SegundoNivel/"+this->dent+"O_E.png").c_str());
                Efect = false;
            }
        }
      
        ~gamePeople(){
            Clean();
            
        }

};

class Objects{
public:
    ALLEGRO_BITMAP *C[10];
    bool taken[10];
    pair<int, int> ObjectsCord[10];
    int HowLeft;
    int HowLeftMult;
    Objects(){    
        HowLeft = 5; 
        HowLeftMult = 5;     
    }
    void set_BitMaps(){
        for (int i = 1; i < 6; ++i){
            C[i-1] = al_load_bitmap(("./Resources/SegundoNivel/C"+to_string(i)+".png").c_str());
            if(!C[i-1])
                cout << "bitmap object no listo"<<  "./Resources/SegundoNivel/C"+to_string(i)+".png"<< endl;
            taken[i-1] = false;
        }  
        for (int i = 1; i < 6; ++i){
            C[i+4] = al_load_bitmap(("./Resources/SegundoNivel/C"+to_string(i)+".png").c_str());
            if(!C[i+4])
                cout << "bitmap object no listo"<<  "./Resources/SegundoNivel/C"+to_string(i)+".png"<< endl;
            taken[i+4] = false;
        }    
    }
    void setObjectsCord(pair<int,int> cord[]){
        for(int i=0 ;i<10;i++)
            ObjectsCord[i] = cord[i];
    }

    void find_object(){

        HowLeft--;
    }
    void find_objectMult(){

        HowLeft--;
    }

    bool is_ready(){
        return (HowLeft == 0);
    }
    bool is_readyMult(){
        return (HowLeft == 0);
    }
    ~Objects(){
        for (int i = 0; i < 10; ++i)
            al_destroy_bitmap(C[i]);
        
    }

    
};


class Game{
	public:
		int FPS = 2;
		int numEnemys;
		int numPeople,numObjects;
        int PeopleLeft, PeopleLeftMulti;
        bool NeedPeople;
        bool NeedEnemys, NeedObjects;
        Objects gameObject;
		Maze maze;
		ALLEGRO_DISPLAY *display;
		ALLEGRO_BITMAP *wall, *graund, *Menu, *MenuJugar,  *MenuArcade, *MenuMultiplayer, *MenuSalir;
        ALLEGRO_BITMAP *Loss, *win, *Win1 ,*Win2;
		gamePlayer gameplayer, gameMulti;
		gameEnemy *gamenemy;
        gamePeople gamepeople[8];
		ALLEGRO_EVENT_QUEUE *event_queue;
		ALLEGRO_TIMER *timer;
		ALLEGRO_SAMPLE *gotera;
		ALLEGRO_VIDEO *Open;

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
			if(this->wall)
				al_destroy_bitmap(this->wall);
			if(this->display)
				al_destroy_display(this->display);
			if(this->event_queue)
				al_destroy_event_queue(this->event_queue);	
			if(this->timer)
				al_destroy_timer(this->timer);
			if(this->graund)
				al_destroy_bitmap(this->graund);
			if(this->MenuArcade)
				al_destroy_bitmap(this->MenuArcade);
			if(this->MenuSalir)
				al_destroy_bitmap(this->MenuSalir);
			if(this->MenuJugar)
				al_destroy_bitmap(this->MenuJugar);
			if(this->Menu)
				al_destroy_bitmap(this->Menu);
            if(this->Win1)
                al_destroy_bitmap(this->Win1);
            if(this->Win2)
                al_destroy_bitmap(this->Win2);
            if(this->win)
                al_destroy_bitmap(this->win);
            if(this->Loss)
                al_destroy_bitmap(this->Loss);
			if(this->Open)
				al_close_video(this->Open);
			if(this->gotera)
				al_destroy_sample(this->gotera);
			
			delete this->gamenemy;

			
		}

		void EnemysMove(){
			for (int i = 0; i < this->numEnemys; ++i)
				if(gamenemy[i].rad == 0)
					this->gamenemy[i].moveAnimation();
          /*  for (int i =  this->numEnemys; i < 2*this->numEnemys; ++i)
                if(pow(this->gameplayer.stateX - this->gamenemy[i].stateX, 2) + pow(this->gameplayer.stateY - this->gamenemy[i].stateY, 2) < pow(this->gamenemy[i].rad,2))
                    this->gamenemy[i].moveAnimation();*/
		}

        void PeopleMove(bool multiPlayerstate){
              
            for (int i = 0; i < numPeople; ++i)
                    this->gamepeople[i].moveAnimation();

            if(multiPlayerstate)
                for (int i = numPeople; i < 2*numPeople; ++i)
                    this->gamepeople[i].moveAnimation();
                
        }

        void PeopleChange(bool multiPlayerstate){
              
            for (int i = 0; i < numPeople; ++i)
                    if(this->gamepeople[i].stateX == this->gameplayer.stateX and this->gamepeople[i].stateY == this->gameplayer.stateY and this->gamepeople[i].Efect and (this->gameObject.is_ready() || !this->NeedObjects)){
                        this->gamepeople[i].changeState();
                        this->PeopleLeft--;
                    }
            if(multiPlayerstate)        
                 for (int i = numPeople; i < 2*numPeople; ++i)
                    if(this->gamepeople[i].stateX == this->gameMulti.stateX and this->gamepeople[i].stateY == this->gameMulti.stateY and this->gamepeople[i].Efect and (this->gameObject.is_readyMult() || !this->NeedObjects)){
                        this->gamepeople[i].changeState();
                        this->PeopleLeftMulti--;
                    }
        }

        void objectFind(bool multiPlayerstate){
              
            for (int i = 0; i < numObjects/2; ++i)
                    if(this->gameObject.ObjectsCord[i].second == this->gameplayer.stateX and this->gameObject.ObjectsCord[i].first == this->gameplayer.stateY and !this->gameObject.taken[i]){
                        this->gameObject.taken[i] = true;
                        this->gameObject.find_object();
                    }
            if(multiPlayerstate)
                for (int i = numObjects/2; i < numObjects; ++i)
                    if(this->gameObject.ObjectsCord[i].second == this->gameMulti.stateX and this->gameObject.ObjectsCord[i].first == this->gameMulti.stateY and !this->gameObject.taken[i]){
                        this->gameObject.taken[i] = true;
                        this->gameObject.find_objectMult();
                    }
                
        }
		void recalculateEnemysMove(){
          

			for (int i = 0; i < this->numEnemys; ++i)
                if(gamenemy[i].rad == 0)
				    this->gamenemy[i].setmoveList(this->maze,this->gameplayer);
                else if(gamenemy[i].Available)
                        gamenemy[i].rad--;
            

            
		}
		

		bool init(int NumEnemys){
			this->numEnemys = NumEnemys;
			this->numPeople = 4;
            this->numObjects = 10;
            this->NeedObjects = true;
            this->NeedEnemys = false;
            this->NeedPeople = true;
            srand (time(NULL));
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
			if(!al_init_video_addon()){
				fprintf(stderr, "failed to initialize video!\n");
				return false;
			}

			if(!al_init_acodec_addon()){
				fprintf(stderr, "failed to initialize audio codecs!\n");
				return false;
			}

			if (!al_reserve_samples(2)){
				fprintf(stderr, "failed to reserve samples!\n");
				return false;
			}
			
			this->maze.initMaze(mazeSize, numEnemys, numPeople, numObjects);
			this->maze.MakeMaze();


			map = this->maze.getMaze();
			
		   
			this->display = al_create_display(mapsize*resourceSize, mapsize*resourceSize);
			if(!this->display) {
				fprintf(stderr, "failed to create display!\n");
				this->Clean();
				return false;
			}


			this->event_queue = al_create_event_queue();
			if(!this->event_queue) {
				fprintf(stderr, "failed to create event_queue!\n");
				this->Clean();
				
				return false;
			}
		 	this->timer = al_create_timer(1.0/this->FPS);
			if(!this->timer) {
				fprintf(stderr, "failed to create timer!\n");
				this->Clean();
				return false;

			}

			this->Menu = al_load_bitmap("./Resources/Fondos/FondoMenu-01.png");
			if(!this->Menu) {
				fprintf(stderr, "failed to create Menu!\n");
				
				this->Clean();
				return false;
			}

			this->MenuSalir = al_load_bitmap("./Resources/Fondos/FondoMenu-05.png");
			if(!this->MenuSalir) {
				fprintf(stderr, "failed to create MenuSalir!\n");
				
				this->Clean();
				return false;
			}
			this->MenuArcade = al_load_bitmap("./Resources/Fondos/FondoMenu-04.png");
			if(!this->MenuArcade) {
				fprintf(stderr, "failed to create MenuArcade!\n");
				
				this->Clean();
				return false;
			}
			this->MenuJugar = al_load_bitmap("./Resources/Fondos/FondoMenu-02.png");
			if(!this->MenuJugar) {
				fprintf(stderr, "failed to create MenuJugar!\n");
				
				this->Clean();
				return false;
			}
			this->MenuMultiplayer = al_load_bitmap("./Resources/Fondos/FondoMenu-03.png");
			if(!this->MenuMultiplayer) {
				fprintf(stderr, "failed to create MenuMJ!\n");
				
				this->Clean();
				return false;
			}

			gotera = al_load_sample( "./Resources/Gotera.wav" );
			if(!this->gotera) {
					fprintf(stderr, "failed to create Gotera!\n");
					
					this->Clean();
					return false;
			}

            if(!initAssets(1)){
                    fprintf(stderr, "failed to create Assets!\n");
                    
                    this->Clean();
                    return false;
            }

			
			return true;
		}


		void PaintMap(int Move, gamePlayer &ActualPlayerPint ){

			int xStart =0, xEnd = mapsize, yStart=0, yEnd = mapsize;
			pair<int, int> playerPaint = make_pair((ActualPlayerPint.stateY+1)*resourceSize,(ActualPlayerPint.stateX+1)*resourceSize); 
			pair<int, int> enemyPaint[6], objectPaints[5];
            pair<int, int> peoplePaint[4];

			for (int i = 0; i < this->numEnemys; ++i)
				enemyPaint[i] = make_pair((this->gamenemy[i].stateY+1)*resourceSize,(this->gamenemy[i].stateX+1)*resourceSize); 
            for (int i = 0; i < this->numPeople; ++i)
                peoplePaint[i] = make_pair((this->gamepeople[i].stateY+1)*resourceSize,(this->gamepeople[i].stateX+1)*resourceSize); 
            for (int i = 0; i < this->numObjects; ++i)
                objectPaints[i] = make_pair((this->gameObject.ObjectsCord[i].first+1)*resourceSize,(this->gameObject.ObjectsCord[i].second+1)*resourceSize); 

			if(ActualPlayerPint.stateY > mapsize/2 ){

				yStart = ActualPlayerPint.stateY - mapsize/2;
				yEnd = mapsize + ActualPlayerPint.stateY - mapsize/2;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].first = (this->gamenemy[i].stateY+1 - yStart)*resourceSize;
                for (int i = 0; i < this->numPeople; ++i)   
                    peoplePaint[i].first = (this->gamepeople[i].stateY+1 - yStart)*resourceSize;
                for (int i = 0; i < this->numObjects; ++i)   
                    objectPaints[i].first = (this->gameObject.ObjectsCord[i].first+1 - yStart)*resourceSize;
				playerPaint.first = resourceSize*(mapsize/2+ 1);

			}

			if(ActualPlayerPint.stateX > mapsize/2 ){

				xStart = ActualPlayerPint.stateX - mapsize/2;
				xEnd = mapsize + ActualPlayerPint.stateX - mapsize/2;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].second = (this->gamenemy[i].stateX+1 - xStart)*resourceSize;
                for (int i = 0; i < this->numPeople; ++i)   
                    peoplePaint[i].second = (this->gamepeople[i].stateX+1 - xStart)*resourceSize;
                for (int i = 0; i < this->numObjects; ++i)   
                    objectPaints[i].second = (this->gameObject.ObjectsCord[i].second+1 - xStart)*resourceSize;
                
				playerPaint.second = resourceSize*(mapsize/2+ 1);
			}

			if(ActualPlayerPint.stateY > 2*mazeSize - mapsize/2  ){
				yStart =  2*mazeSize-mapsize +1 ;
				yEnd = 2*mazeSize+1;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].first = (this->gamenemy[i].stateY - yStart+1)*resourceSize;
                for (int i = 0; i < this->numPeople; ++i)   
                    peoplePaint[i].first = (this->gamepeople[i].stateY - yStart+1)*resourceSize;
                for (int i = 0; i < this->numObjects; ++i)   
                    objectPaints[i].first = (this->gameObject.ObjectsCord[i].first - yStart+1)*resourceSize;
				playerPaint.first = (ActualPlayerPint.stateY-yStart+1)*resourceSize;
			}
			if (ActualPlayerPint.stateX > 2*mazeSize-mapsize/2){
				xStart =  2*mazeSize-mapsize +1;
				xEnd = 2*mazeSize+1;
				for (int i = 0; i < this->numEnemys; ++i)	
					enemyPaint[i].second = (this->gamenemy[i].stateX+1 -xStart )*resourceSize;
                for (int i = 0; i < this->numPeople; ++i)   
                    peoplePaint[i].second = (this->gamepeople[i].stateX+1 -xStart )*resourceSize;
                for (int i = 0; i < this->numObjects; ++i)   
                    objectPaints[i].second = (this->gameObject.ObjectsCord[i].second+1 -xStart )*resourceSize;
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
            if(NeedEnemys){
                if(ActualPlayerPint.type){
                    for (int i = 0; i < this->numEnemys; ++i){
                        if(this->gamenemy[i].stateX + 1 >= xStart and this->gamenemy[i].stateX + 1 < xEnd and this->gamenemy[i].stateY + 1 >= yStart  and this->gamenemy[i].stateY + 1 <yEnd and gamenemy[i].rad == 0 and gamenemy[i].Available){
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
                }else{
                    for (int i = this->numEnemys; i < 2*this->numEnemys; ++i){
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
            }  
    			
         
            if(NeedPeople){
                if(ActualPlayerPint.type){
                    for (int i = 0; i < this->numPeople; ++i){
                        if(this->gamepeople[i].stateX + 1 >= xStart and this->gamepeople[i].stateX + 1 < xEnd and this->gamepeople[i].stateY + 1 >= yStart  and this->gamepeople[i].stateY + 1 <yEnd){
                            if(gamepeople[i].state == 0)
                                al_draw_bitmap_region(gamepeople[i].BitMap,0,0, resourceSize,  resourceSize, peoplePaint[i].second+ Move,  peoplePaint[i].first , 0);
                            if(gamepeople[i].state == 1)
                                al_draw_bitmap_region(gamepeople[i].up,0,0, resourceSize,  resourceSize, peoplePaint[i].second+ Move,  peoplePaint[i].first , 0);
                            if(gamepeople[i].state == 2)
                                al_draw_bitmap_region(gamepeople[i].left,0,0, resourceSize,  resourceSize, peoplePaint[i].second+ Move,  peoplePaint[i].first , 0);
                            if(gamepeople[i].state == 3)
                                al_draw_bitmap_region(gamepeople[i].right,0,0, resourceSize,  resourceSize, peoplePaint[i].second+ Move,  peoplePaint[i].first , 0);
                        }
                    } 
                }else{
                    for (int i = this->numPeople; i < 2*this->numPeople; ++i){
                        if(this->gamepeople[i].stateX + 1 >= xStart and this->gamepeople[i].stateX + 1 < xEnd and this->gamepeople[i].stateY + 1 >= yStart  and this->gamepeople[i].stateY + 1 <yEnd){
                            if(gamepeople[i].state == 0)
                                al_draw_bitmap_region(gamepeople[i].BitMap,0,0, resourceSize,  resourceSize, peoplePaint[i].second+ Move,  peoplePaint[i].first , 0);
                            if(gamepeople[i].state == 1)
                                al_draw_bitmap_region(gamepeople[i].up,0,0, resourceSize,  resourceSize, peoplePaint[i].second+ Move,  peoplePaint[i].first , 0);
                            if(gamepeople[i].state == 2)
                                al_draw_bitmap_region(gamepeople[i].left,0,0, resourceSize,  resourceSize, peoplePaint[i].second+ Move,  peoplePaint[i].first , 0);
                            if(gamepeople[i].state == 3)
                                al_draw_bitmap_region(gamepeople[i].right,0,0, resourceSize,  resourceSize, peoplePaint[i].second+ Move,  peoplePaint[i].first , 0);
                        }
                    } 
                }
               
            }
                

            if(NeedObjects){
                if(ActualPlayerPint.type){
                    for (int i = 0; i < this->numObjects/2; ++i){
                        if(this->gameObject.ObjectsCord[i].second + 1 >= xStart and this->gameObject.ObjectsCord[i].second + 1 < xEnd and this->gameObject.ObjectsCord[i].first + 1 >= yStart  and this->gameObject.ObjectsCord[i].first + 1 <yEnd){
                                if(!gameObject.taken[i])
                                    al_draw_bitmap_region(gameObject.C[i],0,0, resourceSize,  resourceSize, objectPaints[i].second+ Move,  objectPaints[i].first , 0);
                        }
                    }
                }else{

                     for (int i = this->numObjects/2; i < this->numObjects; ++i){
                        if(this->gameObject.ObjectsCord[i].second + 1 >= xStart and this->gameObject.ObjectsCord[i].second + 1 < xEnd and this->gameObject.ObjectsCord[i].first + 1 >= yStart  and this->gameObject.ObjectsCord[i].first + 1 <yEnd){
                                if(!gameObject.taken[i])
                                    al_draw_bitmap_region(gameObject.C[i],0,0, resourceSize,  resourceSize, objectPaints[i].second+ Move,  objectPaints[i].first , 0);
                        }
                    }
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

		    if(NeedEnemys)  
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
            

			for(int i = 0; i < this->numEnemys; ++i){ 
                if(gamenemy[i].stateX == gameplayer.stateX and  gamenemy[i].stateY == gameplayer.stateY and this->NeedEnemys  and gamenemy[i].rad == 0){
                    return true;
                }else if(gamenemy[i].stateX == gameplayer.stateX and  gamenemy[i].stateY == gameplayer.stateY and this->NeedEnemys  and !gamenemy[i].Available){
                    gamenemy[i].Available = true;
                }
            }

			return false;
		}

		

		int winnigGame(bool multiPlayerstate){
            if(NeedObjects){
                    if(!this->gameObject.is_ready() and !multiPlayerstate)
                        return 0;


            }

            if(NeedPeople){
                if(this->PeopleLeft != 0 and !multiPlayerstate)
                    return 0;

            }

            if(multiPlayerstate){

                if(this->PeopleLeft == 0 and this->gameObject.is_ready())
                   map[ maze.endCord.first+1][maze.endCord.second-1] = ' ';
                if(this->PeopleLeftMulti == 0 and this->gameObject.is_readyMult())
                    map[ maze.endCord.first+1][maze.endCord.second-1] = ' ';
                if(this->gameplayer.stateY == this->maze.endCord.first and this->gameplayer.stateX +1== this->maze.endCord.second-1)
                    return 1;
                if(this->gameMulti.stateY == this->maze.endCord.first and this->gameMulti.stateX +1== this->maze.endCord.second-1)
                    return 2;
                return 0;

            }
            cout << maze.endCord.first << " " << maze.endCord.second -1<< endl;
            cout << this->gameplayer.stateY << " " << this->gameplayer.stateX << endl;
            for(int i = 0; i< (2*mapsize) + 1 ; i++){
                for (int j = 0; j < (2*mapsize) + 1 ; ++j){

                    cout << map[i][j] ;
                }
                cout << endl;
            }
            map[ maze.endCord.first+1][maze.endCord.second-1] = ' ';
			return(this->gameplayer.stateY == this->maze.endCord.first and this->gameplayer.stateX +1== this->maze.endCord.second-1 );
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
                        al_stop_samples();
                        al_clear_to_color(al_map_rgb(0,0,0));
                        al_flip_display();  
						int result = HistoryMode();
						if(result == 0){
							cout << "Gano felicitaciones" << endl;
						}else if(result== 1){
							cout << "Perdio Historia" << endl;
						}else{
							return;
						}
					}else if(ev.mouse.x > 460 and ev.mouse.x < 683  and ev.mouse.y >518 and ev.mouse.y < 560){
                        al_stop_samples();
						int result = multiPlayer();
						if(result == 0){
							cout << "Gano felicitaciones" << endl;
						}else if(result== 1){
							cout << "Perdio Multiplayer" << endl;
						}else{
							return;
						}

					}else if(ev.mouse.x > 460 and ev.mouse.x < 683  and ev.mouse.y >585 and ev.mouse.y < 626){
                        al_stop_samples();
                         al_flip_display();
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
            
			
            for(int i = 0; i < 2; ++i){
                initAssets(i);
                this->playVideo(this->Open);
                int result = this->playGame(0,i);
                if(result == 1){
                    al_draw_bitmap(Loss, 0, 0, 0);
                    al_flip_display();
                    while(true){
                        ALLEGRO_EVENT ev;
                        al_wait_for_event(this->event_queue, &ev);

                        if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                                break;
                        }


                    }
                    return result;
                }else if(result  == 2)
                    return result;
                al_draw_bitmap(win, 0, 0, 0);
                al_flip_display();
                while(true){
                    ALLEGRO_EVENT ev;
                    al_wait_for_event(this->event_queue, &ev);

                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                            break;
                    }


                }


            }

			return 0;

		}

        int playGame(int type, int set){

            if(type == 0){
                if(set == 0)
                    restarGame(mapsize,4,0,0,false,true,false,set);
                else if(set ==1)
                    restarGame(mapsize,0,4,10,true,false,true,set);

            }else if(type == 1){
                restarGame(mapsize,2,4,10,true,false,true,set);
                al_resize_display(this->display,2*al_get_display_width(this->display), al_get_display_height(this->display));
            }else if(type == 2){
                restarGame(mapsize,0,4,10, 1 == rand()%2,1 == rand()%2,1 == rand()%2,rand()%2); 
            }
            al_start_timer(this->timer);
            al_clear_to_color(al_map_rgb(0,0,0));
            this->PaintMap(0, this->gameplayer);
            if(type == 1)
                this->PaintMap(resourceSize * mapsize, this->gameMulti);
            al_flip_display();  
            int times = 0;
            int result;
            while(true){
                ALLEGRO_EVENT ev;
                
                al_wait_for_event(this->event_queue, &ev);
                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    return 2;
                }else if(ev.type == ALLEGRO_EVENT_TIMER) {
                       
                    result = this->winnigGame(type ==1);
                    if(result >0){
                        
                        if(type ==2){
                            restarGame(mapsize+2*times,2,4,10, 1 == rand()%2,1 == rand()%2,1 == rand()%2,rand()%2); 
                            times++;
                        }else if(type == 1){

                            if(result == 1){

                                al_draw_bitmap(Win1, 0, 0, 0);
                                al_flip_display();
                                while(true){
                                    ALLEGRO_EVENT ev;
                                    al_wait_for_event(this->event_queue, &ev);

                                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                                            break;
                                    }


                                }
                               
                            }else{

                                al_draw_bitmap(Win2, 0, 0, 0);
                                al_flip_display();
                                while(true){
                                    ALLEGRO_EVENT ev;
                                    al_wait_for_event(this->event_queue, &ev);

                                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                                            break;
                                    }


                                } 
                            }
                            return 0;
                        }else{
                           return 0; 
                        }
                        
                    }

                    
                    if(this->isEndGame()){
                        return 1;
                    }
                    if(this->NeedEnemys)
                        this->EnemysMove();
                     
                    if(this->NeedPeople){
                        this->PeopleMove(type ==1);
                        PeopleChange(type ==1);
                    }
                    if(this->NeedObjects){
                        objectFind(type == 1);
                    }
                    //this->enemyMovePaint();
                    
                    //al_flip_display();
                    //al_rest(0.05);    
                    this->PaintMap(0, this->gameplayer);
                    if(type == 1)
                        this->PaintMap(resourceSize * mapsize, this->gameMulti);
                    al_flip_display();  
                    result = this->winnigGame(type ==1);
                    if(result >0){
                        
                        if(type ==2){
                            restarGame(mapsize+2*times,2,4,10, 1 == rand()%2,1 == rand()%2,1 == rand()%2,rand()%2); 
                            times++;
                        }else if(type == 1){

                            if(result == 1){

                                al_draw_bitmap(Win1, 0, 0, 0);
                                al_flip_display();
                                while(true){
                                    ALLEGRO_EVENT ev;
                                    al_wait_for_event(this->event_queue, &ev);

                                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                                            break;
                                    }


                                }
                               
                            }else{

                                al_draw_bitmap(Win2, 0, 0, 0);
                                al_flip_display();
                                while(true){
                                    ALLEGRO_EVENT ev;
                                    al_wait_for_event(this->event_queue, &ev);

                                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                                            break;
                                    }


                                } 
                            }
                            return 0;
                        }else{
                           return 0; 
                        }
                        
                    }

                    

                }if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                     
                    result = this->winnigGame(type ==1);
                    if(result >0){
                        
                        if(type ==2){
                            restarGame(mapsize+2*times,2,4,10, 1 == rand()%2,1 == rand()%2,1 == rand()%2,rand()%2); 
                            times++;
                        }else if(type == 1){

                            if(result == 1){

                                al_draw_bitmap(Win1, 0, 0, 0);
                                al_flip_display();
                                while(true){
                                    ALLEGRO_EVENT ev;
                                    al_wait_for_event(this->event_queue, &ev);

                                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                                            break;
                                    }


                                }
                               
                            }else{

                                al_draw_bitmap(Win2, 0, 0, 0);
                                al_flip_display();
                                while(true){
                                    ALLEGRO_EVENT ev;
                                    al_wait_for_event(this->event_queue, &ev);

                                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                                            break;
                                    }


                                } 
                            }
                            return 0;
                        }else{
                           return 0; 
                        }
                        
                    }

                    if(this->isEndGame()){
                        return 1;
                    }
                    this->gameplayer.moveAnimation(ev);
                    if(type == 1)
                        this->gameMulti.moveAnimation(ev);
                     
                    if(this->NeedPeople){
                        PeopleChange(type==1);
                    }
                    if(this->NeedObjects){
                        objectFind(type == 1);
                    }
                  
                    
                    
                    //this->playerMovePaint();
                    //al_flip_display();
                    //al_rest(0.05);    
                    this->PaintMap(0, this->gameplayer);
                    if(type == 1)
                        this->PaintMap(resourceSize * mapsize, this->gameMulti);
                    al_flip_display();  
                    int result = this->winnigGame(type ==1);
                    if(result >0){
                        
                        if(type ==2){
                            restarGame(mapsize+2*times,2,4,10, 1 == rand()%2,1 == rand()%2,1 == rand()%2,rand()%2); 
                            times++;
                        }else if(type == 1){

                            if(result == 1){

                                al_draw_bitmap(Win1, 0, 0, 0);
                                al_flip_display();
                                while(true){
                                    ALLEGRO_EVENT ev;
                                    al_wait_for_event(this->event_queue, &ev);

                                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                                            break;
                                    }


                                }
                               
                            }else{

                                al_draw_bitmap(Win2, 0, 0, 0);
                                al_flip_display();
                                while(true){
                                    ALLEGRO_EVENT ev;
                                    al_wait_for_event(this->event_queue, &ev);

                                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                                            break;
                                    }


                                } 
                            }
                            return 0;
                        }else{
                           return 0; 
                        }
                        
                    }

                    if(this->NeedEnemys)
                        this->recalculateEnemysMove();
                }
            }


        }
        
        void playVideo(ALLEGRO_VIDEO *Video){


            al_start_video(Video, al_get_default_mixer());
            al_resize_display(this->display ,al_get_video_scaled_width(Video), al_get_video_scaled_height(Video));
           
            ALLEGRO_BITMAP * bitmap;
            while(al_is_video_playing(Video)){
                bitmap = al_get_video_frame(Video);

                if(bitmap)
                    al_draw_bitmap(bitmap, 0, 0, 0);
               
                
                al_flip_display();
                al_rest(0.01);
            }
        
            ALLEGRO_BITMAP *press = al_load_bitmap("./Resources/Cortos/press_enter.png");
            al_draw_bitmap(bitmap, 0, 0, 0);
            al_draw_bitmap(press, 0, 0, 0);
            al_flip_display();
            while(true){
                ALLEGRO_EVENT ev;
                al_wait_for_event(this->event_queue, &ev);

                if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                        break;
                }


            }
            al_destroy_bitmap(bitmap);
            al_destroy_bitmap(press);
            al_resize_display(this->display, mapsize*resourceSize, mapsize*resourceSize);

        }

		int  multiPlayer(){
			
			return(playGame(1, rand()%2));

		}

		int  arcade(){
            return(playGame(2, rand()%2));
			
		}

		void restarGame(int mapsize, int numEnemys, int numPeople,int numObjects, bool needPeople, bool NeedEnemys, bool needObjects , int set){

			this->maze.initMaze(mapsize, numEnemys, numPeople, numObjects);
            this->maze.MakeMaze();
			this->gameplayer.setCord(this->maze.playerCord);
            map = this->maze.getMaze();
            this->numEnemys = numEnemys;
            this->numPeople = numPeople;
            this->numObjects = numObjects;
            this->NeedPeople = needPeople;
            this->NeedEnemys = NeedEnemys;
            this->NeedObjects = needObjects;
            this->PeopleLeft = 4;
            this->PeopleLeftMulti = 4;
            if(!initAssets(set))
                cout << "error con assets";
			for(int i =0;i<numEnemys; i++)
				this->gamenemy[i].setCord(maze.enemysCord[i]);
            for(int i =0;i<numPeople; i++)
                this->gamepeople[i].setCord(maze.peopleCord[i]);
		}

        bool initAssets(int set){

            string wallPath,graundPath,openPath, losePath, winPath;

            if(set == 0){
                wallPath = "./Resources/PrimerNivel/Arbusto.png";
                graundPath = "./Resources/PrimerNivel/piso.png";
                openPath = "./Resources/Cortos/open.ogv";
                losePath = "./Resources/Fondos/Lost_lvl1.png";
                winPath = "./Resources/Fondos/Victory_lvl1.png";
            }else if(set==1){
                wallPath = "./Resources/SegundoNivel/wall.png";
                graundPath = "./Resources/SegundoNivel/graund.png";
                openPath = "./Resources/Cortos/Level2.ogv";
                losePath = "./Resources/Fondos/Lost_lvl2.png";
                winPath = "./Resources/Fondos/Victory_lvl2.png";

            }else{
                wallPath = "./Resources/PrimerNivel/Arbusto.png";
                graundPath = "./Resources/PrimerNivel/piso.png";
                openPath = "./Resources/Cortos/open.ogv";
                losePath = "./Resources/Fondos/Lost_lvl1.png";
                winPath = "./Resources/Fondos/Victory_lvl1.png";
            }

            this->wall = al_load_bitmap(wallPath.c_str());
            if(!this->wall) {
                fprintf(stderr, "failed to create wall bitmap!\n");
                this->Clean();

                return false;
            }
            this->graund = al_load_bitmap(graundPath.c_str());
            if(!this->graund) {
                fprintf(stderr, "failed to create graund bitmap!\n");
                this->Clean();
                return false;
            }
            this->Loss = al_load_bitmap(losePath.c_str());
            if(!this->Loss) {
                fprintf(stderr, "failed to create Loss bitmap!\n");
                this->Clean();
                return false;
            }
            this->win = al_load_bitmap(winPath.c_str());
            if(!this->win) {
                fprintf(stderr, "failed to create graund bitmap!\n");
                this->Clean();
                return false;
            }
            this->Win1 = al_load_bitmap("./Resources/Fondos/multiplayer_1.png");
            if(!this->Win1) {
                fprintf(stderr, "failed to create graund bitmap!\n");
                this->Clean();
                return false;
            }
            this->Win2 = al_load_bitmap("./Resources/Fondos/multiplayer_2.png");
            if(!this->Win2) {
                fprintf(stderr, "failed to create graund bitmap!\n");
                this->Clean();
                return false;
            }
            this->Open = al_open_video(openPath.c_str());
            if(!this->Open) {
                fprintf(stderr, "failed to create Video open!\n");
                
                this->Clean();
                return false;
            }          

            this->gameObject.setObjectsCord(this->maze.ObjectsCord);
            this->gameObject.set_BitMaps();
            this->gameplayer.changeState(set);
            this->gameplayer.setCord(this->maze.playerCord);
            if(!this->gameplayer.BitMap) {
                fprintf(stderr, "failed to create player bitmap!\n");
                this->Clean();
                return false;
            }

            this->gameMulti.changeState(set);
            this->gameMulti.type = false;
            this->gameMulti.setCord(this->maze.playerCord);
            if(!this->gameMulti.BitMap) {
                fprintf(stderr, "failed to create multi player bitmap!\n");
                this->Clean();
                return false;
            }

            this->gamenemy = new gameEnemy[2*numEnemys];
            for(int i =0;i<2*numEnemys; i++){
                this->gamenemy[i].changeState(set);
                this->gamenemy[i].setCord(maze.enemysCord[i]);
                this->gamenemy[i].rad = 3;
                if(!this->gamenemy[i].BitMap) {
                    
                    fprintf(stderr, "failed to create enemy bitmap!\n");
                    this->Clean();
                    return false;
                }
            }

            for(int i =0;i<2*numPeople; i++){
                if(i == 0 || i == 4)
                    this->gamepeople[i].dent = "P1";
                if(i == 1 || i == 5)
                    this->gamepeople[i].dent = "P2";
                if(i == 2 || i == 6)
                    this->gamepeople[i].dent = "P3";
                if(i == 3 || i == 7)
                    this->gamepeople[i].dent = "P3";
                if(!this->gamepeople[i].Efect)
                    this->gamepeople[i].changeState();
                this->gamepeople[i].setCord(maze.peopleCord[i]);
                //cout << this->gamepeople[i].stateX << " " <<  this->gamepeople[i].stateY << endl;
                if(!this->gamepeople[i].BitMap) {
                    cout << i << endl;
                    fprintf(stderr, "failed to create people bitmap!\n");
                    this->Clean();
                    return false;
                }
            }


            return true;


        } 


};

#endif