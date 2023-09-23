#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace CeltEngine
{
    class VulkanInstance
    {
    public:
        void Init();
        void Shutdown();

        vk::Instance GetInstance() const { return m_Instance; }
    private:
        vk::Instance m_Instance;
        vk::DispatchLoaderDynamic m_DynamicInstanceDispatcher;
        vk::DebugUtilsMessengerEXT m_DebugMessenger;
    };
}
