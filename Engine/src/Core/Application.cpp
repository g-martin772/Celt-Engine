#include "cepch.h"
#include "Application.h"

#include "Event.h"

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

        WindowConfig windowConfig;
        m_Window = std::make_shared<Window>(windowConfig);
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
        m_Window.reset();
    }

    void Application::AddEventCallback(const std::function<bool(Event&)>& callback)
    {
        m_Callbacks.push_back(callback);
    }
}
