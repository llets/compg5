#include <Magick++.h>

#include "glut_backend.h"
#include "mainhandler.h"

#define WINDOW_WIDTH  600  
#define WINDOW_HEIGHT 600

int main(int argc, char** argv)
{
    srand(time(0));
    Magick::InitializeMagick(*argv);
    GLUTBackendInit(argc, argv);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Tutorial 35")) {
        return 1;
    }

    mainhandler* app = new mainhandler();

    if (!app->Init()) {
        return 1;
    }

    app->Run();
    delete app;

    return 0;
}