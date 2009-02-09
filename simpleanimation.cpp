/*
 *  The Mana World
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id: simpleanimation.cpp 3319 2007-06-04 21:48:47Z b_lindeijer $
 */

#include "simpleanimation.h"

#include "graphics.h"
#include "log.h"

#include "resources/image.h"
#include "resources/resourcemanager.h"
#include "resources/imageset.h"


SimpleAnimation::SimpleAnimation(xmlNodePtr animationNode):
    mAnimationTime(0),
    mAnimationPhase(0)
{
    mAnimation = new Animation();

    ImageSet *imageset = ResourceManager::getInstance()->getImageSet(
        XML::getProperty(animationNode, "imageset", ""),
        XML::getProperty(animationNode, "width", 0),
        XML::getProperty(animationNode, "height", 0)
    );

    // Get animation frames
    for (   xmlNodePtr frameNode = animationNode->xmlChildrenNode;
            frameNode != NULL;
            frameNode = frameNode->next)
    {
        int delay = XML::getProperty(frameNode, "delay", 0);
        int offsetX = XML::getProperty(frameNode, "offsetX", 0);
        int offsetY = XML::getProperty(frameNode, "offsetY", 0);
        offsetY -= imageset->getHeight() - 32;
        offsetX -= imageset->getWidth() / 2 - 16;

        if (xmlStrEqual(frameNode->name, BAD_CAST "frame"))
        {
            int index = XML::getProperty(frameNode, "index", -1);

            if (index < 0)
            {
                logger->log("No valid value for 'index'");
                continue;
            }

            Image *img = imageset->get(index);

            if (!img)
            {
                logger->log("No image at index " + (index));
                continue;
            }

            mAnimation->addFrame(img, delay, offsetX, offsetY);
        }
        else if (xmlStrEqual(frameNode->name, BAD_CAST "sequence"))
        {
            int start = XML::getProperty(frameNode, "start", -1);
            int end = XML::getProperty(frameNode, "end", -1);

            if (start < 0 || end < 0)
            {
                logger->log("No valid value for 'start' or 'end'");
                continue;
            }

            while (end >= start)
            {
                Image *img = imageset->get(start);

                if (!img)
                {
                    logger->log("No image at index " +
                            (start));
                    continue;
                }

                mAnimation->addFrame(img, delay, offsetX, offsetY);
                start++;
            }
        }
        else if (xmlStrEqual(frameNode->name, BAD_CAST "end"))
        {
            mAnimation->addTerminator();
        }
    }

    mCurrentFrame = mAnimation->getFrame(0);
}

void SimpleAnimation::update(unsigned int timePassed)
{
    mAnimationTime += timePassed;
    while (mAnimationTime > mCurrentFrame->delay)
    {
        mAnimationTime -= mCurrentFrame->delay;
        mAnimationPhase++;
        if (mAnimationPhase >= mAnimation->getLength())
        {
            mAnimationPhase = 0;
        }
        mCurrentFrame = mAnimation->getFrame(mAnimationPhase);
    }
}

Image *SimpleAnimation::getCurrentImage() const
{
    return mCurrentFrame->image;
}

SimpleAnimation::~SimpleAnimation()
{
    delete mAnimation;
}
