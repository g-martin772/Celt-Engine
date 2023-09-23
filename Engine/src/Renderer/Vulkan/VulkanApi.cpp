#include "cepch.h"
#include "VulkanApi.h"

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Core/Application.h"
#include "Core/Logger.h"
#include "Core/Window.h"

namespace CeltEngine
{
    void VulkanApi::Init()
    {
        m_Instance.Init();

        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Current()->GetWindow()->GetNativeWindow());

        VkSurfaceKHR surface;
        if(glfwCreateWindowSurface(m_Instance.GetInstance(), window, nullptr, &surface) != VK_SUCCESS)
            CE_ERROR("Failed to create window surface!");
        else
            CE_TRACE("Creating window surface: succesful");

        m_Surface = surface;
        
        DeviceRequirements deviceRequirements{};
        deviceRequirements.Compute = true;
        deviceRequirements.Graphics = true;
        deviceRequirements.Transfer = true;
        deviceRequirements.Present = true;
        deviceRequirements.Surface = m_Surface;
        m_Device.Init(deviceRequirements, &m_Instance);
    }

    void VulkanApi::Shutdown()
    {
        m_Device.Shutdown();
        m_Instance.GetInstance().destroySurfaceKHR(m_Surface);
        m_Instance.Shutdown();
    }
}
