#include "Application.h"

extern CeltEngine::Application* CreateApplication();

int main(int argc, char** argv)
{
    CeltEngine::Application* app = CreateApplication();
    app->Run();
    delete app;
    return 0;
}