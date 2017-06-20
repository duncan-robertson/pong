#include <SDL2/SDL.h>
#include <GL/gl.h>

#include "Graphics.h"

namespace Graphics 
{
    class SDL2GLContext: public Context
    {
        public:
        SDL2GLContext(std::string name)
        {
            SCREEN_WIDTH = 1280;
            SCREEN_HEIGHT = 720;
            ORTHO_HEIGHT = 500;
            ORTHO_WIDTH = ((float)SCREEN_WIDTH/SCREEN_HEIGHT)*ORTHO_HEIGHT;
            name_ = name;

            SDL_Init(SDL_INIT_VIDEO);

            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetSwapInterval(1);

            window_ = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

            context_ = SDL_GL_CreateContext(window_);
        }

        ~SDL2GLContext()
        {
            SDL_GL_DeleteContext(context_);
            SDL_DestroyWindow(window_);
        }

        void SetBackground(const float &red, const float &green, const float &blue, const float &alpha)
        {
            glClearColor(red, green, blue, alpha);
        }

        void Clear()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void SwapBuffers()
        {
            SDL_GL_SwapWindow(window_);
        }

        private:
        SDL_Window *window_;
        SDL_GLContext context_;
        std::string name_;
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
        float ORTHO_HEIGHT;
        float ORTHO_WIDTH;

    };

    Context* GetContext(std::string name)
    {
        Context* c = new SDL2GLContext(name);
        return c;
    }
}