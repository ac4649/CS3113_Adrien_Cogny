//
//  LevelLoader.cpp
//  Assignment4
//
//  Created by Adrien Cogny on 3/18/16.
//  Copyright © 2016 Ivan Safrin. All rights reserved.
//

#include "LevelLoader.h"


void LevelLoader::loadLevelData()
{
    
    
    levelMatrix = new vector<vector<Tile*>*>; //initialize the levelMatrix
    
    ifstream fileStream(filename);
    std::cout << filename << std::endl;
    
    string line;
    while (getline(fileStream, line))
    {
        if(line == "[header]")
        {
            if(!readHeader(fileStream))
            {
                //not a valid file
                return;
            }
        }
        else if(line == "[layer]")
        {
            readLayerData(fileStream);
        }
        else if(line == "[ObjectLayer]")
        {
            readEntityData(fileStream);
        }
    }
    
    mapSize = mapWidth*mapHeight;
    
}

bool LevelLoader::readHeader(ifstream& fileStream)
{
    
    string line;
    mapWidth = -1;
    mapHeight = -1;
    
    bool foundGravityX = false;
    bool foundGravityY = false;

    while(getline(fileStream, line))
    {
        if(line == "")
        {
            break;
        }
        
        istringstream sStream(line);
        
        string key,value;
        
        getline(sStream, key, '=');
        
        getline(sStream, value);
        
        
        if(key == "width")
        {
            mapWidth = atoi(value.c_str());
        }
        else if(key == "height")
        {
            mapHeight = atoi(value.c_str());
        }
        
        if (key == "GravityX")
        {
            levelGravityX = atof(value.c_str()); //this keeps the gravity for the level if there is one in the level definition
            
            foundGravityX = true;
        }
        if (key == "GravityY") {
            levelGravityY = atof(value.c_str()); //this keeps the gravity for the level if there is one in the level definition
            foundGravityY = true;

        }
    }
    
    
    if (!foundGravityX)
    {
        //no gravity defined in level, go to default
        levelGravityX = 0.0f;
    }
    if (!foundGravityY)
    {
        levelGravityY = -9.8f;
    }
    
    if(mapWidth == -1 || mapHeight == -1)
    {
        return false;
    }
    else
    {
        
        //because I am using vectors as the levelMatrix, no need to allocate size
        //create and allocate the levelMatrix
        return true;
    }
    
    
}


bool LevelLoader::readLayerData(ifstream& fileStream)
{
    
    string line;
    
    while(getline(fileStream, line))
    {
        if(line == "")
        {
            break;
        }
        
        istringstream sStream(line);
        
        string key,value;
        
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if(key == "data")
        {
            for(int y=0; y < mapHeight; y++)
            {
                getline(fileStream, line);
                istringstream lineStream(line);
                string tile;
                vector<Tile*>* curLine = new vector<Tile*>;
                for(int x=0; x < mapWidth; x++)
                {
                    
                    //create the vector for the line
                    int theValue;
                    
                    getline(lineStream, tile, ',');
                    unsigned char val =  (unsigned char)atoi(tile.c_str());
                    if(val > 0)
                    {
                        // be careful, the tiles in this format are indexed from 1 not 0
                        theValue = val-1;
                    }
                    else
                    {
                        theValue = 0;
                    }
                    
                    bool solidValue = false;
                    
                    if (theValue == 1 || theValue  == 16 || theValue == 2 || theValue == 3)
                    {
                        solidValue = true;
                    }
                    
                    bool deathValue = false;

                    if (theValue == 36 || theValue  == 49 || theValue == 50)
                    {
                        deathValue = true;
                    }
                    
                    Tile* theCurTile = new Tile(theValue,x,y,solidValue,deathValue);
                    
                    curLine->push_back(theCurTile);
                    
                    
                }

                levelMatrix->push_back(curLine);
            }
        }
    }
    
    return true;
    
}


