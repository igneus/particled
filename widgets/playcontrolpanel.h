/*
Window with three buttons: play, pause and stop.
*/

#include <guichan.hpp>

class PlayControlPanel : public gcn::Window
{
 public:

  PlayControlPanel();
  ~PlayControlPanel();

  bool isPlaying();
  bool isPaused();
  bool isStopped();

  void mouseClicked(gcn::MouseEvent &mouseEvent);

 private:

  void changeState(int newState);

  gcn::Button *playButton, *pauseButton, *stopButton;

  static const int PLAYING=1, PAUSED=2, STOPPED=3;
  int state;
};
