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

#include "TMXReader/tmxreader.h"

#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <iostream>
#include <zlib.h>

TMXReader::TMXReader()
{
}

TMXReader::TMXReader(const char *fileName)
{
    m_sfileName = fileName;

    QFile file(m_sfileName);
    QDomDocument qDoc;

    if(file.exists() && qDoc.setContent(&file))
    {
        elements = qDoc.elementsByTagName("layer");

        m_iXSize = qDoc.firstChildElement("map").attribute("width").toInt();//atoi(mapNode->first_attribute("width")->value());
        m_iYSize = qDoc.firstChildElement("map").attribute("height").toInt();
    }

    file.close();
}
TMX_err_type TMXReader::parseTMX()
{
    int x = m_iXSize;
    int y = m_iYSize;

    TMX_err_type returnError = ERR_NO_MAIN;
    for(int i = 0; i < elements.length(); i++)
    {
        if(elements.at(i).firstChildElement("properties").firstChildElement("property").attribute("name") == "Main")
        {
            returnError = NO_ERROR;

            unsigned long decompressedSize = x*y*sizeof(unsigned int);
            parseData(elements.at(i).firstChildElement("data"), &data, decompressedSize);

        }
        else
        {
            //map change
            TMX_err_type changeError = parseChange(elements.at(i).toElement());
            returnError |= changeError;
        }
    }

    return returnError;
}

TMX_err_type TMXReader::parseChange(QDomElement changeNode)
{
    TMX_err_type changeStatus = NO_ERROR;
    QDomNodeList properties = changeNode.firstChildElement("properties").elementsByTagName("property");
    //*rapidxml::xml_node<>* propertyNode = dataNode->first_node("properties")->first_node("property");
    int xPos = -1,yPos = -1,id = -1, xSize = -1,ySize = -1;
    bool set = false;
    for(int i = 0; i < properties.length(); i++)
    {

        if(properties.at(i).toElement().attribute("name") == "ID")
        {
            id = atoi(properties.at(i).toElement().attribute("value").toStdString().c_str());
        }
        else if(properties.at(i).toElement().attribute("name") == "X")
        {
            xPos = atoi(properties.at(i).toElement().attribute("value").toStdString().c_str());
        }
        else if(properties.at(i).toElement().attribute("name") ==  "Y")
        {
            yPos = atoi(properties.at(i).toElement().attribute("value").toStdString().c_str());
        }
        else if(properties.at(i).toElement().attribute("name") ==  "Width")
        {
            xSize = atoi(properties.at(i).toElement().attribute("value").toStdString().c_str());
        }
        else if(properties.at(i).toElement().attribute("name") == "Height")
        {
            ySize = atoi(properties.at(i).toElement().attribute("value").toStdString().c_str());
        }
        else if(properties.at(i).toElement().attribute("name") == "BitSet")
        {
            set = true;
        }
    }
    if(xPos == -1)
    {
        changeStatus |= ERR_CHANGE_X;
    }
    if(yPos == -1)
    {
        changeStatus |= ERR_CHANGE_Y;
    }
    if(xSize == -1)
    {
        changeStatus |= ERR_CHANGE_WIDTH;
    }
    if(ySize == -1)
    {
        changeStatus |= ERR_CHANGE_HEIGHT;
    }
    if(id == -1)
    {
        changeStatus |= ERR_CHANGE_ID;
    }

    if (changeStatus == NO_ERROR)
    {
        unsigned int *layerData = new unsigned int[m_iXSize*m_iYSize*sizeof(int)];
        parseData(changeNode.firstChildElement("data").toElement(), &layerData , m_iXSize*m_iYSize*sizeof(int));
        int *changeData = new int[xSize*ySize*sizeof(int)];
        //*
        for(int i  = 0; i < ySize; i++)
        {
            for(int j  = 0; j < xSize; j++)
            {
                changeData[(i*xSize)+j] = layerData[(yPos + i)*m_iXSize + xPos + j];

            }
        }//*/
        MapChange ch(xPos, yPos, xSize, ySize, id, set, changeData);
        changes.push_back(ch);//*/
        delete [] layerData;
        delete [] changeData;
    }

    return changeStatus;
}

void TMXReader::parseData(QDomElement dataNode, unsigned int** tiledData, unsigned long decompressedsize)
{
        //unsigned int *mapData = *tiledData;
        std::string xmldata;
        if(dataNode.attribute("encoding") == "base64")
        {
            xmldata = dataNode.firstChild().nodeValue().toStdString();
            xmldata.erase( remove(xmldata.begin(), xmldata.end(), '\n'), xmldata.end());
            xmldata.erase( remove(xmldata.begin(), xmldata.end(), '\r'), xmldata.end());
            xmldata.erase( remove(xmldata.begin(), xmldata.end(), ' '), xmldata.end());
            std::string decodedstr = base64_decode(xmldata);
            char *decoded = (char*)malloc(decodedstr.length());
            decodedstr.copy(decoded, decodedstr.length());
            unsigned long compressedsize = (unsigned long) decodedstr.length();

            unsigned char *decompressed = (unsigned char*) malloc(decompressedsize);

            if(dataNode.attribute("compression") != "")
            {
                z_stream strm;
                memset(&strm, 0, sizeof(strm));

                strm.next_in = (unsigned char*)decoded;
                strm.avail_in = compressedsize;
                strm.zalloc = Z_NULL;
                strm.zfree = Z_NULL;
                strm.opaque = Z_NULL;

                int test = inflateInit2(&strm, 32+MAX_WBITS);

                strm.next_out = decompressed;
                strm.avail_out = decompressedsize;
                if(test == Z_OK)
                {
                    test = inflate(&strm, Z_NO_FLUSH);
                    if ((test == Z_OK)  || (test == Z_STREAM_END))
                    {
                        test = inflateEnd(&strm);
                    }
                }
                *tiledData = reinterpret_cast<unsigned int*>(decompressed);
            }
            else
            {
                *tiledData = reinterpret_cast<unsigned int*>(decoded);
            }
        }
        else
        {
            if(dataNode.attribute("encoding") == "csv")
            {
                std::string xmlstring = dataNode.firstChild().nodeValue().toStdString();
                replace( xmlstring.begin(), xmlstring.end(), ',', ' ');
                std::stringstream *csvdata = new std::stringstream(xmlstring);
                unsigned long mapSize = decompressedsize;
                *tiledData = (unsigned int*)malloc(mapSize);

                for(unsigned int i = 0; i < mapSize; i++)
                {
                    *csvdata >> (*tiledData)[i];
                }
            }
            else
            {
                //XML
                *tiledData = (unsigned int*)malloc(decompressedsize);
                QDomNodeList tileList = dataNode.elementsByTagName("tile");
                for(int i = 0; i < tileList.length(); i++)
                {
                    //cout << tileNode->first_attribute("gid")->value() << endl;
                    (*tiledData)[i] = atoi(tileList.at(i).toElement().attribute("gid").toStdString().c_str());
                }

            }
        }
        //data = *mapdata;
}
int TMXReader::getX()
{
    return m_iXSize;
}

int TMXReader::getY()
{
    return m_iYSize;
}

int TMXReader::getNumberOfChanges()
{
    return changes.size();
}

bool TMXReader::hasChanges()
{
    return getNumberOfChanges() > 0;
}

unsigned int *TMXReader::getData()
{
    return data;
}

MapChange TMXReader::getChange(int id)
{
    for(int i = 0; i < getNumberOfChanges(); i++)
    {
        if(changes[i].getID() == id)
        {
            return changes[i];
        }
    }
    return changes[id];
}
