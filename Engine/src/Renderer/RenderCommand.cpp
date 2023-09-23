#include "cepch.h"
#include "RenderCommand.h"

#include "Renderer.h"
#include "Core/Logger.h"
#include "Vulkan/VulkanRenderCommand.h"

namespace CeltEngine
{
    static IRenderCommand* s_Api;
    
    void RenderCommand::Init()
    {
        switch (Renderer::GetRenderApi()) {
            case RenderAPI::None:
                CE_ERROR("No graphics API selected!");
                break;
            case RenderAPI::OpenGL:
                break;
            case RenderAPI::DirectX:
                break;
            case RenderAPI::Vulkan:
                s_Api = new VulkanRenderCommand();
                break;
            default:
                CE_ERROR("No graphics API selected!");
                break;
        }

        s_Api->Init();
    }

    void RenderCommand::Shutdown()
    {
        s_Api->Shutdown();
        delete s_Api;
    }
}
