#include "cepch.h"
#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Application.h"

namespace CeltEngine
{
    Window::Window(const WindowConfig& config)
    {
        m_Config = config;
        
        if(!glfwInit())
            CE_BREAK;

        m_Window = glfwCreateWindow(config.Width, config.Height, config.Name.c_str(), nullptr, nullptr);

        if(!m_Window)
        {
            glfwTerminate();
            CE_BREAK;
        }
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::OnUpdate()
    {
        if(glfwWindowShouldClose(m_Window))
            Application::Current()->ShutDown();
        
        glfwPollEvents();
        glfwSwapBuffers(m_Window);

    }

    void Window::OnEvent(Event& e)
    {
    }
}
