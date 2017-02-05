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

#include "shader.h"
#include "objects.h"
#include "alphabet.h"

#if defined(__APPLE__) && defined(MacApp)
    #include "MacApp.h"
#endif

typedef struct Font {
    GLuint texture;
    GLfloat height_ratio;
    GLuint vertex_buffer;
    GLuint uv_buffers[36];
} Font;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float ORTHO_HEIGHT = 500;
const float ORTHO_WIDTH = ((float)SCREEN_WIDTH/SCREEN_HEIGHT)*ORTHO_HEIGHT;
const glm::mat4 Projection = glm::ortho(-ORTHO_WIDTH, ORTHO_WIDTH, -ORTHO_HEIGHT, ORTHO_HEIGHT, 0.0f, 3.0f);
const glm::mat4 View = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));

SDL_Window *window;
SDL_GLContext context;

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

#if defined(__APPLE__) && defined(MacApp)
    setResourcePath();
#endif

    return 0;
}

void loadFont(Font *f) {
    f->texture = loadAlphabet();
    if(f->texture) {
        f->height_ratio = 7/5;
        const GLfloat &height_ratio = f->height_ratio;
        GLfloat vertex_array[18] = {
            -0.5f, 0.5f*height_ratio, 0.0f,
            -0.5f, -0.5f*height_ratio, 0.0f,
            0.5f, 0.5f*height_ratio, 0.0f,
            -0.5f, -0.5f*height_ratio, 0.0f,
            0.5f, -0.5f*height_ratio, 0.0f,
            0.5f, 0.5f*height_ratio, 0.0f};
        glGenBuffers(1, &(f->vertex_buffer));
        glBindBuffer(GL_ARRAY_BUFFER, f->vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array), vertex_array, GL_STATIC_DRAW);

        GLfloat uv_array[12];
        unsigned int width_offset;
        unsigned int height_offset;
        for(unsigned int i=0; i<36; i++) {
            width_offset = i%10;
            height_offset = i/10;

            uv_array[0] = (0.10f * width_offset) + 0.001f;
            uv_array[1] = (0.25f * height_offset) + 0.001f;;
            uv_array[2] = (0.10f * width_offset) + 0.001f;
            uv_array[3] = (0.25f + (0.25f * height_offset)) - 0.001f;
            uv_array[4] = (0.10f + (0.10f *  width_offset)) - 0.001f;
            uv_array[5] = (0.25f * height_offset) + 0.001f;
            uv_array[6] = (0.10f * width_offset) + 0.001f;
            uv_array[7] = (0.25f + (0.25f * height_offset)) - 0.001f;
            uv_array[8] = (0.10f + (0.10f * width_offset)) - 0.001f;
            uv_array[9] = (0.25f + (0.25f * height_offset)) - 0.001f;
            uv_array[10] = (0.10f + (0.10f * width_offset)) - 0.001f;
            uv_array[11] = (0.25f * height_offset) + 0.001f;

            glGenBuffers(1, &(f->uv_buffers[i]));
            glBindBuffer(GL_ARRAY_BUFFER, f->uv_buffers[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(uv_array), uv_array, GL_STATIC_DRAW);
        }
    }
}

