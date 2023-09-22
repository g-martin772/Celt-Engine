#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include "VulkanInstance.h"
#include "Renderer/Renderer.h"

namespace CeltEngine
{
    class VulkanApi : public IRenderApi
    {
    public:
        void Init();
        void Shutdown();

        VulkanInstance GetInstance() const { return m_Instance; }
    private:
        VulkanInstance m_Instance;
    };
}
