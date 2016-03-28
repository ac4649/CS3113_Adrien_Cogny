#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>

#include <SDL_image.h>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "Assignment4.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;


#include "Matrix.h"
#include "Matrix.h"
#include "ShaderProgram.h"
#include <Vector>

#include "LevelLoader.h"
#include "TileMapCollisionChecker.h"

#define TILE_SIZE 30.0f
#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 7



#define FIXED_TIMESTEP 0.008333333333 //120 FPS
#define MAX_TIMESTEPS 6 //maximum number of timesteps want to used

//helper objects
LevelLoader* theLevelLoader;
ShaderProgram* theProgram;
TileMapCollisionChecker* theCollisionChecker;




//window properties
float windowSizeHeight = 360;
float windowSizeWidth = 640;

float totalUnitsHeight = 250.0f;
float totalUnitsWidth = 500.0f;



float lastFrameTicks = 0.0f;


bool done = false;


//Global Matrices
Matrix projectionMatrix;
Matrix viewMatrix;
Matrix tileMapModelMatrix;


//lerp function
float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}


//load texture function
GLuint LoadTexture(const char *image_path)
{
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}


//level texture
GLuint tileMapTexture;


void DrawSpriteUnorderedSheetSprite(Entity *displayedEntity)
{
    
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, displayedEntity->textureID);
    
    
    theProgram->setModelMatrix(displayedEntity->modelMatrix);
    
    //displayedEntity->modelMatrix.identity();
    //displayedEntity->modelMatrix.Translate(-totalUnitsWidth/2, -totalUnitsHeight, 0);
    
    theProgram->setProjectionMatrix(projectionMatrix);
    theProgram->setViewMatrix(viewMatrix);
    
    float u = displayedEntity->textureLocationX / displayedEntity->textureSheetWidth;
    float v = displayedEntity->textureLocationY / displayedEntity->textureSheetHeight;
    
    float spriteNormalizedWidth = displayedEntity->textureWidth/displayedEntity->textureSheetWidth;
    float spriteNormalizedHeight = displayedEntity->textureHeight/displayedEntity->textureSheetHeight;
    
    GLfloat texCoords[] = {
        u, v+spriteNormalizedHeight,
        u+spriteNormalizedWidth, v,
        u, v,
        u+spriteNormalizedWidth, v,
        u, v+spriteNormalizedHeight,
        u+spriteNormalizedWidth, v+spriteNormalizedHeight
    };
    
    float vertices[] =
    {
        
        displayedEntity->x-displayedEntity->width/2, displayedEntity->y-displayedEntity->height/2,
        displayedEntity->x+displayedEntity->width/2, displayedEntity->y+displayedEntity->height/2,
        displayedEntity->x-displayedEntity->width/2, displayedEntity->y+displayedEntity->height/2,
        
        displayedEntity->x+displayedEntity->width/2, displayedEntity->y+displayedEntity->height/2,
        displayedEntity->x-displayedEntity->width/2, displayedEntity->y-displayedEntity->height/2,
        displayedEntity->x+displayedEntity->width/2, displayedEntity->y-displayedEntity->height/2
        
    };
    
    glVertexAttribPointer(theProgram->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(theProgram->positionAttribute);
    
    glVertexAttribPointer(theProgram->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(theProgram->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(theProgram->positionAttribute);
    glDisableVertexAttribArray(theProgram->texCoordAttribute);
    
    
}



ShaderProgram *setup() // will return the shaderProgram pointer
{
   
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Platformer Demo - Adrien Cogny", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 360);
    ShaderProgram* program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    //set the projection Matrix
    projectionMatrix.setOrthoProjection(-totalUnitsWidth/2.0,totalUnitsWidth/2.0,-totalUnitsHeight/2,totalUnitsHeight/2,-1.0f, 1.0f);
    
    glUseProgram(program->programID);

    
    theLevelLoader = new LevelLoader(RESOURCE_FOLDER"level0.txt");
    
    theLevelLoader->loadLevelData();
    tileMapTexture = LoadTexture("tiles.png");
    
    
    theCollisionChecker = new TileMapCollisionChecker();
    
    
    //translates the tile map model matrix so their 0,0 coincides with the top right corner of the screen
    //program->setModelMatrix(tileMapModelMatrix);
    
    
    //tileMapModelMatrix.Translate(-totalUnitsWidth/2, totalUnitsHeight/2, 0);
    
    
    //remove alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return program;
}

void processEvents(SDL_Event event)
{
    
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            
            done = true;
            
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                
                //space set the velocity of the player in y to something other than 0
                Entity* player = theLevelLoader->getEntityForIndex(0);
                
                if (player->collidedBottom == true)
                {
                    player->velocity_y = 6.0;
                    player->collidedBottom = false;

                }

            }
        }
        
        
        
    }
    
    
    
}

