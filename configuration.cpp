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
 *  $Id: configuration.cpp 3671 2007-10-22 19:07:38Z gmelquio $
 */

/* Fake configuration implementation */

#include "configuration.h"

// #include <libxml/xmlwriter.h>

// #include "configlistener.h"
#include "log.h"

#include "utils/tostring.h"

void Configuration::init()
{
}

void Configuration::setValue(const std::string &key, std::string value)
{
    mOptions[key] = value;

    // Notify listeners
    /*
    ListenerMapIterator list = mListenerMap.find(key);
    if (list != mListenerMap.end()) {
        Listeners listeners = list->second;
        for (ListenerIterator i = listeners.begin(); i != listeners.end(); i++)
        {
            (*i)->optionChanged(key);
        }
    }
    */
}

void Configuration::setValue(const std::string &key, float value)
{
    setValue(key, toString((value == (int)value) ? (int)value : value));
}

std::string Configuration::getValue(const std::string &key, std::string deflt)
{
    OptionIterator iter = mOptions.find(key);
    return ((iter != mOptions.end()) ? iter->second : deflt);
}

float Configuration::getValue(const std::string &key, float deflt)
{
    OptionIterator iter = mOptions.find(key);
    return (iter != mOptions.end()) ? atof(iter->second.c_str()) : deflt;
}

/* void Configuration::addListener(
        const std::string &key, ConfigListener *listener)
{
    mListenerMap[key].push_front(listener);
}

void Configuration::removeListener(
        const std::string &key, ConfigListener *listener)
{
    mListenerMap[key].remove(listener);
}
*/
