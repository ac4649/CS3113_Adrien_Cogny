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


//helper objects
LevelLoader* theLevelLoader;





//window properties
float windowSizeHeight = 360;
float windowSizeWidth = 640;

float totalUnitsHeight = 5.0f;
float totalUnitsWidth = 8.88f;

//Global Matrices
Matrix projectionMatrix;
Matrix viewMatrix;




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

    
    theLevelLoader = new LevelLoader("level1.txt");

    return program;
}






int main(int argc, char *argv[])
{
    
    ShaderProgram* theProgram = setup();
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
