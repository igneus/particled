#include "effectviewer.h"
#include "../graphics.h"

EffectViewer::EffectViewer():
  backgroundColour(0,0,0)
{
  particleEngine = new Particle(NULL);
  particleEngine->setupEngine();

  m = new Map(1,1,200,200);
  particleEngine->setMap(m);

  effectX = 50;
  effectY = 50;
}

EffectViewer::~EffectViewer()
{
  delete particleEngine;
  delete m;
}

void EffectViewer::setEffect(std::string &effect)
{
  particleEngine->addEffect(effect, effectX, effectY);
}

void EffectViewer::cleanParticles()
{
  particleEngine->clear();
}

void EffectViewer::setEffectPosition(int x, int y)
{
  effectX = x;
  effectY = y;
}

void EffectViewer::setBackgroundColour(gcn::Color rgb)
{
  backgroundColour.r = rgb.r;
  backgroundColour.g = rgb.g;
  backgroundColour.b = rgb.b;
}

void EffectViewer::draw(gcn::Graphics *graphics)
{
  particleEngine->update();

  graphics->setColor(backgroundColour);
  graphics->fillRectangle(this->getDimension());

  m->draw(static_cast<Graphics*>(graphics), 0, 0);
}

int EffectViewer::particleCount()
{
  return m->getNumSprites();
}

bool EffectViewer::isEffectOver()
{
  return (this->particleCount() == 1);
}
