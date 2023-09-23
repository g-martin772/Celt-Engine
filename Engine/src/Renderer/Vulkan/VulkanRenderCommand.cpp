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
}
