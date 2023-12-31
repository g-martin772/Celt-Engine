﻿#pragma once

#include <glm/glm.hpp>

#include "Core.h"
#include "Event.h"

typedef struct GLFWwindow GLFWwindow;

namespace CeltEngine
{
    struct WindowConfig
    {
        std::string Name = "Celt-Engine";
        int Width = 1280, Height = 720;
        bool VSync = false, Fullscreen = false, Resizable = true, Maximized = false;
    };
    
    class Window
    {
    public:
        Window(const WindowConfig& config);
        virtual ~Window();
        
        void OnUpdate();
        void OnEvent(Event& e);

        glm::vec2 GetSize();

        void* GetNativeWindow() const;
    private:
        WindowConfig m_Config;
        GLFWwindow* m_Window;
    };
}
