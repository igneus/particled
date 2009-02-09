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
 *  $Id: resource.h 3740 2007-11-16 11:06:46Z gmelquio $
 */

#ifndef _TMW_RESOURCE_H
#define _TMW_RESOURCE_H

#include <ctime>
#include <string>

/**
 * A generic reference counted resource object.
 */
class Resource
{
    friend class ResourceManager;

    public:
        /**
         * Constructor
         */
        Resource(): mRefCount(0) {}

        /**
         * Increments the internal reference count.
         */
        void
        incRef();

        /**
         * Decrements the reference count and deletes the object
         * if no references are left.
         *
         * @return <code>true</code> if the object was deleted
         *         <code>false</code> otherwise.
         */
        void
        decRef();

        /**
         * Return the path identifying this resource.
         */
        const std::string&
        getIdPath() { return mIdPath; }

    protected:
        /**
         * Destructor.
         */
        virtual
        ~Resource();

    private:
        std::string mIdPath; /**< Path identifying this resource. */
        time_t mTimeStamp;   /**< Time at which the resource was orphaned. */
        unsigned mRefCount;  /**< Reference count. */
};

#endif
