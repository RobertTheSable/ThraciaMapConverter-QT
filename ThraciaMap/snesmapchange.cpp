/*
 *  Thracia 776 Map Converter
 *
 *  Copyright (C) 2015 Robert the Sable
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3
 *  as published by the Free Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  <Description> For those of you not familiar with the GNU GPL system,
 *  the license is in the file named "COPYING".
 */ 

#include "snesmapchange.h"
#include <cstdlib>

SNESMapChange::SNESMapChange(int id, int x, int y, int width, int height, bool bitset, int *tiledData)
{
    m_iID = id;
    m_iX = x;
    m_iY = y;
    m_iWidth = width;
    m_iHeight = height;
    m_bSet = bitset;
    int mapsize = width*height;
    m_sMapData = (unsigned short*) malloc(2*mapsize);
    for(int i = 0; i < mapsize; i++)
    {
        m_sMapData[i] = (tiledData[i] - 1) * 8;
    }
}

bool SNESMapChange::isBitSet()
{
    return m_bSet;
}

unsigned char SNESMapChange::getX()
{
    return m_iX;
}
unsigned char SNESMapChange::getY()
{
    return m_iY;
}
unsigned char SNESMapChange::getWidth()
{
    return m_iWidth;
}
unsigned char SNESMapChange::getHeight()
{
    return m_iHeight;
}

unsigned short *SNESMapChange::getData()
{
    return m_sMapData;
}
