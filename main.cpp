/*
particled

TMW particle effect viewer

A lot of code copied from various parts of TMW client.
*/

#include <iostream>
#include <vector>

#include <physfs.h>

#include <SDL.h>
#include <SDL_image.h>

#include <libxml/parser.h>

#include <getopt.h>

#include "main.h"
#include "log.h"
#include "map.h"
#include "particle.h"
#include "resources/image.h"
#include "resources/resourcemanager.h"
#include "resources/dye.h"
#include "graphics.h"
#include "configuration.h"

struct Options
{
  Options():
    printHelpAndExit(false),
    winWidth(200),
    winHeight(200),
    effectX(0),
    effectY(0),
    quitOnEffectEnd(false),
    loop(false),
    backgroundColour(0,0,0)
  {};

  bool printHelpAndExit;

  int winWidth, winHeight;
  int effectX, effectY;

  std::string dataDir; 
  std::string effectFile;

  bool quitOnEffectEnd;
  bool loop;

  gcn::Color backgroundColour;
};


Graphics *graphics;

Logger *logger;

Particle *particleEngine;

// in particlEd Configuration is empty (mock!)
Configuration config;

Options options;

void initEngine();
void exitEngine();
void parseOptions(int argc, char *argv[], Options &options);
void printHelp();
void startEffect();

int main(int argc, char* argv[])
{
  Map *m;

  logger = new Logger();

  // parse options; set some defaults; control required ones
  parseOptions(argc, argv, options);
  if (options.effectX == 0)
    options.effectX = options.winWidth / 2;
  if (options.effectY == 0)
    options.effectY = options.winHeight / 2;
  if (options.effectFile.empty()) {
    logger->error("Please, specify particle effect definition file.");
    return(1);
  }

  if (options.printHelpAndExit) {
    printHelp();
    return(0);
  }

  PHYSFS_init(argv[0]);

  logger->log("-> initialize engine");
  initEngine();

  // create map, load particle effect
  particleEngine = new Particle(NULL);
  particleEngine->setupEngine();

  m = new Map(1,1,200,200);
  particleEngine->setMap(m);

  startEffect();

  logger->log("-> game loop");

  gcn::Rectangle *gfxRect = new gcn::Rectangle(0,0,
					       graphics->getWidth(),
					       graphics->getHeight());

  SDL_Event event;
  bool quit = false;

  // event loop
  for (int i = 0; i < 2000; i++) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
	quit = true;
      } else if (event.type == SDL_KEYDOWN) {
	switch (event.key.keysym.sym) {
	case SDLK_ESCAPE:
	case SDLK_q:
	  quit = true;
	case SDLK_r:
	  // start new effect
	  startEffect();
	  break;
	case SDLK_n:
	  // delete all particles and start new effect
	  particleEngine->clear();
	  startEffect();
	  break;
	}
      }
    }

    particleEngine->update();

    graphics->setColor(options.backgroundColour);
    graphics->fillRectangle(*gfxRect);

    m->draw(graphics, 0, 0);

    // std::cout << m->getNumSprites() << std::endl;

    graphics->updateScreen();

    // end if particle effect has ended
    if (m->getNumSprites() == 1) {
      if (options.quitOnEffectEnd) {
	quit = true;
      } else if (options.loop) {
	startEffect();
      }
    }

    if (quit) {
      break;
    }

    SDL_Delay(10);
  }

  delete gfxRect;

  logger->log("-> game loop ended");

  delete particleEngine;
  delete m;

  logger->log("-> exit engine");
  exitEngine();

  logger->log("-> done.");

  delete logger;

  PHYSFS_deinit();
  return(0);
}

/**
 * Do all initialization stuff.
 */
void initEngine()
{
  // Initialize SDL
  logger->log("[SDL] Initializing SDL...");
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    std::cerr << "Could not initialize SDL: " <<
      SDL_GetError() << std::endl;
    exit(1);
  }
  atexit(SDL_Quit);

  SDL_EnableUNICODE(1);
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  std::string title = "particlEd: ";
  title += options.effectFile;

  SDL_WM_SetCaption(title.c_str(), NULL);
  logger->log("[SDL] OK.");

  // Resource Manager
  logger->log("[RM] Initializing Resource Manager");
  ResourceManager *resman = ResourceManager::getInstance();

  logger->log("[RM] adding directory to search path");
  if (options.dataDir.empty()) {
    logger->log("No data directory specified; setting \".\"");
    resman->addToSearchPath(".", false);
  } else {
    resman->addToSearchPath(options.dataDir, false);
  }

  logger->log("[RM] OK.");

  logger->log("[GFX] Setting up video.");
  int width = options.winWidth;
  int height = options.winHeight;
  int bpp = 0;
  bool fullscreen = 0;
  bool hwaccel = 0;

  graphics = new Graphics();

  // Try to set the desired video mode
  if (!graphics->setVideoMode(width, height, bpp, fullscreen, hwaccel))
    {
      std::cerr << "Couldn't set "
		<< width << "x" << height << "x" << bpp << " video mode: "
		<< SDL_GetError() << std::endl;
      exit(1);
    }

  logger->log("[GFX] initializing for drawing");

  // Initialize for drawing
  graphics->_beginDraw();

  logger->log("[GFX] Ok.");

  // gui = new Gui(graphics);
}