void graphical_print(const Font &f, const GLfloat &x, const GLfloat &y, const GLfloat &font_width, const GLuint &MatrixID, const std::string &text) {
    GLfloat offset = (font_width/2);
    GLfloat letter_spacing = font_width*1.15f;
    char current_letter;
    glm::mat4 Model;
    glm::mat4 MVP;
    GLubyte whitespace;

    for(unsigned int i=0; i<text.size(); i++) {
        whitespace = 0;
        current_letter = tolower(text[i]);

        switch(current_letter) {
            case 'a':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[0]);
                break;
            case 'b':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[1]);
                break;
            case 'c':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[2]);
                break;
            case 'd':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[3]);
                break;
            case 'e':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[4]);
                break;
            case 'f':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[5]);
                break;
            case 'g':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[6]);
                break;
            case 'h':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[7]);
                break;
            case 'i':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[8]);
                break;
            case 'j':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[9]);
                break;
            case 'k':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[10]);
                break;
            case 'l':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[11]);
                break;
            case 'm':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[12]);
                break;
            case 'n':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[13]);
                break;
            case 'o':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[14]);
                break;
            case 'p':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[15]);
                break;
            case 'q':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[16]);
                break;
            case 'r':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[17]);
                break;
            case 's':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[18]);
                break;
            case 't':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[19]);
                break;
            case 'u':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[20]);
                break;
            case 'v':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[21]);
                break;
            case 'w':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[22]);
                break;
            case 'x':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[23]);
                break;
            case 'y':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[24]);
                break;
            case 'z':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[25]);
                break;
            case '1':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[26]);
                break;
            case '2':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[27]);
                break;
            case '3':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[28]);
                break;
            case '4':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[29]);
                break;
            case '5':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[30]);
                break;
            case '6':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[31]);
                break;
            case '7':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[32]);
                break;
            case '8':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[33]);
                break;
            case '9':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[34]);
                break;
            case '0':
                glBindBuffer(GL_ARRAY_BUFFER, f.uv_buffers[35]);
                break;
            default:
                whitespace = 1;
                break;
        }
        if(!whitespace) {
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glBindBuffer(GL_ARRAY_BUFFER, f.vertex_buffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            Model = glm::mat4();
            Model = glm::translate(Model, glm::vec3(x+offset+(i*letter_spacing), y+offset, 0.0f));
            Model = glm::scale(Model, glm::vec3(font_width, font_width, 0.0f));
            glm::mat4 MVP = Projection * View * Model;
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

            glDrawArrays(GL_TRIANGLES, 0, 18);
        }
    }
}

