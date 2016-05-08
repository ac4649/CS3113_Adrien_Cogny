#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>

#include <SDL_image.h>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "Final_Project.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

#include "CoreFunctions.h"


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

ShaderProgram* programUntextured;

EnemyAI* theAI;
 
CoreFunctions* coreFunctionObject;


//window properties
float windowSizeHeight = 360;
float windowSizeWidth = 640;

float totalUnitsHeight = 250.0f;
float totalUnitsWidth = 500.0f;



float lastFrameTicks = 0.0f;


bool done = false;
int state = 0; // 0 = menu state



/* Menu */
Matrix menuModelMatrix;

Entity menuBackground;
Entity menuButton;
Entity menuText;



//Global Matrices
Matrix projectionMatrix;
Matrix viewMatrix;
Matrix tileMapModelMatrix;

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

//drawing text function
void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing, float startX,float startY)
{
    float texture_size = 1.0/16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int i=0; i < text.size(); i++)
    {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        
        vertexData.insert(vertexData.end(),
                          {
                              (startX+(size+spacing) * i) + (-0.5f * size), 0.5f * size + startY,
                              (startX+(size+spacing) * i) + (-0.5f * size),  -0.5f * size + startY,
                              (startX+(size+spacing) * i) + (0.5f * size), 0.5f * size + startY,
                              (startX+(size+spacing) * i) + (0.5f * size),  -0.5f * size + startY,
                              (startX+(size+spacing) * i) + (0.5f * size),  0.5f * size + startY,
                              (startX+(size+spacing) * i) + (-0.5f * size),  -0.5f * size + startY,
                          });
        
        texCoordData.insert(texCoordData.end(),
                            {
                                texture_x, texture_y,
                                texture_x, texture_y + texture_size,
                                texture_x + texture_size, texture_y,
                                texture_x + texture_size, texture_y + texture_size,
                                texture_x + texture_size, texture_y,
                                texture_x, texture_y + texture_size,
                            });
    }
    
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

