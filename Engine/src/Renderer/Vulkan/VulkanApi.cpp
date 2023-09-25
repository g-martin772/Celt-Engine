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

        m_GraphicsCommandPool.Init(&m_Device, m_Device.GetQueueIndices().Graphics);
        m_MainCommandBuffer = m_GraphicsCommandPool.AllocateCommandBuffer();
        
        m_SwapChain.Init(&m_Device, { 100.0f, 100.0f }, 3);
        
        // TODO: Provide useful values!
        m_MainRenderPass.Init(&m_Device, glm::vec4(0.0f, 0.0f, 100.0f, 100.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    void VulkanApi::Shutdown()
    {
        m_MainRenderPass.Destroy();
        m_SwapChain.Destroy();
        m_MainCommandBuffer->Free();
        m_GraphicsCommandPool.Destroy();
        m_Device.Shutdown();
        m_Instance.GetInstance().destroySurfaceKHR(m_Surface);
        m_Instance.Shutdown();
    }
}
