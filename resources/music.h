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
 *  $Id: music.h 3642 2007-10-19 17:46:46Z gmelquio $
 */

#ifndef _TMW_MUSIC_H
#define _TMW_MUSIC_H

#include <SDL_mixer.h>

#include "resource.h"

/**
 * Defines a class for loading and storing music.
 */
class Music : public Resource
{
    public:
        /**
         * Destructor.
         */
        virtual ~Music();

        /**
         * Loads a music from a buffer in memory.
         *
         * @param buffer     The memory buffer containing the music data.
         * @param bufferSize The size of the memory buffer in bytes.
         *
         * @return <code>NULL</code> if the an error occurred, a valid pointer
         *         otherwise.
         */
        static Resource *load(void *buffer, unsigned bufferSize);

        /**
         * Plays the music.
         *
         * @param loops     Number of times to repeat the playback.
         *
         * @return <code>true</code> if the playback started properly
         *         <code>false</code> otherwise.
         */
        virtual bool
        play(int loops);

        /**
         * Stops the music.
         */
        virtual void
        stop();

    protected:
        /**
         * Constructor.
         */
        Music(Mix_Chunk *music);

        //Mix_Music *music;
        Mix_Chunk *mChunk;
        int mChannel;
};

#endif
