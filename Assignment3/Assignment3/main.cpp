#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "Assignment3.app/Contents/Resources/"
#endif


#include "Matrix.h"
#include "ShaderProgram.h"
#include <Vector>
#include "Entity.h"
#include "Bullet.h"


//SOUND

#include <SDL_mixer.h>

Mix_Chunk *playerFireSound;

//

#define SPRITESIDEPERCENT 10.0f

#define FIXED_TIMESTEP 0.0166666666f //60 FPS
#define MAX_TIMESTEPS 6 //maximum number of timesteps want to used


SDL_Window* displayWindow;

bool done = false;
bool inMenu = true;
bool inGame = false;

bool won = false;
bool lost = false;

float windowSizeHeight = 360;
float windowSizeWidth = 640;

float totalUnitsHeight = 5.0f;
float totalUnitsWidth = 8.88f;

float lastFrameTicks = 0.0f;

int numberEnemiesPerLine;
int curLeftColumn;
int curRightColumn;

int score; 

Matrix projectionMatrix;
Matrix viewMatrix;

SDL_Event event;

//load texture function
GLuint LoadTexture(const char *image_path)
{
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

// game entities
Entity* player;
std::vector<Entity*> invaders;
std::vector<Bullet*> playerBullets; //keeps the bullets fired by the player
std::vector<Bullet*> enemyBullets; //keeps the enemy's bullets
std::vector<GLuint> invaderSprites;

/*
// the bar entities are going to be used for collision detection between the enemies and the size, they will not be displayed as they are simply the limits of the playing field.
Entity* topBar;
Entity* bottomBar;
Entity* leftBar;
Entity* rightBar;
*/

// menu entity
Matrix menuModelMatrix;

Entity menuBackground;
Entity menuButton;
Entity menuText;


void cleanUp()
{
    //remove any ennemies and bullets
    
    for (int i = 0 ; i < invaders.size(); i++) {
        if (invaders[i] != nullptr)
        {
            delete invaders[i];
        }
        invaders[i] = nullptr;

    }
    invaders.clear();
    for (int i = 0; i < enemyBullets.size(); i++) {
        if (enemyBullets[i] != nullptr)
        {
            delete enemyBullets[i];
        }
        enemyBullets[i] = nullptr;

    }
    enemyBullets.clear();
    for (int i = 0; i < playerBullets.size(); i++) {
        if (playerBullets[i] != nullptr)
        {
            delete playerBullets[i];
        }
        playerBullets[i] = nullptr;
    }
    playerBullets.clear();
    
    if (player != nullptr)
    {
        delete player;
        player = nullptr;
    }
    
    
}

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



//DrawBullet
void DrawBullet(ShaderProgram *program, Bullet *displayedEntity)
{
    
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
        displayedEntity->x-displayedEntity->width/2, displayedEntity->y-displayedEntity->height/2,
        displayedEntity->x+displayedEntity->width/2, displayedEntity->y+displayedEntity->height/2,
        displayedEntity->x-displayedEntity->width/2, displayedEntity->y+displayedEntity->height/2,
        
        displayedEntity->x+displayedEntity->width/2, displayedEntity->y+displayedEntity->height/2,
        displayedEntity->x-displayedEntity->width/2, displayedEntity->y-displayedEntity->height/2,
        displayedEntity->x+displayedEntity->width/2, displayedEntity->y-displayedEntity->height/2
        
    };
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
 
 
}

//spritesheet display
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
        displayedEntity->x-displayedEntity->width/2, displayedEntity->y-displayedEntity->height/2,
        displayedEntity->x+displayedEntity->width/2, displayedEntity->y+displayedEntity->height/2,
        displayedEntity->x-displayedEntity->width/2, displayedEntity->y+displayedEntity->height/2,
        
        displayedEntity->x+displayedEntity->width/2, displayedEntity->y+displayedEntity->height/2,
        displayedEntity->x-displayedEntity->width/2, displayedEntity->y-displayedEntity->height/2,
        displayedEntity->x+displayedEntity->width/2, displayedEntity->y-displayedEntity->height/2
        
    };
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
}

