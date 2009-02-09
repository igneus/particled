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
 *  $Id: map.h 4057 2008-04-08 18:43:44Z b_lindeijer $
 */

#ifndef _TMW_MAP_H_
#define _TMW_MAP_H_

#include <list>
#include <vector>
#include <string>

class Graphics;
class Image;
class Particle;
class Sprite;

typedef std::list<Sprite*> Sprites;
typedef Sprites::iterator SpriteIterator;

/**
 * A "map" cut down oly to include sprites.
 */
class Map 
{

    public:

        /**
         * Constructor, taking map and tile size as parameters.
         */
        Map(int width, int height, int tileWidth, int tileHeight);

        /**
         * Destructor.
         */
        ~Map();

        /**
         * Draws the map to the given graphics output. This method draws all
         * layers, sprites and overlay effects.
         *
         * TODO: For efficiency reasons, this method could take into account
         * the clipping rectangle set on the Graphics object. However,
         * currently the map is always drawn full-screen.
         */
        void draw(Graphics *graphics, int scrollX, int scrollY);

        /**
         * Returns the width of this map.
         */
        int
        getWidth() const { return mWidth; }

        /**
         * Returns the height of this map.
         */
        int
        getHeight() const { return mHeight; }

        /**
         * Returns the tile width of this map.
         */
        int
        getTileWidth() const { return mTileWidth; }

        /**
         * Returns the tile height used by this map.
         */
        int
        getTileHeight() const { return mTileHeight; }

        /**
         * Adds a sprite to the map.
         */
        SpriteIterator
        addSprite(Sprite *sprite);

        /**
         * Removes a sprite from the map.
         */
        void
        removeSprite(SpriteIterator iterator);

        /**
         * Adds a particle effect
         */
        void addParticleEffect(const std::string &effectFile, int x, int y);

        /**
         * Initializes all added particle effects
         */
        void
        initializeParticleEffects(Particle* particleEngine);

	int getNumSprites();

    private:

        /**
         * Tells whether the given coordinates fall within the map boundaries.
         */
        bool contains(int x, int y) const;

        int mWidth, mHeight;
        int mTileWidth, mTileHeight;
        int mMaxTileHeight;
        Sprites mSprites;

        // Particle effect data
        struct ParticleEffectData
        {
            std::string file;
            int x;
            int y;
        };
        std::list<ParticleEffectData> particleEffects;
};

#endif
