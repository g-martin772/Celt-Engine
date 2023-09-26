#pragma once
#include <glm/glm.hpp>

namespace CeltEngine
{
    class IRenderCommand
    {
    public:
        virtual ~IRenderCommand() = default;
        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual bool BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void Resize(glm::vec2 newSize) = 0;

        virtual void SetClearColor(glm::vec4 color) = 0;
    };
    
    class RenderCommand
    {
    public:
        static void Init();
        static void Shutdown();

        static bool BeginFrame();
        static void EndFrame();

        static void Resize(glm::vec2 newSize);

        static void SetClearColor(glm::vec4 color);
    };
}