/** Starts effect defined in options */
void startEffect()
{
  logger->log("Adding effect.");
  particleEngine->addEffect(options.effectFile, 
			    options.effectX, 
			    options.effectY);

}

/** Clear the engine */
void exitEngine()
{
  //delete gui;
  delete graphics;

  // Shutdown libxml
  xmlCleanupParser();

  ResourceManager::deleteInstance();
}

/** parse given colour string and put the colour to options.backgroundColour */
void setBackgroundColour(char *bgc_str)
{
  // use dyeing engine to parse the colour:
  std::string s(bgc_str);
  Palette pal(s);
  int col[3] = {255,255,255};

  pal.getColor(255, col);

  options.backgroundColour.r = col[0];
  options.backgroundColour.g = col[1];
  options.backgroundColour.b = col[2];
}

/** Parse command line options */
void parseOptions(int argc, char *argv[], Options &options)
{
  static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"win-width", required_argument, 0, 'W'},
    {"win-height", required_argument, 0, 'H'},
    {"effect-x", required_argument, 0, 'x'},
    {"effect-y", required_argument, 0, 'y'},
    {"quit-on-end", no_argument, 0, 'q'},
    {"loop", no_argument, 0, 'l'},
    {"datadir", required_argument, 0, 'd'},
    {"bg-colour", required_argument, 0, 'b'},
    {0, 0, 0, 0}
  };

  const char *optstring = "hW:H:x:y:qld:b:";

  while (1) {
    int result = getopt_long(argc, argv, optstring, long_options, NULL);

    if (result == -1)
      break;

    switch (result) {
    case 'h':
      options.printHelpAndExit = true;
      break;
    case 'W':
      options.winWidth = (int) atoi(optarg);
      break;
    case 'H':
      options.winHeight = (int) atoi(optarg);
      break;
    case 'x':
      options.effectX = (int) atoi(optarg);
      break;
    case 'y':
      options.effectY = (int) atoi(optarg);
      break;
    case 'q':
      options.quitOnEffectEnd = true;
      if (options.loop) {
	logger->log("Error: --loop shouldn't be used together with --quit-on-end");
      }
      break;
    case 'l':
      options.loop = true;
      if (options.quitOnEffectEnd) {
	logger->log("Error: --quit-on-end shouldn't be used together with --loop");
      }
      break;
    case 'd':
      options.dataDir = optarg;
      break;
    case 'b':
      setBackgroundColour(optarg);
      break;
    default:
      break;
    }
  }

  // non-option argument: name of effect file
  if (optind < argc) {
    // std::cout << (argc - optind) << std::endl;
    options.effectFile = argv[optind];
  }
}

void printHelp()
{
  std::cout << "-- particlEd --" << std::endl;
  std::cout << "---------------" << std::endl;
  std::cout << "TMW particle effect viewer" << std::endl;
  std::cout << "based on TMW code written by TMW development team" << std::endl;
  std::cout << std::endl;
  std::cout << "OPTIONS:" << std::endl;
  std::cout << "-h -W WIDTH -H HEIGHT -x X -y Y -q -d DIR -b COLOUR   EFFECT" << std::endl;
  std::cout << std::endl;
  std::cout << "-h --help       print this help and exit." << std::endl;
  std::cout << "-W --win-width  set window width (default 200)" << std::endl;
  std::cout << "-h --win-height                  (default 200)" << std::endl;
  std::cout << "-x --effect-x   set x of the effect (default 100)" << std::endl;
  std::cout << "-Y --effect-y                       (default 100)" << std::endl;
  std::cout << "-q --quit-on-end Close program as soon as effect ends" << std::endl;
  std::cout << "-l --loop        restart effect whenever it ends" << std::endl; 
  std::cout << "-d --datadir    set TMW data directory for PhysFS (default \".\")" << std::endl;
  std::cout << "-b --bg-colour  hexadecimal RGB colour (e.g. \"#ff0000\" - red)" << std::endl;
  std::cout << std::endl;
  std::cout << "EFFECT          effect file location (relative to given DIR)" << std::endl;
}