void DrawLevel(float elapsed)
{
    

    theProgram->setModelMatrix(tileMapModelMatrix);
    theProgram->setProjectionMatrix(projectionMatrix);
    theProgram->setViewMatrix(viewMatrix);
    glBindTexture(GL_TEXTURE_2D, tileMapTexture);
    
    //tileMapModelMatrix.identity(); //resets to initial position
    //tileMapModelMatrix.Translate(0, -0.005, 0);
    
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    
    int height = theLevelLoader->getLevelHeight();
    int width = theLevelLoader->getLevelWidth();

    //std::cout << height << " " << width <<std::endl;
    

    for(int y=0; y < height; y++)
    {
        for(int x=0; x < width; x++)
        {
            
            Tile* theCurTile = theLevelLoader->getLevelDataAtIndex(y,x);
            
            int levelDataXY = theCurTile->getTileValue();
            
            
            float u = (float)(levelDataXY % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
            float v = (float)(levelDataXY / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
            
            float spriteWidth = 1.0f/(float)SPRITE_COUNT_X;
            float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;
            
            
            
            vertexData.insert(vertexData.end(),
            {
                TILE_SIZE * x, -TILE_SIZE * y,
                TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                TILE_SIZE * x, -TILE_SIZE * y,
                (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
            });
            
            texCoordData.insert(texCoordData.end(),{
                u, v,
                u, v+(spriteHeight),
                u+spriteWidth, v+(spriteHeight),
                u, v,
                u+spriteWidth, v+(spriteHeight),
                u+spriteWidth, v
            });
            
        }
        //std::cout << endl;
    }
    
    glUseProgram(theProgram->programID);
    glVertexAttribPointer(theProgram->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(theProgram->positionAttribute);
    glVertexAttribPointer(theProgram->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(theProgram->texCoordAttribute);
    
    
    glDrawArrays(GL_TRIANGLES, 0, theLevelLoader->getMapSize()*6);
    
    glDisableVertexAttribArray(theProgram->positionAttribute);
    glDisableVertexAttribArray(theProgram->texCoordAttribute);
    

    
}

void DrawEntities(float elapsed)
{
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    Entity* player = theLevelLoader->getEntityForIndex(0);
    
    if(keys[SDL_SCANCODE_LEFT])
    {
        //make player go left
        player->acceleration_x = -10;
        
    }
    else if(keys[SDL_SCANCODE_RIGHT])
    {
        //make player go right
        player->acceleration_x = 10;
        
    }
    
    //move the entities
    
    //change the velocity with respect to friction
    player->velocity_x = lerp(player->velocity_x, 0.0f, FIXED_TIMESTEP * player->friction_x);
    player->velocity_y = lerp(player->velocity_y, 0.0f, FIXED_TIMESTEP * player->friction_y);
    
    //change the velocity with respect to acceleration
    player->velocity_x += player->acceleration_x * FIXED_TIMESTEP;
    player->velocity_y += player->acceleration_y * FIXED_TIMESTEP;
    
    //change the velocity with respect to the gravity
    player->velocity_x += player->gravity_x*FIXED_TIMESTEP;
    player->velocity_y += player->gravity_y*FIXED_TIMESTEP;
    
    //change the player position with respect to velocity
    player->x += player->velocity_x * FIXED_TIMESTEP;
    player->y += player->velocity_y * FIXED_TIMESTEP;
    
    
    //update the entities tileMap coordinates to do the collision checks
    theLevelLoader->updateAllEntityTileMapCoordinates();
    
    
    //do collision checking
    for (int i = 0; i < theLevelLoader->getNumEntities(); i++)
    {
        Entity* curEntity = theLevelLoader->getEntityForIndex(i);
        theCollisionChecker->checkAndResolveCollisionWithEntity(curEntity, theLevelLoader);
        
        
        
        int errorCodeReceived = theCollisionChecker->getErrorCode();
        
        if (errorCodeReceived != 0)
        {
            std::cout << "COLLISION DETECTION FAILED WITH ERROR: " << errorCodeReceived << std::endl;
            
            if (errorCodeReceived == 1)
            {
                std::cout << "Out of bounds on x, resetting player location" << std::endl;
                
                player->tileMapX = theLevelLoader->getDefaultPlayerX();
                player->tileMapY = theLevelLoader->getDefaultPlayerY();
                
                player->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
                
                
            }
            else if (errorCodeReceived == 2)
            {
                std::cout << "Out of bounds on y, resetting player location" << std::endl;
                
                player->tileMapX = theLevelLoader->getDefaultPlayerX();
                player->tileMapY = theLevelLoader->getDefaultPlayerY();
                
                player->updateWorldCoordinatesFromTileMapCoords(TILE_SIZE);
                
            }
            
        }


    }
    
    
    
    
    
    for (int i = 0; i < theLevelLoader->getNumEntities(); i++)
    {
        Entity* theCurEntity = theLevelLoader->getEntityForIndex(i);
        
        //std::cout << theCurEntity->x << std::endl;
        //std::cout << theCurEntity->y << std::endl;
        
        //display the entity
        DrawSpriteUnorderedSheetSprite(theCurEntity);

    }
    
    
    
    
}





int main(int argc, char *argv[])
{
    
    theProgram = setup();
    
    
    
    std::cout << "level Data: " << std::endl;
    
    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            Tile* curLevelTile = theLevelLoader->getLevelDataAtIndex(i, j);
            
            int curLevel = curLevelTile->getTileValue();
            
            std::cout << curLevel;
        }
        std::cout << std::endl;
    }
    
    
    SDL_Event event;

    while (!done)
    {

        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        
        processEvents(event);
        
        
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        
        
        float fixedElapsed = elapsed;
        
        if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS)
        {
            fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
        }
        
        
        
        
        while  (fixedElapsed >= FIXED_TIMESTEP)
        {
            fixedElapsed -= FIXED_TIMESTEP;
            
            DrawLevel(fixedElapsed);
            DrawEntities(fixedElapsed);
            
        }

        
        //place the player in the middle of the screen
        viewMatrix.identity();
        viewMatrix.Translate(-(theLevelLoader->getEntityForIndex(0))->x, -(theLevelLoader->getEntityForIndex(0))->y, 0);
        
        SDL_GL_SwapWindow(displayWindow);

        
    }
    
    SDL_Quit();
    return 0;
}
