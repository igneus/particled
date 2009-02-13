/**
Widget which views particle effect.
*/

#include <guichan.hpp>
#include <string>
#include "../particle.h"
#include "../map.h"
#include "../graphics.h"

class EffectViewer : public gcn::Container
{
 public:

  EffectViewer();
  ~EffectViewer();

  /* Loads new effect */
  void setEffect(std::string &effect);
  /* Cleans all particles. */
  void cleanParticles();

  void setEffectPosition(int x, int y);
  void setBackgroundColour(gcn::Color rgb);

  void draw(Graphics *graphics);

  /* Returns number of particles. */
  int particleCount();

  /* Says if effect has already ended. */
  bool isEffectOver();

 private:

  Particle *particleEngine;
  Map *m;
  int effectX, effectY;
  gcn::Color backgroundColour;
};
