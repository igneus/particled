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
 *  $Id: monsterdb.cpp 4043 2008-04-07 08:37:23Z b_lindeijer $
 */

#include <algorithm>

#include "monsterdb.h"

#include "resourcemanager.h"
#include "spritedef.h"

#include "../log.h"

#include "../utils/dtor.h"
#include "../utils/xml.h"

namespace
{
    MonsterDB::MonsterInfos mMonsterInfos;
    MonsterInfo mUnknown;
    bool mLoaded = false;
}

void
MonsterDB::load()
{
    if (mLoaded)
        return;

    mUnknown.setSprite("error.xml");
    mUnknown.setName("unnamed");

    logger->log("Initializing monster database...");

    XML::Document doc("monsters.xml");
    xmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "monsters"))
    {
        logger->error("Monster Database: Error while loading monster.xml!");
    }

    //iterate <monster>s
    for_each_xml_child_node(monsterNode, rootNode)
    {
        if (!xmlStrEqual(monsterNode->name, BAD_CAST "monster"))
            continue;

        MonsterInfo *currentInfo = new MonsterInfo();

        currentInfo->setName(XML::getProperty(monsterNode, "name", "unnamed"));

        std::string targetCursor;
        targetCursor = XML::getProperty(monsterNode, "targetCursor", "medium");
        if (targetCursor == "small")
        {
            currentInfo->setTargetCursorSize(Being::TC_SMALL);
        }
        else if (targetCursor == "medium")
        {
            currentInfo->setTargetCursorSize(Being::TC_MEDIUM);
        }
        else if (targetCursor == "large")
        {
            currentInfo->setTargetCursorSize(Being::TC_LARGE);
        }
        else
        {
            logger->log("MonsterDB: Unknown target cursor type \"%s\" for %s "
                        "- using medium sized one",
                        targetCursor.c_str(), currentInfo->getName().c_str());
            currentInfo->setTargetCursorSize(Being::TC_MEDIUM);
        }

        //iterate <sprite>s and <sound>s
        for_each_xml_child_node(spriteNode, monsterNode)
        {
            if (xmlStrEqual(spriteNode->name, BAD_CAST "sprite"))
            {
                currentInfo->setSprite((const char*) spriteNode->xmlChildrenNode->content);
            }
            else if (xmlStrEqual(spriteNode->name, BAD_CAST "sound"))
            {
                std::string event = XML::getProperty(spriteNode, "event", "");
                const char *filename;
                filename = (const char*) spriteNode->xmlChildrenNode->content;

                if (event == "hit")
                {
                    currentInfo->addSound(MONSTER_EVENT_HIT, filename);
                }
                else if (event == "miss")
                {
                    currentInfo->addSound(MONSTER_EVENT_MISS, filename);
                }
                else if (event == "hurt")
                {
                    currentInfo->addSound(MONSTER_EVENT_HURT, filename);
                }
                else if (event == "die")
                {
                    currentInfo->addSound(MONSTER_EVENT_DIE, filename);
                }
                else
                {
                    logger->log("MonsterDB: Warning, sound effect %s for "
                                "unknown event %s of monster %s",
                                filename, event.c_str(),
                                currentInfo->getName().c_str());
                }
            }
            else if (xmlStrEqual(spriteNode->name, BAD_CAST "attack"))
            {
                const int id = XML::getProperty(spriteNode, "id", 0);
                const std::string particleEffect = XML::getProperty(
                        spriteNode, "particle-effect", "");
                SpriteAction spriteAction = SpriteDef::makeSpriteAction(
                        XML::getProperty(spriteNode, "action", "attack"));
                currentInfo->addMonsterAttack(id, particleEffect, spriteAction);
            }
            else if (xmlStrEqual(spriteNode->name, BAD_CAST "particlefx"))
            {
                currentInfo->addParticleEffect(
                    (const char*) spriteNode->xmlChildrenNode->content);
            }
        }
        mMonsterInfos[XML::getProperty(monsterNode, "id", 0)] = currentInfo;
    }

    mLoaded = true;
}

void
MonsterDB::unload()
{
    for_each(mMonsterInfos.begin(), mMonsterInfos.end(),
             make_dtor(mMonsterInfos));
    mMonsterInfos.clear();

    mLoaded = false;
}


const MonsterInfo&
MonsterDB::get(int id)
{
    MonsterInfoIterator i = mMonsterInfos.find(id);

    if (i == mMonsterInfos.end())
    {
        logger->log("MonsterDB: Warning, unknown monster ID %d requested", id);
        return mUnknown;
    }
    else
    {
        return *(i->second);
    }
}
