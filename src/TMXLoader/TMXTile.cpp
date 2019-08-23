//
//  TMXTile.cpp
//  TMXLoader
//
//  Created by Marty on 06/09/2015.
//  Copyright (c) 2015 Martin Grant. All rights reserved.
//  Available under MIT license. See License.txt for more information.
//
//  Uses RapidXML for file parsing.
//  Copyright (c) 2006, 2007 Marcin Kalicinski
//  http://rapidxml.sourceforge.net/
//  See /RapidXML/License.txt for more information.
//
//  www.midnightpacific.com
//  contact@midnightpacific.com
//  @_martingrant
//  http://bitbucket.org/martingrant/tmxloader
//

#include "TMXTile.h"

TMXTile::TMXTile()
{
}

TMXTile::TMXTile(unsigned int tileID, std::unordered_map<std::string, std::string> &propertiesMap) : m_tileID(tileID), m_propertiesMap(propertiesMap)
{
    //TODO Should all be properties of the tileset in Tiled
    collision_distance = 5;

    if (tileID == 34 || tileID == 36 || tileID == 259 || tileID == 323)
    {
        collision_rect = {0, 0, 16, 5};
    }
    else if (tileID == 290)
    {
        collision_rect = {0, 0, 5, 5};
    }
    else if (tileID == 257 || tileID == 289 || tileID == 261)
    {
        collision_rect = {11, 0, 5, 16};
    }
    else if (tileID == 225)
    {
        collision_rect = {11, 11, 5, 5};
    }
    else if (tileID == 258 || tileID == 262)
    {
        collision_rect = {0, 0, 5, 16};
    }
    else if (tileID == 259)
    {
        collision_rect = {0, 0, 16, 16};
    }
    else if (tileID == 2 || tileID == 3 || tileID == 4)
    {
        collision_rect = {0, 11, 16, 5};
    }
    else if (tileID == 226 || tileID == 260)
    {
        collision_rect = {0, 11, 5, 5};
        collision_distance = 5;
    }
}

TMXTile::~TMXTile()
{
    m_propertiesMap.clear();
    std::unordered_map<std::string, std::string>().swap(m_propertiesMap);
}

unsigned int TMXTile::getTileID()
{
    return m_tileID;
}

std::string TMXTile::getProperty(std::string propertyName)
{
    std::unordered_map<std::string, std::string>::const_iterator iterator = m_propertiesMap.find(propertyName);

    if (iterator == m_propertiesMap.end())
    {
        std::cout << "TMXLoader: property '" << propertyName << "' not found." << std::endl;
    }
    else
    {
        return iterator->second;
    }

    return nullptr;
}

void TMXTile::printData()
{
    std::cout << "Tile ID: " << m_tileID << "\n Tile Properties: " << std::endl;
    for (auto index = m_propertiesMap.begin(); index != m_propertiesMap.end(); ++index)
    {
        std::cout << "\n"
                  << index->first << " - " << index->second << std::endl;
    }
}

SDL_Rect TMXTile::getCollisionBoundary()
{
    return collision_rect;
}

int TMXTile::getCollisionDistance()
{
    return collision_distance;
}