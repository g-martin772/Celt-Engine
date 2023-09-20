#include <iostream>

#include "Core/Entrypoint.h"

class Sandbox : public CeltEngine::Application
{
public:
    explicit Sandbox(const CeltEngine::ApplicationConfig& config)
        : Application(config)
    {
        
    }

private:
    void OnUpdate() override
    {

    }

    void OnShutDown() override
    {
        
    }
};

CeltEngine::Application* CreateApplication(int argc, char** argv)
{
    CeltEngine::ApplicationConfig config;
    config.Argc = argc;
    config.Argv = argv;
    config.Name = "Sandbox";
    return new Sandbox(config);
}
