INCS=-I~/dist/TMW/include -I/usr/include/SDL

all: helloworld

helloworld: helloworld.o playcontrolpanel.o
	g++ -o helloworld helloworld.o playcontrolpanel.o -lSDL -lSDL_image -lSDL_ttf -lguichan -lguichan_sdl -L~/dist/TMW/lib $(INCS)

helloworld.o: helloworld.cpp
	g++ -c helloworld.cpp $(INCS)

playcontrolpanel.o: playcontrolpanel.cpp playcontrolpanel.h
	g++ -c playcontrolpanel.cpp
