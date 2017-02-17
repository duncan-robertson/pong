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
#include <cmath>

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

typedef struct WindowContext {
    SDL_Window *window;
    SDL_GLContext context;
} WindowContext;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float ORTHO_HEIGHT = 500;
const float ORTHO_WIDTH = ((float)SCREEN_WIDTH/SCREEN_HEIGHT)*ORTHO_HEIGHT;
const glm::mat4 Projection = glm::ortho(-ORTHO_WIDTH, ORTHO_WIDTH, -ORTHO_HEIGHT, ORTHO_HEIGHT, 0.0f, 3.0f);
const glm::mat4 View = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));

unsigned char init(WindowContext *win_context) {
    SDL_Window *window;
    SDL_GLContext context;
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

    win_context->window = window;
    win_context->context = context;

    return 0;
}

void loadFont(Font *f) {
    f->texture = loadAlphabet();
    if(f->texture) {
        f->height_ratio = 7.0f/5.0f;
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

            uv_array[0] = (0.10f * width_offset) + 0.002f;
            uv_array[1] = (0.25f * height_offset) + 0.002f;;
            uv_array[2] = (0.10f * width_offset) + 0.002f;
            uv_array[3] = (0.25f + (0.25f * height_offset)) - 0.002f;
            uv_array[4] = (0.10f + (0.10f *  width_offset)) - 0.002f;
            uv_array[5] = (0.25f * height_offset) + 0.002f;
            uv_array[6] = (0.10f * width_offset) + 0.002f;
            uv_array[7] = (0.25f + (0.25f * height_offset)) - 0.002f;
            uv_array[8] = (0.10f + (0.10f * width_offset)) - 0.002f;
            uv_array[9] = (0.25f + (0.25f * height_offset)) - 0.002f;
            uv_array[10] = (0.10f + (0.10f * width_offset)) - 0.002f;
            uv_array[11] = (0.25f * height_offset) + 0.002f;

            glGenBuffers(1, &(f->uv_buffers[i]));
            glBindBuffer(GL_ARRAY_BUFFER, f->uv_buffers[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(uv_array), uv_array, GL_STATIC_DRAW);
        }
    }
}

void graphical_print(const Font &f, const GLfloat &x, const GLfloat &y, const GLfloat &font_width, const GLuint &MatrixID, const std::string &text) {
    GLfloat offset = (font_width/2);
    GLfloat height_offset = offset*(7.0f/5);
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
            Model = glm::translate(Model, glm::vec3(x+offset+(i*letter_spacing), y+height_offset, 0.0f));
            Model = glm::scale(Model, glm::vec3(font_width, font_width, 0.0f));
            glm::mat4 MVP = Projection * View * Model;
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

            glDrawArrays(GL_TRIANGLES, 0, 18);
        }
    }
}

void cleanup(WindowContext *win_context) {
    SDL_GL_DeleteContext(win_context->context);
    SDL_DestroyWindow(win_context->window);
}