bool LevelLoader::readEntityData(ifstream& fileStream)
{
    
    string line;
    string type;
    Entity* curEntityRead = nullptr;
    
    
    
    while(getline(fileStream, line))
    {
        if(line == "")
        {
            break;
        }
        
        istringstream sStream(line);
        string key,value;
        
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if(key == "type")
        {
            
            
            /*
             
             Entity Types:
             
             0 = Player
             1 = Slime
             2 = BlueSlime
             
             */
            
            
            type = value;

            if (type == "Player")
            {
                curEntityRead = new Entity();
                
                //set the spritesheet to use and the coordinates
                curEntityRead->setTexture("playersprite.png", 0, 0, 66, 92, 66, 92);
                curEntityRead->width = TILE_SIZE;
                curEntityRead->height = TILE_SIZE;
                curEntityRead->EntityType = 0;
                
                
            }
            else if (type == "Slime")
            {
                curEntityRead = new BaseEnemy();
                
                //set the spritesheet to use and the coordinates
                curEntityRead->setTexture("pinkslime.png", 0, 0, 66, 92, 66, 92);
                curEntityRead->width = TILE_SIZE;
                curEntityRead->height = TILE_SIZE;
                curEntityRead->EntityType = 1;
                
                curEntityRead->acceleration.setx(-1.0);
                curEntityRead->velocity.setx(-1.0);
            }
            else if (type == "BlueSlime")
            {
                curEntityRead = new BaseEnemy();
                
                //set the spritesheet to use and the coordinates
                curEntityRead->setTexture("blueSlime.png", 0, 0, 66, 92, 66, 92);
                curEntityRead->width = TILE_SIZE;
                curEntityRead->height = TILE_SIZE;
                curEntityRead->EntityType = 2;
                
                curEntityRead->acceleration.setx(-1.0);
                curEntityRead->velocity.setx(-1.0);
            }
            else if (type == "Snail")
            {
                curEntityRead->setTexture("snail.png", 0, 0, 31, 53, 31, 53);
                curEntityRead->width = TILE_SIZE;
                curEntityRead->height = TILE_SIZE;
                curEntityRead->EntityType = 3;
                
                curEntityRead->acceleration.setx(2.0);
                curEntityRead->velocity.setx(0.0);
            }
        }
        else if(key == "location" && curEntityRead != nullptr)
        {
            istringstream lineStream(value);
            string xPosition, yPosition;
            
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
            
            std::cout << xPosition.c_str() << std::endl;
            std::cout << yPosition.c_str() << std::endl;

            curEntityRead->tileMapPosition.setx(atof(xPosition.c_str()));
            curEntityRead->tileMapPosition.sety(atof(yPosition.c_str()));
            
            curEntityRead->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
            
            if (type == "Player")
            {
                defaultPlayerX = curEntityRead->tileMapPosition.getx();
                defaultPlayerY = curEntityRead->tileMapPosition.gety();
            }

        }
        
        //set the gravity for the current entity
        
        if(curEntityRead != nullptr)
        {
            curEntityRead->gravity.setx(levelGravityX);
            curEntityRead->gravity.sety(levelGravityY);

        }

        
        
    }
    
    levelEntities.push_back(curEntityRead);

    return true;
    
}

const vector<vector<Tile*>*>* LevelLoader::getLevelMatrix()
{
    return levelMatrix;
}
Tile* LevelLoader::getLevelDataAtIndex(int height, int width)
{
    
    if (height >= mapHeight || height < 0)
    {
        //std::cout << "Beyond level height" << std::endl;
        return nullptr;
    }
    if (width >= mapWidth || width < 0)
    {
        //std::cout << "Beyond level width" << std::endl;
        return nullptr;
    }
    Tile* returnValue = (*(*levelMatrix)[height])[width];
    
    return returnValue;
}
int LevelLoader::getLevelHeight()
{
    return mapHeight;
}
int LevelLoader::getLevelWidth()
{
    return mapWidth;
}
int LevelLoader::getMapSize()
{
    return mapSize;
}

int LevelLoader::getNumEntities()
{
    return levelEntities.size();
}

Entity* LevelLoader::getEntityForIndex(int index)
{
    
    return levelEntities[index];
    
}

void LevelLoader::updateAllEntityTileMapCoordinates()
{
    for (int i = 0; i < levelEntities.size(); i++)
    {
        levelEntities[i]->updateTileMapCoordinatesFromWorldCoords(TILE_SIZE);
    }
}


float LevelLoader::getDefaultPlayerX()
{
    
    return defaultPlayerX;
    
}
float LevelLoader::getDefaultPlayerY()
{
    
    return defaultPlayerY;
    
}

void LevelLoader::outPutLevelSolid()
{
    
    for (int i = 0; i < getLevelHeight(); i++) {
        for (int j = 0; j < getLevelWidth(); j++) {
            
            Tile* curTile = (*(*levelMatrix)[i])[j];
            
            std::cout << curTile->getSolidValue();
            
        }
        std::cout << std::endl;
    }
    
}

void LevelLoader::addDeath()
{
    numberOfDeaths = numberOfDeaths + 1;
}

void LevelLoader::resetPlayerPosition(Entity* player)
{
    
    player->tileMapPosition.setx(getDefaultPlayerX());
    player->tileMapPosition.sety(getDefaultPlayerY());
    
    player->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
    
    
    player->velocity.setxyz(0.0, 0.0, 0.0);
    
}