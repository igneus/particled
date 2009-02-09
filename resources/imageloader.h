/*
 *  The Mana World
 *  Copyright 2007 The Mana World Development Team
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
 *  $Id: imageloader.h 3515 2007-08-25 16:56:52Z gmelquio $
 */

#ifndef _TMW_IMAGELOADER_H
#define _TMW_IMAGELOADER_H

#include <string>

#include <guichan/image.hpp>
#include <guichan/imageloader.hpp>

class Image;
struct SDL_Surface;

class ProxyImage : public gcn::Image
{
    public:
        ProxyImage(SDL_Surface *);
        ~ProxyImage();
        void free();
        int getWidth() const;
        int getHeight() const;
        gcn::Color getPixel(int x, int y);
        void putPixel(int x, int y, gcn::Color const &color);
        void convertToDisplayFormat();

        /**
         * Gets the image handled by this proxy.
         */
        ::Image *getImage() const
        { return mImage; }

    private:
        ::Image *mImage; /**< The real image. */

        /**
         * An SDL surface kept around until Guichan is done reading pixels from
         * an OpenGL texture.
         */
        SDL_Surface *mSDLImage;
};

class ImageLoader : public gcn::ImageLoader
{
    public:
        gcn::Image *load(std::string const &filename, bool convertToDisplayFormat);
};

#endif
