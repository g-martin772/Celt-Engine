#include "cepch.h"
#include "Renderer.h"

#include "Core/Logger.h"
#include "OpenGL/OpenGLApi.h"
#include "Vulkan/VulkanApi.h"

namespace CeltEngine
{
    static RenderAPI s_RenderApi = RenderAPI::None;
    static IRenderApi* s_Instance;

    void Renderer::PickRenderApi(RenderAPI api)
    {
        if (s_RenderApi != RenderAPI::None)
            std::cout << ("Render API already picked, ignoring new pick") << std::endl;
        s_RenderApi = api;
    }

    void Renderer::Init()
    {
        switch (s_RenderApi)
        {
        case RenderAPI::OpenGL:
            s_Instance = new OpenGLApi();
            break;
        case RenderAPI::Vulkan:
            s_Instance = new VulkanApi();
            break;
        default:
            CE_WARN("No Render API picked, defaulting to Vulkan");
            s_Instance = new VulkanApi();
            break;
        }

        s_Instance->Init();   
    }

    void Renderer::Shutdown()
    {
    }

    RenderAPI Renderer::GetRenderApi()
    {
        return s_RenderApi;
    }
}
