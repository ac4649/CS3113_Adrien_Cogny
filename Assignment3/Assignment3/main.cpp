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

#define SPRITESIDEPERCENT 10.0f

#define FIXED_TIMESTEP 0.0166666666f //60 FPS
#define MAX_TIMESTEPS 6 //maximum number of timesteps want to used


SDL_Window* displayWindow;


bool inMenu = true;
bool inGame = false;

float windowSizeHeight = 360;
float windowSizeWidth = 640;

float totalUnitsHeight = 5.0f;
float totalUnitsWidth = 8.88f;

float lastFrameTicks = 0.0f;

int numberEnemiesPerLine;


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
Entity player;
std::vector<Entity*> invaders;
std::vector<Bullet*> playerBullets; //keeps the bullets fired by the player
std::vector<Bullet*> enemyBullets; //keeps the enemy's bullets
std::vector<GLuint> invaderSprites;


// menu entity
Matrix menuModelMatrix;

Entity menuBackground;
Entity menuButton;
Entity menuText;

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
            (startX+(size+spacing) * i) + (-0.5f * size), 0.5f * size,
            (startX+(size+spacing) * i) + (-0.5f * size),  -0.5f * size,
            (startX+(size+spacing) * i) + (0.5f * size), 0.5f * size,
            (startX+(size+spacing) * i) + (0.5f * size),  -0.5f * size,
            (startX+(size+spacing) * i) + (0.5f * size),  0.5f * size,
            (startX+(size+spacing) * i) + (-0.5f * size),  -0.5f * size,
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
    program->setModelMatrix(displayedEntity->modelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
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

    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
}
/*
//spritesheet display
void DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX, int spriteCountY)
{
    float u = (float)(((int)index) % spriteCountX) / (float) spriteCountX;
    float v = (float)(((int)index) / spriteCountX) / (float) spriteCountY;
    float spriteWidth = 1.0/(float)spriteCountX;
    float spriteHeight = 1.0/(float)spriteCountY;
    GLfloat texCoords[] = {
        u, v+spriteHeight,
        u+spriteWidth, v,
        u, v,
        u+spriteWidth, v,
        u, v+spriteHeight,
        u+spriteWidth, v+spriteHeight
    };
    
    float vertices[] =
    {
        -0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f,
        -0.5f,-0.5f,
        0.5f, -0.5f};
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
}
*/

