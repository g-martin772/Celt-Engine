#pragma once
#include <mutex>
#include <queue>
#include <functional>

#include "Core.h"
#include "Event.h"
#include "Window.h"

namespace CeltEngine
{
    struct ApplicationConfig
    {
        std::string Name = "CeltEngine";
        int Argc = 0;
        char** Argv = nullptr;
    };
    
    class Application
    {
    public:
        Application(const ApplicationConfig& config);
        virtual ~Application();

        static Application* Current();
        void Run();
        void OnEvent(Event& event);
        void ShutDown();

        void AddEventCallback(const std::function<bool(Event&)>& callback);

        template <typename T, bool Block = false>
        void DispatchEvent(T& e) {
            static_assert(std::is_assignable_v<Event, T>);

            if constexpr (Block) {
                OnEvent(e);
            } else {
                std::scoped_lock lock(m_EventMutex);
                m_EventQueue.push(e);
            }
        }

        const ApplicationConfig& GetSpecification() const { return m_Config; }
        Shared<Window> GetWindow() const { return m_Window; }

    private:
        virtual void OnUpdate() = 0;
        virtual void OnShutDown() = 0;
    private:
        bool m_Running;

        ApplicationConfig m_Config;

        std::mutex m_EventMutex;
        std::queue<Event> m_EventQueue;
        std::vector<std::function<bool(Event&)>> m_Callbacks;

        Shared<Window> m_Window;
    };
}
