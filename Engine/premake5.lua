project "CeltEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    objdir ("%{wks.location}/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    
    pchheader "cepch.h"
    pchsource "src/cepch.cpp"
    
    files {
        "src/**.h",
        "src/**.cpp"
    }
    
    defines {
        "CELT_ENGINE",
        "_CRT_SECURE_NO_WARNINGS"
    }
    
    vulkanSDK = os.getenv("VULKAN_SDK")
    
    includedirs {
        "src",
        "%{wks.location}/Dependencies/glfw/include",
        "%{wks.location}/Dependencies/glad/include",
        vulkanSDK .. "/Include"
    }
    
    links {
        "glfw",
        "glad"
    }
    
    filter "system:windows"
        systemversion "latest"
        
        defines {
            "CE_WINDOWS"
        }

    filter "system:linux"
        defines {
            "CE_LINUX"
        }

    filter "configurations:Debug"
        defines { "CE_DEBUG" }
        runtime "Debug"
        symbols "On"
        
        links {
            vulkanSDK .. "/Lib/vulkan-1.lib",
            vulkanSDK .. "/Lib/shaderc_combined.lib"
        }
    
    filter "configurations:Release"
        defines { "CE_RELEASE" }
        runtime "Release"
        optimize "On"
        
        links {
            vulkanSDK .. "/Lib/vulkan-1.lib",
            vulkanSDK .. "/Lib/shaderc_combinedd.lib"
        }
        
