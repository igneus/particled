#include <iostream>

#include <guichan.hpp>
#include "playcontrolpanel.h"

PlayControlPanel::PlayControlPanel() : gcn::Window("play controller")
{
  playButton = new gcn::Button(">");
  playButton->setWidth(15);
  playButton->setPosition(10,10);
  this->add(playButton);
  playButton->addMouseListener(this);
  
  pauseButton = new gcn::Button("||");
  pauseButton->setWidth(15);
  pauseButton->setPosition(35,10);
  this->add(pauseButton);
  playButton->addMouseListener(this);
  
  stopButton = new gcn::Button("#");
  stopButton->setWidth(15);
  stopButton->setPosition(60,10);
  this->add(stopButton);
  playButton->addMouseListener(this);

  this->resizeToContent();

  changeState(STOPPED);
}

PlayControlPanel::~PlayControlPanel()
{
  delete playButton;
  delete pauseButton;
  delete stopButton;
}

void PlayControlPanel::mouseClicked(gcn::MouseEvent &mouseEvent)
{
  if (mouseEvent.isConsumed()) {
    return;
  }

  gcn::Widget* sourceWidget = mouseEvent.getSource();
  if (sourceWidget == dynamic_cast<gcn::Widget*>(playButton)) {
    std::cout << "Click on PLAY button" << std::endl;
    changeState(PLAYING);
  } else if (sourceWidget == dynamic_cast<gcn::Widget*>(pauseButton)) {
    std::cout << "Click on PAUSE button" << std::endl;
    changeState(PAUSED);
  } else if (sourceWidget == dynamic_cast<gcn::Widget*>(stopButton)) {
    std::cout << "Click on STOP button" << std::endl;
    changeState(STOPPED);
  }

  mouseEvent.consume();
}

bool PlayControlPanel::isPlaying()
{
  return false;
}

bool PlayControlPanel::isPaused()
{
  return false;
}

bool PlayControlPanel::isStopped()
{
  return false;
}

/* sets state and notifies buttons to change their look */
void PlayControlPanel::changeState(int newState)
{
  state = newState;
}
