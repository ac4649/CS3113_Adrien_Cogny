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

#define TILE_SIZE 25.0f
#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 7

//helper objects
LevelLoader* theLevelLoader;
ShaderProgram* theProgram;





//window properties
float windowSizeHeight = 360;
float windowSizeWidth = 640;

float totalUnitsHeight = 250.0f;
float totalUnitsWidth = 500.0f;




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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

//level texture
GLuint tileMapTexture;


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
    
    
    int levelHeight = theLevelLoader->getLevelHeight();
    int levelWidth = theLevelLoader->getLevelWidth();
    
    program->setModelMatrix(tileMapModelMatrix);
    tileMapModelMatrix.Translate(-totalUnitsWidth/2, totalUnitsHeight/2, 0);//translates the tile map so its 0,0 coincides with the top right corner of the screen
    
    

    return program;
}

void processEvents()
{
    
    
}

void DrawLevel()
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
            int levelDataXY = theLevelLoader->getLevelDataAtIndex(y,x);
            
            
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





int main(int argc, char *argv[])
{
    
    theProgram = setup();
    
    
    
    std::cout << "level Data: " << std::endl;
    
    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            int curLevel = theLevelLoader->getLevelDataAtIndex(i, j);
            std::cout << curLevel;
        }
        std::cout << std::endl;
    }
    
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);

        
        DrawLevel();
        
        SDL_GL_SwapWindow(displayWindow);

        
    }
    
    SDL_Quit();
    return 0;
}