//spritesheet display
// if the entity contains the sprite location as well as the height and width on the sprite sheet.
void DrawSpriteUnorderedSheetSprite(ShaderProgram *program, Entity *displayedEntity)
{
    
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

/*
//if location, height and width on the sprite sheet need to be specified.
void DrawSpriteUnorderedSheetSprite(ShaderProgram *program, Entity* displayedEntity, int spriteX, int spriteY, int spriteHeight, int spriteWidth)
{
    
    float u = 1.0/(float)spriteX;
    float v = 1.0/(float)spriteY;
    float spriteNormalizedWidth = 1.0/(float)spriteWidth;
    float spriteNormalizedHeight = 1.0/(float)spriteHeight;
    
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
    //std::cout << "Drawing from unordered sprite sheet" << std::endl;
    
}

*/
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
    
    
    //create the player
    player.x = 0;
    player.y = -totalUnitsHeight/2 + totalUnitsHeight/25;
    player.height = SPRITESIDEPERCENT/100*totalUnitsHeight;
    player.width = SPRITESIDEPERCENT/100*totalUnitsHeight;
    
    player.textureID = theMainTextureID;
    player.textureLocationX = 0;
    player.textureLocationY = 941;
    player.textureHeight = 75;
    player.textureWidth = 112;
    player.textureSheetHeight = 1024;
    player.textureSheetWidth = 1024;
    
    
    player.speed = 5;
    player.direction_x = 1;
    player.direction_y = 0;
    player.fireSpeed = 1; // fires 1 bullet every second.
    player.fireDirection_Y = 1.0;
    player.fireDirection_X = 0.0;
    player.bulletTexture = theMainTextureID;
    
    
    //Add the ennemies to the ennemies vector
    
    float offset = SPRITESIDEPERCENT/100*totalUnitsWidth*10/100;
    
    std::cout << (SPRITESIDEPERCENT/100*totalUnitsWidth) << std::endl;
    std::cout << totalUnitsWidth << std::endl;
    std::cout << totalUnitsWidth/(SPRITESIDEPERCENT/100*totalUnitsWidth + offset) << std::endl;
    std::cout << (SPRITESIDEPERCENT/100*totalUnitsWidth + offset)-totalUnitsWidth/2 << std::endl;
    
    
    numberEnemiesPerLine = totalUnitsWidth/(SPRITESIDEPERCENT/100*totalUnitsWidth + offset);
    int numberLines = (totalUnitsHeight/4)/(SPRITESIDEPERCENT/100*totalUnitsHeight + offset);
    
    std::cout << numberEnemiesPerLine << std::endl;
    std::cout << numberLines << std::endl;
    
    //for (int j = 0; j < (totalUnitsHeight/4)/(SPRITESIDEPERCENT/100*totalUnitsHeight); j++)
    //{

        for (int i = 0; i < totalUnitsWidth/(SPRITESIDEPERCENT/100*totalUnitsWidth + offset); i++)
        {
            
            Entity* enemy = new Entity;
            
            //create the enemy
            enemy->x = i*(SPRITESIDEPERCENT/100*totalUnitsWidth + offset)-totalUnitsWidth/2;
            enemy->y = totalUnitsHeight/2-(SPRITESIDEPERCENT/100*totalUnitsHeight);//-j*SPRITESIDEPERCENT/100*totalUnitsHeight;
            
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
            /*
            if (j%2 == 0)
            {
                enemy->direction_x = 1;
            }
            else
            {
                enemy->direction_x = -1;
            }
             */
            
            enemy->direction_y = 0;
            enemy->fireSpeed = rand()%2; // fires random timeing.
            enemy->fireDirection_Y = -1.0;
            enemy->fireDirection_X = 0.0;
            
            enemy->bulletTexture = theMainTextureID;

            
            invaders.push_back(enemy);
            
        }
    //}
    
    std::cout << "Done Setup" << std::endl;
    
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
    
    glBindTexture(GL_TEXTURE_2D, menuBackground.textureID);
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
                // player fire bullet in the positive x direction
                
                Bullet* theFiredBullet = player.fire();
                
                //std::cout << "Player: " << theFiredBullet << std::endl;
                
                if (theFiredBullet != nullptr)
                {
                    //if a bullet was fired;
                    playerBullets.push_back(theFiredBullet);

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
        player.x = player.x - player.speed*player.direction_x*elapsed;
        
    }
    else if(keys[SDL_SCANCODE_RIGHT])
    {
        //make player go right
        player.x = player.x + player.speed*player.direction_x*elapsed;
        
    }
    
    for (int i = 0; i < invaders.size(); i++)
    {
        //move the invaders and do the detection of the invaders with the walls
        
        invaders[i]->x = invaders[i]->x + invaders[i]->speed*elapsed*invaders[i]->direction_x;
        
        //do side collisions
        if (invaders[i]->x + invaders[i]->width/2 > totalUnitsWidth/2)
        {
            //colides with right side
            invaders[i]->y = invaders[i]->y - invaders[i]->height;
            invaders[i]->direction_x = -invaders[i]->direction_x;
            
            float penetration = fabs(totalUnitsWidth/2 - invaders[i]->x);
            //std::cout << "Penetration = " << penetration << std::endl;
            
            invaders[i]->x = invaders[i]->x - penetration; // remove the collision from the wall
            

            
        }
        if (invaders[i]->x - invaders[i]->width/2 < (-totalUnitsWidth/2))
        {
            
            float penetration = fabs(invaders[i]->x+totalUnitsWidth/2);
            //std::cout << "Penetration = " << penetration << std::endl;
            
            //colides with left side
            
            invaders[i]->y = invaders[i]->y - invaders[i]->height;
            invaders[i]->direction_x = -invaders[i]->direction_x;
            
            invaders[i]->x = invaders[i]->x + penetration; // remove the collision from the wall
            
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
                
                delete invaders[i];
                delete playerBullets[j];
                invaders.erase(invaders.begin()+i);
                playerBullets.erase(playerBullets.begin()+j);
                
                //std::cout << "Number of Invaders left = " << invaders.size() << std::endl;
                
                
                
            }
            
            
            
        }
        
        
    }
    
    for (int i = 0; i < invaders.size(); i++)
    {
        //std::cout << "Fire" << std::endl;
        Bullet* theFiredBullet = invaders[i]->fire();
        
        if (theFiredBullet != nullptr)
        {
            //std::cout << "New Enemy Bullet" << std::endl;
            //if a bullet was fired;
            //std::cout << "Enemy: " << theFiredBullet << std::endl;
            
            enemyBullets.push_back(theFiredBullet);
            
            
        }
    }
    
    
    //check collision detection between player and enemy bullets
    for (int i = 0; i < enemyBullets.size(); i++)
    {
        
        //std::cout << "Enemy Bullet right Side: " << enemyBullets[i]->x + enemyBullets[i]->width/2 << " vs Player left side" << player.x-player.width/2  << std::endl;
        
        //std::cout << "Enemy Bullet left Side: " << enemyBullets[i]->x - enemyBullets[i]->width/2 << " vs Player Right side x" << player.x-player.width/2  << std::endl;
        
        //std::cout << "Enemy Bullet top Side: " << enemyBullets[i]->y + enemyBullets[i]->height/2  << " vs Player bottom side x" << player.y-player.height/2  << std::endl;
        
        
        //std::cout << "Enemy Bullet bottom Side: " << enemyBullets[i]->y - enemyBullets[i]->height/2  << " vs Player top side x" << player.y + player.height/2  << std::endl;
        
        if (enemyBullets[i]->x + enemyBullets[i]->width/2 < player.x-player.width/2 || enemyBullets[i]->x - enemyBullets[i]->width/2 > player.x-player.width/2 || enemyBullets[i]->y + enemyBullets[i]->height/2 < player.y-player.height/2 || enemyBullets[i]->y - enemyBullets[i]->height/2 > player.y + player.height/2)
        {
            //no collision
        }
        else
        {
            std::cout << "YOU GOT SHOT by bullet " << i << std::endl;
            std::cout << "Player (xmax, xmin, ymax, ymin): (" << player.x+player.width/2 << ", " << player.x-player.width/2 << ", " << player.y + player.height/2 << ", " << player.y - player.height/2 << ")" << std::endl;
            std::cout << "Bullet (xmax, xmin, ymax, ymin): (" << enemyBullets[i]->x+enemyBullets[i]->width/2 << ", " << enemyBullets[i]->x-enemyBullets[i]->width/2 << ", " << enemyBullets[i]->y + enemyBullets[i]->height/2 << ", " << enemyBullets[i]->y - enemyBullets[i]->height/2 << ")" << std::endl;
            delete enemyBullets[i];
            enemyBullets.erase(enemyBullets.begin()+i);

        }
    }
    
    
}

