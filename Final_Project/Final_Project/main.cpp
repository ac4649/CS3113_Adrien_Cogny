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


//Annimation Timers
float menuAnimationTimer;
float deathAnimationTimer;
float winAnimationTimer;

bool animatingMenu;
bool animatingDeath;
bool animatingWin;


//SOUND

#include <SDL_mixer.h>

Mix_Chunk *playerJumpSound;
Mix_Chunk *playerLand;
Mix_Chunk *playerWin;
Mix_Chunk *playerLoss;

Mix_Music *menuMusic;
Mix_Music *levelMusic;



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
int state = 0; // 0 = menu state, 1 = Game State, 2 = Won State, 3 = death state, 4 = finished game



/* Menu */
Matrix menuModelMatrix;
Entity menuBackground;
Entity menuButton;
Entity menuText;

Matrix deathModelMatrix;
Entity deathBackground;
Entity deathButton;
Entity deathText;

Matrix wonModelMatrix;
Entity wonBackground;
Entity wonButton;
Entity wonText;

Matrix completedModelMatrix;
Entity completedBackground;
Entity completedButton;
Entity completedText;

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
        
        //float textX = startX+(size+spacing) * i + (-0.5f * size);
        
        //std::cout << "textX " << textX << std::endl;
        //std::cout << "textY " << startY << std::endl;

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
void DrawColoredTriangle(ShaderProgram *program, Entity *displayedEntity)
{
    
    
}


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

    
    theLevelLoader = new LevelLoader(RESOURCE_FOLDER"level0.txt");
    
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
    
    
    
    deathBackground.textureID = LoadTexture("Red_Background.png");
    deathText.textureID = LoadTexture("font1.png");
    deathButton.textureID = LoadTexture("sheet.png");
    
    completedBackground.textureID = LoadTexture("purpleBackground.png");
    completedText.textureID = LoadTexture("font1.png");
    completedButton.textureID = LoadTexture("sheet.png");
    
    
    //remove alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    //Initialize sound
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    playerJumpSound = Mix_LoadWAV("jump.wav");
    levelMusic = Mix_LoadMUS("FIGURATIVE_THEATRE_-_03_-_SATORI_vocal_mix.mp3");
    
    
    Mix_PlayMusic(levelMusic, -1);
    
    
    menuAnimationTimer = 0.0;
    deathAnimationTimer = 0.0;
    winAnimationTimer = 0.0;
    
    animatingMenu = true;
    animatingWin = false;
    animatingDeath = false;
    
    
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
                        Mix_PlayChannel(-1, playerJumpSound, 0);
                        
                        player->collidedBottom = false;
                        
                    }
                }
                else if (state == 2)
                {
                    //go to the next level
                    std::cout << "Go Next Level" << std::endl;
                    bool nextLevelAvailable = theLevelLoader->goToNextLeve();
                    if (!nextLevelAvailable)
                    {
                        std::cout << "Congratulations on Finishing the Game" << std::endl;
                        state = 4;
                        
                    }
                    else
                    {
                        theLevelLoader->loadLevelData();
                        theLevelLoader->outPutLevelSolid();
                        
                        std::cout << "level Data: " << std::endl;
                        
                        for (int i = 1; i < 10; i++)
                        {
                            for (int j = 1; j < 10; j++)
                            {
                                Tile* curLevelTile = theLevelLoader->getLevelDataAtIndex(i, j);
                                
                                int curLevel = curLevelTile->getTileValue();
                                
                                std::cout << curLevel << " " ;
                            }
                            std::cout << std::endl;
                        }
                        
                        state = 1;
                    }
                    
                    
                }
                else if (state == 3)
                {
                    state = 1;
                }
                
            }
            //quitting when the player presses q
            else if (event.key.keysym.scancode == SDL_SCANCODE_Q)
            {
                //exit the game
                
                if (state == 0 || state == 4)
                {
                    done = true;
                }
                else
                {
                    state = 0;
                    animatingMenu = true;
                }
                
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_M)
            {
                //exit the game
                
                if (state == 4)
                {
                    state = 0;
                    animatingMenu = true;
                }
                
            }
            
        }
        
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            
            if (state == 0 || state == 2 || state == 3)
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
    
    menuText.position.setx(0);
    menuText.position.sety(0);
    
    
}

