#pragma once

namespace CeltEngine
{
    enum class RenderAPI
    {
        None = 0,
        OpenGL = 1,
        DirectX = 2,
        Vulkan = 3
    };

    class IRenderApi
    {
    public:
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
    };
    
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void PickRenderApi(RenderAPI api = RenderAPI::OpenGL);
        static RenderAPI GetRenderApi();
    };
    
}
