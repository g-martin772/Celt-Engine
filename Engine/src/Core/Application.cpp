#include "cepch.h"
#include "Application.h"

#include "Event.h"
#include "Logger.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

namespace CeltEngine
{
    static Application* s_Application;
    
    Application* Application::Current()
    {
        return s_Application;
    }

    Application::Application(const ApplicationConfig& config)
    {
        s_Application = this;

        Logger::Init("latest.log");
        
        Renderer::PickRenderApi(RenderAPI::Vulkan);
        
        WindowConfig windowConfig;
        m_Window = std::make_shared<Window>(windowConfig);

        RenderCommand::Init();
    }

    Application::~Application()
    {
        s_Application = nullptr;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            {
                std::scoped_lock lock(m_EventMutex);
                while (!m_EventQueue.empty())
                {
                    OnEvent(m_EventQueue.front());
                    m_EventQueue.pop();
                }
            }

            m_Window->OnUpdate();
            
            OnUpdate();
        }
    }

    void Application::OnEvent(Event& event)
    {
        if (event.Type == EventType::WindowClose)
        {
            ShutDown();
            return;
        }
        
        for (const auto& fn : m_Callbacks)
        {
            if (event.Handled)
                return;
            event.Handled = fn(event);
        }
    }

    void Application::ShutDown()
    {
        m_Running = false;
        OnShutDown();

        RenderCommand::Shutdown();
        Logger::Shutdown();
    }

    void Application::AddEventCallback(const std::function<bool(Event&)>& callback)
    {
        m_Callbacks.push_back(callback);
    }
}