//setup function
ShaderProgram *setup() // will return the shaderProgram pointer
{
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Space Invaders - Adrien Cogny", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
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
    
    //load menu textures
    menuBackground.textureID = LoadTexture("purpleBackground.png");
    menuText.textureID = LoadTexture("font1.png");
    menuButton.textureID = LoadTexture("sheet.png");

    
    //load all the textures for the enemies into
    
    int theMainTextureID = LoadTexture("sheet.png");
    
    player = new Entity();
    //create the player
    player->x = 0;
    player->y = -totalUnitsHeight/2 + totalUnitsHeight*1/100;
    player->height = SPRITESIDEPERCENT/100*totalUnitsHeight;
    player->width = SPRITESIDEPERCENT/100*totalUnitsHeight;
    
    player->textureID = theMainTextureID;
    player->textureLocationX = 0;
    player->textureLocationY = 941;
    player->textureHeight = 75;
    player->textureWidth = 112;
    player->textureSheetHeight = 1024;
    player->textureSheetWidth = 1024;
    
    
    player->speed = 5;
    player->direction_x = 1;
    player->direction_y = 0;
    player->fireSpeed = 1; // fires 1 bullet every second.
    player->fireDirection_Y = 1.0;
    player->fireDirection_X = 0.0;
    player->bulletTexture = theMainTextureID;
    
    
    //Add the ennemies to the ennemies vector
    
    //float offset = SPRITESIDEPERCENT/100*totalUnitsWidth*50/100;
    
    //std::cout << (SPRITESIDEPERCENT/100*totalUnitsWidth) << std::endl;
    //std::cout << totalUnitsWidth << std::endl;
    //std::cout << totalUnitsWidth/(SPRITESIDEPERCENT/100*totalUnitsWidth) + offset << std::endl;
    //std::cout << (SPRITESIDEPERCENT/100*totalUnitsWidth) + offset -totalUnitsWidth/2 << std::endl;
    
    
    numberEnemiesPerLine = totalUnitsWidth/(2*(SPRITESIDEPERCENT/100*totalUnitsWidth));
    int numberLines = 4;
    
    std::cout << numberEnemiesPerLine << std::endl;
    //std::cout << offset << std::endl;
    std::cout << numberLines << std::endl;
    
    for (int j = 0; j < 4; j++)
    {

        for (int i = 0; i < numberEnemiesPerLine; i++)
        {
            
            Entity* enemy = new Entity;
            
            //create the enemy
            enemy->x = i*SPRITESIDEPERCENT/100*totalUnitsWidth;
            enemy->y = totalUnitsHeight/2-(SPRITESIDEPERCENT/100*totalUnitsHeight)+j*SPRITESIDEPERCENT/100*totalUnitsHeight;
            
            //std::cout << "New Enemy (" << i << "," << j << ") x = " << enemy->x << " y = " << enemy->y << std::endl;
            
            enemy->height = SPRITESIDEPERCENT/100*totalUnitsHeight;
            enemy->width = SPRITESIDEPERCENT/100*totalUnitsWidth;
            
            enemy->textureID = theMainTextureID;
            enemy->textureLocationX = 444;
            enemy->textureLocationY = 0;
            enemy->textureHeight = 91;
            enemy->textureWidth = 91;
            enemy->textureSheetHeight = 1024;
            enemy->textureSheetWidth = 1024;
            
            enemy->speed = 1;
            enemy->direction_x = 1;
            
            enemy->direction_y = 0;
            enemy->fireSpeed = rand()%10 + 1; // fires random timeing but still fires at a min rate of 1
            enemy->fireDirection_Y = -1.0;
            enemy->fireDirection_X = 0.0;
            
            enemy->bulletTexture = theMainTextureID;
            
            enemy->points = 10;
            
            enemy->columnNumber = i;

            
            invaders.push_back(enemy);
            
        }
    }
    
    curRightColumn = numberEnemiesPerLine-1;
    curLeftColumn = 0;
    std::cout << "Done Setup" << std::endl;
    
    
    
    
    //Initialize sound
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    playerFireSound = Mix_LoadWAV("sfx.wav");
    
    
    
    return program;
}


