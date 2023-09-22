#include "cepch.h"
#include "Window.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Application.h"
#include "Logger.h"
#include "Renderer/Renderer.h"

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

        if (Renderer::GetRenderApi() == RenderAPI::OpenGL)
        {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
        else if (Renderer::GetRenderApi() == RenderAPI::Vulkan)
        {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        }
        
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
        glfwSwapInterval(config.VSync);

        if (Renderer::GetRenderApi() == RenderAPI::OpenGL)
        {
            if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
                CE_FATAL("Failed to load OpenGL!");
            CE_INFO("Loading OpenGL version %d.%d", GLVersion.major, GLVersion.minor);
            CE_ASSERT(GLVersion.major >= 4);
        }
        else if (Renderer::GetRenderApi() == RenderAPI::Vulkan)
        {
            
        }
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
