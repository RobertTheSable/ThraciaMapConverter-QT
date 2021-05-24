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

#ifndef SNESMAPCHANGE_H
#define SNESMAPCHANGE_H

class SNESMapChange
{
private:
    int m_iID;
    unsigned char m_iX;
    int m_iY;
    int m_iWidth;
    int m_iHeight;
    int m_bSet;
    unsigned short* m_sMapData;
public:
    SNESMapChange(int id, int x, int y, int width, int height, bool bitset, int* tiledData);
    bool isBitSet();
    unsigned char getX();
    unsigned char getY();
    unsigned char getWidth();
    unsigned char getHeight();
    unsigned short* getData();
};

#endif // SNESMAPCHANGE_H