void updateDeathScene(ShaderProgram* program, float elapsed)
{
    //generate the background
    deathBackground.position.setx(0);
    deathBackground.position.sety(0);
    deathBackground.height = totalUnitsHeight;
    deathBackground.width = totalUnitsWidth;
    
    deathBackground.textureLocationX = 0;
    deathBackground.textureLocationY = 0;
    deathBackground.textureHeight = 256;
    deathBackground.textureWidth = 256;
    deathBackground.textureSheetHeight = 144;
    deathBackground.textureSheetWidth = 256;
    
    //generate the button
    deathButton.position.setx(0);
    deathButton.position.sety(0);
    deathButton.height = 0.6*39/222*totalUnitsHeight;
    deathButton.width = 0.6*222/222*totalUnitsWidth;
    
    deathButton.textureLocationX = 0;
    deathButton.textureLocationY = 0;
    deathButton.textureHeight = 39;
    deathButton.textureWidth = 222;
    deathButton.textureSheetHeight = 1024;
    deathButton.textureSheetWidth = 1024;
    
    
}

void updateWonScene(ShaderProgram* program, float elapsed)
{
    //generate the background
    wonBackground.position.setx(0);
    wonBackground.position.sety(0);
    wonBackground.height = totalUnitsHeight;
    wonBackground.width = totalUnitsWidth;
    
    wonBackground.textureLocationX = 0;
    wonBackground.textureLocationY = 0;
    wonBackground.textureHeight = 256;
    wonBackground.textureWidth = 256;
    wonBackground.textureSheetHeight = 144;
    wonBackground.textureSheetWidth = 256;
    
    //generate the button
    wonButton.position.setx(0);
    wonButton.position.sety(0);
    wonButton.height = 0.6*39/222*totalUnitsHeight;
    wonButton.width = 0.6*222/222*totalUnitsWidth;
    
    wonButton.textureLocationX = 0;
    wonButton.textureLocationY = 0;
    wonButton.textureHeight = 39;
    wonButton.textureWidth = 222;
    wonButton.textureSheetHeight = 1024;
    wonButton.textureSheetWidth = 1024;
    
    
}


void updateCompletedScene(ShaderProgram* program, float elapsed)
{
    //generate the background
    completedBackground.position.setx(0);
    completedBackground.position.sety(0);
    completedBackground.height = totalUnitsHeight;
    completedBackground.width = totalUnitsWidth;
    
    completedBackground.textureLocationX = 0;
    completedBackground.textureLocationY = 0;
    completedBackground.textureHeight = 256;
    completedBackground.textureWidth = 256;
    completedBackground.textureSheetHeight = 144;
    completedBackground.textureSheetWidth = 256;
    
    //generate the button
    completedButton.position.setx(0);
    completedButton.position.sety(0);
    completedButton.height = 0.6*39/222*totalUnitsHeight;
    completedButton.width = totalUnitsWidth;
    
    completedButton.textureLocationX = 0;
    completedButton.textureLocationY = 0;
    completedButton.textureHeight = 39;
    completedButton.textureWidth = 222;
    completedButton.textureSheetHeight = 1024;
    completedButton.textureSheetWidth = 1024;
    
    
}