bool ProcessMenuEvents()
{
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            return true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {

                    inMenu = false;
                    inGame = true;
                    std::cout << "Starting Game" << std::endl;
            }
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            
            float unitX = (((float)event.button.x / windowSizeWidth) * totalUnitsWidth ) - totalUnitsWidth/2.0;
            float unitY = (((float)(360-event.button.y) / windowSizeHeight) * totalUnitsHeight ) - totalUnitsHeight/2.0;
            
            std::cout << "Mouse Click :" << unitX << ", " << unitY << std::endl;
            std::cout << "Target Click :" << menuButton.x << " +/- " << menuButton.width/2.0 << ", " << menuButton.y << " +/- " << menuButton.height/2.0 << std::endl;

            if (menuButton.x - menuButton.width/2.0 < unitX && unitX < menuButton.x + menuButton.width/2.0 )
            {
                if (menuButton.y - menuButton.height/2.0 < unitY && unitY < menuButton.y + menuButton.height/2.0 )
                {
                    inMenu = false;
                    inGame = true;
                    std::cout << "Starting Game" << std::endl;

                }
            }
            
        }
        
    }
    
    return false;
    
}

void updateMenu(ShaderProgram* program, float elapsed)
{
    //generate the background
    menuBackground.x = 0;
    menuBackground.y = 0;
    menuBackground.height = totalUnitsHeight;
    menuBackground.width = totalUnitsWidth;
    
    menuBackground.textureLocationX = 0;
    menuBackground.textureLocationY = 0;
    menuBackground.textureHeight = 256;
    menuBackground.textureWidth = 256;
    menuBackground.textureSheetHeight = 144;
    menuBackground.textureSheetWidth = 256;
    
    //generate the button
    menuButton.x = 0;
    menuButton.y = 0;
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
    
    DrawText(program, menuText.textureID, "PLAY SPACE INVADERS", 0.3f, 0.00000000001f,-2.5,0);
    
    SDL_GL_SwapWindow(displayWindow);
    glClear(GL_COLOR_BUFFER_BIT);
    
}

bool ProcessGameEvents(float elapsed)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            return true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {

                if (won == true || lost == true)
                {
                    // the user wants to play again
                    won = false;
                    lost = false;
                    cleanUp();
                    setup();
                }
                else
                {
                    // player fire bullet in the positive x direction
                    
                    Bullet* theFiredBullet = player->fire();
                    
                    //std::cout << "Player: " << theFiredBullet << std::endl;

                    if (theFiredBullet != nullptr)
                    {
                        
                        Mix_PlayChannel(-1, playerFireSound, 0);

                        //if a bullet was fired;
                        playerBullets.push_back(theFiredBullet);

                    }
                }
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_E)
            {
                std::cout << "E is pressed" << std::endl;
                std::cout << won << std::endl;
                std::cout << lost << std::endl;

                if (won == true || lost == true)
                {
                    std::cout << "Terminating Game" << std::endl;
                    done = true;
                    inGame = false;
                    
                }
            }
        }
        
        
    }
    
    return false;
}

