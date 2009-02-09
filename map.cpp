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
 *  $Id: map.cpp 4569 2008-09-04 20:09:57Z b_lindeijer $
 */

#include "map.h"

#include <queue>
#include <cassert>

#include <guichan/color.hpp>
#include <guichan/rectangle.hpp>

#include "graphics.h"
#include "particle.h"
#include "sprite.h"

#include "resources/resourcemanager.h"
#include "resources/image.h"

#include "utils/dtor.h"
#include "utils/tostring.h"

extern volatile int tick_time;

Map::Map(int width, int height, int tileWidth, int tileHeight):
    mWidth(width), mHeight(height),
    mTileWidth(tileWidth), mTileHeight(tileHeight),
    mMaxTileHeight(height)
{
}

Map::~Map()
{
}

bool spriteCompare(const Sprite *a, const Sprite *b)
{
    return a->getPixelY() < b->getPixelY();
}

void Map::draw(Graphics *graphics, int scrollX, int scrollY)
{
    int endPixelY = graphics->getHeight() + scrollY + mTileHeight - 1;

    // TODO: Do this per-layer
    endPixelY += mMaxTileHeight - mTileHeight;

    int startX = scrollX / mTileWidth;
    int startY = scrollY / mTileHeight;
    int endX = (graphics->getWidth() + scrollX + mTileWidth - 1) / mTileWidth;
    int endY = endPixelY / mTileHeight;

    // Make sure sprites are sorted
    mSprites.sort(spriteCompare);

    for(Sprites::const_iterator si = mSprites.begin();
	si != mSprites.end();
	si++) {
      (*si)->draw(graphics, -scrollX, -scrollY);
    }
}

bool Map::contains(int x, int y) const
{
    return x >= 0 && y >= 0 && x < mWidth && y < mHeight;
}

SpriteIterator Map::addSprite(Sprite *sprite)
{
    mSprites.push_front(sprite);
    return mSprites.begin();
}

void Map::removeSprite(SpriteIterator iterator)
{
    mSprites.erase(iterator);
}

void Map::addParticleEffect(const std::string &effectFile, int x, int y)
{
    ParticleEffectData newEffect;
    newEffect.file = effectFile;
    newEffect.x = x;
    newEffect.y = y;
    particleEffects.push_back(newEffect);
}

void Map::initializeParticleEffects(Particle* particleEngine)
{
    for (std::list<ParticleEffectData>::iterator i = particleEffects.begin();
         i != particleEffects.end();
         i++
        )
    {
        particleEngine->addEffect(i->file, i->x, i->y);
    }
}

int Map::getNumSprites()
{
  int c = 0;
  for (SpriteIterator i = mSprites.begin(); i != mSprites.end(); i++) {
    c++;
  }

  return c;
}