void renderMenu(ShaderProgram* program)
{
    //drawing Triangles
    viewMatrix.identity();

    program->setModelMatrix(menuModelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
    menuModelMatrix.identity(); //resets to initial position
    
    std::cout << sin(menuAnimationTimer) << std::endl;
    
    if (animatingMenu)
    {
        viewMatrix.Scale(sin(menuAnimationTimer), sin(menuAnimationTimer), sin(menuAnimationTimer));
    }
    if (sin(menuAnimationTimer) > 0.9)
    {
        //stop the annimation
        animatingMenu = false;
        
    }
    
    
    DrawSpriteUnorderedSheetSprite(theProgram, &menuBackground);
    
    glBindTexture(GL_TEXTURE_2D, menuButton.textureID);
   DrawSpriteUnorderedSheetSprite(program, &menuButton);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    //std::cout << "Menu X " << menuBackground.position.getx() << std::endl;
    //std::cout << "Menu Y " << menuBackground.position.gety() << std::endl;

    
    DrawText(program, menuText.textureID, "PLAY PLATFORMER", 19.0f, 0.000f,-130.0,0.0);
    
}



void renderDeathScene(ShaderProgram* program)
{
    //drawing Triangles
    viewMatrix.identity();

    program->setModelMatrix(deathModelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
    deathModelMatrix.identity(); //resets to initial position
    
    
    std::cout << sin(menuAnimationTimer) << std::endl;
    
    if (animatingDeath)
    {
        //viewMatrix.Rotate(deathAnimationTimer);
        if (deathAnimationTimer < 2)
        {
            viewMatrix.Scale(deathAnimationTimer, 1, 1 );
        }
        else
        {
            viewMatrix.Scale(2-(deathAnimationTimer-3),1,1);
        }
        
    }
    if (deathAnimationTimer > 3)
    {
        //stop the annimation
        animatingDeath = false;
        
    }
    
    
    
    DrawSpriteUnorderedSheetSprite(program, &deathBackground);
    
    glBindTexture(GL_TEXTURE_2D, deathButton.textureID);
    DrawSpriteUnorderedSheetSprite(program, &deathButton);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    string DisplayedString = "Number Of Deaths: ";
    DisplayedString = DisplayedString + std::to_string(theLevelLoader->getDeathCount());
    
    std::cout << DisplayedString << std::endl;
    DrawText(program, deathText.textureID, DisplayedString , 20.0f, 0.0,-150.0f,60.0f);

    
    DrawText(program, deathText.textureID, "You Lost - Play Again?", 13.0f, 0.0f,-140.0f,0.0);
    
    
}

void renderWonScene(ShaderProgram* program)
{
    //drawing Triangles
    viewMatrix.identity();

    program->setModelMatrix(menuModelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
    wonModelMatrix.identity(); //resets to initial position
    
    DrawSpriteUnorderedSheetSprite(program, &menuBackground);
    
    glBindTexture(GL_TEXTURE_2D, menuButton.textureID);
    DrawSpriteUnorderedSheetSprite(program, &menuButton);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    DrawText(program, menuText.textureID, "Next Level?", 20, 0.000f,-130.0,0.0);
    
}

void renderCompletedScene(ShaderProgram* program)
{
    //drawing Triangles
    viewMatrix.identity();
    
    program->setModelMatrix(completedModelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
    completedModelMatrix.identity(); //resets to initial position
    
    DrawSpriteUnorderedSheetSprite(program, &completedBackground);
    
    glBindTexture(GL_TEXTURE_2D, completedButton.textureID);
    DrawSpriteUnorderedSheetSprite(program, &completedButton);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    
    DrawText(program, deathText.textureID, "Congratulations!!!" , 20.0f, 0.0,-150,100);

    
    string DisplayedString = "Number Of Deaths: ";
    DisplayedString = DisplayedString + std::to_string(theLevelLoader->getDeathCount());
    
    std::cout << DisplayedString << std::endl;
    DrawText(program, deathText.textureID, DisplayedString , 20.0f, 0.0,-150.0f,60.0f);
    
    DrawText(program, completedText.textureID, "(q)uit, (m)enu?", 15, 0.000f,-100,0.0);
    
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
                    theLevelLoader->resetLevel();
                    animatingDeath = true;

                    state = 3;

                }
                
            }
            else if (errorCodeReceived == 2)
            {
                std::cout << "Out of bounds on y" << std::endl;
                
                if (curEntity->EntityType == 0 || curEntity->EntityType == 0)
                {
                    theLevelLoader->resetPlayerPosition(curEntity);
                    theLevelLoader->addDeath();
                    theLevelLoader->resetLevel();
                    animatingDeath = true;
                    state = 3;
                }
                

                
            }
            else if (errorCodeReceived == 3)
            {
                std::cout << "Touched Death Tile" << std::endl;
                theLevelLoader->resetPlayerPosition(curEntity);
                theLevelLoader->addDeath();
                theLevelLoader->resetLevel();
                animatingDeath = true;
                state = 3;
            }
            
        }
        
    }

    
    
    // Check if the player won
    
    
    
    Tile* curPlayerTile = theLevelLoader->getLevelDataAtIndex(player->tileMapPosition.gety(), player->tileMapPosition.getx());
    
    if (curPlayerTile->getTileValue() == 55)
    {
        
        std::cout << "PLAYER WON" << std::endl;
        state = 2;
        animatingWin = true;
        
    }
    
    //do player collisions with entities.
    
    for (int i = 0; i < theLevelLoader->getNumEntities(); i++)
    {
        bool isColliding = false;
        
        Entity* curEntity = theLevelLoader->getEntityForIndex(i);
        
        if (i > 0)
        {
            
            isColliding = player->collidesWithEntity(curEntity);

        }
        
        if (isColliding == true)
        {
            std::cout << "COLLISION" << std::endl;
            //die
            theLevelLoader->resetPlayerPosition(curEntity);
            theLevelLoader->addDeath();
            theLevelLoader->resetLevel();
            animatingDeath = true;
            
            state = 3;
            
        }

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
    
    //animatingMenu = true;
    
    menuAnimationTimer = menuAnimationTimer + elapsed;
    
    viewMatrix.identity();
    updateMenu(theProgram, elapsed);
    
    renderMenu(theProgram);
    
    
}

void DrawDeath(float elapsed)
{
    
    deathAnimationTimer = deathAnimationTimer + elapsed;

    
    viewMatrix.identity();
    updateDeathScene(theProgram, elapsed);
    
    renderDeathScene(theProgram);
    
    
}


void DrawWon(float elapsed)
{
    
    winAnimationTimer = winAnimationTimer + elapsed;
    
    viewMatrix.identity();
    updateWonScene(theProgram, elapsed);
    
    renderWonScene(theProgram);
    
    
}

void DrawCompleted(float elapsed)
{
    
    winAnimationTimer = winAnimationTimer + elapsed;
    
    viewMatrix.identity();
    updateCompletedScene(theProgram, elapsed);
    
    renderCompletedScene(theProgram);
    
    
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
            
            std::cout << curLevel << " " ;
        }
        std::cout << std::endl;
    }
    
    
    
    SDL_Event event;

    while (!done)
    {
        if (!animatingMenu) {
            menuAnimationTimer = 0.0f;
        }

        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        if (state == 1) {
            SDL_ShowCursor(0);

        }
        else
        {
            SDL_ShowCursor(1);
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
            else if (state == 2)
            {
                
                DrawWon(FIXED_TIMESTEP);
                
            }
            else if (state == 3)
            {
                DrawDeath(FIXED_TIMESTEP);
            }
            else if (state == 4)
            {
                DrawCompleted(FIXED_TIMESTEP);
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
        else if (state == 2)
        {
            DrawWon(fixedElapsed);
        }
        else if (state == 3)
        {
            
            DrawDeath(fixedElapsed);
            
        }
        else if (state == 4)
        {
            DrawCompleted(fixedElapsed);
        }
        else
        {
            std::cout << "Undefined State" << std::endl;
        }

        SDL_GL_SwapWindow(displayWindow);

        
    }
    
    SDL_Quit();

    return 0;
}
