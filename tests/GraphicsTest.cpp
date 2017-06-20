#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Graphics.h"

void* waitKey(void *ptr)
{
    int *p = (int*)ptr;
    char c = 0;
    while(c != 'q')
    {
        std::cin.get(c);
    }
    *p = 1;
    return 0;
}

int main()
{
    Graphics::Context* c = Graphics::GetContext("Test Window");

    //int quit = 0;
    //int *quitP = &quit;
    //pthread_t t;
    //int ret = pthread_create(&t, NULL, waitKey, (void*)quitP);

    srand(time(NULL));
    float r;
    float g;
    float b;
    int i = 0;
    while(i < 100)
    {
        r = static_cast<float>(rand())/RAND_MAX;
        g = static_cast<float>(rand())/RAND_MAX;
        b = static_cast<float>(rand())/RAND_MAX;
        c->SetBackground(r, g, b, 1.0f);
        c->Clear();
        c->SwapBuffers();
        usleep(100*1000);
        i++;
    }

    

    //pthread_join(t, NULL);
    return 0;
}