void updateGame(ShaderProgram* program, float elapsed)
{

    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if(keys[SDL_SCANCODE_LEFT])
    {
        //make player go left
        player->x = player->x - player->speed*player->direction_x*elapsed;
        
    }
    else if(keys[SDL_SCANCODE_RIGHT])
    {
        //make player go right
        player->x = player->x + player->speed*player->direction_x*elapsed;
        
    }
    
    //player side collisions
    if (player->x < -totalUnitsWidth/2) {
        float penetration = fabs(player->x+totalUnitsWidth/2);
        player->x = player->x + penetration + 0.05/100*totalUnitsWidth/2;
    }
    
    if (player->x > totalUnitsWidth/2) {
        float penetration = fabs(player->x-totalUnitsWidth/2);
        player->x = player->x - penetration - 0.05/100*totalUnitsWidth/2;
    }
    
    for (int i = 0; i < invaders.size(); i++)
    {
        //move the invaders and do the detection of the invaders with the walls
        
        invaders[i]->x = invaders[i]->x + invaders[i]->speed*elapsed*invaders[i]->direction_x;
        
        //do side collisions
        if (invaders[i]->columnNumber == curRightColumn)
        {
            //std::cout << invaders[i]->columnNumber << std::endl;
            //possible collision with the right side;
            if (invaders[i]->x + invaders[i]->width/2 > totalUnitsWidth/2)
            {
                //colides with right side
                //
                //make all of the invaders go down one
                for (int j = 0; j < invaders.size(); j++)
                {
                    invaders[j]->y = invaders[j]->y - invaders[j]->height;
                    invaders[j]->direction_x = -invaders[j]->direction_x;
                    
                    float penetration = fabs(totalUnitsWidth/2 - invaders[i]->x);
                    std::cout << "left side = " << penetration << std::endl;
                    
                    invaders[j]->x = invaders[j]->x - penetration - 0.05/100*totalUnitsHeight/2;
                    
                }
                
                
            }
            
        }
        else if (invaders[i]->columnNumber == curLeftColumn)
        {
            if (invaders[i]->x - invaders[i]->width/2 < (-totalUnitsWidth/2))
            {
                
                
                //std::cout << "Penetration = " << penetration << std::endl;
                
                //colides with left side
                for (int j = 0; j < invaders.size(); j++)
                {
                    invaders[j]->y = invaders[j]->y - invaders[j]->height;
                    invaders[j]->direction_x = -invaders[j]->direction_x;
                    
                    float penetration = fabs(invaders[i]->x+totalUnitsWidth/2);
                    std::cout << "right side = " << penetration << std::endl;
                    
                    invaders[j]->x = invaders[j]->x + penetration + 0.05/100*totalUnitsHeight/2;
                }
                
            }
        }
        
        //if the invaders fall down the screen, replace them at the top;
        if (invaders[i]->y < -totalUnitsHeight/2)
        {
            // below the screen -> reset the enemy at the top center of the screen
            invaders[i]->y = totalUnitsHeight/2+(SPRITESIDEPERCENT/100*totalUnitsHeight);
            invaders[i]->x = 0.0;
        }
        
    }
    
    //update bullets
    
    for (int i = 0; i < playerBullets.size(); i++) {
        
        playerBullets[i]->y = playerBullets[i]->y + playerBullets[i]->speed*elapsed*playerBullets[i]->direction_y;
        
        
        //if the bullets go off the screen, delete them
        
        if (playerBullets[i]->y - playerBullets[i]->height/2 > totalUnitsHeight/2)
        {
            //remove bullet;
            delete playerBullets[i];
            playerBullets.erase(playerBullets.begin()+i);
        }
        
        
    }
    
    for (int i = 0; i < enemyBullets.size(); i++) {
        
        enemyBullets[i]->y = enemyBullets[i]->y + enemyBullets[i]->speed*elapsed*enemyBullets[i]->direction_y;
        
        
        //if the bullets go off the screen, delete them
        
        if (enemyBullets[i]->y - enemyBullets[i]->height/2 < -totalUnitsHeight/2)
        {
            //remove bullet;
            delete enemyBullets[i];
            enemyBullets.erase(enemyBullets.begin()+i);
            
        }
        
        
    }
    
    //do collision detections on bullets and invaders to see if they hit any bullets

    
    for (int i = 0; i < invaders.size(); i++)
    {
        
        for (int j = 0; j < playerBullets.size(); j++)
        {
            //do circle-line collision on the bullets and invaders
            
            
            float invaderRadius = sqrtf(invaders[i]->width/2*invaders[i]->width/2+invaders[i]->height/2*invaders[i]->height/2);
            
            float deltaX = invaders[i]->x - playerBullets[j]->x;
            float deltaY = invaders[i]->y - playerBullets[j]->y;
                                             
            
            float distanceBetweenInvaderCenterAndBulletTop = sqrtf(deltaX*deltaX + deltaY*deltaY);
            
            //std::cout << "invaderRadius = " << invaderRadius << "distance between centers = " << distanceBetweenInvaderCenterAndBulletTop << std::endl;
            
            if  (distanceBetweenInvaderCenterAndBulletTop < invaderRadius)
            {
                
                
                std::cout << "HIT invader = " << i << std::endl;
                std::cout << "Bullet number = " << j << std::endl;
                //std::cout << "invaderRadius = " << invaderRadius << "\n distance between center and top of bullet = " << distanceBetweenInvaderCenterAndBulletTop << std::endl;
                
                score += invaders[i]->points;
                
                delete invaders[i];
                delete playerBullets[j];
                invaders.erase(invaders.begin()+i);
                playerBullets.erase(playerBullets.begin()+j);
                
                
                std::cout << "SCORE = " << score << std::endl;
                
                std::cout << "Number of Invaders left = " << invaders.size() << std::endl;
                
                
                
            }
            
            
            
        }
        
        
    }
    
    for (int i = 0; i < invaders.size(); i++)
    {
        //std::cout << "Fire" << std::endl;
        Bullet* theFiredBullet = invaders[i]->fire();
        
        if (theFiredBullet != nullptr)
        {
            
            enemyBullets.push_back(theFiredBullet);
            
            
        }
    }
    
    
    if (invaders.size() == 0) {
        std::cout << "YOU HAVE WON" << std::endl;
        DrawText(program, menuText.textureID, "YOU HAVE WON", 0.3f, 0.0000000000000001f,-2.5,0);
        DrawText(program, menuText.textureID, "Press space to play again,", 0.3f, 0.0000000000000001f, -4.0, -0.5);
        DrawText(program, menuText.textureID, "or (e)xit", 0.3f, 0.0000000000000001f, -4.0, -1);
        
        won = true;
        
    }
    else if (lost != true)
    {
        
        //show the score
        
        
        std::string theScoreString = "SCORE: " + std::to_string(score);
        DrawText(program, menuText.textureID, theScoreString , 0.3f, 0.00000000001f,-3.0,1.5);
        
    }
    
    //check collision detection between player and enemy bullets
    for (int i = 0; i < enemyBullets.size(); i++)
    {
        
        if (player->collidesWithBullet(enemyBullets[i]))
        {
            
            delete enemyBullets[i];
            enemyBullets.erase(enemyBullets.begin()+i);
            
            
            
            std::cout << "YOU HAVE LOST" << std::endl;
            
            lost = true;
            //done = true;
            
        }
        
    }
    
    if (lost == true)
    {

        //clear the screen
        
        
        
        DrawText(program, menuText.textureID, "YOU HAVE LOST ", 0.3f, 0.0000000000000001f,-4,0);
        DrawText(program, menuText.textureID, "Press space to play again,", 0.3f, 0.0000000000000001f, -4.0, -0.5);
        DrawText(program, menuText.textureID, "or (e)xit", 0.3f, 0.0000000000000001f, -4.0, -1);

        lost = true;
        score = 0;

    }
    
    
    //update the first and last columns
    bool curLeftColumnStillPresent = false;
    bool curRightColumnStillPresent = false;
    
    for (size_t i = 0; i < invaders.size(); i++)
    {
        if (invaders[i]->columnNumber == curLeftColumn) {
            curLeftColumnStillPresent = true;
        }
        else if (invaders[i]->columnNumber == curRightColumn)
        {
            curRightColumnStillPresent = true;
        }
    }
    
    if (curLeftColumnStillPresent == false) {
        curLeftColumn++;
    }
    if (curRightColumnStillPresent == false) {
        curRightColumn-- ;
    }
  
}

