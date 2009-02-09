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
 *  $Id: resourcemanager.h 3999 2008-03-23 01:27:13Z b_lindeijer $
 */

#ifndef _TMW_RESOURCE_MANAGER_H
#define _TMW_RESOURCE_MANAGER_H

#include <ctime>
#include <map>
#include <string>
#include <vector>

class Resource;
class Image;
class ImageSet;
class SpriteDef;
struct SDL_Surface;

/**
 * A class for loading and managing resources.
 */
class ResourceManager
{

    friend class Resource;

    public:

        typedef Resource *(*loader)(void *, unsigned);
        typedef Resource *(*generator)(void *);

        /**
         * Constructor.
         */
        ResourceManager();

        /**
         * Destructor.
         */
        ~ResourceManager();

        /**
         * Sets the write directory.
         *
         * @param path The path of the directory to be added.
         * @return <code>true</code> on success, <code>false</code> otherwise.
         */
        bool
        setWriteDir(const std::string &path);

        /**
         * Adds a directory or archive to the search path. If append is true
         * then the directory is added to the end of the search path, otherwise
         * it is added at the front.
         *
         * @return <code>true</code> on success, <code>false</code> otherwise.
         */
        bool
        addToSearchPath(const std::string &path, bool append);

        /**
        * Searches for zip files and adds them to the search path.
        */
        void
        searchAndAddArchives(const std::string &path,
                             const std::string &ext,
                             bool append);

        /**
         * Creates a directory in the write path
         */
        bool
        mkdir(const std::string &path);

        /**
         * Checks whether the given file or directory exists in the search path
         */
        bool
        exists(const std::string &path);

        /**
         * Checks whether the given path is a directory.
         */
        bool
        isDirectory(const std::string &path);
    
        /**
         * Returns the real path to a file
         * 
         * @param file The file to get the real path to.
         * @return The real path.
         */
        std::string getPath(const std::string &file);

        /**
         * Creates a resource and adds it to the resource map.
         *
         * @param idPath The resource identifier path.
         * @param fun    A function for generating the resource.
         * @param data   Extra parameters for the generator.
         * @return A valid resource or <code>NULL</code> if the resource could
         *         not be generated.
         */
        Resource *get(std::string const &idPath, generator fun, void *data);

        /**
         * Loads a resource from a file and adds it to the resource map.
         *
         * @param path The file name.
         * @param fun  A function for parsing the file.
         * @return A valid resource or <code>NULL</code> if the resource could
         *         not be loaded.
         */
        Resource *load(std::string const &path, loader fun);

        /**
         * Convenience wrapper around ResourceManager::get for loading
         * images.
         */
        Image*
        getImage(const std::string &idPath);

        /**
         * Creates a image set based on the image referenced by the given
         * path and the supplied sprite sizes
         */
        ImageSet*
        getImageSet(const std::string &imagePath, int w, int h);

        /**
         * Creates a sprite definition based on a given path and the supplied
         * variant.
         */
        SpriteDef *getSprite(std::string const &path, int variant = 0);

        /**
         * Releases a resource, placing it in the set of orphaned resources.
         */
        void release(Resource *);

        /**
         * Allocates data into a buffer pointer for raw data loading. The
         * returned data is expected to be freed using <code>free()</code>.
         *
         * @param fileName The name of the file to be loaded.
         * @param fileSize The size of the file that was loaded.
         *
         * @return An allocated byte array containing the data that was loaded,
         *         or <code>NULL</code> on fail.
         */
        void*
        loadFile(const std::string &fileName, int &fileSize);

        /**
         * Retrieves the contents of a text file.
         */
        std::vector<std::string>
        loadTextFile(const std::string &fileName);

        /**
         * Loads the given filename as an SDL surface. The returned surface is
         * expected to be freed by the caller using SDL_FreeSurface.
         */
        SDL_Surface*
        loadSDLSurface(const std::string& filename);

        /**
         * Returns an instance of the class, creating one if it does not
         * already exist.
         */
        static ResourceManager*
        getInstance();

        /**
         * Deletes the class instance if it exists.
         */
        static void
        deleteInstance();

    private:
        /**
         * Deletes the resource after logging a cleanup message.
         */
        static void
        cleanUp(Resource *resource);

        void cleanOrphans();

        static ResourceManager *instance;
        typedef std::map<std::string, Resource*> Resources;
        typedef Resources::iterator ResourceIterator;
        Resources mResources;
        Resources mOrphanedResources;
        time_t mOldestOrphan;
};

#endif
