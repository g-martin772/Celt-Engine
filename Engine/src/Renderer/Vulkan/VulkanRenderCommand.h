#pragma once
#include "Renderer/RenderCommand.h"

namespace CeltEngine
{
    class VulkanRenderCommand : public IRenderCommand
    {
    public:
        void Init() override;
        void Shutdown() override;
    };
}