// if the entity contains the sprite location as well as the height and width on the sprite sheet.
void DrawSpriteUnorderedSheetSprite(ShaderProgram *program, Entity *displayedEntity)
{
    
    //bind the texture
    glBindTexture(GL_TEXTURE_2D, displayedEntity->textureID);
    
    
    program->setModelMatrix(displayedEntity->modelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
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
        displayedEntity->position.getx()-displayedEntity->width/2, displayedEntity->position.gety()-displayedEntity->height/2,
        displayedEntity->position.getx()+displayedEntity->width/2, displayedEntity->position.gety()+displayedEntity->height/2,
        displayedEntity->position.getx()-displayedEntity->width/2, displayedEntity->position.gety()+displayedEntity->height/2,
        
        displayedEntity->position.getx()+displayedEntity->width/2, displayedEntity->position.gety()+displayedEntity->height/2,
        displayedEntity->position.getx()-displayedEntity->width/2, displayedEntity->position.gety()-displayedEntity->height/2,
        displayedEntity->position.getx()+displayedEntity->width/2, displayedEntity->position.gety()-displayedEntity->height/2
        
    };
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
}

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
        
        displayedEntity->position.getx()-displayedEntity->width/2, displayedEntity->position.gety()-displayedEntity->height/2,
        displayedEntity->position.getx()+displayedEntity->width/2, displayedEntity->position.gety()+displayedEntity->height/2,
        displayedEntity->position.getx()-displayedEntity->width/2, displayedEntity->position.gety()+displayedEntity->height/2,
        
        displayedEntity->position.getx()+displayedEntity->width/2, displayedEntity->position.gety()+displayedEntity->height/2,
        displayedEntity->position.getx()-displayedEntity->width/2, displayedEntity->position.gety()-displayedEntity->height/2,
        displayedEntity->position.getx()+displayedEntity->width/2, displayedEntity->position.gety()-displayedEntity->height/2
        
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
   
    coreFunctionObject = new CoreFunctions();
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Platformer Demo - Adrien Cogny", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    SDL_SetWindowFullscreen(displayWindow, 0);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 360);
    ShaderProgram* program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    programUntextured = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

    
    //set the projection Matrix
    
    projectionMatrix.setOrthoProjection(-totalUnitsWidth/2.0,totalUnitsWidth/2.0,-totalUnitsHeight/2,totalUnitsHeight/2,-1.0f, 1.0f);
    
    //projectionMatrix.setPerspectiveProjection(65.0f, totalUnitsWidth/totalUnitsHeight, -1.0f, 1.0f);
    
    glUseProgram(program->programID);
    
    glPointSize(10.0f);

    
    theLevelLoader = new LevelLoader(RESOURCE_FOLDER"level2.txt");
    
    theLevelLoader->loadLevelData();
    
    //display the solidity of the level:
    theLevelLoader->outPutLevelSolid();
    
    tileMapTexture = LoadTexture("tiles.png");
    
    
    theCollisionChecker = new TileMapCollisionChecker();
    theAI = new EnemyAI(theCollisionChecker,theLevelLoader);
    
    
    //translates the tile map model matrix so their 0,0 coincides with the top right corner of the screen
    //program->setModelMatrix(tileMapModelMatrix);
    
    
    //tileMapModelMatrix.Translate(-totalUnitsWidth/2, totalUnitsHeight/2, 0);
    
    
    
    //load menu textures
    menuBackground.textureID = LoadTexture("purpleBackground.png");
    menuText.textureID = LoadTexture("font1.png");
    menuButton.textureID = LoadTexture("sheet.png");
    
    
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
                
                if (state == 0)
                {
                    state = 1;
                }
                else if (state == 1)
                {
                    //space set the velocity of the player in y to something other than 0
                    Entity* player = theLevelLoader->getEntityForIndex(0);
                    
                    if (player->collidedBottom == true)
                    {
                        player->velocity.sety(30.0f);
                        player->collidedBottom = false;
                        
                    }
                }


            }
            //quitting when the player presses q
            else if (event.key.keysym.scancode == SDL_SCANCODE_Q)
            {
                //exit the game
                
                if (state == 0)
                {
                    done = true;
                }
                else
                {
                    state = 0;
                }
                
            }
            
        }
        
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            
            float unitX = (((float)event.button.x / windowSizeWidth) * totalUnitsWidth ) - totalUnitsWidth/2.0;
            float unitY = (((float)(360-event.button.y) / windowSizeHeight) * totalUnitsHeight ) - totalUnitsHeight/2.0;
            
            std::cout << "Mouse Click :" << unitX << ", " << unitY << std::endl;
            std::cout << "Target Click :" << menuButton.position.getx() << " +/- " << menuButton.width/2.0 << ", " << menuButton.position.gety() << " +/- " << menuButton.height/2.0 << std::endl;
            
            if (menuButton.position.getx() - menuButton.width/2.0 < unitX && unitX < menuButton.position.getx() + menuButton.width/2.0 )
            {
                if (menuButton.position.gety() - menuButton.height/2.0 < unitY && unitY < menuButton.position.gety() + menuButton.height/2.0 )
                {
                    state = 1;
                    std::cout << "Starting Game" << std::endl;
                    
                }
            }
            
        }
        
        
    }
    
    
    
}

void updateMenu(ShaderProgram* program, float elapsed)
{
    //generate the background
    menuBackground.position.setx(0);
    menuBackground.position.sety(0);
    menuBackground.height = totalUnitsHeight;
    menuBackground.width = totalUnitsWidth;
    
    menuBackground.textureLocationX = 0;
    menuBackground.textureLocationY = 0;
    menuBackground.textureHeight = 256;
    menuBackground.textureWidth = 256;
    menuBackground.textureSheetHeight = 144;
    menuBackground.textureSheetWidth = 256;
    
    //generate the button
    menuButton.position.setx(0);
    menuButton.position.sety(0);
    menuButton.height = 0.6*39/222*totalUnitsHeight;
    menuButton.width = 0.6*222/222*totalUnitsWidth;
    
    menuButton.textureLocationX = 0;
    menuButton.textureLocationY = 0;
    menuButton.textureHeight = 39;
    menuButton.textureWidth = 222;
    menuButton.textureSheetHeight = 1024;
    menuButton.textureSheetWidth = 1024;
    
    
}

