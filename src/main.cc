#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include <GL/glew.h>

#ifdef __APPLE__
    #include <gl.h>
#else
    #include <GL/gl.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "shader.hh"
#include "image_loader.hh"
#include "objects.hh"
#if defined(__APPLE__) && defined(MacApp)
    #include "MacApp.hh"
#endif

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float ORTHO_HEIGHT = 500;
const float ORTHO_WIDTH = ((float)SCREEN_WIDTH/SCREEN_HEIGHT)*ORTHO_HEIGHT;

SDL_Window* window;
SDL_GLContext context;
const Uint8 *keys;

int init() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetSwapInterval(1);

    window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if(!window) {
        printf("Window creation failed\n%s\n", SDL_GetError());
        return 1;
    }

    context = SDL_GL_CreateContext(window);
    if(!context) {
        printf("Context creation failed\n%s\n", SDL_GetError());
        return 1;
    }

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        printf("Glew initialization failure\n%s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

void basicAi(const Game::Ball &ball, Game::Paddle *paddle) {
    if(ball.x_speed() > 0) {
        GLfloat ball_center = ball.y() + (ball.height()/2);
        GLfloat paddle_center = paddle->y() + (paddle->height()/2);
        if(ball_center < paddle_center) {
            paddle->moveDown();
        }
        else {
            paddle->moveUp();
        }
    }
}

unsigned char wallCollision(const Game::Paddle &paddle) {
    if(paddle.y() < -ORTHO_HEIGHT) {
        return 0x01;
    }
    else if(paddle.y() + paddle.height() > ORTHO_HEIGHT) {
        return 0x02;
    }
    else {
        return 0;
    }
}

unsigned char wallCollision(const Game::Ball &ball) {
    unsigned char out = 0;

    if(ball.x() < -ORTHO_WIDTH) {
        out += 0x01;
    }
    if(ball.x() + ball.width() > ORTHO_WIDTH) {
        out += 0x02;
    }
    if(ball.y() < -ORTHO_HEIGHT) {
        out += 0x04;
    }
    if(ball.y() + ball.height() > ORTHO_HEIGHT) {
        out += 0x08;
    }

    return out;
}

void wallBounce(const unsigned char &collision, Game::Ball *ball) {
    if(ball) {
        if(collision & 0x01) {
            //ball->x(-ORTHO_WIDTH + (ball->x() + ORTHO_WIDTH));
            ball->x(-ORTHO_WIDTH);
            ball->x_speed(std::abs(ball->x_speed()));
        }
        if(collision & 0x02) {
            //ball->x(ORTHO_WIDTH - (ball->x()+ball->width() - ORTHO_WIDTH) - ball->width());
            ball->x(ORTHO_WIDTH - ball->width());
            ball->x_speed(-std::abs(ball->x_speed()));
        }
        if(collision & 0x04) {
            //ball->y(-ORTHO_HEIGHT - (ball->y() + ORTHO_HEIGHT));
            ball->y(-ORTHO_HEIGHT);
            ball->y_speed(std::abs(ball->y_speed()));
        }
        if(collision & 0x08) {
            //ball->y(ORTHO_HEIGHT - ball->height() - (ball->y()+ball->height() - ORTHO_HEIGHT));
            ball->y(ORTHO_HEIGHT - ball->height());
            ball->y_speed(-std::abs(ball->y_speed()));
        }
    }
}

void wallBounce(const unsigned char &collision, Game::Paddle *paddle) {
    if(paddle) {
        if(collision == 0x01) {
            paddle->y(-ORTHO_HEIGHT);
        }
        else if(collision == 0x02) {
            paddle->y(ORTHO_HEIGHT - paddle->height());
        }
    }
}

unsigned char paddleCollision(const Game::Paddle &paddle, const Game::Ball &ball) {
    if(ball.x() < paddle.x() + paddle.width() &&
            ball.x() + ball.width() > paddle.x() &&
            ball.y()  < paddle.y() + paddle.height() &&
            ball.y() + ball.height() > paddle.y()) {
        return 0x01;
    }
    else {
        return 0;
    }
}

void paddleBounce(const Game::Paddle &paddle, Game::Ball *ball) {
    if(ball && ball->x_speed() != 0) {
        if(ball->x_speed() < 0) {
            ball->x(paddle.x() + paddle.width() + (ball->x() - paddle.x() + paddle.width()));
            ball->x_speed(std::abs(ball->x_speed()));
        }
        else {
            ball->x(paddle.x() - ball->width() - (ball->x() + ball->width() - paddle.x()));
            ball->x_speed(-std::abs(ball->x_speed()));
        }
    }
}

int main(int argc, char* argv[]) {
    //Initializae SDL and OpenGL

    init();

    //Declare and set graphical variables
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

#if defined(__APPLE__) && defined(MacApp)
    setResourcePath();
#endif

    GLuint program1 = LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");

    GLuint MatrixID = glGetUniformLocation(program1, "MVP");
    //GLuint program1 = LoadShaders("git/pong/Pong.app/Contents/Resources/shaders/vertex.glsl", "git/pong/Pong.app/Contents/Resources/shaders/fragment.glsl");
    //GLuint MatrixID = glGetUniformLocation(program1, "MVP");


    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Prepare the rendering view
    glm::mat4 Projection = glm::ortho(-ORTHO_WIDTH, ORTHO_WIDTH, -ORTHO_HEIGHT, ORTHO_HEIGHT, 0.0f, 3.0f);
    glm::mat4 View = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
    
    //Prepare runloop variables
    unsigned int time_delta;
    int run_loop = 1;
    SDL_Event e;
    keys = SDL_GetKeyboardState(NULL);

    Game::Ball ball(-15, -15, 30, 30, 17, 17);
    Game::Paddle paddle1(-ORTHO_WIDTH + 50, -100, 30, 200, 15);
    Game::Paddle paddle2(paddle1);
    paddle2.x(ORTHO_WIDTH - 50 - paddle2.width());

    std::vector<Game::Rect*> objects;
    objects.push_back(static_cast<Game::Rect*>(&ball));
    objects.push_back(static_cast<Game::Rect*>(&paddle1));
    objects.push_back(static_cast<Game::Rect*>(&paddle2));

    unsigned char collision;
    
    while(run_loop) {
        //Start timing the frame
        time_delta = SDL_GetTicks();

        //Manage events
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    run_loop = 0;
                    break;
                default:
                    break;
            }
        }

        if(run_loop){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if(keys[SDL_SCANCODE_UP] && !keys[SDL_SCANCODE_DOWN]){
                paddle1.moveUp();
            }
            else if(keys[SDL_SCANCODE_DOWN] && !keys[SDL_SCANCODE_UP]){
                paddle1.moveDown();
            }
            collision = wallCollision(paddle1);
            if(collision)
                wallBounce(collision, &paddle1);

            basicAi(ball, &paddle2);
            collision = wallCollision(paddle2);
            if(collision)
                wallBounce(collision, &paddle2);

            ball.move();
            collision = paddleCollision(paddle1, ball);
            if(collision)
                paddleBounce(paddle1, &ball);

            collision = paddleCollision(paddle2, ball);
            if(collision)
                paddleBounce(paddle2, &ball);

            collision = wallCollision(ball);
            if(collision) {
                wallBounce(collision, &ball);
            }

            //Render all objects
            glUseProgram(program1);
            for(unsigned int i = 0; i<objects.size(); i++) {
                objects[i]->draw(Projection, View, MatrixID);
            }

            //Swap
            SDL_GL_SwapWindow(window);

            //Calculate amount of time to delay
            time_delta = SDL_GetTicks() - time_delta;

            //Delay next frame (hard set to 60fps)
            if(time_delta > 0 && time_delta <= 16)
                SDL_Delay(16 - time_delta);
        }
    }
    
    //Cleanup
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteProgram(program1);
    glDeleteVertexArrays(1, &VertexArrayID);

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
