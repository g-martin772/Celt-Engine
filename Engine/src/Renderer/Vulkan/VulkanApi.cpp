#include "cepch.h"
#include "VulkanApi.h"

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace CeltEngine
{
    void VulkanApi::Init()
    {
        m_Instance.Init();
    }

    void VulkanApi::Shutdown()
    {
        m_Instance.Shutdown();
    }
}
