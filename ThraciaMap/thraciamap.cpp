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

#include "thraciamap.h"

#include <cstdlib>
//#include <iostream>
#include <fstream>

using namespace std;

ThraciaMap::ThraciaMap(int x, int y, unsigned int* tiledData)
{
    m_iXSize = x;
    m_iYSize = y;
    int mapsize = x*y;
    m_mapData = (unsigned short*) malloc(2*mapsize);
    for(int i = 0; i < mapsize; i++)
    {
        m_mapData[i] = (tiledData[i] - 1) * 8;
    }

}

void ThraciaMap::writeToFile(char *fileName)
{
    ofstream mapFile(fileName, ios_base::binary);
    mapFile.write(reinterpret_cast<const char*>(&m_iXSize) , 2);
    mapFile.write(reinterpret_cast<const char*>(&m_iYSize), 2);
    mapFile.write(reinterpret_cast<char*>(m_mapData), 2*m_iXSize*m_iYSize);
    mapFile.close();
}

void ThraciaMap::writeChangesToFile(char *filename)
{
    ofstream mapFile(filename, ios_base::binary);
    unsigned char num, num2 = 0;
    mapFile << string(6*changes.size() + 2, '\0').c_str();
    for(unsigned int i = 0; i < changes.size(); i++)
    {
        unsigned short pos1 = mapFile.tellp();
        unsigned char x = changes[i].getX();
        unsigned char y = changes[i].getY();
        unsigned char width = changes[i].getWidth();
        unsigned char height = changes[i].getHeight();
        if(changes[i].isBitSet())
        {
            num2++;
            x &= 0x80;
        }
        else
        {
            num++;
        }
        //unsigned char* mData = reinterpret_cast<unsigned char*>(changes[i].getData());
        mapFile.write(reinterpret_cast<const char*>(changes[i].getData()), 2*width*height);
        unsigned short pos2 = mapFile.tellp();
        mapFile.seekp(2+(i*6));

        mapFile.write(reinterpret_cast<const char*>(&pos1), 2);
        mapFile << x << y << width << height;
        mapFile.seekp(pos2);
    }
    mapFile.seekp(ios_base::beg);
    mapFile << num << num2;
    mapFile.close();
}

void ThraciaMap::addChange(int id, int x, int y, int width, int height, bool bitset, int *tiledData)
{
    SNESMapChange ch(id, x, y, width, height, bitset, tiledData);
    changes.push_back(ch);
}
