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


SDL_Window* displayWindow;


bool inMenu = true;
bool inGame = false;

float windowSizeHeight = 360;
float windowSizeWidth = 640;

float totalUnitsHeight = 10.0;
float totalUnitsWidth = 10.0;

float lastFrameTicks = 0.0f;

Matrix projectionMatrix;
Matrix viewMatrix;


SDL_Event event;



class Entity {
public:
    
    void Draw();
    
    float x;
    float y;
    float width;
    float height;
    
    float rotation;
    
    int textureID;
    float textureLocationX;
    float textureLocationY;
    float textureWidth;
    float textureHeight;
    
    float textureSheetHeight;
    float textureSheetWidth;

    
    float speed;
    float direction_x;
    float direction_y;
    
    Matrix modelMatrix;
    
};


// game entities
Entity player;
std::vector<Entity> invaders;
std::vector<Entity> bullets;


// menu entity
Matrix menuModelMatrix;

Entity menuBackground;
Entity menuText;
Entity menuButton;


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

//spritesheet display
// if the entity contains the sprite location as well as the height and width on the sprite sheet.
void DrawSpriteUnorderedSheetSprite(ShaderProgram *program, Entity displayedEntity)
{
    float u = displayedEntity.textureLocationX / displayedEntity.textureSheetWidth;
    float v = displayedEntity.textureLocationY / displayedEntity.textureSheetHeight;
    float spriteNormalizedWidth = displayedEntity.textureWidth/displayedEntity.textureSheetWidth;
    float spriteNormalizedHeight = displayedEntity.textureHeight/displayedEntity.textureSheetHeight;
    
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
        displayedEntity.x-displayedEntity.width/2, displayedEntity.y-displayedEntity.height/2,
        displayedEntity.x+displayedEntity.width/2, displayedEntity.y+displayedEntity.height/2,
        displayedEntity.x-displayedEntity.width/2, displayedEntity.y+displayedEntity.height/2,
        
        displayedEntity.x+displayedEntity.width/2, displayedEntity.y+displayedEntity.height/2,
        displayedEntity.x-displayedEntity.width/2, displayedEntity.y-displayedEntity.height/2,
        displayedEntity.x+displayedEntity.width/2, displayedEntity.y-displayedEntity.height/2
        
    };
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
}
//if location, height and width on the sprite sheet need to be specified.
void DrawSpriteUnorderedSheetSprite(ShaderProgram *program, Entity displayedEntity, int spriteX, int spriteY, int spriteHeight, int spriteWidth)
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
        displayedEntity.x-displayedEntity.width/2, displayedEntity.y-displayedEntity.height/2,
        displayedEntity.x+displayedEntity.width/2, displayedEntity.y-displayedEntity.height/2,
        displayedEntity.x+displayedEntity.width/2, displayedEntity.y+displayedEntity.height/2,
        
        displayedEntity.x+displayedEntity.width/2, displayedEntity.y+displayedEntity.height/2,
        displayedEntity.x-displayedEntity.width/2, displayedEntity.y-displayedEntity.height/2,
        displayedEntity.x-displayedEntity.width/2, displayedEntity.y+displayedEntity.height/2
    };
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //std::cout << "Drawing from unordered sprite sheet" << std::endl;
    
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
    
    
    return program;
}


bool ProcessMenuEvents()
{
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            return true;
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
    
    menuBackground.textureID = LoadTexture("purpleBackground.png");
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
    
    menuButton.textureID = LoadTexture("sheet.png");
    menuButton.textureLocationX = 0;
    menuButton.textureLocationY = 0;
    menuButton.textureHeight = 39;
    menuButton.textureWidth = 222;
    menuButton.textureSheetHeight = 1024;
    menuButton.textureSheetWidth = 1024;
    
    
    //generate the text;
    
    
}

void renderMenu(ShaderProgram* program)
{
    //drawing Triangles
    
    program->setModelMatrix(menuModelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
    menuModelMatrix.identity(); //resets to initial position
    
    glBindTexture(GL_TEXTURE_2D, menuBackground.textureID);
    DrawSpriteUnorderedSheetSprite(program, menuBackground);
    
    glBindTexture(GL_TEXTURE_2D, menuButton.textureID);
    DrawSpriteUnorderedSheetSprite(program, menuButton);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    SDL_GL_SwapWindow(displayWindow);
    glClear(GL_COLOR_BUFFER_BIT);
    
}

bool ProcessGameEvents()
{
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            return true;
        }
        
        
        
        
    }
    
    return false;
}

void updateGame(ShaderProgram* program, float elapsed)
{
    
    
}

void renderGame(ShaderProgram* program)
{
    
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
        
        if (inMenu)
        {
            //do menu interactions here
            done = ProcessMenuEvents();
            updateMenu(program,elapsed);
            renderMenu(program);
            
            
        }
        else if (inGame)
        {
            //do game interactions here
            done = ProcessGameEvents();
            updateGame(program,elapsed);
            renderGame(program);
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