void basicAi(const Game::Ball &ball, Game::Paddle *paddle) {
    if(paddle->x() * ball.x_speed() > 0) {
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
    unsigned char out;
    if(ball.x() < paddle.x() + paddle.width() &&
            ball.x() + ball.width() > paddle.x() &&
            ball.y()  < paddle.y() + paddle.height() &&
            ball.y() + ball.height() > paddle.y()) {
        if(ball.y() + ball.height() > paddle.y() + paddle.height()) {
            out = 0x02;
        }
        else if(ball.y() < paddle.y()) {
            out = 0x03;
        }
        else if(ball.y() > paddle.y() + (paddle.height()/2) + (ball.height()/2)) {
            out = 0x04;
        }
        else if(ball.y() + ball.height() < paddle.y() + (paddle.height()/2) - (ball.height()/2)) {
            out = 0x05;
        }
        else {
            out = 0x01;
        }
    }
    else {
        out = 0;
    }
    return out;
}

void paddleBounce(const unsigned char &type, const Game::Paddle &paddle, Game::Ball *ball) {
    assert(ball);

    if(ball->x_speed() < 0) {
        ball->x(paddle.x() + paddle.width() + (ball->x() - paddle.x() + paddle.width()));
    }
    else {
        ball->x(paddle.x() - ball->width() - (ball->x() + ball->width() - paddle.x()));
    }

    switch(type) {
        case 0x02:
            ball->y_speed(std::abs(ball->y_speed()) * 1.15f);
            ball->x_speed(ball->x_speed() * -1.10f);
            break;
        case 0x03:
            ball->y_speed(-std::abs(ball->y_speed()) * 1.15f);
            ball->x_speed(ball->x_speed() * -1.10f);
            break;
        case 0x04:
            ball->y_speed(std::abs(ball->y_speed()) * 1.10f);
            ball->x_speed(ball->x_speed() * -1.05f);
            break;
        case 0x05:
            ball->y_speed(-std::abs(ball->y_speed()) * 1.10f);
            ball->x_speed(ball->x_speed() * -1.05f);
            break;
        default:
            ball->x_speed(ball->x_speed() * -0.95f);
            if(std::abs(ball->x_speed()) < std::abs(ball->origin_x_speed())) {
                if(ball->x_speed() < 0) {
                    ball->x_speed(-ball->origin_x_speed());
                }
                else {
                    ball->x_speed(ball->origin_x_speed());
                }
            }
            break;
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

void gameLoop(SDL_Window *window) {
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

    //Set GL values
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDisable(GL_DEPTH_TEST);
    
    //Prepare runloop variables
    unsigned int time_delta, game_delay = 0;
    unsigned char collision, p1_score = 0, p2_score = 0, 
                  game_start = 0, pause = 0, space_down = 0,
                  no_exit = 1, game_over = 0;
    SDL_Event e;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    //Load font
    Font game_font;
    loadFont(&game_font);
    if(!game_font.texture) {
        std::cout << "Error loading font\n";
        return;
    }
    glBindTexture(GL_TEXTURE_2D, game_font.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Vector containing everything to be drawn
    std::vector<Game::Rect*> objects;

    //Manually allocated, must be freed
    Game::Rect *background = addBackground(&objects);

    //Init all game objects
    Game::Ball ball(-15, -15, 30, 30, 17, 17);
    Game::Paddle paddle1(-ORTHO_WIDTH + 50, -100, 30, 200, 30);
    Game::Paddle paddle2(paddle1);
    paddle2.x(ORTHO_WIDTH - 50 - paddle2.width());

    objects.push_back(static_cast<Game::Rect*>(&ball));
    objects.push_back(static_cast<Game::Rect*>(&paddle1));
    objects.push_back(static_cast<Game::Rect*>(&paddle2));
    
    while(no_exit) {
        if(time_delta && game_delay && pause) {
            game_delay += SDL_GetTicks()-time_delta;
        }
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

        if(keys[SDL_SCANCODE_SPACE] && !space_down) {
            space_down = 0x01;
        }
        else if(!keys[SDL_SCANCODE_SPACE] && space_down) {
            space_down = 0;
        }

        if(game_start && (space_down & 0x01)) {
            pause ^= 0x01;
            space_down++;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(game_over) {
            glUseProgram(program2);
            if(game_over == 1) {
                graphical_print(game_font, -((100*6*1.15f)+100)/2, ((-7.0f/5)*100)/2, 100, MatrixID, "you win");
            }
            else {
                graphical_print(game_font, -((100*7*1.15f))/2, ((-7.0f/5)*100)/2, 100, MatrixID, "you lose");
            }

            graphical_print(game_font, -((50*9*1.15))/2, -160, 50, MatrixID, "play again");
            graphical_print(game_font, -((50*10*1.15f)+50)/2, -250, 50, MatrixID, "press space");

            if(space_down & 0x01) {
                pause = 0;
                game_over = 0;
                game_start = 1;
                p1_score = 0;
                p2_score = 0;
                ball.x(-ball.width()/2);
                ball.y(-ball.height()/2);
                ball.speed_reset();
                game_delay = SDL_GetTicks()+1000;
                space_down++;
            }
        }
        else if(pause) {
            glUseProgram(program2);
            graphical_print(game_font, -((150*4*1.15f)+150)/2, ((-7.0f/5)*150)/2, 150, MatrixID, "pause");
        }
        else if(!game_start) {
            glUseProgram(program2);
            graphical_print(game_font, -((200*3*1.15f)+200)/2, ((-7.0f/5)*200)/2, 200, MatrixID, "pong");
            graphical_print(game_font, -((50*10*1.15f)+50)/2, -250, 50, MatrixID, "press space");

            basicAi(ball, &paddle1);
            collision = wallCollision(paddle1);
            if(collision)
                wallBounce(collision, &paddle1);

            if(space_down & 0x01) {
                game_start = 1;
                p1_score = 0;
                p2_score = 0;
                ball.x(-ball.width()/2);
                ball.y(-ball.height()/2);
                ball.speed_reset();
                game_delay = SDL_GetTicks()+1000;
                space_down++;
            }
        }
        else if(no_exit){
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
        }

        if(!game_delay && !pause) {
            basicAi(ball, &paddle2);
            collision = wallCollision(paddle2);
            if(collision)
                wallBounce(collision, &paddle2);

            ball.move();
            collision = paddleCollision(paddle1, ball);
            if(collision)
                paddleBounce(collision, paddle1, &ball);

            collision = paddleCollision(paddle2, ball);
            if(collision)
                paddleBounce(collision, paddle2, &ball);

            collision = wallCollision(ball);
            if(collision & 0x01) {
                p2_score++;
                if(p2_score > 9) {
                    if(!game_start)
                        p2_score = 0;
                    else {
                        game_over = 2;
                        pause = 1;
                        game_start = 0;
                    }
                }
                ball.x(-ball.width()/2);
                ball.y(-ball.height()/2);
                ball.speed_reset();
                game_delay = SDL_GetTicks()+1000;
            }
            else if (collision & 0x02) {
                p1_score++;
                if(p1_score > 9) {
                    if(!game_start)
                        p1_score = 0;
                    else {
                        game_over = 1;
                        pause = 1;
                        game_start = 0;
                    }
                }
                ball.x(-ball.width()/2);
                ball.y(-ball.height()/2);
                ball.speed_reset();
                game_delay = SDL_GetTicks()+1000;
            }
            else if(collision) {
                wallBounce(collision, &ball);
            }
        }

        glUseProgram(program2);
        if(p1_score > 9) 
            graphical_print(game_font, -250 - (150*1.15f), 250, 150, MatrixID, std::to_string(p1_score));
        else
            graphical_print(game_font, -250, 250, 150, MatrixID, std::to_string(p1_score));

        graphical_print(game_font, 100, 250, 150, MatrixID, std::to_string(p2_score));

        glUseProgram(program1);
        for(unsigned int i = 0; i<objects.size(); i++) {
            objects[i]->draw(Projection, View, MatrixID);
        }

        SDL_GL_SwapWindow(window);
    }
    objects.clear();
    free(background);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteProgram(program1);
    glDeleteVertexArrays(1, &VertexArrayID);
}

int main(int argc, char* argv[]) {
    WindowContext primary;

    if(init(&primary)) {
        std::cout << "Fatal initialization error. Aborting\n";
    }
    else {
        SDL_Window *window = primary.window;

        gameLoop(window);
        
        cleanup(&primary);
    }

    SDL_Quit();

    return 0;
}
