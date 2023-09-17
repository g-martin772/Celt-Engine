#include <iostream>

#include "Core/Entrypoint.h"

class Sandbox : public CeltEngine::Application
{
public:
    explicit Sandbox(const CeltEngine::ApplicationConfig& config)
        : Application(config)
    {
        AddEventCallback(OnTestEvent);
        
        CeltEngine::Event e1;
        DispatchEvent<CeltEngine::Event>(e1);

        CeltEngine::Event e2;
        DispatchEvent<CeltEngine::Event, true>(e2);
    }

private:
    static bool OnTestEvent(CeltEngine::Event& event)
    {
        std::cout << "test" << std::endl;
        return true;
    }
    
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