void renderGame(ShaderProgram* program)
{
    
    program->setModelMatrix(menuModelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
    menuModelMatrix.identity(); //resets to initial position
    
    
    glBindTexture(GL_TEXTURE_2D, player->textureID);
    DrawSpriteUnorderedSheetSprite(program, player);
    
    
    for (int i = 0; i < invaders.size(); i++)
    {

        DrawSpriteUnorderedSheetSprite(program, invaders[i]);

    }
    
    for (int i = 0; i < playerBullets.size(); i++)
    {

        DrawBullet(program, playerBullets[i]);
        
    }
    
    for (int i = 0; i < enemyBullets.size(); i++)
    {

        DrawBullet(program, enemyBullets[i]);
        
    }
    
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    SDL_GL_SwapWindow(displayWindow);
    glClear(GL_COLOR_BUFFER_BIT);
    
}




int main(int argc, char *argv[])
{

    
    ShaderProgram* program = setup();
    
    while (!done)
    {
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        
        float fixedElapsed = elapsed;
        
        if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS)
        {
            fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
        }

        
        if (inMenu)
        {
            //do menu interactions here
            done = ProcessMenuEvents();
            while  (fixedElapsed >= FIXED_TIMESTEP)
            {
                fixedElapsed -= FIXED_TIMESTEP;
                updateMenu(program,fixedElapsed);

            }
            updateMenu(program, fixedElapsed);
            
            renderMenu(program);
            
            
        }
        else if (inGame)
        {
            //do game interactions here
            done = ProcessGameEvents(elapsed);
            
            while  (fixedElapsed >= FIXED_TIMESTEP)
            {
                fixedElapsed -= FIXED_TIMESTEP;
                updateGame(program,fixedElapsed);
                
            }
            updateGame(program, fixedElapsed);
            
            renderGame(program);

            
        }
        else
        {
            //unrecognized state
            done = true;
            std::cout << "Something went wrong - Unrecognized Game State" << std::endl;
        }
        
    }
    
    cleanUp();
    
    
    SDL_Quit();
    return 0;
}
