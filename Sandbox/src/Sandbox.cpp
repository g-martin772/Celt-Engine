#include "Core/Entrypoint.h"

class Sandbox : public CeltEngine::Application
{
    
};

CeltEngine::Application* CreateApplication()
{
    return new Sandbox();
}