#pragma once
#include "Renderer/Renderer.h"

namespace CeltEngine
{
    class OpenGLApi : public IRenderApi
    {
    public:
        void Init() override;
        void Shutdown() override;
    
    };
}
