#pragma once
#include "Renderer/RenderCommand.h"

namespace CeltEngine
{
    class VulkanRenderCommand : public IRenderCommand
    {
    public:
        void Init() override;
        void Shutdown() override;
        bool BeginFrame() override;
        void EndFrame() override;

        void Resize(glm::vec2 newSize) override;

        void SetClearColor(glm::vec4 color) override;
    };
}
