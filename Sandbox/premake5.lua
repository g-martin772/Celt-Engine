project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")
    objdir ("%{wks.location}/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }
    
    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }
    
    includedirs {
        "src",
        "%{wks.location}/Engine/src",
    }
    
    links {
        "CeltEngine",
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
        
        links {
          "dl",
          "pthread"
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