void renderMenu(ShaderProgram* program)
{
    //drawing Triangles
    
    program->setModelMatrix(menuModelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
    menuModelMatrix.identity(); //resets to initial position
    
    DrawSpriteUnorderedSheetSprite(program, &menuBackground);
    
    glBindTexture(GL_TEXTURE_2D, menuButton.textureID);
    DrawSpriteUnorderedSheetSprite(program, &menuButton);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    DrawText(program, menuText.textureID, "PLAY PLATFORMER", 0.3f, 0.00000000001f,0.0,0.0);
    
}



void DrawLevel(float elapsed)
{
    

    theProgram->setModelMatrix(tileMapModelMatrix);
    theProgram->setProjectionMatrix(projectionMatrix);
    theProgram->setViewMatrix(viewMatrix);
    
    programUntextured->setProjectionMatrix(projectionMatrix);
    programUntextured->setViewMatrix(viewMatrix);
    
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
            
            //if (levelDataXY != 0)
            //{
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
            //}
            
            
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
        player->acceleration.setx(-10);
        //std::cout << "GO LEFT" << std::endl;
        
    }
    else if(keys[SDL_SCANCODE_RIGHT])
    {
        //make player go right
        player->acceleration.setx(10);
        //std::cout << "GO RIGHT" << std::endl;

        
    }
    
    //move the entities

    for (int i = 0; i < theLevelLoader->getNumEntities(); i++)
    {
        Entity* curEntity = theLevelLoader->getEntityForIndex(i);
        curEntity->moveX(elapsed);
        theCollisionChecker->checkAndResolveCollisionXWithEntity(curEntity, theLevelLoader);
        
        
        curEntity->moveY(elapsed);
        theCollisionChecker->checkAndResolveCollisionYWithEntity(curEntity, theLevelLoader);
        
        theCollisionChecker->checkAndResolveCollisionOnEdges(curEntity, theLevelLoader);
        
        
        if (curEntity->EntityType != 0 || curEntity->EntityType != 0)
        {
            std::cout << curEntity->EntityType << std::endl;
            theAI->simpleBackForth((BaseEnemy* )curEntity);

        }
        
        
        
        
        //check collisionChecker   
        while (theCollisionChecker->isErrorCode() == true)
        {
            
            int errorCodeReceived = theCollisionChecker->getErrorCode();
            
            std::cout << errorCodeReceived << std::endl;
            
            std::cout << "COLLISION DETECTION FAILED WITH ERROR: " << errorCodeReceived << std::endl;
            
            if (errorCodeReceived == 1)
            {
                std::cout << "Out of bounds on x" << std::endl;
                
                if (curEntity->EntityType == 0 || curEntity->EntityType == 0)
                {
                    theLevelLoader->resetPlayerPosition(curEntity);
                    theLevelLoader->addDeath();

                }
                
            }
            else if (errorCodeReceived == 2)
            {
                std::cout << "Out of bounds on y" << std::endl;
                
                if (curEntity->EntityType == 0 || curEntity->EntityType == 0)
                {
                    theLevelLoader->resetPlayerPosition(curEntity);
                    theLevelLoader->addDeath();
                }
                

                
            }
            
        }
        
    }

    
    
    // Check if the player won
    
    
    
    Tile* curPlayerTile = theLevelLoader->getLevelDataAtIndex(player->tileMapPosition.gety(), player->tileMapPosition.getx());
    
    if (curPlayerTile->getTileValue() == 55)
    {
        
        std::cout << "PLAYER WON" << std::endl;
        state = 2;
        //change state to exit game
        
        
    }

    
    
    for (int i = 0; i < theLevelLoader->getNumEntities(); i++)
    {
        Entity* theCurEntity = theLevelLoader->getEntityForIndex(i);
        
        //display the entity
        theCurEntity->DrawSpriteUnorderedSheetSprite(theProgram, projectionMatrix, viewMatrix, programUntextured);

    }
    
    
    
    
}

void DrawMenu(float elapsed)
{
    
    updateMenu(theProgram, elapsed);
    
    renderMenu(theProgram);
    
    
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
        
        if (state == 0) {
            SDL_ShowCursor(1);

        }
        else
        {
            SDL_ShowCursor(0);
        }
        
        
        processEvents(event);
        
        
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        
        
        float fixedElapsed = elapsed;
        
        if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS)
        {
            fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
        }
        

        while  (fixedElapsed >= FIXED_TIMESTEP)
        {
            fixedElapsed -= FIXED_TIMESTEP;
            
            if (state == 0)
            {
                DrawMenu(FIXED_TIMESTEP);
            }
            else if (state == 1)
            {
                DrawLevel(FIXED_TIMESTEP);
                DrawEntities(FIXED_TIMESTEP);
            }
            
            
        }
        
        if (state == 0)
        {
            DrawMenu(fixedElapsed);
        }
        else if (state == 1)
        {
            DrawLevel(fixedElapsed);
            DrawEntities(fixedElapsed);
            //place the player in the middle of the screen
            viewMatrix.identity();
            viewMatrix.Translate(-(theLevelLoader->getEntityForIndex(0))->position.getx(), -(theLevelLoader->getEntityForIndex(0))->position.gety(), 0);
            
        }

        SDL_GL_SwapWindow(displayWindow);

        
    }
    
    SDL_Quit();

    return 0;
}
