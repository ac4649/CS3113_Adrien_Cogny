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
#define RESOURCE_FOLDER "Assignment2.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;


//entity classs

class Entity {
    public:
    
        void Draw();
    
        float x;
        float y;
    
        float rotation;
    
        int textureID;
        float width;
        float height;
    
        float speed;
        float direction_x;
        float direction_y;
};


//globals
SDL_Event event;
bool done = false;

Entity leftPaddle;
Entity rightPaddle;
Entity ball;
Entity bottomWall;
Entity topWall;


Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;

Matrix LeftPaddleProjectionMatrix;
Matrix LeftPaddleModelMatrix;
Matrix LeftPaddleViewMatrix;

Matrix RightPaddleProjectionMatrix;
Matrix RightPaddleModelMatrix;
Matrix RightPaddleViewMatrix;

float windowSizeHeight = 360;
float windowSizeWidth = 640;

float totalUnitsHeight = 11.25;
float totalUnitsWidth = 20;

float lastFrameTicks = 0.0f;


void resetBoard()
{
    //set the initial coordinates of the 5 entities
    leftPaddle.x = 0.5-totalUnitsWidth/2;
    leftPaddle.y = 0;
    leftPaddle.width = totalUnitsWidth/100;
    leftPaddle.height = totalUnitsHeight/4;
    leftPaddle.speed = 5;
    leftPaddle.direction_x = 0;
    leftPaddle.direction_y = 1.5;
    
    rightPaddle.x = totalUnitsWidth/2-0.5;
    rightPaddle.y = 0;
    rightPaddle.width = totalUnitsWidth/100;
    rightPaddle.height = totalUnitsHeight/4;
    rightPaddle.speed = 5;
    rightPaddle.direction_x = 0;
    rightPaddle.direction_y = 1.5;
    
    ball.x = 0;
    ball.y = 0;
    ball.height = 1;
    ball.width = 1;
    ball.speed = 7;
    ball.direction_x = 0.5;
    ball.direction_y = 0.5;
    
    bottomWall.x = 0;
    bottomWall.y = -totalUnitsHeight/2;
    bottomWall.width = totalUnitsWidth-1;
    bottomWall.height = 1;
    bottomWall.speed = 0;
    bottomWall.direction_x = 0;
    bottomWall.direction_y = 0;
    
    topWall.x = 0;
    topWall.y = totalUnitsHeight/2;
    topWall.width = totalUnitsWidth-1;
    topWall.height = 1;
    topWall.speed = 0;
    topWall.direction_x = 0;
    topWall.direction_y = 0;
}


//setup function

ShaderProgram *setup() // will return the shaderProgram pointer
{
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSizeWidth, windowSizeHeight, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    #ifdef _WINDOWS
        glewInit();
    #endif
    
    glViewport(0, 0, 640, 360);
    ShaderProgram* program = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

    //set the projection Matrix
    projectionMatrix.setOrthoProjection(-totalUnitsWidth/2.0,totalUnitsWidth/2.0,-totalUnitsHeight/2,totalUnitsHeight/2,-1.0f, 1.0f);
    
    glUseProgram(program->programID);
    
    resetBoard();
    

    return program;
}

void ProcessEvents(ShaderProgram* program)
{

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if(event.type == SDL_KEYDOWN)
        {
            float ticks = (float)SDL_GetTicks()/1000.0f;
            float elapsed = ticks - lastFrameTicks;
            lastFrameTicks = ticks;
            
            if(event.key.keysym.scancode == SDL_SCANCODE_UP)
            {
                
                //if up is pressed,move the right paddle up
                rightPaddle.y = rightPaddle.y+rightPaddle.speed*rightPaddle.direction_y*elapsed;
                
                //std::cout << rightPaddle.y << std::endl;
            }
            else if(event.key.keysym.scancode == SDL_SCANCODE_DOWN)
            {
                
                //if up is pressed,move the right paddle up
                rightPaddle.y = rightPaddle.y-rightPaddle.speed*rightPaddle.direction_y*elapsed;
                
                //std::cout << rightPaddle.y << std::endl;
            }
        }
    }

    
}

