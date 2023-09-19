#include "cepch.h"
#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Application.h"

void glfwSetWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    CeltEngine::WindowResizeEvent e(width, height);
    CeltEngine::Application::Current()->DispatchEvent<CeltEngine::WindowResizeEvent>(e);
}

void glfwSetWindowCloseCallback(GLFWwindow* window)
{
    CeltEngine::WindowCloseEvent e;
    CeltEngine::Application::Current()->DispatchEvent<CeltEngine::WindowCloseEvent>(e);
}

void glfwSetWindowFocusCallback(GLFWwindow* window, int focused)
{
    if(focused)
    {
        CeltEngine::WindowFocusEvent e;
        CeltEngine::Application::Current()->DispatchEvent<CeltEngine::WindowFocusEvent>(e);
    }
    else
    {
        CeltEngine::WindowLostFocusEvent e;
        CeltEngine::Application::Current()->DispatchEvent<CeltEngine::WindowLostFocusEvent>(e);
    }
}

void glfwSetWindowPosCallback(GLFWwindow* window, int x, int y)
{
    CeltEngine::WindowMovedEvent e(x, y);
    CeltEngine::Application::Current()->DispatchEvent<CeltEngine::WindowMovedEvent>(e);
}

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    CeltEngine::FrameBufferResizeEvent e(width, height);
    CeltEngine::Application::Current()->DispatchEvent<CeltEngine::FrameBufferResizeEvent>(e); 
}

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

        glfwSetWindowSizeCallback(m_Window, glfwSetWindowSizeCallback);
        glfwSetWindowCloseCallback(m_Window, glfwSetWindowCloseCallback);
        glfwSetWindowFocusCallback(m_Window, glfwSetWindowFocusCallback);
        glfwSetWindowPosCallback(m_Window, glfwSetWindowPosCallback);
        glfwSetFramebufferSizeCallback(m_Window, glfwFramebufferSizeCallback);

        glfwMakeContextCurrent(m_Window);
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);

    }

    void Window::OnEvent(Event& e)
    {
    }
}
