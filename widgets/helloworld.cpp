/* 
igneus 22.1.2009
my first personal experience with guichan
*/

#include <iostream>

#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <guichan/imagefont.hpp>

#include "SDL.h"

#include "playcontrolpanel.h"

SDL_Surface* screen;
SDL_Event event;
gcn::SDLInput* input;
gcn::SDLGraphics* graphics;
gcn::SDLImageLoader* loader;

gcn::Gui* gui;
gcn::Container* top;
gcn::ImageFont* font;
gcn::Label* label;
gcn::Window* window;
gcn::TextField* edit;
gcn::Button* button;

PlayControlPanel* playController;

void init()
{
  int w, h, b;
  w = 640;
  h = 480;
  b = 50;

  SDL_Init(SDL_INIT_VIDEO);
  screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE);
  SDL_EnableUNICODE(1);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  graphics = new gcn::SDLGraphics();
  graphics->setTarget(screen);
  input = new gcn::SDLInput();

  loader = new gcn::SDLImageLoader();
  gcn::Image::setImageLoader(loader);

  font = new gcn::ImageFont("sansserif8.png", " !\"#$%&'()=+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~|");
  gcn::Widget::setGlobalFont(font);

  gui = new gcn::Gui();
  gui->setGraphics(graphics);
  gui->setInput(input);

  top = new gcn::Container();
  top->setDimension(gcn::Rectangle(b, b, w-2*b, h-2*b));
  gui->setTop(top);

  label = new gcn::Label("Hello, (mana)world");
  label->setPosition(50, 80);
  top->add(label);

  window = new gcn::Window("My small window");
  window->setPosition(100,200);
  edit = new gcn::TextField("hi, greg");
  edit->setPosition(20,20);
  edit->setWidth(210);
  window->add(edit);
  button = new gcn::Button("Create!");
  button->adjustSize();
  button->setPosition(20,50);
  window->add(button);
  window->resizeToContent();
  top->add(window);

  playController = new PlayControlPanel();
  playController->setPosition(300,300);
  top->add(playController);
}

void halt()
{
  delete playController;

  delete edit;
  delete button;
  delete window;
  delete label;
  delete font;
  delete top;
  delete gui;

  delete loader;
  delete input;
  delete graphics;

  SDL_Quit();
}

void run()
{
  bool running = true;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN &&
	  event.key.keysym.sym == SDLK_ESCAPE) {
	running = false;
      } else if (event.type == SDL_QUIT) {
	running = false;
      }

      input->pushInput(event);
    }

    gui->logic();
    gui->draw();

    SDL_Flip(screen);
  }
}

int main(int argc, char **argv)
{
  try {
    init();
    run();
    halt();
  } catch (gcn::Exception e) {
    std::cerr << "GuiChan error: '" << e.getMessage() << "'" << std::endl;
  } catch (std::exception e) {
    std::cerr << "Standard error: '" << e.what() << "'" << std::endl;
  } catch (...) {
    std::cerr << "Unknown exception." << std::endl;
  }
}
