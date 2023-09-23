#include "cepch.h"
#include "VulkanApi.h"

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace CeltEngine
{
    void VulkanApi::Init()
    {
        m_Instance.Init();
        constexpr DeviceRequirements deviceRequirements{true, true, true, true};
        m_Device.Init(deviceRequirements, &m_Instance);
    }

    void VulkanApi::Shutdown()
    {
        m_Device.Shutdown();
        m_Instance.Shutdown();
    }
}
