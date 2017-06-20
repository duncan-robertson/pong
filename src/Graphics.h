#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>

namespace Graphics
{    
    class Context
    {
        public:
        virtual void SetBackground(const float &red, const float &green,
                           const float &blue, const float &alpha)=0;
        virtual void Clear(void)=0;
        virtual void SwapBuffers(void)=0;
    };

    Context* GetContext(std::string name);
}

#endif