void cleanup() {
    SDL_DestroyWindow(window);
    SDL_Quit();
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
            ball->x(-ORTHO_WIDTH);
            ball->x_speed(std::abs(ball->x_speed()));
        }
        if(collision & 0x02) {
            ball->x(ORTHO_WIDTH - ball->width());
            ball->x_speed(-std::abs(ball->x_speed()));
        }
        if(collision & 0x04) {
            ball->y(-ORTHO_HEIGHT);
            ball->y_speed(std::abs(ball->y_speed()));
        }
        if(collision & 0x08) {
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

//Return pointer is allocated through malloc
//Free pointer only after the related values have been cleared from the vector
Game::Rect* addBackground(std::vector<Game::Rect*> *objects) {
    float section = (ORTHO_HEIGHT*2)/30;
    float offset = section/2;
    float height = section*2;
    float width = 10;
    unsigned int quantity = 10;
    
    Game::Rect *out  = static_cast<Game::Rect*>(malloc(sizeof(Game::Rect)*quantity));

    for(unsigned int i=0; i<quantity; i++) {
        new(out+i) Game::Rect(-(width/2), i * (height+section) - ORTHO_HEIGHT + offset, width, height);
        objects->push_back(out+i);
    }

    return out;
}

void gameLoop() {
    //Init VertexArray
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //Init Shaders
    GLuint program1 = LoadShaders("shaders/rawColorV.glsl", "shaders/rawColorF.glsl");
    GLuint program2 = LoadShaders("shaders/textureV.glsl", "shaders/textureF.glsl");
    GLuint MatrixID = glGetUniformLocation(program1, "MVP");
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Set GL preferences
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDisable(GL_DEPTH_TEST);

    //Init the main render matrices
    
    //Prepare runloop variables
    unsigned int time_delta, game_delay, no_exit = 1;
    SDL_Event e;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    //TESTING
    Font game_font;
    loadFont(&game_font);
    
#define X {255,100,200,255}
#define O {0, 0, 0, 0}
    GLubyte data[][4] = {
        X, X, X, X,
        X, O, O, X,
        X, X, X, X,
        X, O, O, X,
        X, O, O, X
    };
#undef X
#undef O
    GLuint testTex;
    glGenTextures(1, &testTex);
    glBindTexture(GL_TEXTURE_2D, testTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 5, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

#define TL -58,  28, 0
#define TR  0,  28, 0
#define BL -58, -0, 0
#define BR  0, -0, 0
    const GLfloat test_quad_vertex[] = {
        TL, BL, TR,
        BL, BR, TR
    };
#undef TL
#undef TR
#undef BL
#undef BR
#define TL 0, 0
#define TR 0.10, 0
#define BL 0, 0.25
#define BR 0.10, 0.25
    const GLfloat test_quad_uv[] = {
        TL, BL, TR,
        BL, BR, TR
    };
#undef TL
#undef TR
#undef BL
#undef BR

    GLuint vertex_test, uv_test;
    glGenBuffers(1, &vertex_test);
    glGenBuffers(1, &uv_test);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_test);
    glBufferData(GL_ARRAY_BUFFER, sizeof(test_quad_vertex), test_quad_vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, uv_test);
    glBufferData(GL_ARRAY_BUFFER, sizeof(test_quad_uv), test_quad_uv, GL_STATIC_DRAW);

    GLuint alphabet_texture = loadAlphabet();

    if(!alphabet_texture)
        std::cout << "Error loading alphabet texture\n";
    else {
        glBindTexture(GL_TEXTURE_2D, alphabet_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    //END TESTING

    //Vector containing everything to be drawn
    std::vector<Game::Rect*> objects;

    //Manually allocated, must be freed
    Game::Rect *background = addBackground(&objects);

    //Init all game objects
    Game::Ball ball(-15, -15, 30, 30, 17, 17);
    Game::Paddle paddle1(-ORTHO_WIDTH + 50, -100, 30, 200, 15);
    Game::Paddle paddle2(paddle1);
    paddle2.x(ORTHO_WIDTH - 50 - paddle2.width());

    objects.push_back(static_cast<Game::Rect*>(&ball));
    objects.push_back(static_cast<Game::Rect*>(&paddle1));
    objects.push_back(static_cast<Game::Rect*>(&paddle2));

    unsigned char collision;
    unsigned char p1_score = 0, p2_score = 0;
    
    while(no_exit) {
        //Start timing the frame
        time_delta = SDL_GetTicks();
        if(game_delay && time_delta > game_delay) {
            game_delay = 0;
        }

        //Manage events
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    no_exit = 0;
                    break;
                default:
                    break;
            }
        }

        if(keys[SDL_SCANCODE_ESCAPE] || keys[SDL_SCANCODE_Q])
            no_exit = 0;

        if(no_exit){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Allow paddle to still move even if game is delayed
            if(keys[SDL_SCANCODE_UP] && !keys[SDL_SCANCODE_DOWN]){
                paddle1.moveUp();
            }
            else if(keys[SDL_SCANCODE_DOWN] && !keys[SDL_SCANCODE_UP]){
                paddle1.moveDown();
            }
            collision = wallCollision(paddle1);
            if(collision)
                wallBounce(collision, &paddle1);

            if(!game_delay) {
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
                if(collision & 0x01) {
                    p2_score++;
                    if(p2_score > 9)
                        p2_score = 0;
                    ball.x(-ball.width()/2);
                    ball.y(-ball.height()/2);
                    game_delay = SDL_GetTicks()+1000;
                }
                else if (collision & 0x02) {
                    p1_score++;
                    if(p1_score > 9)
                        p1_score = 0;
                    ball.x(-ball.width()/2);
                    ball.y(-ball.height()/2);
                    game_delay = SDL_GetTicks()+1000;
                }
                else if(collision) {
                    wallBounce(collision, &ball);
                }
            }

            glUseProgram(program2);
            graphical_print(game_font, -250, 300, 150, MatrixID, std::to_string(p1_score));
            graphical_print(game_font, 100, 300, 150, MatrixID, std::to_string(p2_score));

            glUseProgram(program1);
            for(unsigned int i = 0; i<objects.size(); i++) {
                objects[i]->draw(Projection, View, MatrixID);
            }

            //Swap
            SDL_GL_SwapWindow(window);

            //Calculate amount of time to delay
            time_delta = SDL_GetTicks() - time_delta;

            //Delay next frame (hard set to 60fps)
            //if(time_delta > 0 && time_delta <= 16)
            //    SDL_Delay(16 - time_delta);
        }
    }
    objects.clear();
    free(background);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteProgram(program1);
    glDeleteVertexArrays(1, &VertexArrayID);
}

int main(int argc, char* argv[]) {
    //Initialize SDL and OpenGL
    init();

    gameLoop();
    
    cleanup();
    return 0;
}
