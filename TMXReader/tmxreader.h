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

#ifndef TMXREADER_H
#define TMXREADER_H

#include "base64/base64.h"
#include "TMXReader/mapchange.h"

#include <vector>
#include <string>
#include <QtXml/QtXml>
#include <QFile>

typedef int TMX_err_type;
#define NO_ERROR  0
#define ERR_NO_MAIN  1
#define ERR_CHANGE_X  2
#define ERR_CHANGE_Y  4
#define ERR_CHANGE_WIDTH  8
#define ERR_CHANGE_HEIGHT  16
#define ERR_CHANGE_ID  32
#define ERR_MULTIPLE_MAIN 64

class TMXReader
{
public:
    TMXReader();
    TMXReader(const char*);
    TMX_err_type parseTMX();
    TMX_err_type parseChange(QDomElement changeNode);
    void parseData(QDomElement dataNode, unsigned int**, unsigned long);
    int getX();
    int getY();
    int getNumberOfChanges();
    bool hasChanges();
    unsigned int* getData();
    MapChange getChange(int id);
    //void myinflate(unsigned char* decoded, unsigned char** decompressed, unsigned long compressedsize, unsigned long decompressedsize);

private:
    const char* m_sfileName;
    int m_iXSize;
    int m_iYSize;
    unsigned int* data;
    QDomNodeList elements;
    std::vector<MapChange> changes;

};



#endif // TMXREADER_H