void renderGame(ShaderProgram* program)
{
    
    program->setModelMatrix(menuModelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
    menuModelMatrix.identity(); //resets to initial position
    
    
    glBindTexture(GL_TEXTURE_2D, player.textureID);
    DrawSpriteUnorderedSheetSprite(program, &player);
    
    
    for (int i = 0; i < invaders.size(); i++)
    {
        //std::cout << "Invader " << i << " x = " << invaders[i]->x << " y = " << invaders[i]->y << std::endl;
        glBindTexture(GL_TEXTURE_2D, invaders[i]->textureID);
        DrawSpriteUnorderedSheetSprite(program, invaders[i]);

    }
    
    for (int i = 0; i < playerBullets.size(); i++)
    {
        //std::cout << "Bullet " << i << " x = " << bullets[i]->x << " y = " << bullets[i]->y << std::endl;
        DrawBullet(program, playerBullets[i]);
        
    }
    
    for (int i = 0; i < enemyBullets.size(); i++)
    {
        //std::cout << "Bullet " << i << " x = " << bullets[i]->x << " y = " << bullets[i]->y << std::endl;
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
    
    bool done = false;
    while (!done) {
        
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
            
            //std::cout << "Number of Player Bullets = " << bullets.size() << std::endl;
            //std::cout << "Number of Enemy Bullets = " << enemyBullets.size() << std::endl;
            
            //std::cout << "PAUSE" << std::endl;
            
        }
        else
        {
            //unrecognized state
            done = true;
            std::cout << "Something went wrong - Unrecognized Game State" << std::endl;
        }
        
    }
    
    SDL_Quit();
    return 0;
}
