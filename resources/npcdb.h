/*
 *  The Mana World
 *  Copyright 2008 The Mana World Development Team
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
 *  $Id: npcdb.h 4250 2008-05-19 16:18:38Z crush_tmw $
 */

#ifndef _TMW_NPC_DB_H
#define _TMW_NPC_DB_H

#include <map>
#include <list>
#include <string>

struct NPCsprite
{
    std::string sprite;
    int variant;
};

struct NPCInfo
{
    std::list<NPCsprite*> sprites;
    std::list<std::string> particles;
};

typedef std::map<int, NPCInfo*> NPCInfos;

/**
 * NPC information database.
 */
namespace NPCDB
{
    void
    load();

    void
    unload();

    const NPCInfo& get(int id);

    typedef NPCInfos::iterator NPCInfosIterator;
}

#endif
