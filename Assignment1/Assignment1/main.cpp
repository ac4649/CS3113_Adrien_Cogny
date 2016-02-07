#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>


#include "Matrix.h"
#include "ShaderProgram.h"




#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "Assignment1.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
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

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    
    
    //setup
    glViewport(0, 0, 640, 360);
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl",RESOURCE_FOLDER"fragment_textured.glsl");
    
    //textures
    GLuint medalTexture = LoadTexture("flat_medal1.png");
    
    Matrix medalProjectionMatrix;
    Matrix medalModelMatrix;
    Matrix medalViewMatrix;
    
    medalProjectionMatrix.setOrthoProjection(-3.55,3.55,-2.0f,2.0f,-1.0f, 1.0f);
    
    GLuint smileyTexture = LoadTexture("happy.gif");
    
    Matrix smileyProjectionMatrix;
    Matrix smileyModelMatrix;
    Matrix smileyViewMatrix;
    
    smileyProjectionMatrix.setOrthoProjection(-3.55,3.55,-2.0f,2.0f,-1.0f, 1.0f);
    
    
    GLuint diceTexture = LoadTexture("dieWhite_border1.png");
    
    
    Matrix diceProjectionMatrix;
    Matrix diceModelMatrix;
    Matrix diceViewMatrix;
    
    diceProjectionMatrix.setOrthoProjection(-3.55,3.55,-2.0f,2.0f,-1.0f, 1.0f);

    
    glUseProgram(program.programID);
    
    
    //x, y, z locations
    
    float medalX = 0.0f;
    float medalY = 0.0f;
    float medalZ = 0.0f;
    
    float smileyX = -0.8f;
    float smileyY = -0.8f;
    float smileyZ = 0.0f;

    
    
    
    
    float lastFrameTicks = 0.0f;

    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        //get the time
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        

        //generate the medal
        
        program.setModelMatrix(medalModelMatrix);
        program.setProjectionMatrix(medalProjectionMatrix);
        program.setViewMatrix(medalViewMatrix);
        
        glBindTexture(GL_TEXTURE_2D, medalTexture);
        
        float vertices[] = {-0.2, -0.2, 0.2, -0.2, 0.2, 0.2, -0.2, -0.2, 0.2, 0.2, -0.2, 0.2};
        
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float texCoord[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoord);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        float angleRotation = angleRotation + elapsed * (0.1*3.14159621/180);
        
        medalModelMatrix.Rotate(angleRotation);
        
        
        
        //generate the happy face texture
        program.setModelMatrix(smileyModelMatrix);
        program.setProjectionMatrix(smileyProjectionMatrix);
        program.setViewMatrix(smileyViewMatrix);
        
        glBindTexture(GL_TEXTURE_2D, smileyTexture);
        
        //start the texture in the center of the screen and then translate it down by 0.5
        float smileyVertex[] = {-0.2, -0.2, 0.2, -0.2, 0.2, 0.2, -0.2, -0.2, 0.2, 0.2, -0.2, 0.2};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, smileyVertex);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float smileyTexCoord[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, smileyTexCoord);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        smileyProjectionMatrix.identity();
        smileyProjectionMatrix.Translate(0, -0.5, 0.0);
        
        float smileyXTranslate = elapsed*1;
        smileyX = smileyX + smileyXTranslate;
        
        if (smileyX > 1.0)
        {
            smileyX = -smileyX;
        }
        
        smileyProjectionMatrix.Translate(smileyX,0, 0);
        
        
        
        //generate the dice
        program.setModelMatrix(diceModelMatrix);
        program.setProjectionMatrix(diceProjectionMatrix);
        program.setViewMatrix(diceViewMatrix);
        
        glBindTexture(GL_TEXTURE_2D, diceTexture);
        
        //start the texture in the center of the screen and then translate it up by 1.0
        float diceVertices[] = {-0.2, -0.2, 0.2, -0.2, 0.2, 0.2, -0.2, -0.2, 0.2, 0.2, -0.2, 0.2};
        
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, diceVertices);
        glEnableVertexAttribArray(program.positionAttribute);
        
        float dicetexCoord[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, dicetexCoord);
        glEnableVertexAttribArray(program.texCoordAttribute);
        
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        float diceScale = 1+sin(ticks);
        
        std::cout << diceScale << std::endl;
        diceModelMatrix.identity();
        diceModelMatrix.Translate(0.0, 1.0, 0.0);
        diceModelMatrix.Scale(diceScale, diceScale, 0.0);
        
        SDL_GL_SwapWindow(displayWindow);
        
    }
    
    SDL_Quit();
    return 0;
}
