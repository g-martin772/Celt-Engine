#include "cepch.h"
#include "Renderer.h"

namespace CeltEngine
{
    static RenderAPI s_RenderApi = RenderAPI::None;


    void Renderer::PickRenderApi(RenderAPI api)
    {
        if (s_RenderApi != RenderAPI::None)
            std::cout << ("Render API already picked, ignoring new pick") << std::endl;
        s_RenderApi = api;
    }

    void Renderer::Init()
    {
        
    }

    void Renderer::Shutdown()
    {
    }

    RenderAPI Renderer::GetRenderApi()
    {
        return s_RenderApi;
    }
}
