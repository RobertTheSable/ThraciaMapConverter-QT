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

#ifndef MAPCHANGE_H
#define MAPCHANGE_H

class MapChange
{
public:
    MapChange(int x, int y, int xSize, int ySize, int ID, bool bitSet, int *tiledData);
    void getData(int**);
    bool bitSet();
    int getID();
    int getXPos();
    int getYPos();
    int getXSize();
    int getYSize();
private:
    int m_iID;
    int m_iXPos;
    int m_iYPos;
    int m_iXSize;
    int m_iYSize;
    bool m_bBitSet;
    int *m_iData;
};

#endif // MAPCHANGE_H
