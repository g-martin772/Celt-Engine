#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include "VulkanImage.h"

namespace CeltEngine
{
    class VulkanDevice;
}

namespace CeltEngine
{
    class VulkanSwapChain
    {
    public:
        void Init(VulkanDevice* device, glm::vec2 size, uint32_t framesInFlight = 2);
        void Destroy();

        void AcquireNextImageIndex(vk::Semaphore semaphore, vk::Fence fence, uint64_t timeout = UINT64_MAX);
        void Present(vk::Queue graphicsQueue, vk::Queue presentQueue, vk::Semaphore waitSemaphore);

        void Update(glm::vec2 size);

        vk::SwapchainKHR GetSwapChain() const { return m_SwapChain; }
    private:
        void CreateSwapChain();
        void DestroySwapChain();
    private:
        glm::vec2 m_Size = { 0.0f, 0.0f };
        vk::Extent2D m_Extent = { 0, 0 };
        uint32_t m_FramesInFlight = 3, m_CurrentFrame = 0;
        std::vector<vk::Image> m_Images;
        std::vector<vk::ImageView> m_Views;
        VulkanDevice* m_Device = nullptr;
        vk::SwapchainKHR m_SwapChain;
        vk::Format m_Format = vk::Format::eB8G8R8A8Unorm;
        vk::ColorSpaceKHR m_ColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        VulkanImage2D m_DepthImage;
    };
}
