#pragma once
#include <mutex>
#include <queue>
#include <functional>

#include "Event.h"

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

    private:
        virtual void OnUpdate() = 0;
        virtual void OnShutDown() = 0;
    private:
        bool m_Running;

        std::mutex m_EventMutex;
        std::queue<Event> m_EventQueue;
        std::vector<std::function<bool(Event&)>> m_Callbacks;
        
    };
}
