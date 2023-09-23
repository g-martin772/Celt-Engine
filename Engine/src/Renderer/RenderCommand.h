#pragma once

namespace CeltEngine
{
    class IRenderCommand
    {
    public:
        virtual ~IRenderCommand() = default;
        virtual void Init() = 0;
        virtual void Shutdown() = 0;
    };
    
    class RenderCommand
    {
    public:
        static void Init();
        static void Shutdown();
    };
}
