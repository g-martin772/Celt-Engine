#include "Application.h"

extern CeltEngine::Application* CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
    CeltEngine::Application* app = CreateApplication(argc, argv);
    app->Run();
    delete app;
    return 0;
}