void Update(ShaderProgram* program)
{
    
    //do all the movements and collision detections
    
    
    //get the time
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    
    //move everything
    ball.x = ball.x + ball.speed*ball.direction_x*elapsed;
    ball.y = ball.y + ball.speed*ball.direction_y*elapsed;
    
    
    
    //collision detection
    
    //check if the ball collides with walls

    if (ball.y + ball.height/2 > topWall.y - topWall.height/2) {
        //collided
        //std::cout << "Colided top" << std::endl;
        //reverse the y direction of the ball
        ball.direction_y = -ball.direction_y;
    }
    else if (ball.y - ball.height/2 < bottomWall.y + bottomWall.height/2)
    {
        //std::cout << "Colided bottom" << std::endl;
        //reverse the y direction of the ball;
        ball.direction_y = -ball.direction_y;
    }
    
    //check if the paddles collide with the top of the screen
    
    if (rightPaddle.y + rightPaddle.height/2 > totalUnitsHeight/2)
    {
        //collided
        //std::cout << "Right Paddle Colided top" << std::endl;
        //reverse the y direction of the ball
        rightPaddle.y = totalUnitsHeight/2 - rightPaddle.height/2;
    }
    else if (rightPaddle.y - rightPaddle.height/2 < -totalUnitsHeight/2)
    {
        //std::cout << "Right Paddle Colided bottom" << std::endl;
        //reverse the y direction of the ball;
        rightPaddle.y = -totalUnitsHeight/2 + rightPaddle.height/2;
    }
    
    if (leftPaddle.y + leftPaddle.height/2 > totalUnitsHeight/2)
    {
        //collided
        //std::cout << "Right Paddle Colided top" << std::endl;
        //reverse the y direction of the ball
        leftPaddle.y = totalUnitsHeight/2 - leftPaddle.height/2;
    }
    else if (leftPaddle.y - leftPaddle.height/2 < -totalUnitsHeight/2)
    {
        //std::cout << "Right Paddle Colided bottom" << std::endl;
        //reverse the y direction of the ball;
        leftPaddle.y = -totalUnitsHeight/2 + leftPaddle.height/2;
    }
    
    
    //check ball collisions
    
    if (ball.x + ball.width/2 > rightPaddle.x - rightPaddle.width/2)
    {
        if (ball.y - ball.height/2 < rightPaddle.y + rightPaddle.height/2)
        {
            //std::cout << "Colided with the right paddle" << std::endl;
            //collided with left paddle
            ball.direction_x = -ball.direction_x;
            ball.direction_y = -ball.direction_y;
            
        }
        else if (ball.y + ball.height/2 > rightPaddle.y - rightPaddle.height/2)
        {
            //std::cout << "Colided with the right paddle" << std::endl;
            //collided with left paddle
            ball.direction_x = -ball.direction_x;
            ball.direction_y = -ball.direction_y;
        }
        else
        {
            std::cout << "Didn't hit the paddle, it's out of bounds, reset" << std::endl;
            std::cout << "left Player wins" << std::endl;
            resetBoard();
        }
        
    }
    
    
    if (ball.x + ball.width/2 < leftPaddle.x - leftPaddle.width/2)
    {
        if (ball.y - ball.height/2 < leftPaddle.y + leftPaddle.height/2)
        {
            //std::cout << "Colided with the left paddle" << std::endl;
            //collided with left paddle
            ball.direction_x = -ball.direction_x;
            ball.direction_y = -ball.direction_y;
            
        }
        else if (ball.y + ball.height/2 > leftPaddle.y - leftPaddle.height/2)
        {
            //std::cout << "Colided with the left paddle" << std::endl;
            //collided with left paddle
            ball.direction_x = -ball.direction_x;
            ball.direction_y = -ball.direction_y;
        }
        else
        {
            std::cout << "Didn't hit the paddle, it's out of bounds, reset" << std::endl;
            std::cout << "right Player wins" << std::endl;

            resetBoard();
        }

        
    }

    
}

