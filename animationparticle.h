/*
 *  The Mana World
 *  Copyright 2006 The Mana World Development Team
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
 */

#ifndef _ANIMATION_PARTICLE
#define _ANIMATION_PARTICLE

#include <libxml/tree.h>

#include "imageparticle.h"

class Animation;
class Map;
class SimpleAnimation;

class AnimationParticle : public ImageParticle
{
    public:
        AnimationParticle(Map *map, Animation *animation);

        AnimationParticle(Map *map, xmlNodePtr animationNode);

        ~AnimationParticle();

        virtual bool update();

    private:
        SimpleAnimation *mAnimation; /**< Used animation for this particle */
};

#endif
