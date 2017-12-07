# coloca aqui tu camino hacia aleph
ALEPH = /home/mator/programacion/c++/Ayda/Aleph-w

# Coloca aqui tu camino hacia clang
CXX = clang++

# De aqui en adelante se te aconseja no jurungar!

OPTIMIZATION = -O0

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings \
       -Wno-parentheses 

FLAGS = -DDEBUG -D_GLIBCXX__PTHREADS -g $(OPTIMIZATION) -std=c++14 $(WARN)

INCLUDE = -I. -I $(ALEPH)  -I ./MazeGenerator
Alegro = `pkg-config --cflags --libs allegro-5 allegro_acodec-5 allegro_audio-5  allegro_video-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5`
LIBS = -L $(ALEPH) \
       -lAleph -lc -lm -lgsl -lgslcblas -lgmp -lmpfr -lasprintf -lpthread

MazeOut: MazeOut.cpp
	$(CXX) $(FLAGS) $(INCLUDE) $(Alegro) $@.cpp -o $@ $(LIBS)

clean:
	rm MazeOut

master:
	clear | rm MazeOut | make