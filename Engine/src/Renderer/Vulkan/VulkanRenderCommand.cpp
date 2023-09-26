#include "cepch.h"
#include "VulkanRenderCommand.h"

#include "VulkanApi.h"

namespace CeltEngine
{
    static VulkanApi* s_Api; 
    
    void VulkanRenderCommand::Init()
    {
        s_Api = new VulkanApi();
        s_Api->Init();
    }

    void VulkanRenderCommand::Shutdown()
    {
        s_Api->Shutdown();
        delete s_Api;
    }

    bool VulkanRenderCommand::BeginFrame()
    {
        return true;
    }

    void VulkanRenderCommand::EndFrame()
    {
    }

    void VulkanRenderCommand::Resize(glm::vec2 newSize)
    {
    }

    void VulkanRenderCommand::SetClearColor(glm::vec4 color)
    {
    }
}