void Render(ShaderProgram* program)
{
    
    glClear(GL_COLOR_BUFFER_BIT);

    //drawing
    float vertices[] = // contains all the data for all the points
    {
        
        //ball display
        //ball left triangle
        ball.x-ball.width/2,ball.y-ball.height/2,
        ball.x-ball.width/2,ball.y+ball.height/2,
        ball.x+ball.width/2,ball.y-ball.height/2,
        
        // ball right triangle
        ball.x-ball.width/2,ball.y+ball.height/2,
        ball.x+ball.width/2,ball.y-ball.height/2,
        ball.x+ball.width/2,ball.y+ball.height/2,
        
        
        //left paddle display
        //left paddle left triangle
        leftPaddle.x-leftPaddle.width/2,leftPaddle.y-leftPaddle.height/2,
        leftPaddle.x-leftPaddle.width/2,leftPaddle.y+leftPaddle.height/2,
        leftPaddle.x+leftPaddle.width/2,leftPaddle.y-leftPaddle.height/2,
        
        // left paddle right triangle
        leftPaddle.x-leftPaddle.width/2,leftPaddle.y+leftPaddle.height/2,
        leftPaddle.x+leftPaddle.width/2,leftPaddle.y-leftPaddle.height/2,
        leftPaddle.x+leftPaddle.width/2,leftPaddle.y+leftPaddle.height/2,
        
        //right paddle display
        //right paddle left triangle
        rightPaddle.x-rightPaddle.width/2,rightPaddle.y-rightPaddle.height/2,
        rightPaddle.x-rightPaddle.width/2,rightPaddle.y+rightPaddle.height/2,
        rightPaddle.x+rightPaddle.width/2,rightPaddle.y-rightPaddle.height/2,
        
        // right paddle right triangle
        rightPaddle.x-rightPaddle.width/2,rightPaddle.y+rightPaddle.height/2,
        rightPaddle.x+rightPaddle.width/2,rightPaddle.y-rightPaddle.height/2,
        rightPaddle.x+rightPaddle.width/2,rightPaddle.y+rightPaddle.height/2,
        

        //top wall display
        //top wall left triangle
        topWall.x-topWall.width/2,topWall.y-topWall.height/2,
        topWall.x-topWall.width/2,topWall.y+topWall.height/2,
        topWall.x+topWall.width/2,topWall.y-topWall.height/2,
        
        // top wall right triangle
        topWall.x-topWall.width/2,topWall.y+topWall.height/2,
        topWall.x+topWall.width/2,topWall.y-topWall.height/2,
        topWall.x+topWall.width/2,topWall.y+topWall.height/2,

        //bottom wall display
        //bottom wall left triangle
        bottomWall.x-bottomWall.width/2,bottomWall.y-bottomWall.height/2,
        bottomWall.x-bottomWall.width/2,bottomWall.y+bottomWall.height/2,
        bottomWall.x+bottomWall.width/2,bottomWall.y-bottomWall.height/2,
        
        // bottom wall right triangle
        bottomWall.x-bottomWall.width/2,bottomWall.y+bottomWall.height/2,
        bottomWall.x+bottomWall.width/2,bottomWall.y-bottomWall.height/2,
        bottomWall.x+bottomWall.width/2,bottomWall.y+bottomWall.height/2
    };
    
    
    program->setModelMatrix(modelMatrix);
    program->setProjectionMatrix(projectionMatrix);
    program->setViewMatrix(viewMatrix);
    
    
    modelMatrix.identity(); //resets to initial position
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    
    glEnableVertexAttribArray(program->positionAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 60);
    
    SDL_GL_SwapWindow(displayWindow);


}

void CleanUp(ShaderProgram* program)
{
    
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(displayWindow);
    
    delete program;
    
}

int main(int argc, char *argv[])
{
    
    //setup
    ShaderProgram* program = setup();
    
    while (!done)
    {
        
        ProcessEvents(program);
        
        Update(program);
        
        Render(program);

    }
    
    CleanUp(program);
    
    SDL_Quit();
    return 0;
}
